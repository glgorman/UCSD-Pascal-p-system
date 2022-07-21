
#include "stdafx.h"
#include "afxmt.h"
#include <iostream>
#include "defines.h"
#include "symbol_table.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "scripts.h"
#include "frames.h"
#include "frames1.h"
#include "extras.h"

using namespace std;

bTreeType<char*> *m_root;

frame theFrame;
frame *lframe::m_pFrame = NULL;

void lframe::set_global ()
{
	m_pFrame = &theFrame;
}

void lframe::load_binary_file (char *fname)
{
	CString msg;
	msg.Format(_T("The Unicorn is in the Garden!%s"),fname);
	AfxMessageBox(msg,IDOK);
}

#define threadRipText

//	Simple way to create a b-tree. Take the words in order of
//	most-frequent to least frequent	usage and sort them by
//	frequency of use and then alphabetic position into a tree.
//	Spools pointers from a sorted array of to the get node
//	function.  If getNode returns a different pointer -- then
//	it means that the node already exists in the tree, so the
//	pointer in the list that we are converting is redirected
//	to the duplicate object and the orininal is destroyed.
#if 0
int compare (char *a, char *b)
{
	return strcmp (a,b);
}
#endif

template<class nodeType>
bTreeType<nodeType>::bTreeType  (script *theScript)
{
	theScript->m_index.owner = false;
#if 0
#ifdef debug
	strcpy (m_tagid,"bTreeRoot");
#endif
#endif

	m_pData = NULL;	
	bTreeType<nodeType> *theNode;
	int wordNum, needed = theScript->m_index.size();
	char *aNode;
	root = markov = branch1 = branch2 = NULL;
	if (theScript->m_index._symbols.size()==0)
		return;

	aNode = theScript->m_index._symbols [0].ascii;
	if (aNode!=NULL) {
		m_pData = aNode;
//		DEBUG_STR << "adding node \"" << aNode << "\"\n";
	}
	for (wordNum=1;wordNum<needed;wordNum++) {
		aNode = theScript->m_index._symbols[wordNum].ascii;
#ifdef trace_additions
		DEBUG_STR << "adding node \"" << aNode << "\"\n";
#endif
		theNode = getNode (aNode);
		if (aNode!=theNode->m_pData) {
			delete aNode;
			theScript->m_index._symbols[wordNum].ascii = theNode->m_pData; }
	}
//	Sanity test on tree to see if every token was added!
	for (wordNum=1;wordNum<needed;wordNum++) {
		aNode = theScript->m_index._symbols[wordNum].ascii;
		theNode = findNode (aNode);
		if (compare(aNode,theNode->m_pData)!=0)
		{
#ifdef HAS_CONSOLE
			DEBUG_STR << "Could not find: " << aNode << "\n";
#endif
		}
	}
}

//	-------------------------------------------------------
//
//	Methods for creation and major operations with frames
//
//	--------------------------------------------------------

frame::frame ()
{
	::m_root = NULL;
	m_root = NULL;
//	scripts = new script ("scripts");
//	common = new script ("common");
	content.clear ();// = NULL;
	available = 0;
	count = 0;
	button = false;
}

frame::~frame ()
{
	int j = 0;
	delete m_root;
	m_root = NULL;
//	if (context!=NULL)	
#if 0
	if (content.size()>0) {std::
		iter = content.begin ();
		end = content.end ();
		for (;iter<end;iter++)
			delete (*iter);
	}
	delete scripts;
	delete common;
#endif
//	delete [] context;
}

void frame::load_file_images (bool trace)
{
	char theFile[64];
	std::vector<token>::iterator iter, end;
	char suffix [] = ".txt";
	UINT fail;
	int howmany = scripts.m_index.size();
	iter = scripts.m_index._symbols.begin();
	end = scripts.m_index._symbols.end ();
	int j=0;
	for (iter++;iter<end;iter++,j++)
	{
		strcpy (theFile, scripts.m_index._symbols [j].ascii);
		fail = load_file_image (j,theFile,suffix);
	}
}

