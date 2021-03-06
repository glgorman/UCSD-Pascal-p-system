
#include "stdafx.h"
#include "stdafx.h"
#include <vector>
//#include "../Frame Lisp/symbol_table.h"
//#include "../Frame Lisp/btreetype.h"
//#include "../Frame Lisp/node_list.h"
//#include "../Frame Lisp/text_object.h"
#include "../eliza/eliza.h"

#include "compiler.h"
#include "bodypart.h"

extern subst pascal2c [];

#if 0
class PSYMBOL
{
public:
	int index;
	SYMBOL		SY;		/*SYMBOL FOUND BY INSYMBOL*/
	OPERATOR	OP;		/*CLASSIFICATION LAST SYMBOL*/
	ALPHA		ID;		/*LAST IDENTIFIER FOUND*/			
    VALU		VAL;	/*VALUE THEN LAST CONSTANT*/
	char		*str;	// pointer to string constant
};
#endif

int PASCALSOURCE::SYMBOL_DUMP (LPVOID)
{
	size_t i;
	SYMBOLS::SYMBOL begin, end;
//	begin = FORSY;
//	end = DOSY;
//	begin = STRINGCONST;
//	end = STRINGCONST;
//	begin = LPARENT;
//	end = RPARENT;
//	begin = IFSY;
//	end = THENSY;
//	begin = WITHSY;
//	end = DOSY;
	begin = SYMBOLS::PROCSY;
	end = SYMBOLS::SEMICOLON;

	DWORD t0, t1;
	t0 = GetTickCount();
	CREATE_SYMLIST(NULL);
	t1 = GetTickCount();
	size_t sz = m_symbols.size();
	float throughput;
	throughput = sz*60000.0/(1+t1-t0);
	WRITELN(OUTPUT);
	WRITELN(OUTPUT,"CREATE_SYMLIST took ",int(t1-t0)," msec.");
	WRITELN(OUTPUT,(int)sz," symbools = (",throughput,"/min.)");
	for (i=0;i<sz;i++)
	{
		DEBUG_SY(m_symbols[i],begin,end);
	}
	WRITELN(OUTPUT);
	WRITELN(OUTPUT,(int)sz," decoded");
	return 0;
}

void PASCALSOURCE::SOURCE_DUMP ()
{
	ELIZA eliza;
	text_object source;
	char *buff1, *buf2;
	int line;
	line = 0;
	if (SYSCOMM::m_source==NULL)
	{
		WRITELN(OUTPUT,"NULL source file");
		return;
	}
	else if ((*SYSCOMM::m_source).size()==0)
	{
		WRITELN(OUTPUT,"Empty source file");
		return;
	}
	else do
	{
		buff1 = (*SYSCOMM::m_source)[line];
		source = buff1;
		buf2;
		eliza.process = source;
		eliza.pre_process (pascal2c);
		eliza.process >> buf2;
		WRITE(OUTPUT,buf2);
		delete buf2;
		line++;
	}
	while (buff1!=NULL);
}

