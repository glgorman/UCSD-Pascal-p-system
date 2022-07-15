
#include "stdafx.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "sets.h"
#include "extras.h"

#define noPlayTag true

node<char*>::node<char*> ()
{
	refCount = 1;
	m_pNext = NULL;
	m_pData = NULL;
}

node<char*>::node<char*> (char* str)
{
	refCount = 1;
	m_pData = str;
	m_pNext = NULL;
}

node<char*>::~node<char*> ()
{
// todo - notify the symbol table
// manager that we no longer need
// this char
}

node_list<char*>::node_list<char*> ()
{
	refCount = 1;
	m_nBegin = NULL;
	m_nEnd = NULL;
	m_nPos = NULL;
}

node_list<char*>::~node_list<char*> ()
{
	node<char*> *ptr, *next;
	ptr = m_nBegin;
	while (ptr!=NULL)
	{
		next = ptr->m_pNext;
		delete ptr;
		ptr = next;
	}
}


node_list<char*>::node_list<char*> (char *data, bool)
{
	refCount = 1;
	if (data==NULL)
	{
		m_nBegin = NULL;
		m_nEnd = NULL;
		m_nPos = NULL;
	}
	else {
		m_nBegin = new node<char*>;
		m_nEnd = m_nBegin;
		m_nBegin->m_pData = data;
		m_nBegin->m_pNext = NULL;
		m_nPos = m_nBegin;
	}
}

void node_list<char*>::concat(node<char*> *N)
{
	if (m_nBegin==NULL)
	{
		m_nBegin=N;
		m_nEnd=N;
		m_nPos=N;
	}
	else
	{
		node<char*> *N0 = m_nEnd;
		m_nEnd = N;
		N0->m_pNext = N;
		N->m_pNext = NULL;
	}
}

//template <class nodeType>
node_list<char*>::node_list<char*> (char* str)
{
	bool ignore_whitespace = false;
	refCount = 1;
	m_nBegin = NULL;
	m_nEnd = NULL;
	int length, start, position;
	start = 0;
	position = 0;
	char* buffer = NULL;
	if (str!=NULL) {
		length = strlen (str);
		buffer = new char [length+1];
	}
	else return;
	char c = str [start];
	while (c!=0)
	{
		int n = 0;
		position = start;
		c = str[position];

//	skip leading spaces, linefeeds, tabs
//	and returns;
		if (ignore_whitespace==true)
		while (chartype::whitespace.in(c))
		{
			position++;
			c = str[position];
		}
		start = position;

//	tokenize if alphanumeric	
		if (chartype::alpha.in(c))
		do {
			buffer [n++] = c;
			position++;
			c = str[position];
		}	
		while (chartype::symbol.in(c));

//	else tokenize numeric
		else if (chartype::digits.in(c)) do {
			buffer [n++] = c;
			position++;
			c = str[position];
		}
		while (chartype::digits.in(c));

//	else any other printing chars
		else if (str[position]!=0) do {
			buffer [n++] = c;
			position++;
			c = str[position];
		}
		while ((c!=0)&&(!chartype::symbol.in(c))
			&&(!chartype::whitespace.in(c)));

		buffer [n]=0;
		if (*buffer!=0)
			append_node (buffer);
		start = position;
		c = str[position];
	}
	delete [] buffer;
}

//	given a list of bTreeType<nodeType> objects
//	contruct a usual node_list<nodeType> with the
//	same ascii pointers;

template <class nodeType>
node_list<nodeType>::node_list<nodeType> (bTreeType<nodeType> **source)
{
	int n = 0;
	nodeType ascii;
	refCount = 1;
	m_nBegin = NULL;
	m_nEnd = NULL;
	while (source [n]!=NULL) {
		ascii = source [n]->m_pData;
		appendNode (ascii);
		n++; }
}

template <class nodeType>
node_list<nodeType>::~node_list<nodeType> ()
{
	nosw_liar<nodeType> *m_nPos = m_nBegin;
	while (m_nPos!=NULL) {
		s_node<nodeType,enum language> *chomp = m_nPos;
		m_nPos = m_nPos->m_pNext;
		delete chomp; }
}

