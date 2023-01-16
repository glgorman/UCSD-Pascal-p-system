
#include "stdafx.h"

#include "../Frame Lisp/btreetype.h"
#include "insymbol.h"
#include "../Frame Lisp/objects.h"
#include "compilerdata.h"
#include "declarationpart.h"
#include "bodypart.h"
#include "compiler.h"

#define TRACE_PROCDECL		1
//#define TRACE_SIMPLETYPE	1
//#define TRACE_PACKABLE		1
//#define TRACE_CONSTDECL		1

DECLARATIONPART::DECLARATIONPART()
{
	size_t sz = sizeof(COMPILERDATA);
//	bad magic - use the allocators
//	memset(this,0,sz);
}

DECLARATIONPART::DECLARATIONPART(PASCALCOMPILER *ptr)
{
//	just copy the data from the base class
//	over, whatt could possibly go WRONG?!?

	size_t sz = sizeof(PASCALCOMPILER);
	PASCALCOMPILER *ptr2;
	ptr2 = reinterpret_cast<PASCALCOMPILER*>(this);
	memcpy(ptr2,ptr,sz);

}

DECLARATIONPART::~DECLARATIONPART()
{
	
}

void DECLARATIONPART::USESDECLARATION(bool MAGIC)
{
	ASSERT(false);
}

void DECLARATIONPART::MAIN (const SETOFSYS &FSYS)
{
	SYMBOLS::SYMBOL	LSY0;
	bool	NOTDONE;
#if 0
	int		*DUMMYVAR; 
#endif
	EXIT_CODE E("DECLARATIONPRAT");
	WRITELN(OUTPUT,"DECLARATIONPART::MAIN");

	SETOFSYS DECLSYS = BNF::STATBEGSYS+
		SETOFSYS(4,SYMBOLS::SEPARATSY,SYMBOLS::UNITSY,SYMBOLS::IMPLESY,SYMBOLS::ENDSY);
#if 0	
	DECLSYS.debug_list ("DECLSYS");
	FSYS.debug_list("FSYS");
#endif
	if ((options.NOSWAP)&& (STARTINGUP))
	{
		STARTINGUP = false; /* ALL SEGMENTS ARE IN BY THIS TIME */
		BLOCK(FSYS);
		EXIT_CODE E("DECLARATIONPRAT");
		throw (E);
	}
#if 0
	if (options.NOISY)
		SYSCOMM::UNITWRITE(3,&DUMMYVAR[-1600],35); /*ADJUST DISPLAY STACK AND HEAP*/
#endif
	do {
		NOTDONE = false;
		if (USERINFO.STUPID)
			if (!CODEINSEG)
				if ((LEVEL==1)&& (NEXTSEG==10))
					if (!(options.INMODULE||options.USING))
						USESDECLARATION(true);
		/*To get turtle graphics*/
		if (SY==SYMBOLS::USESSY)
		{
			INSYMBOL();
			USESDECLARATION(false);
		}
		if (SY==SYMBOLS::LABELSY)
		{
			if (options.INMODULE&&options.ININTERFACE)
			{
				CERROR(186);
				SKIP(FSYS-SYMBOLS::LABELSY);
			}
			else
				INSYMBOL();
			LABELDECLARATION(FSYS);
		}
		if (SY==SYMBOLS::CONSTSY)
		{
			INSYMBOL();
			CONSTDECLARATION(FSYS);
		}
		if (SY==SYMBOLS::TYPESY)
		{
			INSYMBOL();
			TYPEDECLARATION(FSYS);
		}
		if (SY==SYMBOLS::VARSY)
		{
			INSYMBOL();
			VARDECLARATION(FSYS);
		}
		if (LEVEL==1)
			GLEV=TOP;
		if (SETOFSYS(3,SYMBOLS::PROCSY,SYMBOLS::FUNCSY,
			SYMBOLS::PROGSY).in(SY))
		{
			if (options.INMODULE)
				if (options.ININTERFACE&&!options.USING)
					PUBLICPROCS=true;
				do 
				{
					LSY0=SY;
					INSYMBOL();
#ifdef  TRACE_PROCDECL
					WRITELN(OUTPUT,"Found prog/func/proc: ",ID);
#endif
					if (LSY0==SYMBOLS::PROGSY)
						if (options.INMODULE)
						{
							CERROR(185 /*SEG DEC !ALLOWED IN UNIT*/);
							PROCDECLARATION(FSYS,SYMBOLS::PROCSY,false,NOTDONE);
						}
						else 
							PROCDECLARATION(FSYS,LSY0,true,NOTDONE);
					else
						PROCDECLARATION(FSYS,LSY0,false,NOTDONE);
				}
				while (SETOFSYS(3,SYMBOLS::PROCSY,
					SYMBOLS::FUNCSY,SYMBOLS::PROGSY).in(SY));
			}
			if ((SY!=SYMBOLS::BEGINSY))
				if (!((options.USING||options.INMODULE)&&(SETOFSYS(2,SYMBOLS::IMPLESY,SYMBOLS::ENDSY).in(SY)))
					&& !(SETOFSYS(2,SYMBOLS::SEPARATSY,SYMBOLS::UNITSY).in(SY)))
					if ((!(options.INCLUDING||NOTDONE))
						||(!BNF::BLOCKBEGSYS.in(SY)))
					{
						CERROR(18);
 						SKIP(FSYS - SETOFSYS(2,SYMBOLS::UNITSY,SYMBOLS::INTERSY));
					}		
	}
 	while (!DECLSYS.in(SY));
	NEWBLOCK = false;
} /*DECLARATIONPART*/

