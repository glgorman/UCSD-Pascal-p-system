
#include "stdafx.h"
#include "stdafx.h"
#include <vector>
#include "../eliza/eliza.h"
#include "insymbol.h"
#include "compilerdata.h"
#include "declarationpart.h"
#include "bodypart.h"
#include "compiler.h"

#define alias(X,...) union { struct {__VA_ARGS__}; char X[1]; }

bTreeType<PSYMBOL> *p_symbols;

extern subst pascal2c [];

#if 0
class PSYMBOL
{
public:
	int index;
	SYMBOL		SY;		/*SYMBOL FOUND BY INSYMBOL*/
	OPERATOR	OP;		/*CLASSIFICATION LAST SYMBOL*/
	ALPHA		ID;		/*LAST identifier FOUND*/			
    VALU		VAL;	/*VALUE THEN LAST CONSTANT*/
	char		*str;	// pointer to string constant
};
#endif

PSYMBOL::PSYMBOL()
{
	memset(this,0,sizeof(PSYMBOL));
}

PSYMBOL &PSYMBOL::operator = (int arg)
{
	memset(this,0,sizeof(PSYMBOL));
	SY = (SYMBOLS::SYMBOL) arg;
	return *this;
}

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
//	begin = SYMBOLS::STRINGCONST;
//	end = SYMBOLS::STRINGCONST;
//	begin = SYMBOLS::IFSY;
//	end = SYMBOLS::THENSY;
//	begin = SYMBOLS::PROCSY;
//	end = SYMBOLS::SEMICOLON;
	begin = SYMBOLS::INTCONST;
	end = SYMBOLS::INTCONST;

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
		DEBUG_SY(m_ptree[i].m_pData,begin,end);
	}
	WRITELN(OUTPUT);
	WRITELN(OUTPUT,(int)sz," decoded");
	return 0;
}