//	Returns a pointer to a permanant char []
//	pointed to by the s_node<nodeType,enum language> pointed to by
//	the m_nPos - then increments the play
//	tag -- which must be provied by the
//	caller!!

template <class nodeType>
nodeType node_list<nodeType>::putNextNode (s_node<nodeType,enum language> *(&m_nPos))
{
	nodeType theWord = NULL;
	if (m_nPos!=NULL) {
		theWord = m_nPos->m_pData;
		m_nPos = m_nPos->m_pNext; }
	return theWord;
}

//	Copies a word from from a word list into a
//	char buffer provided by the caller

#ifndef noPlayTag
template <class nodeType>
void node_list<nodeType>::putTempWord (nodeType theWord)
{
	if (m_nPos==NULL)
		theWord[0]= '\0';
	else if (theWord!=NULL) {
		strcpy (theWord,m_pData);
		m_nPos = m_nPos->m_pNext; }
}
#endif

//	when passed a reference to an (s_node<nodeType,enum language> *) 
//	this will repair the pointer to the	lastNode of this list 
//	if necessary and put the adress of that node into the 
//	argument provided

template <class nodeType>
void node_list<nodeType>::putLastNode (s_node<nodeType,enum language> *(&theWord))
{
	if (m_nBegin==NULL)
		theWord = NULL;
	else if (m_nEnd!=NULL)
		theWord = m_nEnd;
	else {
		theWord = m_nBegin;
		while (theWord->m_pNext!=NULL) {
			if (theWord!=theWord->m_pNext)
				theWord = theWord->m_pNext;
			else
				theWord->m_pNext=NULL; } }
}

//	append an existing s_node<nodeType,enum language> to a node_list<nodeType>
//	WARNING! The argument must by a	dynanicly allocated
//	s_node<nodeType,enum language>, or corrpution will be an aboulute certainty.

template <class nodeType>
void node_list<nodeType>::attachWord (s_node<nodeType,enum language> *theWord)
{
	s_node<nodeType,enum language> *theNode;
	try {
	if (theWord!=NULL)
		theWord->m_pNext = NULL;
	else
		throw (theWord);
	putLastNode (theNode);
	if (theNode==NULL) {
		m_nBegin = theWord;
		m_nEnd = theWord; }	
	else {
		m_nEnd->m_pNext = theWord;
		m_nEnd = theWord; } }
	catch (s_node<nodeType,enum language>*)
	{
//	do nothing
	}
}

node<char*> &node_list<char *>::operator >> (char *(&ptr))
{
	int created, needed;
	node<char*> *nodeptr = m_nBegin;
	char *buffer = NULL;
	needed = 0;
	while (nodeptr!=NULL) {
		int sz1 = strlen (nodeptr->m_pData)+1;
		needed = needed+sz1;
		nodeptr = nodeptr->m_pNext;
	}
	buffer = new char [needed+1];
	strcpy (buffer,"");
	nodeptr = m_nBegin;
	while (nodeptr!=0) {
//		if (nodeptr->check (nodeptr->m_pData)!=delimiter)
		strcat (buffer," ");
		strcat (buffer,nodeptr->m_pData);
		nodeptr = nodeptr->m_pNext;
	}
	created = strlen (buffer);
	ptr = buffer;
	return *m_nBegin;
}

#if 0
node<char*> &node_list<char *>::operator >> (char *(&ptr))
{
	ASSERT(false);
// fixme - handle the case where the ptr is NULL
// or we are at the end of the list
	ptr = m_nPos->m_pData;
	m_nPos = m_nPos->m_pNext;
	return (*m_nPos);
}
#endif

template <class nodeType>
void node_list<nodeType>::append_node (nodeType str)
{
	node<nodeType> *nodeptr;
	char *symbol;
	if (str!=NULL) if (str [0]!=0) {
		symbol = strdup(str);
		nodeptr = new node<nodeType> (symbol);
		concat (nodeptr);
	}
}


