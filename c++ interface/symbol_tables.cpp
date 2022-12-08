
#include "stdafx.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "symbol_table.h"

#define MAX_BUFFER 4096
#pragma warning (disable: 4996)
#define slothful2
#define sortOn

token::token ()
{
	ascii = NULL;
	m_count = 0;
}

symbol_table::symbol_table ()
{
//	flag to indicate whether or not this list owns the
//	character pointers that it has, i.e. to determine
//	whether	or not it is allowed to delete them when the
//	destructor is called
	m_allocate = true;
	owner = true;
	_symbols.resize (0); //  = new token [8];
//	available = 8;
//	.size() = 0;
}

symbol_table::symbol_table (int total)
{
	m_allocate = true;
	_symbols.resize (total); // = new token [total];
	for (long j=0;j<total;j++) {
		_symbols [j].ascii=NULL;
		_symbols [j].m_count=0;
	}
//	available = total;
}

symbol_table::~symbol_table ()
{
	std::vector<token>::iterator iter, end;
	char *ascii;
	iter = _symbols.begin ();
	end = _symbols.end ();
	if (owner==true)
	for (;iter<end;iter++)
	{
		ascii = (*iter).ascii;
		if (ascii!=NULL)
			delete [] ascii;
	}
}

token &symbol_table::operator [] (unsigned int entryNum)
{
	if (entryNum<_symbols.size())
		return _symbols [entryNum];
	else
		throw entryNum;
}

bool symbol_table::compare (token item1, token item2)
{
	char buffer1 [MAX_BUFFER], buffer2 [MAX_BUFFER];
	strcpy_s (buffer1,MAX_BUFFER,item1.ascii);
	strcpy_s (buffer2,MAX_BUFFER,item2.ascii);
	to_lower (buffer1);
	to_lower (buffer2);
	if	((item1.m_count<item2.m_count)
		||((item1.m_count==item2.m_count)
		&&(strcmp(buffer1,buffer2)>0)))
		return false;
	else
		return true;
}

int symbol_table::_compare (const void *p1, const void *p2)
{
	int res;
	token *t1 = (token*)p1;
	token *t2 = (token*)p2;
	res = strcmp(t1->ascii,t2->ascii);
	return res;
}

symbol_table *symbol_table::sort()
{
	int sz = _symbols.size();
	std::qsort((void*)(&_symbols[0]),sz,sizeof(token),&_compare);
	return this;
}

void symbol_table::purge ()
{
	std::vector<token>::iterator iter, end;
	char *ascii;
	iter = _symbols.begin ();
	end = _symbols.end ();
	if (owner==true)
	for (;iter<end;iter++)
	{
		ascii = (*iter).ascii;
		if (ascii!=NULL)
			delete [] ascii;
	}
	_symbols.resize (0);
}

void symbol_table::extend (int howMany)
{
	int N = _symbols.size ();
	_symbols.resize (howMany+N);	
//	available=_symbols.size ();
}

void symbol_table::swap (int item1, int item2)
{
	token temp = _symbols [item2];
	_symbols [item2] = _symbols [item1];
	_symbols [item1] = temp;
}

//	Returns -1 if the ascii token is not found
//	and returns the index position if it is found

int symbol_table::checkWord (char *buffer, int count)
{
	std::vector<token>::iterator iter,end;
	char *str;
	unsigned int N;
	bool result = false;

	iter = _symbols.begin ();
	end = _symbols.end ();
	for (N=0;iter<end;iter++,N++)
	{
		str = (*iter).ascii;
		if (strcmp (buffer,str)==0) {
			(*iter).m_count+=count;
			result = true;
			break;			
		}
	}
	if (result == true)
		return N;
	else
		return -1;
}

//	Allocates the token object and the char [],
//	then adds the token to the end of the list

