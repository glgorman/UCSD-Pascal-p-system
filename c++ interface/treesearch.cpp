
#include "stdafx.h"
#include "afxmt.h"
#include <iostream>
#include "../Frame Lisp/framelisp.h"
#include "../p-system compiler/insymbol.h"
#include "../p-system compiler/compilerdata.h"
#include "../p-system compiler/declarationpart.h"
#include "../p-system compiler/bodypart.h"
#include "../p-system compiler/compiler.h"

// #define DEBUG_SEARCH

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

key_info keywords [] =
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
	result = &pascal0::keywords [index];
	return result;
}

int SEARCH::IDSEARCH(int pos, char *&str)
{
//	SEARCH::RESET_SYMBOLS();
#ifdef DEBUG_SEARCH
	WRITE(OUTPUT,"\nSEARCH::IDSEARCH --> ");
#endif

	size_t i, len, sz;
	bool found = false;
	int syid = -1;
	pascal0::key_info result;
	pascal0::key_info *kp = pascal0::keywords;
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
			result.ID = t->ascii;
			found = true;
			syid = t->m_index;
			break;
		}
	}
#ifdef DEBUG_SEARCH
	if (found==true)
		WRITELN(OUTPUT,"FOUND \"",result.ID,"\"");
	else
		WRITELN(OUTPUT,"NOT FOUND \"",buf,"\"");
#endif
	return syid;
}

int treesearch::compare (ALPHA &str1, ALPHA &str2)
{
	char c1, c2;
	int i,j,k,l;
	int test = 0;
	i = str1.length();
	j = str2.length();
	if (i>j)
		l=i;
	else
		l=j;

	for (k=0;k<l;k++)
	{
		test = 0;
		c1 = tolower(str1[k]);
		c2 = tolower(str2[k]);
// some of the symbols have trailing spaces, which means
// that one string will have a terminatinng null while
// the other will have a trailing space, but only when
// the lengths are unequal.
		if ((c1==0)&&(c2==32)
			||(c2==0)&&(c1==32))
			break;
// perfectly safe to compare the terminating zero with
// any other characther in the regular fashion.
		if (c1!=c2)
		{
			test = (c1>c2?1:-1);
			break;
		}
	}
	if (((i<j)&&(test==0))&&(str2[k]!=32))
		test=-1;

	return test;
}

int treesearch::search(const CTP& n1, CTP& n2, ALPHA &str)
{
#ifdef DEBUG_TREESEARCH
	WRITE(OUTPUT,"treesearch(CTP& n1, CTP& n2,\"",(char*)str,"\")");
#endif

	CTP ptr = n1;
	bool found = false;
	bool quit = false;
	int test;
	int i,j,k,l;
	
	do {
		test = compare (str,ptr->NAME);
		if ((test<0)&&(ptr->LLINK()!=NULL))
			ptr = ptr->LLINK();
		else if ((test>0)&&(ptr->RLINK()!=NULL))
			ptr = ptr->RLINK();
		else if (test==0)
		{
			found=true;
			quit = true;
		}
		else if ((test<0)&&(ptr->LLINK()==NULL))
			quit = true;
		else if ((test>0)&&(ptr->RLINK()==NULL))
			quit = true;
	}
	while (quit==false);
	n2 = ptr;
#if DEBUG_TREESEARCH
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

void PRINTNODE(const CTP &n1, size_t &N, bool verbose)
{
	CTP node;
	node = n1;
//	ADJUST_LINE(N);
//	INDENT_NEWLINE((int)N);
	N++;
	if (verbose==true)
		WRITE(OUTPUT,"NODE(");
//	N+=5;
	if (node->LLINK()!=NULL)
	{
		PRINTNODE(node->LLINK(),N,verbose);			
	}
	else
	{
//		N += 3;
		if (verbose==true)
		  WRITE(OUTPUT,"nil,");
	}
	WRITE (OUTPUT,"\"",node->NAME,"\",");
//	N+= (strlen(node->NAME)+4);
	if (node->RLINK()!=NULL)
	{
		PRINTNODE(node->RLINK(),N,verbose);	
	}
	else
	{
		if (verbose==true)
		  WRITE(OUTPUT,"nil,");
//		N+=3;
	}
	if (verbose==true)
	  WRITE(OUTPUT,")");
	N--;
//	++N;
}

void treesearch::printtree(char *tag, const CTP &n1, bool verbose)
{
	CTP node;
//	size_t J  = 5;
	size_t lex = 0;
	node = n1;
	WRITE(OUTPUT,tag,": ");
	if (node==NULL)
	{
	WRITELN(OUTPUT,"TREE = NULL");
	return;
	}
		WRITE(OUTPUT,"TREE(");
	if (node->LLINK()!=NULL)
	{
		PRINTNODE(node->LLINK(),lex,verbose);	
	}
	else if (verbose==true)
		WRITE(OUTPUT,"nil,");
	WRITE (OUTPUT,"\"",node->NAME,"\",");
	if (node->RLINK()!=NULL)
	{
		PRINTNODE(node->RLINK(),lex,verbose);	
	}
	else if (verbose==true)
		WRITE(OUTPUT,"nil,");
	WRITELN(OUTPUT,")");
}

void PRINTNODE1(const CTP &n1, size_t &N)
{
	CTP node;
	node = n1;

	if (node->LLINK()!=NULL)
	{
		PRINTNODE1(node->LLINK(),N);			
	}
	else
	{
		WRITE(OUTPUT,"<null>");
	}
	WRITE (OUTPUT,",\"",node->NAME,"\",");
	if (node->RLINK()!=NULL)
	{
		PRINTNODE1(node->RLINK(),N);	
	}
	else
	{
		WRITE(OUTPUT,"null");
//		N+=3;l
	}
//	WRITE(OUTPUT,")");
	N--;
//	++N;
}

void treesearch::printtree1(const CTP &n1)
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
	if (node->LLINK()!=NULL)
	{
		PRINTNODE1(node->LLINK(),lex);	
	}
	else
		WRITE(OUTPUT,"null");

	WRITE (OUTPUT,",\"",node->NAME,"\",");
	if (node->RLINK()!=NULL)
	{
		PRINTNODE1(node->RLINK(),lex);	
	}
	else
		WRITE(OUTPUT,"nulll");
	WRITELN(OUTPUT);
}
