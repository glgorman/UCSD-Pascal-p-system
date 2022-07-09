
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

//#define DEBUG_SEARCH

// DEFINED IN COMPILER.H
/*
typedef enum _SYMBOL
{
	IDENT,COMMA,COLON,SEMICOLON,LPARENT,RPARENT,DOSY,TOSY,
	DOWNTOSY,ENDSY,UNTILSY,OFSY,THENSY,ELSESY,BECOMES,LBRACK,
	RBRACK,ARROW,PERIOD,BEGINSY,IFSY,CASESY,REPEATSY,WHILESY,
	FORSY,WITHSY,GOTOSY,LABELSY,CONSTSY,TYPESY,VARSY,PROCSY,
	FUNCSY,PROGSY,FORWARDSY,INTCONST,REALCONST,STRINGCONST,
	NOTSY,MULOP,ADDOP,RELOP,SETSY,PACKEDSY,ARRAYSY,RECORDSY,
	FILESY,OTHERSY,LONGCONST,USESSY,UNITSY,INTERSY,IMPLESY,
	EXTERNLSY,SEPARATSY,MAXSYMBOL
}	SYMBOL;

typedef enum _OPERATOR
{
	MUL,RDIV,ANDOP,IDIV,IMOD,PLUS,MINUS,OROP,LTOP,LEOP,
	GEOP,GTOP,NEOP,EQOP,INOP,NOOP,MAXOPERATOR,
} OPERATOR;	
*/
/*
struct key_info
{
	ALPHA		ID;
	SYMBOL		SY;
	OPERATOR	OP;
	key_info() { };
	key_info(char *STR, SYMBOL _SY, OPERATOR _OP)
	{
		strcpy_s(ID,16,STR);
		SY = _SY;
		OP = _OP;
	}
};
*/

key_info key_map[] =
{
	key_info("DO",DOSY,NOOP),
	key_info("WITH",WITHSY,NOOP),
	key_info("IN",SETSY,INOP),
	key_info("TO",TOSY,NOOP),
	key_info("GOTO",GOTOSY,NOOP),
	key_info("SET",SETSY,NOOP),
	key_info("DOWNTO",DOWNTOSY,NOOP),
	key_info("LABEL",LABELSY,NOOP),
	key_info("PACKED",PACKEDSY,NOOP),
	key_info("END",ENDSY,NOOP),
	key_info("CONST",CONSTSY,NOOP),
	key_info("ARRAY",ARRAYSY,NOOP),
	key_info("UNTIL",UNTILSY,NOOP),
	key_info("TYPE",TYPESY,NOOP),
	key_info("RECORD",RECORDSY,NOOP),
	key_info("OF",OFSY,NOOP),
	key_info("VAR",VARSY,NOOP),
	key_info("FILE",FILESY,NOOP),
	key_info("THEN",THENSY,NOOP),
	key_info("PROCEDURE",PROCSY,NOOP),
	key_info("USES",USESSY,NOOP),
	key_info("ELSE",ELSESY,NOOP),
	key_info("FUNCTION",FUNCSY,NOOP),
	key_info("UNIT",UNITSY,NOOP),
	key_info("BEGIN",BEGINSY,NOOP),
	key_info("PROGRAM",PROGSY,NOOP),
	key_info("INTERFACE",INTERSY,NOOP),
	key_info("IF",IFSY,NOOP),
	key_info("SEGMENT",SEPARATSY,NOOP),
	key_info("IMPLEMENTATION",IMPLESY,NOOP),
	key_info("CASE",CASESY,NOOP),
	key_info("FORWARD",FORWARDSY,NOOP),
	key_info("EXTERNAL",EXTERNLSY,NOOP),
	key_info("REPEAT",REPEATSY,NOOP),
	key_info("NOT",NOTSY,NOOP),
	key_info("OTHERWISE",OTHERSY,NOOP),
	key_info("WHILE",WHILESY,NOOP),
	key_info("AND",RELOP,ANDOP),
	key_info("DIV",MULOP,IDIV),
	key_info("MOD",MULOP,IMOD),
	key_info("FOR",FORSY,NOOP),
	key_info("OR",RELOP,OROP),
};

namespace SEARCH
{
	char *operators[] = 
	{
		"~","!","%","&","&&","|","||","*","-","+","/",
		"<>",">=","<=","==",
	};
	char *keywords[] = 
	{
		"DO","WITH","IN","TO","GOTO","SET","DOWNTO","LABEL",
		"PACKED","END","CONST","ARRAY","UNTIL","TYPE","RECORD",
		"OF","VAR","FILE","THEN","PROCEDURE","USES",
		"ELSE","FUNCTION","UNIT","BEGIN","PROGRAM","INTERFACE",
		"IF","SEGMENT","IMPLEMENTATION","CASE","FORWARD","EXTERNAL",
		"REPEAT","NOT","OTHERWISE","WHILE","AND","DIV","MOD",
		"FOR","OR",NULL
	};
	frame m_pFrame;
	symbol_table *m_keywords;
	key_info *get_key_info (int index);
	void RESET_SYMBOLS();
	int IDSEARCH(int pos, char *&str);
//	SYMBOL SY(token *t);
};

void SEARCH::RESET_SYMBOLS()
{
	frame &f = SEARCH::m_pFrame;
	symbol_table *t=NULL;
	t = f.cons(keywords)->sort();
 	m_keywords = t;
}

key_info *SEARCH::get_key_info (int index)
{
	key_info *result;
	ASSERT((index>=0)&&(index<=42));

	result = &key_map[index];
	return result;
}

int SEARCH::IDSEARCH(int pos, char *&str)
{
#ifdef DEBUG_SEARCH
	WRITE(OUTPUT,"\nSEARCH::IDSEARCH() ");
	SEARCH::RESET_SYMBOLS();
#endif

	size_t i, len, sz;
	bool found = false;
	int syid = -1;
	key_info result;
	key_info *kp = key_map;
	char c1, buf[32];
	char *str1=str+pos;
	for (len=0;len<32;len++)
	{
		c1=str1[len];
		if (!chartype::symbol.in(c1))
			break;
	}
	memcpy(buf,str1,len);
	buf[len]=0;

#ifdef DEBUG_SEARCH
	WRITELN (OUTPUT,"searching for \"",buf,"\"");
#endif

	symbol_table &T = *SEARCH::m_keywords;
	sz = T.size();
	token *t;
	for (i=0;i<sz;i++)
	{
		t = &T[(int)i];
		len = strlen(t->ascii);
		if (strcmp(buf,t->ascii)==0)
		{
			strcpy_s(result.ID,16,t->ascii);
			found = true;
			syid = t->m_index;
			break;
		}
	}
#ifdef DEBUG_SEARCH
	if (found==true)
		WRITELN(OUTPUT,"FOUND \"",result.ID,"\"\n");
	else
		WRITELN(OUTPUT,"NOT FOUND \"",buf,"\"\n");
#endif
	return syid;
}

int TREESEARCH(const CTP& n1, CTP& n2, ALPHA &str)
{
#ifdef DEBUG_TREESEARCH
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
//	INDENT_NEWLINE((int)N);
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
