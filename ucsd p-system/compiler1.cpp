
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

void PASCALSOURCE::DEBUG_SY (const PSYMBOL &p)
{
	int count;
	float val;
	static bool debug_real = false;
	static bool debug_int = false;
	static bool debug_string = true;
	static bool debug_ident = false;
	static bool debug_key = false;
	static bool symbol_numbers = true;
	static bool line_breaks = false;
	static bool print_stop = false;

	const PSYMBOL &q = p;
	SYMBOL	start, stop;
	start = CASESY;
	stop = ENDSY;

#if 0
	if (q.SY==start)
	{
		debug_key = true;
		debug_string = true;
		debug_int = true;
		debug_ident = true;
		symbol_numbers = true;
		line_breaks = true;
	}
	if ((q.SY==stop)&&(print_stop==false))
	{
		debug_key = false;
		debug_int = false;
		debug_ident = false;
		symbol_numbers = false;
	}
	if ((q.SY==RPARENT)&&(debug_key==true))
	{
		line_breaks = true;
	}
#endif
	count = q.index;
	
	if ((SY==REALCONST)&&(debug_real==true))
	{
		if (symbol_numbers==true)
		{
			WRITELN (OUTPUT);
			WRITE(OUTPUT,count,": ");
		}
		val = p.VAL.VALP->REEL; 
		WRITE (OUTPUT,' ',SYMBOL_NAMES2[SY]);
		WRITE (OUTPUT,'(',val,')');
//		WRITELN (OUTPUT);
	}
	else if ((SY==STRINGCONST)&&(debug_string==true))
	{
		if (symbol_numbers==true)
		{
			WRITELN (OUTPUT);
			WRITE(OUTPUT,count,": ");
		}
		val = p.VAL.IVAL;
		WRITE (OUTPUT,' ',SYMBOL_NAMES2[SY]);
		if (p.str!=NULL)
			WRITE (OUTPUT,"(\"",p.str,"\")");
		else
			WRITE (OUTPUT,"(\"",char(val),"\")");
//		WRITELN (OUTPUT);
	}
	else if ((SY==INTCONST)&&(debug_int==true))
	{
		if (symbol_numbers==true)
		{
			WRITELN (OUTPUT);
			WRITE(OUTPUT,count,": ");
		}
		WRITE (OUTPUT,' ',SYMBOL_NAMES2[SY]);
		WRITE (OUTPUT,'(',p.VAL.IVAL,')');
//		WRITELN (OUTPUT);
	}
	else if ((SY==IDENT)&&(debug_ident==true))
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
		symbol_numbers = false;
	}
	if ((q.SY==SEMICOLON)&&(line_breaks==true))
	{
		WRITELN(OUTPUT);
	}
#if 0
	if (q.SY==stop)
	{
		debug_key = false;
		debug_int = false;
		debug_ident = false;
		symbol_numbers = false;
		line_breaks = false;
	}
#endif
}

int PASCALSOURCE::SYMBOL_DUMP (LPVOID)
{
	bool busy = true;
	PSYMBOL	p;
	m_symbols.resize (65536);
	int sz = 0;
	int max_symbol = 32768+4096;
	while (busy)
	{
		INSYMBOL();
		ASSERT((SY>=0)&&(SY<MAXSYMBOL));	
		p.index = sz;
		p.SY = SY;
		p.OP = OP;
		p.VAL = VAL;
		if (SY==IDENT)
			memcpy(p.ID,ID,16);
		else
			memset(p.ID,0,16);
		if ((SY==STRINGCONST)
			&&(SCONST->SVAL[0]>0))
			p.str = strdup(&SCONST->SVAL[1]);
		else
			(p.str=NULL);
		
		memcpy(&m_symbols[sz],&p,sizeof(PSYMBOL));
		sz++;
		
		if (sz%50==0)
			WRITE(OUTPUT,".");
		if (sz>max_symbol)
			busy=false;
		if (SY==OTHERSY)
			break;
	}
	int i;
	for (i=0;i<max_symbol;i++)
	{
//		ID = symbols[sz].ID;
//		symbols[sz].index;
		PSYMBOL *r = &(m_symbols[i]);
		OP = r->OP;
		SY = r->SY;
		VAL = r->VAL;
		if (r->str!=NULL)
		{
			r->str[15]=0; // just in case
			strcpy_s(ID,16,r->str);
		}
		DEBUG_SY(m_symbols[i]);
	}
	WRITELN(OUTPUT);
	WRITELN(OUTPUT,sz," decoded");
	return 0;
}

#if 0
void BODYPART::LINKERREF(IDCLASS KLASS, int ID, int ADDR)
{
	int ioresult;
	PASCALCOMPILER	*m_ptr;
	FILE *file = &m_ptr->REFFILE;
	REFARRAY *list = m_ptr->REFLIST;
	const char *data = (const char*)(&list[0]);
	if (m_ptr->NREFS>REFSPERBLK)/*WRITE BUFFER*/
	{
		ioresult = SYSCOMM::BLOCKWRITE(file,data,1,m_ptr->REFBLK);
		if (ioresult!=1)
			CERROR(402);
		m_ptr->REFBLK++;
		m_ptr->NREFS=1;
	};
	// with	(m_ptr->REFLIST[m_ptr->NREFS])
	REFARRAY &ARR = m_ptr->REFLIST[m_ptr->NREFS];
	if (VARS.in(KLASS))
		ARR->KEY=ID+32;
	else /*PROC*/
		ARR->KEY=ID;
	ARR->OFFSET=SEGINX+ADDR;
	NREFS=NREFS+1;
}
#endif


