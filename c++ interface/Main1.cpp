
#include "stdafx.h"
#include "defines.h"
#include <fstream>
#include "symbol_table.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "extras.h"
#include "scripts.h"
#include "frames1.h"
#include "debug.h"

using namespace std;

extern bTreeType<char*> *m_root;

#define loadMarkov
fstream preIndex, postIndoex,debugOut;
extern frame theFrame;

extern void punctuate (char *);
extern char *long2Text (long);
extern void echoList (m_bEnd &m_nList);

debugTest<char*> debugTrace;


template<class X>
int bTreeType<X>::indexMarkovians (X &hansen)
{	
	bTreeType *buggy;
	char gretal [512];
	
	if (branch1!=NULL)
		branch1->indexMarkovians (hansen);

	if (markov!=NULL) {
		strcpy (gretal,hansen);
		strcat (gretal," ");
		strcat (gretal,m_pData);
		buggy = markov;
		buggy->indexMarkovians (gretal);
		if (markov->markov==NULL) {
//			debugOut << "[" << gretal << " ";
//			debugOut << markov->m_pData << "]\n";
		}
	}	
	if (branch2!=NULL)
		branch2->indexMarkovians (hansen);
	return 0;
}

bool frame::load_project ()
{
//	debugTrace.openDebugFiles ();
//	AllocConsole ();

	theFrame.load_make_file ("plan.txt",true);
	theFrame.build_symbol_tables (1);
//	m_root = theFrame.make_tree ();
	::m_root = m_root;
	DEBUG_STR << "\n";

//	theFrame.load_file_images (true);
//	theFrame.write_file_images (true);
	DEBUG_STR.flush ();
//	theFrame.writeKeyFiles (true);
//	theFrame.buildMarkovians (bTree);
//	if (bTree!=NULL)
//		bTree->indexMarkovians ("");
//	debugTrace.closeDebugFiles ();
//	debugTrace.traversalTest (theRoot);
	DEBUG_STR.flush ();
//	FreeConsole ();
	return true;
}

char *numText2 [] =
{
	"zero","one","two","three","four","five",
	"six","seven","eight","nine","ten","eleven",
	"twelve","thirteen","fourteen","fifteen",
	"sixteen","seventeen","eighteen","nineteen",
	"twenty","plus","minus","times","divided",
};

char *numText3 [] =
{
	"0","1","2","3","4","5",
	"6","7","8","9","10","11",
	"12","13","14","15",
	"16","17","18","19"
	"20","+","-","*","/"
};

bool convertNums (s_node<char*,language> *theNode)
{
	bool result = false;
	int j;
	char *ascii,*target,*replace;
	for (j=0;j<25;j++) {
		ascii = theNode->m_pData;
		target = numText2 [j];
		replace = numText3 [j];
		if (strcmp (target,ascii)==0) {
			theNode->m_pData = replace;
			result = true;
			break; } }
	return result;
}

void convertText2Num (text_object &theObject)
{
#if 0
	char *ascii;
	s_node<char*,language> *theNode;
	theObject.rewind ();
	while (theObject.m_bEnd==false) {
		theNode = theObject.m_nPos;
		convertNums (theNode);
		theObject.get (ascii);
	}
#endif
};

bool tellTime (text_object &theInput)
{
	int blah = 0;
	char *ascii;
	bool tell,what,time,is,it;
	bool result = false;
	theInput.rewind ();
	while (theInput.m_bEnd==false) {
		theInput.get (ascii);
		if (strcmp (ascii,"time")==0)
			time = true;
		else if (strcmp (ascii,"what")==0) {
			what = true;
			blah++; }
		else if (strcmp (ascii,"tell")==0) {
			tell = true;
			blah++; }
		else if (strcmp (ascii,"is")==0) {
			is = true;
			blah++; }
		else  if (strcmp (ascii,"it")==0) {
			it = true; blah++; } }
	if ((time==true)&&(blah>2))
		result=true;
	return result;
}



void message (char *caption,text_object &text)
{
	char *ascii;
	text.rewind ();
	DEBUG_STR << caption;
	while (text.m_sList.m_pNext!=NULL) {
		text.get (ascii);
		DEBUG_STR << ascii << " "; }
	DEBUG_STR << "\n";
	DEBUG_STR.flush (); 
	text.rewind ();
}