UINT load_file_image (LPVOID param)
{
	int wordCount;
	script *theScript = (script*)(param);
//	char *fileName = theScript->m_file_name;
	while (theScript->merged==false)
		Sleep (100);
//	waitForConsole;
#ifdef HAS_CONSOLE
	DEBUG_STR << "Loading Image: " << theScript->get_file_name() << "\n";
#endif
//	releaseConsole;
	theScript->load_file_image (NULL);
	theScript->theImage.count_words (wordCount);
	theScript->close_output_file ();
//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Read " << (int) wordCount;
	DEBUG_STR << " words from " << theScript->get_file_name() << "\n\n";
#endif
//	releaseConsole
	return wordCount;
}

void frame::build_event_list ()
{
	int howMany;
	lyric_element ly;
	bool result;

	result = scripts.open_file ();
	if (result==true) {
//		scripts.construct_symbol_table (true);
		scripts.load_file_image (NULL);
		scripts.close_input_file ();
//		scripts->killDelimiters ();
		howMany = scripts.m_index.size();
//		scripts->viewIndex (howMany);
//		scripts.save_index ();
		text_object *image = &scripts.theImage;
		
		m_events.m_text.clear();
		m_events.m_timestamp.clear();
		
		int i=0;
		bool busy = true;
		UINT ts;
		while (busy)
		{
			ts = i*100;
			ly.timestamp  = ts;
			ly.phrase = "Hello World";

			m_events.m_text.push_back (ly.phrase);
			m_events.m_timestamp.push_back (ts);
			i++;
			
			if (i>100)
				break;
		}
	}
}


UINT frame::load_file_image (int j, char *fileName, char *suffix)
{
	bool endOfFile = false;
	int wordCount = 0;
	script *theScript = content [j];
	bool fileOpen = theScript->open_file ();
	if (fileOpen==true)
	{
		LPVOID param = (void*)(theScript);
#ifdef threadImageLoads
		AfxBeginThread (::loadFileImage,param,1);
#else
		::load_file_image (param);
#endif		
	}
	else {
		wordCount = -1;
//		waitForConsole
#ifdef HAS_CONSOLE
		DEBUG_STR << "Failed to open: " << fileName << "\n";
#endif
//		releaseConsole
	}
	return wordCount;
}

text_object &frame::operator [] (int contextNum)
{
	script *the_script;
	text_object *theImage;
	if ((contextNum<=count)&&(contextNum>=0)) {
		the_script = content [contextNum];
		theImage = &the_script->theImage;
	}
	else theImage = NULL;
	return *theImage;
}

void frame::buildIndices (int theOrder)
{
	bool prompt = false;
	load_make_file (m_str_make,true);
	build_symbol_tables (false);
	masterIndex ("common");
}

symbol_table *frame::cons (char **tokens)
{
	symbol_table *s = new symbol_table;
	int i = 0;
	s->m_allocate = false;
	s->_symbols.reserve (40);
	while (tokens[i]!=NULL)
	{
		s->index_word(tokens[i],i);
		i++;
	}
	return s;
}

bTreeType<char*> *frame::make_tree ()
{
	bTreeType<char*> *p_tree;
	common.m_index.owner = false;
	p_tree = new (NULL) bTreeType<char*> (&common);
	return p_tree;
}

bool frame::load_make_file (char *str_file, bool prompt)
{
	int howMany;
	bool result;
	scripts.set_file_name (str_file);
	result = scripts.open_file ();
	if (result==true) {
		scripts.construct_symbol_table (true);
		scripts.close_input_file ();
//		scripts->killDelimiters ();
		howMany = scripts.m_index.size();
//		scripts->viewIndex (howMany);
//		scripts.save_index ();
	}
	return result;
}

bool frame::open_binary_file (char *str_file)
{
	bool valid;
	int result;
//	ASSERT ((ifstream*)m_fileName!=NULL);
#ifdef HAS_CONSOLE
	DEBUG_STR << "Opening: " << str_file << "\n";
#endif
	m_iFile = new std::fstream;
	ASSERT(m_iFile!=NULL);
	m_iFile->clear ();
	m_iFile->open (str_file,ios::in|ios::binary);
	result = m_iFile->is_open ();
	if (result!=0)
		valid = true;
	else {
		valid = false;
#ifdef HAS_CONSOLE
		DEBUG_STR << "Error opening file: " << str_file << "\n";
#endif
	}
//	releaseConsole
	bool eof = false;
	int pos = m_iFile->tellg ();
	return valid;	
}