void symbol_table::addword (char *buffer, int count)
{
	int c = _symbols.capacity ();
	int N = _symbols.size ();
	_symbols.reserve(N+2);
	token t;
	if (m_allocate==true)
	{
		t.ascii = new char [strlen(buffer)+1];
		strcpy (t.ascii,buffer);
	}
	else
		t.ascii=buffer;

	t.m_count = count;
	_symbols.push_back (t);
}

//	A slightly more effecient than the last insertion
//	method moving the last item up a list or table.
//	This one is a little smarter on how it swaps things,
//	cutting the swapping time by about 2/3 ... 

#ifdef slothful2

void symbol_table::sortWord (int where)
{
	token temp = _symbols [where];
	if (_symbols.size()>1) {
		while (compare (_symbols[where-1],temp)==false) {
			_symbols [where].ascii = _symbols [where-1].ascii;
			_symbols [where].m_count = _symbols [where-1].m_count;
			where--;
			if (where==0)
				break; }
	}
	_symbols [where] = temp;
}
#endif

//	Checks where the last token on the list to see where
//	it really belongs, and then uses insertWord to put in
//	into the correct position.  Remember that we only need
//	to call addWord for new words.  We always call insert
//	token, i.e. just to take care of the case where a token
//	needs to be moved up the list because of frequency of
//	use.

void symbol_table::index_word (char *buffer, int times)
{
	if (strlen(buffer)!=0) {
		int position = checkWord (buffer,times);
		if (position<0) {
			addword (buffer,times);
			sortWord (_symbols.size()-1); }
		else
		if (position!=0)
			sortWord (position); }
}

//	remove an object from an index and delete the object
//	pointed to if this index owns the object.

void symbol_table::removeWord (int i)
{
	char *ascii;
	unsigned int j;
	token theWord, nextWord;
	theWord = _symbols [i];
	if (owner==true)
		delete [] theWord.ascii;
	if (i==_symbols.size()-1) {
		j=i;
		_symbols [j].ascii = NULL; }
	else {
		for (j=i;j<_symbols.size();j++) {
			ascii = _symbols [j+1].ascii;
			nextWord = _symbols [j+1];
			_symbols [j] = nextWord;
			theWord = nextWord; }
		_symbols [j].ascii=NULL; }
	_symbols.resize(_symbols.size()-1);
}

void symbol_table::killDelimiters ()
{
	unsigned int i;
	for (i=0; i<_symbols.size(); i++) {
		char *theWord = _symbols [i].ascii;
		char aChar = theWord [0];
		if (isalnum (aChar)==false)
			removeWord (i);
	}
}

//	check to see if the token at position is correctly inserted
//	and reinsert it if it isnt, not presently working!

void symbol_table::sift (int position)
{
	int n = 0;
	token theWord, aWord;
	theWord = _symbols [position];
	aWord = _symbols [n];
	if (strcmp (theWord.ascii,aWord.ascii)==0)
	{
//	delete the char * of the token we are sifting
//	into place if it points to a duplicate string

		if (theWord.ascii!=aWord.ascii)
			delete (aWord.ascii);
		theWord.m_count = theWord.m_count+aWord.m_count;
		aWord.m_count = 0;
	}
}

//	Merges a second symbol_table List with this symbol_tableList,
//  then returns the resulting List.  Resets the owner flag
//	on the list that has been merged, deletes duplicate
//	pointers to char ... etc.

symbol_table symbol_table::merge (symbol_table another)
{
	another.owner = false;
	symbol_table theIndex;
	token theWord, aWord, bWord;
	int aPos, bPos;
	aPos = bPos = 0;
	int required = _symbols.size()+another.size();
	theIndex._symbols.resize(required);
	for (int count=0;count<required;count++) {
		aWord = _symbols [aPos];
		bWord = another [bPos];
		if (aWord.m_count>bWord.m_count) {
			theIndex [count] = aWord;
			aPos++;
			theIndex.sift (count); }
		else {
			theIndex._symbols [count] = bWord;
			bPos++;
			theIndex.sift (count); }
	}
	return theIndex;
}