void PASCALSOURCE::SOURCE_DUMP ()
{
	ELIZA eliza;
	text_object source;
	char *buff1, *buff2;
	int line;
	line = 0;
	if (USERINFO.WORKSYM==NULL)
	{
		WRITELN(OUTPUT,"NULL source file");
		return;
	}
	else if (USERINFO.WORKSYM->size()==0)
	{
		WRITELN(OUTPUT,"Empty source file");
		return;
	}
	else do
	{
		buff1 = USERINFO.WORKSYM->get_sector (line);
		source = buff1;
//		buf2=buff1;
		eliza.process = source;
		eliza.pre_process (pascal2c);
		eliza.process >> buff2;
//		source >> buf2;
//		buf2=buff1;
		WRITE(OUTPUT,buff2);
		delete buff2;
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
		WRITE (OUTPUT," \"",(char*)&p.ID,"\"");
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

void PASCALSOURCE::build_tree ()
{
#if 0
	union
	{
	  struct
	  {
		bTreeType<PSYMBOL> *root,*branch1,*branch2,*markov;
		PSYMBOL m_pData;
	  };
	  char ptr[sizeof(bTreeType<PSYMBOL>)];
	};
	bTreeType<PSYMBOL> &tree = *reinterpret_cast<bTreeType<PSYMBOL>*> (ptr);
#endif

	alias (ptr,bTreeType<PSYMBOL> *root,*branch1,*branch2,*markov; PSYMBOL m_pData; );
	bTreeType<PSYMBOL> *tree = new (ptr) bTreeType<PSYMBOL>;
	SYMBOLS::SYMBOL &SY = m_pData.SY;

	root = tree;	
	SY = SYMBOLS::PROCSY;
  	tree->branch1 = NULL;
}

int PASCALSOURCE::CREATE_SYMLIST (LPVOID)
{
	PSYMBOL	p;
	m_ptree = new bTreeType<PSYMBOL> [65536];
	m_symbols.resize (1024);
	size_t sz = 0;
	size_t max_symbol;
	max_symbol = m_symbols.size()-1;
	
	try {
loop:
		INSYMBOL();
		ASSERT((SY>=0)&&(SY<SYMBOLS::MAXSYMBOL));
		p.SY = SY;
		p.OP = OP;
		p.VAL = VAL;
		p.index = (int) sz;
		if (SY==SYMBOLS::IDENT)
			memcpy(p.ID,ID,IDENTSIZE);
		else
			memset(p.ID,0,IDENTSIZE);
		if ((SY==SYMBOLS::STRINGCONST)
			&&(SCONST->SVAL[0]>0))
			p.str = strdup(&SCONST->SVAL[1]);
		else
			(p.str=NULL);
		
		memcpy(&m_symbols[sz],&p,sizeof(PSYMBOL));
		m_ptree[sz].m_pData = m_symbols[sz];

//		WRITELN(OUTPUT,p.index,": SY=",SYMBOL_NAMES2[SY]," ",p.ID);
		sz++;
		
		if (sz%50==0)
			WRITE(OUTPUT,".");
		if (sz>max_symbol) {
			m_symbols.resize (sz+1024);
			max_symbol = m_symbols.size()-1;
		}
		goto loop;
	}
	catch (EXIT_CODE E)
	{
		if (E.err!=401)
			throw(E);
		else
		{
			WRITELN(OUTPUT,"End of file: <",sz,"> symbols read.");
		}
	}
	return (int) sz;
}

class hexstr: public pstring<16>
{
public:
	void convert(int arg)
	{
		sprintf_s(str,"%08x",arg);
	}
};

void DECLARATIONPART::debug_stack (char *src, stack_frame *ptr)
{
union
{
  struct /*locals*/
  {
//	STP			FSP;
	STP			LSP,LSP1,LSP2;
	DISPRANGE	OLDTOP;
	CTP			LCP,LAST;
	ADDRRANGE	LSIZE,DISPL;
	int			LMIN,LMAX;
	bool		PACKING;
	BITRANGE	NEXTBIT,NUMBITS;
  };
  char fp[sizeof(stack_frame)];
};
//	build_tree();

	WRITELN (OUTPUT,"#### ",src," ####");
	memcpy (fp,ptr,sizeof(stack_frame));
	char hexval[16];
#if 0
	sprintf_s(hexval,"%08x",FSP);
	if (FSP!=NULL)
		WRITELN (OUTPUT,"FSP:     ",hexval);
#endif
	sprintf_s(hexval,"%08x",LSP);
	if (LSP!=NULL)
		WRITELN (OUTPUT,"LSP:     ",hexval);
	sprintf_s(hexval,"%08x",LSP1);
	if (LSP1!=NULL)
		WRITELN (OUTPUT,"LSP1:    ",hexval);
	sprintf_s(hexval,"%08x",LSP2);
	if (LSP2!=NULL)
		WRITELN (OUTPUT,"LSP2:    ",hexval);
	sprintf_s(hexval,"%08x",OLDTOP);
	if (OLDTOP!=NULL)
		WRITELN (OUTPUT,"OLDTOP:  ",hexval);
	if (LCP!=NULL)
	{
		sprintf_s(hexval,"%08x",LCP);
		WRITELN (OUTPUT,"LCP:     ",hexval);
		treesearch::printtree1(LCP);
	}
	if (LAST!=NULL)
	{
		sprintf_s(hexval,"%08x",LAST);
		WRITELN (OUTPUT,"LAST:     ",hexval);
		treesearch::printtree1(LAST);
	}
	sprintf_s(hexval,"%08x",LSIZE);
	if (LSIZE!=NULL)
		WRITELN (OUTPUT,"LSIZE:   ",hexval);
	sprintf_s(hexval,"%08x",DISPL);
	if (DISPL!=NULL)
		WRITELN (OUTPUT,"DISPL:   ",hexval);
	sprintf_s(hexval,"%08x",LMIN);
	if (LMIN!=NULL)
		WRITELN (OUTPUT,"LMIN:    ",hexval);
	sprintf_s(hexval,"%08x",LMAX);
	if (LMAX!=NULL)
		WRITELN (OUTPUT,"LMAX:    ",hexval);
	sprintf_s(hexval,"%08x",PACKING);
	if (PACKING!=NULL)
		WRITELN (OUTPUT,"PACKING: ",hexval);
	sprintf_s(hexval,"%08x",NEXTBIT);
	if (NEXTBIT!=NULL)
		WRITELN (OUTPUT,"MEXTBIT: ",hexval);
	sprintf_s(hexval,"%08x",NUMBITS);
	if (NUMBITS!=NULL)
		WRITELN (OUTPUT,"NUMBITS: ",hexval);
}