void frame::close_input_file ()
{
//	waitForConsole
#ifdef HAS_CONSOLE
	DEBUG_STR << "Closing: " << "FILE" << "\n";
#endif
//	releaseConsole
	m_iFile->close ();
}

bool frame::load_binary_file (char *str_file)
{
	bool status;
	status = open_binary_file(str_file);
	if (status==true)
	{
//		m_ifile.read()
	}
	else
		return false;
	close_input_file();
	return true;
}

UINT indexFile (LPVOID param)
{
	bool fileCheck;
	script *aContext = (script*)(param);
	char *f = aContext->get_file_name();
	fileCheck = aContext->open_file ();
	if (fileCheck==true) {
		aContext->construct_symbol_table (false);
		aContext->close_input_file ();
		aContext->indexed = true;
		aContext->save_index ();
	}
	return 0;
}

//	Since each script possesses a token index
//	as well as a linked list of token objects, it
//	is nice to create an internal script for use
//	during operations.

void frame::build_symbol_tables (bool trace)
{
	int j=0;
	char *str_file;
	vector<script*>::iterator iter, end;
	int howMany = scripts.m_index.size();
	content.resize (howMany);
	iter = content.begin ();
	end = content.end ();
	for (;iter<end;iter++)
	{
		str_file = scripts.m_index._symbols [j].ascii;
		(*iter) = new script ();
		(*iter)->set_file_name (str_file);
		LPVOID param = (LPVOID)((*iter));
#ifdef THREAD_INDEX_LOADS
		AfxBeginThread (::indexFile,param,1);
#else
		if (strlen(str_file)>0)
			::indexFile (param);
#endif
		j++;
	}
}

script *frame::makeScript (char *thefile)
{
	script *aContext;
	aContext = new script (thefile);
	count++;
	aContext->open_file ();
	return aContext;
}

void frame::mergeFile (int j)
{
	char *buffer;
	script *theScript = content [j];
	int wordCount = theScript->m_index.size();
	char *theFileName = theScript->get_file_name();
	while (theScript->indexed==false)
		Sleep (100);

	critical.Lock ();
//	waitForConsole
#if 0
	DEBUG_STR << "Merging Index File: " << theFileName << "\n";
#endif
//	releaseConsole;
	theScript->m_index.owner = false;
	for (int k=0;k<wordCount;k++)
	{ 
		buffer = theScript->m_index._symbols[k].ascii;
		int times = theScript->m_index._symbols[k].m_count;
		common.m_index.index_word (buffer,times);
		delete buffer;
		theScript->m_index._symbols[k].ascii = NULL;
	}
	theScript->merged=true;
	critical.Unlock ();
}

UINT mergeIndex (LPVOID param)
{
	int fileNum = (int)(param);
	theFrame.mergeFile (fileNum);
	return 0;
}

void frame::masterIndex (char *mName)
{
	script *theScript;
	long j,total;
	total = scripts.m_index.size();
	LPVOID param;
	/* The indexes are retained in memory, so make a master index */
	for (j=1;j<=total;j++) {
		theScript = content [j];
		if (theScript->valid==false)
			continue;
		param = (LPVOID)(j);
#ifdef threadMasterIndex
		AfxBeginThread (mergeIndex,param,1);
#else
		mergeIndex (param);
		theScript->purgeIndex ();
#endif
	}
	common.save_index ();
}


UINT saveImageFile (LPVOID param)
{
	script *theScript = (script*)(param);
	char *theFile = theScript->get_file_name();
	while (theScript->imageLoaded==false)
		Sleep (100);
	theScript->saveImageFile ();
	theScript->theImage.rewind ();
	return 0;
}

void frame::write_file_images (bool trace)
{
	int j;
	script *theScript;
	LPVOID param;
	int howmany = scripts.m_index.size();
	for (j=0;j<howmany;j++)
	{
		theScript = content [j];
		if (theScript->valid==false)
			continue;
		param = (LPVOID)(theScript);
#ifdef threadImageWrites
		AfxBeginThread (saveImageFile,param,1);
#else
		saveImageFile (param);
#endif
	}
#ifdef HAS_CONSOLE
	DEBUG_STR << "\n";
#endif
}