void DECLARATIONPART::SIMPLETYPE(const SETOFSYS &FSYS, STP &FSP, ADDRRANGE &FSIZE)
{
	STP LSP = NULL;
	STP LSP1 = NULL;
	CTP LCP = NULL;
	CTP LCP1 = NULL;
	DISPRANGE TTOP;
	int LCNT = 0;
	VALU LVALU2;

#ifdef TRACE_SIMPLETYPE
 	WRITELN (OUTPUT,"DECLARATIONPART::SIMPLETYPE: ");
#endif

	FSIZE=1;
	LSP1 = NULL;
	if (!BNF::SIMPTYPEBEGSYS.in(SY))
	{
		CERROR(1);
		SKIP(BNF::SIMPTYPEBEGSYS+FSYS);
	}
	if (BNF::SIMPTYPEBEGSYS.in(SY))
	{
		if (SY==SYMBOLS::LPARENT)
		{
			TTOP=TOP;
			while (DISPLAY[TOP].OCCUR!=BLCK)
				TOP=TOP - 1;

			LSP = structure::allocate (SCALAR,DECLARED,INTSIZE);
			LCP1=NULL;
			LCNT=0;
			do {
				INSYMBOL();
				if (SY==SYMBOLS::IDENT)
				{
					LCP = identifier::allocate(ID,LSP,KONST); 
					LCP->NEXT=LCP1;
					LCP->VALUES.IVAL=LCNT;
					ENTERID(LCP);
					LCNT=LCNT + 1;
					LCP1=LCP;
					INSYMBOL();
				}
				else
					CERROR(2);
				if (!(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT).in(SY))
				{
					CERROR(6);
					SKIP(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
				}
			}
			while (SY==SYMBOLS::COMMA);
			LSP->FCONST=LCP1;
			TOP=TTOP;
			if (SY==SYMBOLS::RPARENT)
				INSYMBOL();
			else 
				CERROR(4);
		}
		else
		{
			if (SY==SYMBOLS::IDENT)
			{
				SEARCHID(SETOFIDS(2,TYPES,KONST),LCP);
				INSYMBOL();
				if (LCP->KLASS==KONST)
				{
					LSP = structure::allocate (SUBRANGE,INTSIZE);
					LSP->RANGETYPE=LCP->IDTYPE;
					if (STRGTYPE(LSP->RANGETYPE))
					{
						CERROR(148);
						LSP->RANGETYPE=NULL;
					}
					LSP->MIN=LCP->VALUES;
					if (SY==SYMBOLS::COLON)
						INSYMBOL();
					else
						CERROR(5);
					CONSTANT(FSYS,LSP1,LVALU2);
					LSP->MAX=LVALU2;
					if (LSP->RANGETYPE!=LSP1)
						CERROR(107);
				}
				else
				{
					LSP=LCP->IDTYPE;
					if ((LSP==STRGPTR)&&(SY==SYMBOLS::LBRACK))
					{
						INSYMBOL();
						CONSTANT(FSYS+SYMBOLS::RBRACK,LSP1,LVALU2);
						ASSERT(LSP1!=NULL);
						if (LSP1==INTPTR)
						{
							if ((LVALU2.IVAL<=0)||(LVALU2.IVAL>STRGLGTH))
							{
								CERROR(203);
								LVALU2.IVAL=DEFSTRGLGTH;
							}
							if (LVALU2.IVAL!=DEFSTRGLGTH)
							{
								//NEW(LSP,ARRAYS,true,true);
								LSP = structure::allocate (ARRAYS);
								LSP->AELTYPE = STRGPTR;
								LSP->set_packed(true);
								LSP->set_string(true);
								// WITH LSP^,LVALU
								LSP->MAXLENG=LVALU2.IVAL;
								LSP->resize((LVALU2.IVAL+CHRSPERWD)/CHRSPERWD);
							}
						}
						else
							CERROR(15);
						if (SY==SYMBOLS::RBRACK)
							INSYMBOL();
						else
							CERROR(12);
					}
					else if (LSP==INTPTR)
						if (SY==SYMBOLS::LBRACK)
						{ 
							INSYMBOL();
							LSP = structure::allocate (LONGINT);
							LSP->ELTYPE=LONGINTPTR;
							CONSTANT(FSYS+SYMBOLS::RBRACK,LSP1,LVALU2);
							if (LSP1==INTPTR)
								if ((LVALU2.IVAL<=0)||
								(LVALU2.IVAL>MAXDEC))
									CERROR(203);
								else
									LSP->resize(DECSIZE(LVALU2.IVAL));
							else
								CERROR(15);

							if (SY==SYMBOLS::RBRACK)
								INSYMBOL();
							else
								CERROR(12);
						}
						else if (LSP->is_file()==true)
							if (options.INMODULE)
								if (!options.ININTERFACE)
									CERROR(191); /*NO PRIVATE FILES*/
						if (LSP!=NULL)
							FSIZE=(int)LSP->size();
				}
			} /*SY==IDENT*/
			else
			{ 
				LSP = structure::allocate (SUBRANGE,INTSIZE);
				CONSTANT(FSYS+SYMBOLS::COLON,LSP1,LVALU2);
				if (STRGTYPE(LSP1))
				{
					CERROR(148);
					LSP1=NULL;
				}
				LSP->RANGETYPE = LSP1;
				LSP->MIN = LVALU2;
				if (SY==SYMBOLS::COLON)
					INSYMBOL();
				else
					CERROR(5);
				CONSTANT(FSYS,LSP1,LVALU2);
				LSP->MAX=LVALU2;
				if (LSP->RANGETYPE!=LSP1)
					CERROR(107);
			}
			if (LSP!=NULL)
			{ // with LSP
				if (LSP->form()==SUBRANGE)
					if (LSP->RANGETYPE!=NULL)
						if (LSP->RANGETYPE==REALPTR)
							CERROR(399);
						else if (LSP->MIN.IVAL>LSP->MAX.IVAL)
						{
							CERROR(102);
							LSP->MAX.IVAL=LSP->MIN.IVAL;
						}
			}
		}
		FSP=LSP;
		if (!FSYS.in(SY))
		{
#ifdef TRACE_SIMPLETYPE
			FSYS.debug_list ("FSYS");
			WRITELN(OUTPUT,"SY = ",SYMBOL_NAMES1[SY]);
#endif
			CERROR(6);
			SKIP(FSYS); 
		}
	}
	else
		FSP=NULL;
	
#ifdef TRACE_SIMPLETYPE
	WRITELN(OUTPUT,"FSIZE = ",(int)FSIZE);
#endif
} /*SIMPLETYPE*/

bool DECLARATIONPART::PACKABLE(stack_frame *param, STP FSP)
{
	union
	{
	  struct /* TYP1 locals */
	  {
//		STP			FSP0;
		STP			LSP,LSP1,LSP2;
		DISPRANGE	OLDTOP;
		CTP			LCP,LAST;
		ADDRRANGE	LSIZE,DISPL;
		int			LMIN0,LMAX0;
		bool		PACKING;
		BITRANGE	NEXTBIT,NUMBITS;
	  };
	  char fp[sizeof(stack_frame)];
	};
	
	int LMIN, LMAX; // the only local variables declared
	// in the original PASCAL implementation - which is
	// obviously recursive, so these need to have local
	// copies sepaate from the stack frame that is
	// inherited from TYP::FIELDLIST. 2022-08-15 GLG

	memcpy (fp,param,sizeof(stack_frame));

#ifdef TRACE_PACKABLE
	debug_stack ("DECLARATIONPART::PACKABLE",param);
#endif

	bool result=false;
	if ((FSP==NULL)||(!PACKING))
	{
#ifdef TRACE_PACKABLE
		WRITELN(OUTPUT,"returning <false, no_operation>");
#endif
		return result;
	}
	static int lex = 0;
	lex++; // detect runaway recursion causting stack overflow
	if (lex>15)
		ASSERT(false);

	switch (FSP->form())
	{
	case SUBRANGE:
	case SCALAR: 
		if ((FSP!=INTPTR)&&(FSP!=REALPTR))
		{
			GETBOUNDS(FSP,LMIN,LMAX);
			if (LMIN>=0)
			{
				result=true;
				NUMBITS=1;
				LMIN=1;
				while (LMIN<LMAX)
				{
					LMIN=LMIN+1;
					LMIN=LMIN+LMIN-1;
					NUMBITS=NUMBITS+1;
				}
			}
		}
		break;
	case POWER:
		if (PACKABLE(param,FSP->ELSET))
		{ 
			GETBOUNDS(FSP->ELSET,LMIN,LMAX);
			LMAX=LMAX + 1;
			if (LMAX<BITSPERWD)
			{
				result=true;
				NUMBITS=LMAX;
			}
		}
		break;
	default:
		break;
	}
	memcpy (param,fp,sizeof(stack_frame));
	// if we are returning normally - decrement lex
	// so that we dont run out of calls, otherwise they
	// do add up!
	lex--;
#ifdef TRACE_PACKABLE
	if (result==true)
		WRITELN(OUTPUT,"returning <true>");
	else
		WRITELN(OUTPUT,"returning <false>");
#endif
	return result;
}

void DECLARATIONPART::FIELDLIST(stack_frame *param, const SETOFSYS &FSYS, STP &FRECVAR)
{
	union
	{
	  struct /*typ locals from nesting proc*/
	  {
//		STP			FSP;
		STP			LSP,LSP1,LSP2;
		DISPRANGE	OLDTOP;
		CTP			LCP, LAST;
		ADDRRANGE	LSIZE,DISPL;
		int			LMIN,LMAX;
		bool		PACKING;
		BITRANGE	NEXTBIT,NUMBITS;
	  };
	  char fp[sizeof(stack_frame)];
	};
	memcpy (fp,param,sizeof(stack_frame));

	CTP NXT, NXT1;
	NXT=NULL;
	NXT1=NULL;
	LSP=NULL;
	LAST=NULL;

	if (!SETOFSYS(2,SYMBOLS::IDENT,SYMBOLS::CASESY).in(SY))
	{
		CERROR(19);
		SKIP(FSYS+SYMBOLS::IDENT+SYMBOLS::CASESY);
	}
	while (SY==SYMBOLS::IDENT)
	{
		NXT=NXT1;
		do
		{
			if (SY==SYMBOLS::IDENT)
			{
				LCP = (identifier*) identifier::allocate(ID,NULL,FIELD);
				LCP->NEXT=NXT;
				if (PACKING)
					LCP->FISPACKED=true;
				else
					LCP->FISPACKED=false;
				NXT=LCP;
				ENTERID(LCP);
				INSYMBOL();
			}
			else
				CERROR(2);

			SETOFSYS field_separators1(2,SYMBOLS::COMMA,SYMBOLS::COLON);
			SETOFSYS field_separators2 = FSYS+SETOFSYS(4,SYMBOLS::COMMA,
			SYMBOLS::COLON,SYMBOLS::SEMICOLON,SYMBOLS::CASESY);
			if (!field_separators1.in(SY))
			{
				CERROR(6);
				SKIP(field_separators2);
			}
			TEST=SY!=SYMBOLS::COMMA;
			if (!TEST)
				INSYMBOL();
			}
			while (!TEST);

			if (SY==SYMBOLS::COLON)
				INSYMBOL();
			else
				CERROR(5);

			TYP1(FSYS+SYMBOLS::CASESY+SYMBOLS::SEMICOLON,LSP,LSIZE);
			if (LSP!=NULL)
			if (LSP->is_file()==true)
				CERROR(108);

			// FIXME - why is this crashing?
			while (NXT!=NXT1)
			{
				if (NXT==NULL)
					break;
				NXT->IDTYPE=LSP;
				ALLOCATE(param,NXT);
				if (NXT->NEXT==NXT1)
					LAST=NXT;
				NXT=NXT->NEXT;
				NXT1=LCP;
				if (SY==SYMBOLS::SEMICOLON)
				{
					INSYMBOL();
					if (!(SETOFSYS(3,SYMBOLS::IDENT,SYMBOLS::ENDSY,SYMBOLS::CASESY).in(SY)))/* <<<< SMF 2-28-78 */
					{
						CERROR(19);
						SKIP(FSYS+SYMBOLS::IDENT+SYMBOLS::CASESY);
					}
				}
			}
	}
	while (!TEST);

//	NXT=NULL;
	while (NXT1!=NULL)
	{
		// FIXME ?? crashing here with NULL ptr?
		// why?  GLG 2022-08-14
		if (NXT==NULL)
			break;
		LCP=NXT->NEXT;
		NXT->NEXT=NXT;
		NXT=NXT1;
		NXT1=LCP;
	}
	if (SY==SYMBOLS::CASESY)
		VARIANTLIST(param,FSYS,FRECVAR);
	else
		FRECVAR=NULL;
	memcpy (param,fp,sizeof(stack_frame));
} /*FIELDLIST*/

void DECLARATIONPART::ALLOCATE(stack_frame *param, CTP FCP)
{
	union
	{
	 struct /*typ locals from nesting proc*/
	  {
//		STP			FSP;
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
	memcpy (fp,param,sizeof(stack_frame));

	bool ONBOUND;
	ONBOUND=false;
	if (PACKABLE(param,FCP->IDTYPE))
    {
       if ((NUMBITS + NEXTBIT)>BITSPERWD)
	   {
			DISPL=DISPL + 1;
			NEXTBIT=0;
			ONBOUND=true;
	   };
       FCP->FLDADDR=DISPL;
	   FCP->FISPACKED=true;
       FCP->FLDWIDTH=NUMBITS;
	   FCP->FLDRBIT=NEXTBIT;
       NEXTBIT=NEXTBIT + NUMBITS;
	}
	else
	{
		 DISPL=DISPL + ORD(NEXTBIT>0);
		 NEXTBIT=0;
		 ONBOUND=true;
		 FCP->FISPACKED=false;
		 FCP->FLDADDR=DISPL;
		 if (FCP->IDTYPE!=NULL)
			DISPL=DISPL + (int)FCP->IDTYPE->size();
	}
	if (ONBOUND&& (LAST!=NULL))
	if (FCP->FISPACKED)
       if (FCP->FLDRBIT==0)
		   FCP->FISPACKED=false;
       else if ((FCP->FLDWIDTH<=8)&&(FCP->FLDRBIT<=8))
		 {
			 FCP->FLDWIDTH=8;
			 FCP->FLDRBIT=8;
		 }

   memcpy (param,fp,sizeof(stack_frame));
} /*ALLOCATE*/

void DECLARATIONPART::VARIANTLIST(stack_frame *param,const SETOFSYS &FSYS, STP	&FRECVAR)
{
	union
	{
	  struct /*typ locals from nesting proc*/
	  {
//		STP			FSP;
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
	STP LSP3 = NULL;
	memcpy (fp,param,sizeof(stack_frame));
	debug_stack ("DECLARATIONPART::VARIANTLIST",param);

	STP	LSP4;
	CTP	LCP1 = NULL;
	ADDRRANGE MINSIZE,MAXSIZE;
	BITRANGE MAXBIT,MINBIT;
	VALU LVALU1;

	bool GOTTAGNAME;
	LSP = structure::allocate (TAGFLD);
	LSP->TAGFIELDP=NULL;
	LSP->FSTVAR=NULL;
	FRECVAR=LSP;
	INSYMBOL();
	if (SY==SYMBOLS::IDENT)
	{
		LCP = (identifier*) identifier::allocate(FIELD);
		LCP->IDTYPE=NULL;
		LCP->NEXT=NULL;
		if (PACKING)
			LCP->FISPACKED=true;
		else
			LCP->FISPACKED=true;

		GOTTAGNAME=false;
		PRTERR=false;
		SEARCHID(SETOFIDS(1,TYPES),LCP1);
		PRTERR=true;
		if (LCP1==NULL)
		{ 
			GOTTAGNAME=true;
			LCP->NAME = ID;
			ENTERID(LCP);
			INSYMBOL();
			if (SY==SYMBOLS::COLON)
				INSYMBOL();
			else
				CERROR(5);
		}
		if (SY==SYMBOLS::IDENT)
		{
			SEARCHID(SETOFIDS(1,TYPES),LCP1);
			LSP1=LCP1->IDTYPE;
			if (LSP1!=NULL)
			{
				if (LSP1->form()<=SUBRANGE)
				{
				 if (COMPTYPES(REALPTR,LSP1))
					 CERROR(109);
				 LCP->IDTYPE=LSP1;
				 LSP->TAGFIELDP=LCP;
				 if (GOTTAGNAME)
					 ALLOCATE(param,LCP);
				}
				else
					CERROR(110);
			}
			INSYMBOL();
		}
		else {
			CERROR(2);
			SKIP(FSYS+SYMBOLS::OFSY+SYMBOLS::LPARENT);
		}
	}
	else
	{
		CERROR(2);
		SKIP(FSYS+SYMBOLS::OFSY+SYMBOLS::LPARENT);
	}
	LSP->resize(DISPL + ORD(NEXTBIT>0));
	if (SY==SYMBOLS::OFSY)
		INSYMBOL();
	else
		CERROR(8);
	LSP1=NULL;
	MINSIZE=DISPL;
	MAXSIZE=DISPL;
	MINBIT=NEXTBIT;
	MAXBIT=NEXTBIT;
	do
	{
		LSP2=NULL;
		do
		{
			SETOFSYS S3(3,SYMBOLS::COMMA,SYMBOLS::COLON,SYMBOLS::LPARENT);
			CONSTANT(FSYS+S3,LSP3,LVALU1);

			if (LSP->TAGFIELDP!=NULL)
				if (!COMPTYPES(LSP->TAGFIELDP->IDTYPE,LSP3))
					CERROR(111);
				//     NEW(LSP3,VARIANT);
				LSP3 = structure::allocate (VARIANT2);
				LSP3->NXTVAR=LSP1;
				LSP3->SUBVAR=LSP2;
				LSP3->VARVAL=LVALU1;
				LSP1=LSP3;
				LSP2=LSP3;
				TEST=(SY!=SYMBOLS::COMMA?true:false);
				if (!TEST)
					INSYMBOL();
		}
		while (!TEST);

		if (SY==SYMBOLS::COLON)
			INSYMBOL();
		else
			CERROR(5);
		if (SY==SYMBOLS::LPARENT)
			INSYMBOL();
		else
			CERROR(9);
		if (SY==SYMBOLS::RPARENT)
			LSP2=NULL;	
		else
//			DECLARATIONPART::TYP::
			FIELDLIST(param,FSYS+SYMBOLS::RPARENT+SYMBOLS::SEMICOLON,LSP2);
		if (DISPL>MAXSIZE)
		{
			MAXSIZE=DISPL;
			MAXBIT=NEXTBIT;
		}
		else if ((DISPL==MAXSIZE)&& (NEXTBIT>MAXBIT))
			MAXBIT=NEXTBIT;
		while (LSP3!=NULL)
		{
			LSP4=LSP3->SUBVAR;
			LSP3->SUBVAR=LSP2;
			LSP3->resize(DISPL + ORD(NEXTBIT>0));
			LSP3=LSP4;
		}
		if (SY==SYMBOLS::RPARENT)
		{
			INSYMBOL();
			if (!((FSYS+SYMBOLS::SEMICOLON).in(SY)))
			{
				CERROR(6);
				SKIP(FSYS+SYMBOLS::SEMICOLON);	}
			}
			else
				CERROR(4);
			TEST=(SY!=SYMBOLS::SEMICOLON?true:false);
			if (!TEST)
			{
				INSYMBOL();
				DISPL=MINSIZE;
				NEXTBIT=MINBIT;
			}
	}
	while ((!TEST)&&(SY!=SYMBOLS::ENDSY)); /* <<<< SMF 2-28-78 */
	DISPL=MAXSIZE;
	NEXTBIT=MAXBIT;
	LSP->FSTVAR=LSP1;
	memcpy (param,fp,sizeof(stack_frame));
} /*VARIANTLIST*/

void DECLARATIONPART::POINTERTYPE(stack_frame *param)
{
	union
	{
	  struct /*typ locals from nesting proc*/
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
	memcpy (fp,param,sizeof(stack_frame));
	debug_stack ("DECLARATIONPART::POINTERTYPE",param);

//	FSP=LSP;
	LSP = structure::allocate (POINTER,PTRSIZE);
	LSP->ELTYPE=NULL;
	INSYMBOL();
	if (SY==SYMBOLS::IDENT)
	{
		PRTERR=false;
		SETOFIDS S0(1,TYPES);
		SEARCHID(S0,LCP);
		PRTERR=true;
		if (LCP==NULL) /*FORWARD REFERENCED TYPE ID*/
		{
			LCP = identifier::allocate(ID,LSP,TYPES);
			LCP->NEXT=FWPTR;
			FWPTR=LCP;
		}
		else
		{
			if (LCP->IDTYPE!=NULL)
				if ((LCP->IDTYPE->is_file()==false)||options.SYSCOMP)
					LSP->ELTYPE=LCP->IDTYPE;
				else
					CERROR(108);
		}
		INSYMBOL();
	}
	else
		CERROR(2);

	memcpy (param,fp,sizeof(stack_frame));
} /*POINTERTYPE*/ 

void DECLARATIONPART::USESDECLARATION::GETTEXT(bool &FOUND)
{
	int SEGINDEX;
	CTP LCP;
	FOUND=false;
	LCP=MODPTR;
	while ((LCP!=NULL)&&(!FOUND))
	{
		if (LCP->NAME==ID)
			FOUND=true;
		else
			LCP=LCP->NEXT;
		if (FOUND)
		{
			LSEPPROC=SEGTABLE[LCP->SEGID].SEGKIND==4;
			if (!LSEPPROC)
			{
				SEG=LCP->SEGID;
				NEXTPROC=1;
			}
			BEGADDR=SEGTABLE[LCP->SEGID].TEXTADDR;
#if 0
			USEFILE=WORKCODE;
#endif
		}
		else
		{
			FOUND=true;
			if (options.LIBNOTOPEN)
			{
				SYSCOMM::RESET(LIBRARY,SYSTEMLIB);
				if (SYSCOMM::IORESULT()!=0)
				{
					CERROR(187);
					FOUND=false;
				}
				else
				{
					char *segdict = (char*)(&SEGDICT);
					int rd;
					SYSCOMM::BLOCKREAD(LIBRARY,segdict,1,rd);
					if (SYSCOMM::IORESULT()!=1)
					{
						CERROR(187);
						FOUND=false;
					}
				}
			}
		}
		if (FOUND)
		{
			options.LIBNOTOPEN=false;
			SEGINDEX=0;
			FOUND=false;
			while ((SEGINDEX<=MAXSEG)&&(!FOUND))
			{
				if (MAGIC)
					if (SEGDICT.SEGNAME[SEGINDEX]==LNAME)
						FOUND=true;
					else
						SEGINDEX=SEGINDEX+1;
				else if (SEGDICT.SEGNAME[SEGINDEX]==ID)
					FOUND=true;
				else
					SEGINDEX=SEGINDEX + 1;
				if (FOUND)
				{
					USEFILE=SYSLIBRARY;
					BEGADDR=SEGDICT.TEXTADDR[SEGINDEX];
					LSEPPROC=SEGDICT.SEGKIND[SEGINDEX]==4;
					if (!LSEPPROC)
					{
						if (MAGIC)
							SEG=6;
						else
						{
							SEG=NEXTSEG;
							NEXTSEG=NEXTSEG + 1;
							if (NEXTSEG>MAXSEG)
								CERROR(250);
						}
						// WITH SEGTABLE[SEG]
						SEGTABLE[SEG].DISKADDR=0;
						SEGTABLE[SEG].CODELENGTH=0;
						SEGTABLE[SEG].SEGNAME = SEGDICT.SEGNAME[SEGINDEX];
						if (options.INMODULE||MAGIC)
							SEGTABLE[SEG].SEGKIND=0;
						else
							SEGTABLE[SEG].SEGKIND=SEGDICT.SEGKIND[SEGINDEX];
						SEGTABLE[SEG].TEXTADDR=0;
						NEXTPROC=1;
					}
					else
						CERROR(190); /*!IN LIBRARY*/
				}
		}
		if (BEGADDR==0)
		{
			CERROR(195);
			FOUND=false;
		}
		if (FOUND)
		{
			options.USING=true;
			m_src.PREVSYMCURSOR=m_src.SYMCURSOR;
			m_src.PREVLINESTART=m_src.LINESTART;
			m_src.PREVSYMBLK=m_src.SYMBLK - 2;
			m_src.SYMBLK=BEGADDR;
			GETNEXTPAGE();
			INSYMBOL();
		}
		} /*GETTEXT*/
	}
}

DECLARATIONPART::USESDECLARATION::USESDECLARATION(const SETOFSYS &FSYS, bool MAGIC)
{
	SYMBOLS::SYMBOL      LSY;

	/*USESDECLARATION*/
	if (LEVEL!=1)
		CERROR(189);
	if (options.INMODULE&&!options.ININTERFACE)
		CERROR(192);
	if (!MAGIC)
		DLINKERINFO=true;
	if (!options.USING)
		USINGLIST=NULL;
	do
	{
		if ((!MAGIC)&&(SY!=SYMBOLS::IDENT))
			CERROR(2);
		else if (options.USING)
		{
			LCP=USINGLIST;
			while (LCP!=NULL)
			{
				if (LCP->NAME==ID)
					break;
				else
					LCP=LCP->NEXT;
				CERROR(188);
			}
			/*UNIT MUST BE PREDECLARED IN MAIN PROG*/
		}
		else
		{
			if (MAGIC)
			{ 
				LNAME = "TURTLE  ";
				LSY=SY;
				LOP=OP;
				LID = ID;
			}
			else
			{
				LNAME = ID;
				DWORD mem = MEMAVAIL();
				WRITELN(OUTPUT);
				WRITELN(OUTPUT,&(ID[0])," [",mem," words]");
				WRITE(OUTPUT,"<",SCREENDOTS,">");
			}
       // WITH LLEXSTK
			LLEXSTK.DOLDSEG=SEG;
			LLEXSTK.SOLDPROC=NEXTPROC;
			GETTEXT(FOUND);
			if (FOUND)
			{
				LCP = (identifier*) identifier::allocate(LNAME,NULL,MODULE);
				LCP->NEXT=USINGLIST;
				if (LSEPPROC)
					LCP->SEGID=-1; /*NO SEG*/
				else
					LCP->SEGID=SEG;
				ENTERID(LCP);
				USINGLIST=LCP;
				DECLARATIONPART::MAIN(FSYS+SYMBOLS::ENDSY);
				if (NEXTPROC==1) /*NO PROCS DECLARED*/
					LCP->SEGID=-1; /*NO SEG*/
				m_src.SYMBLK=9999;
		   /*FORCE RETURN; /*FIXME SOURCEFILE*/
				GETNEXTPAGE();
			}
			if (!LSEPPROC)
         // WITH LLEXSTK
			{
			   SEG=LLEXSTK.DOLDSEG;
			   NEXTPROC=LLEXSTK.SOLDPROC;
			}
			LSEPPROC=false;
		}
		if (!MAGIC)
		{
			INSYMBOL();
			TEST=SY!=SYMBOLS::COMMA;
			if (TEST)
				if (SY!=SYMBOLS::SEMICOLON)
					CERROR(20);
			else
				INSYMBOL();
		}
	}
	while (!(TEST||MAGIC));
	if (!MAGIC)
		if (SY==SYMBOLS::SEMICOLON)
			INSYMBOL();
		else
			CERROR(14);
	else
		SY=LSY;
	OP=LOP;
	ID = LID;
	if (!options.USING)
	{
		if (options.INMODULE)
			USINGLIST=NULL;
		SYSCOMM::CLOSE(LIBRARY,LOCK);
		options.LIBNOTOPEN=true;
	}
} /*USESDECLARATION*/

void DECLARATIONPART::LABELDECLARATION(const SETOFSYS &FSYS)
{
	bool REDEF;
	LABELP LLP;
	do {
		if (SY==SYMBOLS::INTCONST)
		{
			LLP=DISPLAY[TOP].BLCK.FLABEL;
			REDEF=false;
			while ((LLP!=NULL)&&!REDEF)
			{
				if (LLP->LABVAL!=VAL.IVAL)
					LLP=LLP->NEXTLAB;
				else 
				{
					REDEF=true;
					CERROR(166);
				}
				if (!REDEF)
				{
//					NEW(LLP);
					LLP = (USERLABEL*) new USERLABEL;
			// WITH LLP^
					LLP->LABVAL=VAL.IVAL;
					LLP->CODELBP=NULL;
					LLP->NEXTLAB=DISPLAY[TOP].BLCK.FLABEL;
					DISPLAY[TOP].BLCK.FLABEL=LLP;
				}
				INSYMBOL();
			}
		}
		else
			CERROR(15);
		if (!(FSYS+SYMBOLS::COMMA+SYMBOLS::SEMICOLON).in(SY))
		{
			CERROR(6);
			SKIP(FSYS+SYMBOLS::COMMA+SYMBOLS::SEMICOLON);
		}
		TEST=SY!=SYMBOLS::COMMA;
		if (!TEST)
				INSYMBOL();
	}
	while (!TEST);
	if (SY==SYMBOLS::SEMICOLON)
		INSYMBOL();
	else
		CERROR(14);
} /* LABELDECLARATION */ ;

void DECLARATIONPART::CONSTDECLARATION(const SETOFSYS &FSYS)
{
	CTP LCP;
	STP LSP = NULL;
	VALU LVALU3;
	if (SY!=SYMBOLS::IDENT)
	{
		CERROR(2);
		SKIP(FSYS+SYMBOLS::IDENT);
	}
	while (SY==SYMBOLS::IDENT)
	{
		ID[IDENTSIZE-1]=0; // sanity check
		LCP = (identifier*) identifier::allocate(ID,NULL,KONST);
		INSYMBOL();
		if ((SY==SYMBOLS::RELOP)&& (OP==EQOP))
			INSYMBOL();
		else
			CERROR(16);
		CONSTANT(FSYS+SYMBOLS::SEMICOLON,LSP,LVALU3);
		ENTERID(LCP);
		LCP->IDTYPE=LSP;
		LCP->VALUES=LVALU3;

#ifdef TRACE_CONSTDECL
		WRITELN(OUTPUT,"CONSTDECLARATION ",LCP->NAME," = ",LCP->VALUES.IVAL);
#endif
		if (SY==SYMBOLS::SEMICOLON)
		{
			INSYMBOL();
			if (!(FSYS+SYMBOLS::IDENT).in(SY))
			{
				CERROR(6);
				SKIP(FSYS+SYMBOLS::IDENT);
			}
		}
		else
			if (!((SY==SYMBOLS::ENDSY)&&(options.INMODULE)))
				CERROR(14);
	} /*CONSTDECLARATION*/
}

void DECLARATIONPART::TYPEDECLARATION(const SETOFSYS &FSYS)
{
	CTP LCP,LCP1,LCP2;
	STP LSP;
	LSP = NULL;
	ADDRRANGE LSIZE;
	WRITELN(OUTPUT,"DECLARATIONPART::TYPEDECLARATION");
	if (SY!=SYMBOLS::IDENT)
	{
		CERROR(2);
		SKIP(FSYS+SYMBOLS::IDENT);
	}
	SETOFSYS S0 = FSYS;
	S0+=SYMBOLS::SEMICOLON;
	while (SY==SYMBOLS::IDENT)
	{
		LCP = (identifier*) identifier::allocate(ID,NULL,TYPES);
		INSYMBOL();
		if ((SY==SYMBOLS::RELOP)&&(OP==EQOP))
			INSYMBOL();
		else
			CERROR(16);	
		TYP1(S0,LSP,LSIZE);
		ENTERID(LCP);
		LCP->IDTYPE=LSP;
		LCP1=FWPTR;
		while (LCP1!=NULL)
		{
			if (LCP1->NAME==LCP->NAME)
			{
				LCP1->IDTYPE->ELTYPE=LCP->IDTYPE;
				if (LCP1!=FWPTR)
					LCP2->NEXT=LCP1->NEXT;
				else
					FWPTR=LCP1->NEXT;
			}
			LCP2=LCP1;
			LCP1=LCP1->NEXT;
		}
		if (SY==SYMBOLS::SEMICOLON)
		{
			INSYMBOL();
			if (!(FSYS+SYMBOLS::IDENT).in(SY))
			{
				CERROR(6);
				SKIP(FSYS+SYMBOLS::IDENT);
			}
		}
		else if (!((SY==SYMBOLS::ENDSY)&& (options.INMODULE)))
			CERROR(14);
	}
	WRITELN(OUTPUT,"FINISHING <DECLARATIONPART::TYPEDECLARATION>");
#if 0
	for (int i=0;i<=TOP;i++)
	{
		WRITELN (OUTPUT,"IDENTIFIERS LEVEL = ",i);
		treesearch::printtree("TREE: ",DISPLAY[i].FNAME,i,ACTUALVARS,false);
		WRITELN (OUTPUT);
	}
	if (FWPTR!=NULL)
	{
		CERROR(117);
		identifier::debug1 (FWPTR,false);
		FWPTR=NULL;
	}
#endif
} /*TYPEDECLARATION*/ 

void DECLARATIONPART::VARDECLARATION(const SETOFSYS &FSYS)
{
	CTP LCP,NXT,IDLIST;
	STP LSP;
	ADDRRANGE LSIZE;
	LSP = NULL;
	NXT=NULL;
	WRITELN(OUTPUT,"DECLARATIONPART::VARDECLARATION");

	do {
		do {
			if (SY==SYMBOLS::IDENT)
			{
				LCP = (identifier*) identifier::allocate(ID,NULL,ACTUALVARS);
				LCP->NEXT=NXT;
				LCP->VLEV=LEVEL;
				if (options.INMODULE)
					if (options.ININTERFACE)
						LCP->PUBLIC=true;
					else
						LCP->PUBLIC=false;
				ENTERID(LCP);
				NXT=LCP;
				INSYMBOL();
			}
			else
				CERROR(2);

			if (!(FSYS+SYMBOLS::COMMA+SYMBOLS::COLON+BNF::TYPEDELS).in(SY))
			{
				CERROR(6);
				SKIP(FSYS+SYMBOLS::COMMA+SYMBOLS::COLON+
					SYMBOLS::SEMICOLON+BNF::TYPEDELS);
			}
			TEST=SY!=SYMBOLS::COMMA;
			if (!TEST)
				INSYMBOL();
		}
		while (!TEST);

		if (SY==SYMBOLS::COLON)
			INSYMBOL();
		else
			CERROR(5);
		IDLIST=NXT;

		LSP=NULL;
		TYP1((FSYS+SYMBOLS::SEMICOLON+BNF::TYPEDELS),LSP,LSIZE);
		structures::debug1 (LSP);
		
		CTP NXT1 = NXT;  // mark staring position
		while (NXT!=NULL)
		{
			NXT->IDTYPE=LSP;
			NXT->VADDR=LC;
			LC=LC + LSIZE;
			NXT=NXT->NEXT;
			// Bug in original source?  GLG 2022-08-15
			if ((NXT!=NULL)&&(NXT->NEXT==NULL))
				if (LSP!=NULL)
					if (LSP->is_file()==true)
					{ /*PUT IDLIST INTO LOCAL FILE LIST*/
						NXT->NEXT=DISPLAY[TOP].BLCK.FFILE;
						DISPLAY[TOP].BLCK.FFILE=IDLIST;
					}
			if (NXT==NXT1)
				break; // infinite loop - good luck!
		}
		if (SY==SYMBOLS::SEMICOLON)
		{
			INSYMBOL();
			if (!(FSYS+SYMBOLS::IDENT).in(SY))
			{
				CERROR(6);
				SKIP(FSYS+SYMBOLS::IDENT);
			}
		}
		else if (!((SY==SYMBOLS::ENDSY)&&(options.INMODULE)))
			CERROR(14);
	}
	while (!(SY!=SYMBOLS::IDENT)&&!(BNF::TYPEDELS.in(SY)));
	if (FWPTR!=NULL)
	{
		CERROR(117);
		FWPTR=NULL;
	}
	/*VARDECLARATION*/
}

void DECLARATIONPART::PROCDECLARATION
(
	const SETOFSYS &FSYS,
	SYMBOLS::SYMBOL FSY,
	bool SEGDEC,
	bool &NOTDONE
)
{
	/*PROCDECLARATION*/
	SYMBOLS::SYMBOL	LSY2;
	CTP			LCP,LCP1;
	STP			LSP;
	bool		EXTONLY,FORW;
	ADDRRANGE	LCM;
	LEXSTKREC	LLEXSTK;
	WRITELN(OUTPUT,"DECLARATIONPART::PROCDECLARATION");

	if (SEGDEC) /* SEGMENT DECLARATION */
	{
		if (CODEINSEG)
		{
			CERROR(399);
			SEGINX = 0;
			CURBYTE = 0;
		}
		LLEXSTK.DOLDSEG = SEG;
		SEG = NEXTSEG;
		LLEXSTK.SOLDPROC = NEXTPROC;
		NEXTPROC = 1;
		LSY2 = SY;
		if (SETOFSYS(2,SYMBOLS::PROCSY,SYMBOLS::FUNCSY).in(SY))
			INSYMBOL();
		else
		{
			CERROR(399);
			LSY2 = SYMBOLS::PROCSY;
		}
		FSY = LSY2;
	}
	LLEXSTK.DLLC=LC;
	LC=LCAFTERMARKSTACK;
	if (FSY==SYMBOLS::FUNCSY)
		LC=LC + REALSIZE;
	LINEINFO=LC;
	options.DP=true;
	EXTONLY=false;
	if (SY==SYMBOLS::IDENT)
	{
		if (options.USING||options.INMODULE&&options.ININTERFACE)
			FORW=false;
		else
		{
			SEARCHSECTION(DISPLAY[TOP].FNAME,LCP);
			if (LCP!=NULL)
			{
				if (LCP->KLASS==PROC1)
					FORW=LCP->FORWDECL&&(FSY==SYMBOLS::PROCSY)
					&&(LCP->PFKIND==ACTUAL);
				else
					if (LCP->KLASS==FUNC)
					FORW=LCP->FORWDECL&&(FSY==SYMBOLS::FUNCSY)&&(LCP->PFKIND==ACTUAL);
				else
					FORW=false;
				if (!FORW)
					CERROR(160);
			}
			else
				FORW=false;
		}
		if (!FORW)
		{
			LCP = (identifier*) identifier::allocate(ID,NULL,NONE);
			LCP->LOCALLC=LC;
			LCP->PFDECKIND=DECLARED;
			LCP->PFKIND=ACTUAL;
			LCP->INSCOPE=false;
			LCP->PFLEV=LEVEL;
			LCP->PFNAME=NEXTPROC;
			LCP->PFSEG=SEG;
			if (options.USING)
				PROCTABLE[NEXTPROC]=0;
			if (options.INMODULE)
				if (options.USING)
					LCP->IMPORTED=true;
				else
					LCP->IMPORTED=false;
			if (SEGDEC)
			{
				if (NEXTSEG>MAXSEG)
					CERROR(250);
				NEXTSEG=NEXTSEG+1;
				SEGTABLE[SEG].SEGNAME = ID;
			}
			if (NEXTPROC==MAXPROCNUM)
				CERROR(251);
			else
				NEXTPROC=NEXTPROC + 1;
			if (FSY==SYMBOLS::PROCSY)
				LCP->KLASS=PROC1;
			else
				LCP->KLASS=FUNC;

			ENTERID(LCP);
		}
		else
		{ 
			LCP1=LCP->NEXT;
			while (LCP1!=NULL)
			{
					// WITH LCP1^
				if (LCP->IDTYPE==NULL)
					EXTONLY=true;
				else if (LCP->KLASS==FORMALVARS)
				{
					LCM=LCP->VADDR + PTRSIZE;
					if (LCM>LC)
						LC=LCM;
				}
				else if (LCP->KLASS==ACTUALVARS)
				{
					LCM=LCP->VADDR+(int)LCP->IDTYPE->size();
					if (LCM>LC)
						LC=LCM;
				}
				LCP1=LCP1->NEXT;
			}
			if (SEG!=LCP->PFSEG)
			{
				SEG=LCP->PFSEG;
				NEXTPROC=2;
				if (!SEGDEC)
					CERROR(399);
			}
		}
		INSYMBOL();
	}
	else
	{
		CERROR(2);
		LCP=UPRCPTR;
	}
	// WITH LLEXSTK
	LLEXSTK.DOLDLEV = LEVEL;
	LLEXSTK.DOLDTOP = TOP;
	LLEXSTK.POLDPROC = CURPROC;
	LLEXSTK.DFPROCP = LCP;

	CURPROC=LCP->PFNAME;
	if (LEVEL<MAXLEVEL)
		LEVEL=LEVEL + 1;
	else
		CERROR(251);
	if (TOP<DISPLIMIT)
	{
		TOP=TOP + 1;
		if (FORW)
			DISPLAY[TOP].FNAME=LCP->NEXT;
		else
			DISPLAY[TOP].FNAME=NULL;
		DISPLAY[TOP].BLCK.FLABEL=NULL;
		DISPLAY[TOP].BLCK.FFILE=NULL;
		DISPLAY[TOP].OCCUR=BLCK;
	}
	else CERROR(250);
	if (FSY==SYMBOLS::PROCSY)
	{
		PARAMETERLIST(SETOFSYS(1,SYMBOLS::SEMICOLON),FSY,LCP1,LCP,EXTONLY,FORW);
		if (!FORW)
			LCP->NEXT=LCP1;
	}
	else
	{
		PARAMETERLIST(SETOFSYS(2,SYMBOLS::SEMICOLON,SYMBOLS::COLON),FSY,LCP1,LCP,EXTONLY,FORW);
		if (!FORW)
			LCP->NEXT=LCP1;
		if (SY==SYMBOLS::COLON)
		{
			INSYMBOL();
			if (SY==SYMBOLS::IDENT)
			{
				if (FORW)
					CERROR(122);
				SEARCHID(SETOFIDS(1,TYPES),LCP1);
				LSP=LCP1->IDTYPE;
				LCP->IDTYPE=LSP;
				if (LSP!=NULL)
				if (!(SET<128>(3,SCALAR,SUBRANGE,POINTER).in(LSP->form())))
				{
					CERROR(120);
					LCP->IDTYPE=NULL;
				}
				INSYMBOL();
			}
			else {
				CERROR(2);
				SKIP(FSYS+SYMBOLS::SEMICOLON);
			}
		}
		else if (!FORW)
			CERROR(123);
		}
		if (SY==SYMBOLS::SEMICOLON)
			INSYMBOL();
		else
			CERROR(14);
		LCP->EXTURNAL=false;
		if ((SY==SYMBOLS::EXTERNLSY)||((options.USING)&& (LSEPPROC)))
		{
			if (LEVEL!=2)
				CERROR(183); /*EXTERNAL PROCS MUST BE IN OUTERMOST BLOCK*/;
			if (options.INMODULE)
				if (options.ININTERFACE&&!options.USING)
					CERROR(184); /*NO EXTERNAL DECL IN INTERFACE*/
			if (SEGDEC)
				CERROR(399);
			// WITH LCP^

			LCP->EXTURNAL=true;
			LCP->FORWDECL=false;
			WRITELN(OUTPUT);
			char *name = &(LCP->NAME[0]);
			DWORD mem = MEMAVAIL();
			WRITELN(OUTPUT,name," [",mem," words]");
			WRITE(OUTPUT,"<",SCREENDOTS,">");

			PROCTABLE[CURPROC]=0;
			DLINKERINFO=true;
			if (SY==SYMBOLS::EXTERNLSY)
			{
				INSYMBOL();
				if (SY==SYMBOLS::SEMICOLON)
					INSYMBOL();
				else
					CERROR(14);
				if (!(FSYS.in(SY)))
				{
					CERROR(6);
					SKIP(FSYS);
				}
			}
		}
		else if (options.USING)
			LCP->FORWDECL=false;
		else if ((SY==SYMBOLS::FORWARDSY)||options.INMODULE&& options.ININTERFACE)
		{
			if (FORW)
				CERROR(161);
			else
				LCP->FORWDECL=true;
			if (SY==SYMBOLS::FORWARDSY)
			{
				INSYMBOL();
				if (SY==SYMBOLS::SEMICOLON)
					INSYMBOL();
				else
					CERROR(14);
			}
			if (!(FSYS.in(SY)))
			{
				CERROR(6);
				SKIP(FSYS);
			}
		}
		else
		{
			if (EXTONLY)
				CERROR(7);
			NEWBLOCK = true;
			NOTDONE = true;

	//		MARK(DMARKP);
			// WITH LCP^
			LCP->FORWDECL=false;
			LCP->INSCOPE=true;
			LCP->EXTURNAL=false;

			LLEXSTK.BFSY = SYMBOLS::SEMICOLON;
			LLEXSTK.ISSEGMENT = SEGDEC;
			LLEXSTK.PREVLEXSTACKP = TOS;

	//		NEW(TOS);
			TOS = (LEXSTKREC*) new LEXSTKREC; 
			*TOS = LLEXSTK;
			EXIT_CODE P("PROCDECLARATION");
			throw(P);
		}
	// WITH LLEXSTK  /* FORWARD||EXTERNAL DECLARATION, SO RESTORE STATE */

		LEVEL = LLEXSTK.DOLDLEV;
		TOP = LLEXSTK.DOLDTOP;
		LC = LLEXSTK.DLLC;
		CURPROC = LLEXSTK.POLDPROC;
		if (SEGDEC)
		{
			NEXTPROC = LLEXSTK.SOLDPROC;
			SEG = LLEXSTK.DOLDSEG;
		}
} /* PROCDECLARATION */

void DECLARATIONPART::PARAMETERLIST
(
	const SETOFSYS &FSYS,
	SYMBOLS::SYMBOL &FSY,
	CTP &FPAR,
	CTP FCP,
	bool &EXTONLY,
	bool FORW
)
{
	CTP LCP,LCP1,LCP2,LCP3;
	STP LSP; 
	IDKIND LKIND;
	ADDRRANGE LLC,LEN; 
	int COUNT;

	LCP1=NULL; LLC=LC;
	WRITELN(OUTPUT,"DECLARATIONPART::PARAMETERLIST");
#if 0
	FSYS.debug_list ("FSYS ");
#endif
	SETOFSYS PARAMSY = FSYS+SYMBOLS::LPARENT;
	SETOFSYS ENDPARAMS = FSYS+SYMBOLS::SEMICOLON+SYMBOLS::RPARENT;
	// skip over identifier - error in original source?
	if (SY==SYMBOLS::IDENT)
		INSYMBOL();
	// now we should have either a semicolon or a left paran

	if (!(PARAMSY).in(SY))
	{
		CERROR(7);
		SKIP(PARAMSY);
	}
	if (SY==SYMBOLS::LPARENT)
	{
		if (FORW)
			CERROR(119);
		INSYMBOL();
		if (!SETOFSYS(2,SYMBOLS::IDENT,SYMBOLS::VARSY).in(SY))
		{
			CERROR(7);
			SKIP(FSYS+SYMBOLS::IDENT+SYMBOLS::RPARENT);
		}
		while (SETOFSYS(2,SYMBOLS::IDENT,SYMBOLS::VARSY).in(SY))
		{
			if (SY==SYMBOLS::VARSY)
			{
				LKIND=FORMAL;
				INSYMBOL();
			}
			else
				LKIND=ACTUAL;
			LCP2=NULL;
			COUNT=0;
			do {
				if (SY!=SYMBOLS::IDENT)
					CERROR(2);
				else
				{
//					NEW(LCP,FORMALVARS,false);/*MAY BE ACTUAL(SAME SIZE)*/
					LCP = (identifier*) identifier::allocate(ID,NULL,NONE);
					LCP->NEXT=LCP2;
					if (LKIND==FORMAL)
						LCP->KLASS=FORMALVARS;
					else
						LCP->KLASS=ACTUALVARS;
					LCP->VLEV=LEVEL;
					ENTERID(LCP);
					LCP2=LCP;
					COUNT=COUNT + 1;
					INSYMBOL();
				}
				if (!(FSYS+SYMBOLS::COMMA+SYMBOLS::SEMICOLON+
						SYMBOLS::COLON).in(SY))
				{
					CERROR(7);
					SKIP(FSYS+SYMBOLS::COMMA+SYMBOLS::SEMICOLON+
						SYMBOLS::RPARENT+SYMBOLS::COLON);
				}
				TEST=SY!=SYMBOLS::COMMA;
				if (!TEST)
					INSYMBOL();
			}
			while (!TEST);
			LSP=NULL;
			if (SY==SYMBOLS::COLON)
			{
				INSYMBOL();
				if (SY==SYMBOLS::IDENT)
				{
					SEARCHID(SETOFIDS(1,TYPES),LCP);
					INSYMBOL();
					LSP=LCP->IDTYPE;
					LEN=PTRSIZE;
					if (LSP!=NULL)
						if (LKIND==ACTUAL)
							if (LSP->is_file()==true)
								CERROR(121);
							else
							if (LSP->form()<=POWER)
								LEN=(int)LSP->size();
					LC=LC+COUNT*LEN;
				}
				else
					CERROR(2);
			}
			else if (LKIND==FORMAL)
				EXTONLY=true;
			else
				CERROR(5);
			if (!(ENDPARAMS).in(SY))
			{
				CERROR(7);
				SKIP(ENDPARAMS);
			}
			LCP3=LCP2;
			LCP=NULL;
			while (LCP2!=NULL)
			{
				LCP=LCP2;
				LCP2->IDTYPE=LSP;
				LCP2=LCP2->NEXT;
			}
			if (LCP!=NULL)
			{
				LCP->NEXT=LCP1;
				LCP1=LCP3;
			}
			if (SY==SYMBOLS::SEMICOLON)
			{
				INSYMBOL();
				if (!(FSYS+SYMBOLS::IDENT+SYMBOLS::VARSY).in(SY))
				{
					CERROR(7);
					SKIP(ENDPARAMS);
				}
			}
		} /*while /*/ ;
		if (SY==SYMBOLS::RPARENT)
		{
			INSYMBOL();
			if (!(FSYS+FSY).in(SY))
			{
				CERROR(6);
				SKIP(FSYS+FSY);
			}
		}
		else
			CERROR(4);
		FCP->LOCALLC=LC; LCP3=NULL;
		while (LCP1!=NULL)
		{
			// WITH LCP1^
			LCP2=LCP1->NEXT;
			LCP1->NEXT=LCP3;
			if ((LCP1->IDTYPE!=NULL))
			if (LCP1->KLASS==FORMALVARS)
			{
				LCP1->VADDR=LLC;
				LLC=LLC + PTRSIZE;
			}
			else if (LCP1->KLASS==ACTUALVARS)
				if ((LCP1->IDTYPE->form()<=POWER))
				{
					LCP1->VADDR=LLC;
					LLC=LLC + (int)LCP1->IDTYPE->size();
				}
				else
				{
					LCP1->VADDR=LC;
					LC=LC + (int)LCP1->IDTYPE->size();
					LLC=LLC + PTRSIZE;
				}
			LCP3=LCP1;
			LCP1=LCP2;
		}
		FPAR=LCP3;
	}
	else
		FPAR=NULL;
} /*PARAMETERLIST*/


void DECLARATIONPART::ARRAY (const SETOFSYS &FSYS, stack_frame *param)
{
	union
	{
	struct /*locals*/
	{
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
	memcpy (fp,param,sizeof(stack_frame));

	WRITELN(OUTPUT,"SY==SYMBOLS::ARRAYSY");
	INSYMBOL();
	if (SY==SYMBOLS::LBRACK)
		INSYMBOL();
	else
		CERROR(11);
	LSP1=NULL;
	do {
		LSP = structure::allocate (ARRAYS);
		LSP->AELTYPE=LSP1;
		LSP->INXTYPE=NULL;
		LSP->set_string(false);
		if (PACKING)
		{
			LSP->set_packed(true);
			LSP->set_string(false);
		}
		else
			LSP->set_packed(false);

		LSP1=LSP;
		SIMPLETYPE(FSYS+SYMBOLS::COMMA+SYMBOLS::RBRACK+SYMBOLS::OFSY,LSP2,LSIZE);
		ASSERT(LSIZE>0);
		LSP1->resize (LSIZE);
		if (LSP2!=NULL)
			if (LSP2->form()<=SUBRANGE)
			{
				if (LSP2==REALPTR)
				{
					CERROR(109);
					LSP2=NULL;
				}
				else if (LSP2==INTPTR)
				{
					CERROR(149);
					LSP2=NULL;
				}
				LSP->INXTYPE=LSP2;
			}
			else
			{
				CERROR(113);
				LSP2=NULL;
			}
		TEST=SY!=SYMBOLS::COMMA;
		if (!TEST)
			INSYMBOL();
	}
	while (!TEST);
	if (SY==SYMBOLS::RBRACK)
		INSYMBOL();
	else
		CERROR(12);
	if (SY==SYMBOLS::OFSY)
		INSYMBOL();
	else
		CERROR(8);
// YES - RECURSUVE!
	TYP1(FSYS,LSP,LSIZE);
	if (LSP!=NULL)
		if (LSP->is_file()==true)
		CERROR(108);

	if (PACKABLE(param,LSP))
		if (int(NUMBITS) + int(NUMBITS)<=BITSPERWD)
		{
			LSP1->set_packed(true);
			LSP1->ELSPERWD=BITSPERWD/NUMBITS;
			LSP1->ELWIDTH=NUMBITS;
		}
	do 
	{
		LSP2=LSP1->AELTYPE;
		LSP1->AELTYPE=LSP;
		if (LSP1->INXTYPE!=NULL)
		{
			GETBOUNDS(LSP1->INXTYPE,LMIN,LMAX);
			if (LSP1->ELSPERWD==0)
				LSP1->ELSPERWD=1;
			if (LSP1->is_packed())
				LSIZE=(LMAX-LMIN+LSP1-> ELSPERWD)/LSP1->ELSPERWD;
			else
			{
				ASSERT(LSIZE>0);
				LSIZE=LSIZE*(LMAX-LMIN+1);
			}
			if (LSIZE<=0)
			{
				CERROR(398);
				LSIZE=1;//fixme?
			}
			LSP1->resize(LSIZE);
		}
		LSP=LSP1;
		LSP1=LSP2;
	}
	while (!LSP1==NULL);
	memcpy (param,fp,sizeof(stack_frame));
}

void DECLARATIONPART::TYP1(const SETOFSYS &FSYS, STP &FSP, ADDRRANGE &FSIZE)
{
	union
	{
	struct /*locals*/
	{
//		STP			FSP1;
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
	memset(fp,0,sizeof(stack_frame));
	stack_frame *param = reinterpret_cast<stack_frame*>(fp);
	debug_stack ("DECLARATIONPART::TYP1",param);

	SETOFSYS S4 = FSYS;
	S4+=SYMBOLS::ENDSY;
	S4-=SYMBOLS::SEMICOLON;

	/*TYP*/

	PACKING=false;
	if (!(BNF::TYPEBEGSYS.in(SY)))
	{
		CERROR(10);
		SKIP(FSYS + BNF::TYPEBEGSYS);
	}
	if (!BNF::TYPEBEGSYS.in(SY))
	{
		FSP=NULL;
		FSIZE=1;
		return;
	}
	if (BNF::SIMPTYPEBEGSYS.in(SY))
		SIMPLETYPE(FSYS,FSP,FSIZE);

	else if (SY==SYMBOLS::ARROW)
	{
//#define NOPPOITERS
#ifndef NOPOPINTEERS
 		POINTERTYPE(param);
		FSP=LSP;
#endif
	}
	else 
	{
		if (SY==SYMBOLS::PACKEDSY)
		{
//#define NOPACKED
#ifndef NOPACKED
			WRITELN(OUTPUT,"SY==SYMBOLS::PACKEDSY");
			INSYMBOL();
			PACKING=true;
			if (!(BNF::TYPEDELS.in(SY)))
			{
				CERROR(10);
				SKIP(FSYS + BNF::TYPEDELS);
			}
#endif
		}
		switch (SY)
		{
//#define NOARRAYS
#ifndef NOARRAYS
			case SYMBOLS::ARRAYSY:
				WRITELN(OUTPUT,"SY==SYMBOLS::ARRAYSY");
				INSYMBOL();
				if (SY==SYMBOLS::LBRACK)
					INSYMBOL();
				else
					CERROR(11);
				LSP1=NULL;
				do {
					LSP = structure::allocate (ARRAYS);
					LSP->AELTYPE=LSP1;
					LSP->INXTYPE=NULL;
					LSP->set_string(false);
					if (PACKING)
					{
						LSP->set_packed(true);
						LSP->set_string(false);
					}
					else
						LSP->set_packed(false);

					LSP1=LSP;
					SIMPLETYPE(FSYS+SYMBOLS::COMMA+SYMBOLS::RBRACK+SYMBOLS::OFSY,LSP2,LSIZE);
					ASSERT(LSIZE>0);
					LSP1->resize (LSIZE);
					if (LSP2!=NULL)
						if (LSP2->form()<=SUBRANGE)
						{
							if (LSP2==REALPTR)
							{
								CERROR(109);
								LSP2=NULL;
							}
							else if (LSP2==INTPTR)
							{
								CERROR(149);
								LSP2=NULL;
							}
							LSP->INXTYPE=LSP2;
						}
						else
						{
							CERROR(113);
							LSP2=NULL;
						}
					TEST=SY!=SYMBOLS::COMMA;
					if (!TEST)
						INSYMBOL();
				}
				while (!TEST);
				if (SY==SYMBOLS::RBRACK)
					INSYMBOL();
				else
					CERROR(12);
				if (SY==SYMBOLS::OFSY)
					INSYMBOL();
				else
					CERROR(8);
			// YES - RECURSUVE!
				TYP1(FSYS,LSP,LSIZE);
				if (LSP!=NULL)
					if (LSP->is_file()==true)
					CERROR(108);

				if (PACKABLE(param,LSP))
					if (int(NUMBITS) + int(NUMBITS)<=BITSPERWD)
					{
						LSP1->set_packed(true);
						LSP1->ELSPERWD=BITSPERWD/NUMBITS;
						LSP1->ELWIDTH=NUMBITS;
					}
				do 
				{
					LSP2=LSP1->AELTYPE;
					LSP1->AELTYPE=LSP;
					if (LSP1->INXTYPE!=NULL)
					{
						GETBOUNDS(LSP1->INXTYPE,LMIN,LMAX);
						if (LSP1->ELSPERWD==0)
							LSP1->ELSPERWD=1;
						if (LSP1->is_packed())
							LSIZE=(LMAX-LMIN+LSP1->ELSPERWD)/LSP1->ELSPERWD;
						else
						{
 //							ASSERT(LSIZE>0);
							if (LSIZE==0)
								LSIZE=1;
							LSIZE=LSIZE*(LMAX-LMIN+1);
						}
						if (LSIZE<=0)
						{
							CERROR(398);
							LSIZE=1;//fixme?
						}
						LSP1->resize(LSIZE);
					}
					LSP=LSP1;
					LSP1=LSP2;
				}
				while (!LSP1==NULL);
				break;
#endif		
//#define NORECORDS
#ifndef NORECORDS
			case SYMBOLS::RECORDSY:
				WRITELN(OUTPUT,"SY==SYMBOLS::RECORDSY");	
				INSYMBOL();
				OLDTOP=TOP;
				if (TOP<DISPLIMIT)
				{
					TOP=TOP + 1;
					// WITH DISPLAY[TOP]
					DISPLAY[TOP].FNAME=NULL;
					DISPLAY[TOP].OCCUR=REC;
				}
				else
					CERROR(250);
				DISPL=0;
				NEXTBIT=0;			
				FIELDLIST(param,S4,LSP1);
				DISPL=DISPL + ORD(NEXTBIT>0);
				LSP = structure::allocate (RECORDS,DISPL);
				LSP->FSTFLD=DISPLAY[TOP].FNAME;
				LSP->RECVAR=LSP1;
				TOP=OLDTOP;
				if (SY==SYMBOLS::ENDSY)
					INSYMBOL();
				else
					CERROR(13);
				break;
#endif
//#define NOSETS
#ifndef NOSETS
			case SYMBOLS::SETSY:
				WRITELN(OUTPUT,"SY==SYMBOLS::SETSY");
				INSYMBOL();
				if (SY==SYMBOLS::OFSY)
					INSYMBOL();
				else
					CERROR(8);
				SIMPLETYPE(FSYS,LSP1,LSIZE);
				if (LSP1!=NULL)
					if ((LSP1->form()>SUBRANGE)||
					(LSP1==INTPTR)||(LSP1==REALPTR))
					{
						CERROR(115);
						LSP1=NULL;
					}
					else if (LSP1==REALPTR)
					{
						CERROR(114);
						LSP1=NULL;
					}
				LSP = structure::allocate (POWER);
				LSP->ELSET=LSP1;
				if (LSP1!=NULL)
				{
					GETBOUNDS(LSP1,LMIN,LMAX);
					LSP->resize((LMAX+BITSPERWD)/BITSPERWD);
					if (LSP->size()>255)
					{
						CERROR(169);
						LSP->resize(1);
					}
				}
				else
					LSP->resize(0);
				break;
#endif
//#define NOFILES
#ifndef NOFILES
			case SYMBOLS::FILESY:
				WRITELN(OUTPUT,"SY==SYMBOLS::FILESY");
				if (options.INMODULE)
					if (!options.ININTERFACE)
						CERROR(191); /*NO PRIVATE FILES*/
				INSYMBOL();
				LSP = structure::allocate (FILES);
				LSP->FILTYPE=NULL;
				if (SY==SYMBOLS::OFSY)
				{
					INSYMBOL();
					TYP1(FSYS,LSP1,LSIZE);
				}
				else
					LSP1=NULL;
				LSP->FILTYPE=LSP1;
				if (LSP1!=NULL)
					LSP->resize(FILESIZE + LSP1->size());
				else
					LSP->resize(NILFILESIZE);
				break;
#endif
			default:
				break;			
		}
		FSP=LSP;
		if (!(FSYS.in(SY)))
		{
			CERROR(6);
			SKIP(FSYS);
		}
	}
	if (FSP==NULL)
		FSIZE=1;
	else
		FSIZE=(int)FSP->size();
}/*TYP*/