void PASCALSOURCE::DEBUG_SY (const PSYMBOL &p, SYMBOLS::SYMBOL start, SYMBOLS::SYMBOL stop)
{
	int count;
	int ival;
	float fval;
	static bool symbol_numbers = false;
	static bool debug_key = false;
	static bool debug_ident = false;
	static bool debug_int = false;
	static bool debug_real = false;
	static bool debug_string = false;
	static bool line_breaks = false;
	static bool print_stop = false;

	if (p.SY==start)
	{
		symbol_numbers = true;
		debug_key = true;
		debug_ident = true;
		debug_int = true;
		debug_real = true;
		debug_string = true;
		line_breaks = true;
		print_stop = true;
	}
	if ((p.SY==stop)&&(print_stop==false))
	{
		debug_key = false;
		debug_int = false;
		debug_ident = false;
		symbol_numbers = false;
	}
	if ((p.SY==SYMBOLS::RPARENT)&&(debug_key==true))
	{
		line_breaks = true;
	}
	count = p.index;
	if ((p.SY==SYMBOLS::REALCONST)&&(debug_real==true))
	{
		if (symbol_numbers==true)
		{
			WRITELN (OUTPUT);
			WRITE(OUTPUT,count,": ");
		}
		fval = p.VAL.VALP->REEL; 
		WRITE (OUTPUT,' ',SYMBOL_NAMES2[p.SY]);
		WRITE (OUTPUT,'(',fval,')');
//		WRITELN (OUTPUT);
	}
	else if ((p.SY==SYMBOLS::STRINGCONST)&&(debug_string==true))
	{
		if (symbol_numbers==true)
		{
			WRITELN (OUTPUT);
			WRITE(OUTPUT,count,": ");
		}
		ival = p.VAL.IVAL;
		WRITE (OUTPUT,' ',SYMBOL_NAMES2[p.SY]);
		if (p.str!=NULL)
			WRITE (OUTPUT,"(\"",p.str,"\")");
		else
			WRITE (OUTPUT,"(\"",char(ival),"\")");
//		WRITELN (OUTPUT);
	}
	else if ((p.SY==SYMBOLS::INTCONST)&&(debug_int==true))
	{
		if (symbol_numbers==true)
		{
			WRITELN (OUTPUT);
			WRITE(OUTPUT,count,": ");
		}
		WRITE (OUTPUT,' ',SYMBOL_NAMES2[p.SY]);
		WRITE (OUTPUT,'(',p.VAL.IVAL,')');
//		WRITELN (OUTPUT);
	}
	else if ((p.SY==SYMBOLS::IDENT)&&(debug_ident==true))
	{
		if (symbol_numbers==true)
		{
			WRITELN (OUTPUT);
			WRITE(OUTPUT,count,": ");
		}
		WRITE (OUTPUT," \"",(char*)&(p.ID),"\"");
	}
	else if (debug_key==true)
	{
		if (symbol_numbers==true)
		{
			WRITELN (OUTPUT);
			WRITE(OUTPUT,count,": ");
		}
		WRITE (OUTPUT,SYMBOL_NAMES2[p.SY]);
		if (stop!=SYMBOLS::OTHERSY)
			symbol_numbers = false;
	}
	if ((p.SY==SYMBOLS::SEMICOLON)&&(line_breaks==true))
	{
		WRITELN(OUTPUT);
	}
	if (p.SY==stop)
	{
		debug_key = false;
		debug_int = false;
		debug_real = false;
		debug_string = false;
		debug_ident = false;
		symbol_numbers = false;
		line_breaks = false;
	}
}

int PASCALSOURCE::CREATE_SYMLIST (LPVOID)
{
	PSYMBOL	p;
	m_symbols.resize (1024);
	size_t sz = 0;
	size_t max_symbol;
	max_symbol = m_symbols.size()-1;
	do
	{
		INSYMBOL();
		ASSERT((SY>=0)&&(SY<SYMBOLS::MAXSYMBOL));
		p.SY = SY;
		p.OP = OP;
		p.VAL = VAL;
		p.index = (int) sz;
		if (SY==SYMBOLS::IDENT)
			memcpy(p.ID,ID,16);
		else
			memset(p.ID,0,16);
		if ((SY==SYMBOLS::STRINGCONST)
			&&(SCONST->SVAL[0]>0))
			p.str = strdup(&SCONST->SVAL[1]);
		else
			(p.str=NULL);
		
		memcpy(&m_symbols[sz],&p,sizeof(PSYMBOL));
//		WRITELN(OUTPUT,p.index,": SY=",SYMBOL_NAMES2[SY]," ",p.ID);
		sz++;
		
		if (sz%50==0)
			WRITE(OUTPUT,".");
		if (sz>max_symbol) {
			m_symbols.resize (sz+1024);
			max_symbol = m_symbols.size()-1;
		}
//		if (sz>34000)
//			break;
	}
	while (SY!=SYMBOLS::OTHERSY);
	return (int) sz;
}





