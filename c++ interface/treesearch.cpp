
#include "stdafx.h"
#include "afxmt.h"
#include <iostream>
#include "../Frame Lisp/defines.h"
#include "../Frame Lisp/symbol_table.h"
#include "../Frame Lisp/btreetype.h"
#include "../Frame Lisp/node_list.h"
#include "../Frame Lisp/text_object.h"
#include "../Frame Lisp/scripts.h"
#include "../Frame Lisp/frames.h"
#include "../Frame Lisp/frames1.h"
#include "../Frame Lisp/extras.h"
#include "compiler.h"

namespace SEARCH
{
	char *keywords[] = 
	{
		"DO","WITH","IN","TO","GOTO","SET","DOWNTO","LABEL",
		"PACKED","END","CONST","ARRAY","UNTIL","TYPE","RECORD",
		"OF","VAR","FILE","THEN","PROCSY","PROCEDURE","USES",
		"ELSE","FUNCTION","UNIT","BEGIN","PROGRAM","INTERFACE",
		"IF","SEGMENT","IMPLEMENTATION","CASE","FORWARD","EXTERNAL",
		"REPEAT","NOT","OTHERWISE","WHILE","AND","DIV","MOD",
		"FOR","OR",
	};
	frame m_pFrame;
	symbol_table *m_keywords;
	void RESET_SYMBOLS();
	void IDSEARCH(int &pos, char *&str);
};

void SEARCH::RESET_SYMBOLS()
{
	frame &f = SEARCH::m_pFrame;
	symbol_table *t=NULL;
	t = f.cons(keywords)->sort();
 	m_keywords = t;
}

void SEARCH::IDSEARCH(int &pos, char *&str)
{
	char buf[32];
	SEARCH::RESET_SYMBOLS();
	symbol_table &T = *SEARCH::m_keywords;
	size_t i, len, sz;
	sz = T.size();
	token *t;
	pos = -1;
	for (i=0;i<sz;i++)
	{
		t = &T[(int)i];
		len = strlen(t->ascii);
		memcpy(buf,str,len);
		buf[len]=0;
		if (strcmp(buf,t->ascii)==0)
		{
			pos=(int)i;
//			PASCALCOMPILER::SY;
//			PASCALCOMPILER::OP;
//			PASCALCOMPILER::ID;
			break;
		}
	}
}

int TREESEARCH(const CTP& n1, CTP& n2, ALPHA &str)
{
#if 0
	WRITE(OUTPUT,"TREESEARCH(CTP& n1, CTP& n2,\"",(char*)str,"\")");
#endif
	CTP ptr = n1;
	bool found = false;
	bool quit = false;
	int test = 0; 
	do {
		test = strcmp (str,ptr->NAME);
		if ((test<0)&&(ptr->LLINK!=NULL))
			ptr = ptr->LLINK;
		else if ((test>0)&&(ptr->RLINK!=NULL))
			ptr = ptr->RLINK;
		else if (test==0)
		{
			found=true;
			quit = true;
		}
		else if ((test<0)&&(ptr->LLINK==NULL))
			quit = true;
		else if ((test>0)&&(ptr->RLINK==NULL))
			quit = true;
	}
	while (quit==false);
	n2 = ptr;
#if 0
	WRITELN (OUTPUT," returning ",test);
#endif
	return test;
}

void ADJUST_LINE(size_t &I)
{
	if (I>120){
//		WRITE(OUTPUT,"[",(int)I,"]");
		WRITELN(OUTPUT);
		WRITE(OUTPUT,"\t");
		I=0;
	}
}

void INDENT_NEWLINE (int i)
{
	int j;
	WRITELN(OUTPUT);
	for (j=0;j<i;j++)
	  WRITE(OUTPUT,"--");
}

void PRINTNODE(const CTP &n1, size_t &N)
{
	CTP node;
	node = n1;
//	ADJUST_LINE(N);
	INDENT_NEWLINE((int)N);
	N++;
	WRITE(OUTPUT,"NODE(");
//	N+=5;
	if (node->LLINK!=NULL)
	{
		PRINTNODE(node->LLINK,N);			
	}
	else
	{
//		N += 3;
		WRITE(OUTPUT,"nil");
	}
	WRITE (OUTPUT,",\"",node->NAME,"\",");
//	N+= (strlen(node->NAME)+4);
	if (node->RLINK!=NULL)
	{
		PRINTNODE(node->RLINK,N);	
	}
	else
	{
		WRITE(OUTPUT,"nil");
//		N+=3;
	}
	WRITE(OUTPUT,")");
	N--;
//	++N;
}

void PRINTTREE(const CTP &n1)
{
	CTP node;
//	size_t J  = 5;
	size_t lex = 0;
	node = n1;
	if (node==NULL)
	{
	WRITELN(OUTPUT,"TREE = NULL");
	return;
	}
	WRITE(OUTPUT,"TREE(");
	if (node->LLINK!=NULL)
	{
		PRINTNODE(node->LLINK,lex);	
	}
	else
		WRITE(OUTPUT,"nil");
	WRITE (OUTPUT,",\"",node->NAME,"\",");
	if (node->RLINK!=NULL)
	{
		PRINTNODE(node->RLINK,lex);	
	}
	else
		WRITE(OUTPUT,"nil");
	WRITELN(OUTPUT,")");
}
