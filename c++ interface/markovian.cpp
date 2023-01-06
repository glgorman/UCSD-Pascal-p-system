
#include "stdafx.h"
#include "defines.h"
#include <iostream>
#include <fstream>
#include "symbol_table.h"
#include "btreetype.h"
#include "node_list.h"
#include "text_object.h"
#include "extras.h"

using namespace std;

extern fstream debugOut, preIndex;

//	Initial constructor for a markov snippet in the form
//	of a symbol_table.  Takes a fixed number of words from a
//	textImage and creates the initial snippet.

//	mStream objects are currently derived from text_objects
//	maybe this should be changed since there is a definite
//	goal to genaralize this code to other types of data
//	structures, i.e. probably should be using the node_list
//	class here.

mStream::mStream (int length1, text_object &theImage)
{		
	char *buffer;
	int length = length1, i = 0;
	theImage.rewind ();
	while ((theImage.m_bEnd==false)&&(i<length)) {
		theImage.get (buffer);
		append (buffer);
		i++; }
}

//	Link a node to another node, i.e., so that that this
//	node becomes the root of that node, i.e., in a
//	markov tree.

template<class X>
void bTreeType<X>::link_node (X &str)
{
	bTreeType<X> *theMarkov;
	theMarkov = new (NULL) bTreeType<X> ();
	theMarkov->m_pData = str;
	markov = theMarkov;
	theMarkov->root = this;
}

//	this function actually "uploads" the current
//	mStream into the markov tree.  The stream is
//	also echoed to the preIndex log textt object.
//. Links in a markov snippet under the
//	designated "penultimate" token.

void mStream::indexWordList (bTreeType<char*> *theTree)
{
	int depth=0;
	char *thisWord, *nextWord;
	bTreeType<char*> *theMarkov;
	bTreeType<char*> *theBranch = theTree;

	findPenultimate (thisWord);
	theBranch = theBranch->find_node (thisWord);
	rewind ();
	while (m_bEnd==false) {
		get (nextWord);
		if (theBranch->markov==NULL) {	
			theBranch->link_node (nextWord);
#if 0 //FIXME!
			theBranch = reinterpret_cast<bTreeType<char*> >(theBranch->markov);
#endif
		}
		else {
#if 0
			theMarkov = theBranch->markov;
			theBranch = theMarkov->getNode (nextWord);
#endif
		}
	}
	char *test;
	((text_object)*this) >> test;
	preIndex << "[" << test << "]\n";
	delete [] test;
}

void bTreeType<char*>::indexMetasToHTML ()
{
	bool busy = false;
	while (busy==true)
	{
//		getMarkovian ();
	}
}
/*
void bTreeType<char*>::indexMarkovians1 ()
{
	DEBUG_STR << "\nIterative Markovian Test:\n";

	bTreeType<char*> *prevNode [order+1];
	bTreeType<char*> *next [order+1];
	bTreeType<char*> *position [order+1];

	int n = 0;
	position [0] = findNode1 ();
	prevNode [0] = this;

	while (position [0]!=NULL)
	{	
//	when a tree has been fully explored, back up
//	to the appropriate previous level and continue
//	exploring
		
		if (position [n]==NULL)
		{
			while (position [n]==NULL)
			n--;
		}
		else

//	find the first unexplored markov branch
//	along this path then enter that branch and
//	then prepare to iterate through that tree
	
		while (position [n]->markov!=NULL)
		{
			n++;
			prevNode [n] = position [n-1]->markov;
			position [n] = prevNode [n]->findNode1 ();
			position [n+1] = NULL;
		}

//	iterate through symbol_table from this
//	position

		while (position [n]->markov==NULL)
		{
			next [n] = position [n]->m_pNext (prevNode [n]);
			prevNode [n] = position [n];
			position [n] = next [n];
			position [n+1] = NULL;

			spewMarkovian (position);
		}
	}
}
*/