//	append a node_list<nodeType> to another node_list<nodeType>
//	object by confiscating the other
//	node_list<nodeType>

template <class nodeType>
void node_list<nodeType>::append_list (node_list<nodeType> *m_nList)
{
//s_node<nodeType,enum language> *position;

	putLastNode (position);
	if ((position!=NULL)&&(m_nList!=NULL)) {
		position->m_pNext = m_nList->m_nBegin;
		m_nEnd = position->m_pNext;
		m_nList->m_nBegin = NULL;
		m_nEnd = m_nList->m_nEnd; }
	else if (m_nList!=NULL) {
		m_nBegin = m_nList->m_nBegin;
		m_nList->m_nBegin = NULL;
		m_nEnd = m_nList->m_nEnd; }
}

//	Search a node_list<nodeType> for a prarticular
//	word and return the s_node<nodeType,enum language> pointer.

template <class nodeType>
s_node<nodeType,enum language> *node_list<nodeType>::search (nodeType keyword)
{
	s_node<nodeType,enum language> *result = 0;
	s_node<nodeType,enum language> *theWord;
	nodeType inText;

	theWord = m_nBegin;
	while (theWord!=0) {	
		inText = theWord->m_pData;
		if (strcmp(keyword,inText)==0) {
			result = theWord;
			break; }
		theWord = theWord->m_pNext; }
	return result;
}

template <class nodeType>
s_node<nodeType,enum language> *node_list<nodeType>::removeWord (s_node<nodeType,enum language> *killer)
{
	s_node<nodeType,enum language> *removed;
	removed = killer->m_pNext;
	killer->m_pNext = removed->m_pNext;
	if (m_nEnd==removed)
		m_nEnd = killer;
	return removed;
}

//	Straightforward routine to convert a node_list of
//	type <char*> back to a null terminated string.

template <class Type>
node<Type> &node_list<Type>::operator>> (Type (&arg))
{
	int created, needed;
	s_node<Type,enum language> *theWord = m_nBegin;
	nodeType buffer = NULL;
	needed = 0;
	while (theWord!=0) {
		int sizeOfThisPiece = strlen (theWord->m_pData)+1;
		needed = needed+sizeOfThisPiece;
		theWord = theWord->m_pNext;
	}
	buffer = new char [needed+1];
	strcpy (buffer,"");
	theWord = m_nBegin;
	while (theWord!=0) {
		if (theWord->check (theWord->m_pData)!=delimiter)
			strcat (buffer," ");
		strcat (buffer,theWord->m_pData);
		theWord = theWord->m_pNext;
	}
	created = strlen (buffer);
	arg = buffer;
	return &(*this);
}

//	transfer a word from the beginning of
//	another phrase to the end of this
//	phrase

template <class nodeType>
void node_list<nodeType>::transferFrom (node_list<nodeType> *text)
{
	node_list<nodeType> *aWord;
	try {
	if (&text==NULL)
		throw text;
	aWord = text->m_nBegin;
	if (aWord!=NULL) {
		text->m_nBegin = aWord->m_pNext;
		if (text->m_nBegin==NULL)
			text->m_nEnd=NULL;
		aWord->m_pNext = NULL;
		ASSERT_FALSE();
		// fixe me!
//		attachWord (aWord);
	}
		}
	catch (node_list<nodeType>) {
	}
}

//	transfer a node from the end of this
//	phrase to the beginning of another phrase

template <class nodeType>
void node_list<nodeType>::transferTo (node_list<nodeType> *text)
{
	node_list<nodeType> *aWord,*m_nPos;
	m_nPos=m_nBegin;
	while (m_nPos!=NULL) {
		aWord=m_nPos;
		if (aWord->m_pNext==NULL)
			m_nPos=NULL;
		else {
			m_nEnd=m_nPos;
			m_nPos=m_nPos->m_pNext; }
	}
	aWord->m_pNext = text->m_nBegin;
	text->m_nBegin=aWord;
}