void frame::writeKeyFiles (bool trace)
{
	char theFile[256];
	vector<script*>::iterator iter,end;
	iter = content.begin ();
	end = content.end ();
	int j=0;
	for (;iter<end;iter++,j++)
	{
		strcpy (theFile,scripts.m_index._symbols[j].ascii);
		(*iter)->saveKeyFile ();
		(*iter)->theImage.rewind ();

#ifdef verbose
		DEBUG_STR << m_pText;
#endif
	}
#ifdef threaded
#ifdef HAS_CONSOLE
	DEBUG_STR << "\n";
#endif
#endif
}

//	a text object called "m_echo" is included since there is
//	no other provided method for chomping off the words in a
//	text object!  It's also a great way to help debug -- since
//	the echo image can be dumped to a file to verify that the
//	excerpts were all gone through correctly!

void frame::buildMarkov (text_object &theImage, bTreeType<char*> *theTree)
{
	char *buffer;
	language m_typeid;
	critical.Lock ();
	mStream theExcerpt (order,theImage);
	text_object m_echo;
	theImage.rewind ();
	while (theImage.m_bEnd!=true) {
		theExcerpt.indexWordList (theTree);
#ifdef keysOnly
		do
			buffer = theImage.get (m_typeid);
		while	((m_typeid!=unknown)&&(theImage.m_bEnd!=true));
#else
		buffer = theImage.get (m_typeid);
#endif
		if (buffer!=NULL) {
			theExcerpt.append (buffer);
			m_echo.transferFrom (theExcerpt); }
	}
	critical.Unlock ();
}

//	Iterate through all scripts and add to the
//	markov tree the nodes for each script

void frame::buildMarkovians (bTreeType<char*> *theTree)
{	
	vector<script>::iterator iter,end;
	text_object theImage;
	if (theTree!=NULL)
	for (;iter<end;iter++)
	{
//		waitForConsole
#ifdef HAS_CONSOLE
		DEBUG_STR << "Markov test for: "<< (*iter).get_file_name() << "\n";
#endif
//		releaseConsole;
		theImage = (*iter).theImage;
		buildMarkov (theImage, theTree);
		theImage.rewind ();
	}
	else
	{
#ifdef HAS_CONSOLE
	DEBUG_STR << "Null bTree: Unable to construct Markovains!\n";
	DEBUG_STR << "\n";
#endif
	}
}

//	Search every fileImage for a sentence
//	containing a keyWord; then append the
//	following seintence to the list.

class extractorTag
{
public:
	int count;
	extractorTag ();
	script *Script;
	key_list *keyWords;
	text_object *result;
};

extractorTag::extractorTag ()
{
	Script = NULL;
	result = NULL;
}

UINT ripText (LPVOID param)
{
	bool foundOne;
	text_object found;
//	s_node<char*,language> *marker;
	extractorTag *the = (extractorTag*)(param);
	script *theScript = the->Script;
	text_object theImage = theScript->theImage;
	foundOne = theImage.find_keyword ((*the->keyWords));
	if (foundOne==false) {
#if 0
		marker = theImage.m_nList;
		theImage.rewind ();
		foundOne = theImage.findKeyWord ((*the->keyWords));
		if (foundOne==false)
			theImage.m_nList.m_nBegin = marker;
#endif
	}
	if (foundOne==true) {
		found = theImage.getSentence ();
		the->result->append (found);
	}
	delete the;
	return foundOne;
}

#if 0
text_object frame::ripText (m_bEnd &keyWords)
{
	vector<script>::iterator iter,end;
	iter = content.begin ();
	end = content.end ();
	int n;
	text_object *result = new text_object;
	for (n=0;iter<end;iter++,n++)
	{	
		if ((*iter).valid==false)
			continue;
		if ((*iter).imageLoaded==true) {
			extractorTag *Tag = new extractorTag;
			Tag->keyWords = keyWords;
			Tag->result = result;
			Tag->count = n;	
//			Tag->Script = &(*iter);
#ifdef threadRipText
			AfxBeginThread (::ripText,(LPVOID)(Tag),1);
#else
			::ripText ((LPVOID)(Tag));
#endif
		}
	}
	Sleep (1000);
	return *result;
}
#endif

