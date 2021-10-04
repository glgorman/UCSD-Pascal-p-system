
#include "stdafx.h"
#include "compiler.h"

void IDSEARCH(int,char * &)
{

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
