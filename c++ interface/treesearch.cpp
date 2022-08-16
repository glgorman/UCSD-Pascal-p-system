
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

#include "../p-system compiler/compilerdata.h"
#include "../p-system compiler/declarationpart.h"
#include "../p-system compiler/bodypart.h"
#include "../p-system compiler/compiler.h"

//#define DEBUG_SEARCH

// DEFINED IN COMPILER.H

#if 0
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
#endif

namespace pascal0
{
key_info operators[] = 
{
	key_info(":=",SYMBOLS::BECOMES,NOOP),
	key_info("(*",SYMBOLS::COMMENTSY,NOOP),
	key_info("*)",SYMBOLS::SEPARATSY,NOOP),
	key_info("(",SYMBOLS::LPARENT,NOOP),
	key_info(")",SYMBOLS::RPARENT,NOOP),
	key_info("{",SYMBOLS::COMMENTSY,NOOP),	
	key_info("}",SYMBOLS::SEPARATSY,NOOP),
	key_info("..",SYMBOLS::COLON,NOOP),
	key_info(".",SYMBOLS::PERIOD,NOOP),
	key_info(";",SYMBOLS::SEMICOLON,NOOP),
	key_info(",",SYMBOLS::COMMA,NOOP),
	key_info(":",SYMBOLS::COLON,NOOP),
	key_info("^",SYMBOLS::ARROW,NOOP),
	key_info("[",SYMBOLS::LBRACK,NOOP),
	key_info("]",SYMBOLS::RBRACK,NOOP),	
	key_info("+",SYMBOLS::ADDOP,PLUS),
	key_info("-",SYMBOLS::ADDOP,MINUS),
	key_info("*",SYMBOLS::MULOP,MUL),
	key_info("/",SYMBOLS::MULOP,RDIV),
	key_info("=",SYMBOLS::RELOP,EQOP),
	key_info("<>",SYMBOLS::RELOP,NEOP),
	key_info(">=",SYMBOLS::RELOP,GEOP),
	key_info("<=",SYMBOLS::RELOP,LEOP),
	key_info(">",SYMBOLS::RELOP,GTOP),	
	key_info("<",SYMBOLS::RELOP,LTOP),
	key_info("\'",SYMBOLS::STRINGCONST,NOOP),
	key_info("",SYMBOLS::OTHERSY,NOOP),
};

key_info key_map[] =
{
	key_info("DO",SYMBOLS::DOSY,NOOP),
	key_info("WITH",SYMBOLS::WITHSY,NOOP),
	key_info("IN",SYMBOLS::SETSY,INOP),
	key_info("TO",SYMBOLS::TOSY,NOOP),
	key_info("GOTO",SYMBOLS::GOTOSY,NOOP),
	key_info("SET",SYMBOLS::SETSY,NOOP),
	key_info("DOWNTO",SYMBOLS::DOWNTOSY,NOOP),
	key_info("LABEL",SYMBOLS::LABELSY,NOOP),
	key_info("PACKED",SYMBOLS::PACKEDSY,NOOP),
	key_info("END",SYMBOLS::ENDSY,NOOP),
	key_info("CONST",SYMBOLS::CONSTSY,NOOP),
	key_info("ARRAY",SYMBOLS::ARRAYSY,NOOP),
	key_info("UNTIL",SYMBOLS::UNTILSY,NOOP),
	key_info("TYPE",SYMBOLS::TYPESY,NOOP),
	key_info("RECORD",SYMBOLS::RECORDSY,NOOP),
	key_info("OF",SYMBOLS::OFSY,NOOP),
	key_info("VAR",SYMBOLS::VARSY,NOOP),
	key_info("FILE",SYMBOLS::FILESY,NOOP),
	key_info("THEN",SYMBOLS::THENSY,NOOP),
	key_info("PROCEDURE",SYMBOLS::PROCSY,NOOP),
	key_info("USES",SYMBOLS::USESSY,NOOP),
	key_info("ELSE",SYMBOLS::ELSESY,NOOP),
	key_info("FUNCTION",SYMBOLS::FUNCSY,NOOP),
	key_info("UNIT",SYMBOLS::UNITSY,NOOP),
	key_info("BEGIN",SYMBOLS::BEGINSY,NOOP),
	key_info("PROGRAM",SYMBOLS::PROGSY,NOOP),
	key_info("INTERFACE",SYMBOLS::INTERSY,NOOP),
	key_info("IF",SYMBOLS::IFSY,NOOP),
	key_info("SEGMENT",SYMBOLS::SEPARATSY,NOOP),
	key_info("IMPLEMENTATION",SYMBOLS::IMPLESY,NOOP),
	key_info("CASE",SYMBOLS::CASESY,NOOP),
	key_info("FORWARD",SYMBOLS::FORWARDSY,NOOP),
	key_info("EXTERNAL",SYMBOLS::EXTERNLSY,NOOP),
	key_info("REPEAT",SYMBOLS::REPEATSY,NOOP),
	key_info("NOT",SYMBOLS::NOTSY,NOOP),
	key_info("OTHERWISE",SYMBOLS::OTHERSY,NOOP),
	key_info("WHILE",SYMBOLS::WHILESY,NOOP),
	key_info("AND",SYMBOLS::RELOP,ANDOP),
	key_info("DIV",SYMBOLS::MULOP,IDIV),
	key_info("MOD",SYMBOLS::MULOP,IMOD),
	key_info("FOR",SYMBOLS::FORSY,NOOP),
	key_info("OR",SYMBOLS::RELOP,OROP),
};
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
	pascal0::key_info *get_key_info (int index);
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

pascal0::key_info *SEARCH::get_key_info (int index)
{
	pascal0::key_info *result;
	ASSERT((index>=0)&&(index<=42));
	result = &pascal0::key_map[index];
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
	pascal0::key_info result;
	pascal0::key_info *kp = pascal0::key_map;
	char c1, buf[32];
	char *str1=str+pos;
	for (len=0;len<32;len++)
	{
		c1=str1[len];
		if (!chartypes::ident.in(c1))
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
			strcpy_s(result.ID,IDENTSIZE,t->ascii);
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
	int test;
	int i,j,k,l;
	char c1, c2;
	i = strlen(str);
	do {
		j = strlen(ptr->NAME);
		if (i<j)
			l=i;
		else
			l=j;
		for (k=0;k<l;k++)
		{
			test = 0;
			c1 = tolower(str[k]);
			c2 = tolower(ptr->NAME[k]);
			if (c1!=c2)
			{
				test = (c1>c2?1:-1);
				break;
			}
		}
	// if the string lengths are equal, we should be done
	// but some of the symbols have trailing spaces??
		if (((i<j)&&(test==0))&&(ptr->NAME[i]!=32))
			test=-1;

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

void PRINTNODE1(const CTP &n1, size_t &N)
{
	CTP node;
	node = n1;

	if (node->LLINK!=NULL)
	{
		PRINTNODE1(node->LLINK,N);			
	}
	else
	{
		WRITE(OUTPUT,"<null>");
	}
	WRITE (OUTPUT,",\"",node->NAME,"\",");
	if (node->RLINK!=NULL)
	{
		PRINTNODE1(node->RLINK,N);	
	}
	else
	{
		WRITE(OUTPUT,"null");
//		N+=3;l
	}
	WRITE(OUTPUT,")");
	N--;
//	++N;
}

void PRINTTREE1(const CTP &n1)
{
	CTP node;
	size_t lex = 0;
	node = n1;
	if (node==NULL)
	{
	WRITELN(OUTPUT,"TREE = NULL");
	return;
	}
	WRITELN(OUTPUT,"TREE: IDTYPE = ",(int)n1->IDTYPE);
	if (node->LLINK!=NULL)
	{
		PRINTNODE1(node->LLINK,lex);	
	}
	else
		WRITE(OUTPUT,"null");

	WRITE (OUTPUT,",\"",node->NAME,"\",");
	if (node->RLINK!=NULL)
	{
		PRINTNODE1(node->RLINK,lex);	
	}
	else
		WRITE(OUTPUT,"nulll");
	WRITELN(OUTPUT);
}
