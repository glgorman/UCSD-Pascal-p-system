#include "stdafx.h"

//#include "../Frame Lisp/symbol_table.h"
#include "../Frame Lisp/btreetype.h"
//#include "../Frame Lisp/node_list.h"
//#include "../Frame Lisp/text_object.h"

#include "compiler.h"
#include "unitpart.h"

void LINKERINFO::GETNEXTBLOCK()
{
	CURRENTBLOCK=CURRENTBLOCK + 1;
	if (CURRENTBLOCK>REFBLK)
		CURRENTBLOCK=0;
	
	int ioresult = 1;
//		const char *data = (const char*)(&list[0]);
	_REFVALUE_ *list = (_REFVALUE_*)(&REFLIST[0]);

	SYSCOMM::BLOCKREAD(&REFFILE,(char*)(list),1,CURRENTBLOCK);
	if (ioresult!=1)
	{
		ASSERT(0);	
	}
} /*GETNEXTBLOCK*/

void LINKERINFO::GETREFS(int ID, int LENGTH)
{
	/*GETREFS*/
	int J,MAX,BLOCKCOUNT,COUNT;
	ADDRRANGE LIC;

	if ((NREFS==1)&&(REFBLK==0))
	{
		EXIT_CODE E("GETREFS");
		throw (E);
	}
	COUNT=0;
	
	for (BLOCKCOUNT=0;BLOCKCOUNT<=REFBLK;BLOCKCOUNT++)
	{
		if (CURRENTBLOCK<REFBLK)
			MAX=REFSPERBLK;
		else
			MAX=NREFS-1;
		for (J=1;J<=MAX;J++)
		{
			if (ID==REFLIST[J]->KEY)
			{
				GENWORD(REFLIST[J]->OFFSET);
				COUNT=COUNT+1;
				if (BLOCKCOUNT<REFBLK)
					GETNEXTBLOCK();
			};
			LIC=IC;
			IC=FIC;
			GENWORD(COUNT);
			IC=LIC;
/*NOW FILL REST) 8-WORD RECORD*/
			for (J=1;J<=((8-(COUNT%8))%8);J++)
				GENWORD(0);
		}
	}
} /* GETREFS */

void LINKERINFO::WRITELINKERINFO(bool DECSTUFF)
{
	CTP	FCP;
	NONRESIDENT I;
 /*WRITELINKERINFO*/
	IC=0;
	if (CODEINSEG)
		CERROR(399);
	
	if (options.INMODULE)
		CURRENTBLOCK=REFBLK;

	if (DECSTUFF) /*SKIP if (NO DECLARATIONPART LINKER INFO*/
	{
		FCP=DISPLAY[GLEV].FNAME;
		if (FCP!=NULL)
			GLOBALSEARCH(FCP);
	};
/*NOW) { NONRESIDENT PROCS*/
//	WITH LIREC
	for (I=SEEK;I<=DECOPS;I=(NONRESIDENT)((int)I+1))
	{
	if (PFNUMOF[I]!=0)
	{
		switch (I)
		{
		case SEEK:
			strcpy_s(LIREC.LINAME,15,"FSEEK   ");
			LIREC.NPARAMS=2;
			break;

		case FREADREAL:
			strcpy_s(LIREC.LINAME,15,"FREADREA");
			LIREC.NPARAMS=2;
			break;

		case FWRITEREAL:
			strcpy_s(LIREC.LINAME,15,"FWRITERE");
			LIREC.NPARAMS=5;
			break;

		case FREADDEC:
			strcpy_s(LIREC.LINAME,15,"FREADDEC");
			LIREC.NPARAMS=3;
			break;

		case FWRITEDEC:
			strcpy_s(LIREC.LINAME,15,"FWRITEDE");
			LIREC.NPARAMS=10;
			break;

		case DECOPS:
			strcpy_s(LIREC.LINAME,15,"DECOPS  ");
			LIREC.NPARAMS=0;

		default:
			break;
		}
	};
	for (LGTH=1;LGTH<=8;LGTH++)
		GENBYTE(ORD(LIREC.LINAME[LGTH]));
	if (SEPPROC)
	{
		GENWORD(ORD(SEPPREF));
		GENWORD(ORD(BITE));
		FIC=IC;
		GENWORD(0);
		GENWORD(LIREC.NPARAMS);
		GETREFS(-PFNUMOF[I],1);
	}
	else
	{
		GENWORD(ORD(EXTPROC));
		GENWORD(PFNUMOF[I]);
		GENWORD(LIREC.NPARAMS);
		GENWORD(0);
	};
	PFNUMOF[I]=0;
   };
/* NOW) { EOFMARK }-RECORD*/
   for (LGTH=1;LGTH<=8;LGTH++)
   {
		GENBYTE(ORD(' '));
		GENWORD(ORD(EOFMARK));
		GENWORD(LCMAX);
		GENWORD(0);
		GENWORD(0);
		WRITECODE(true);
		CLINKERINFO=false;
		if (DECSTUFF)
			DLINKERINFO=false;
   }
} /*WRITELINKERINFO*/;

void UNITPART::OPENREFFILE()
{
	SYSCOMM::REWRITE(&REFFILE,"*SYSTEM.INFO[*]");
	if (SYSCOMM::IORESULT()!=0)
		CERROR(402);
} /* OPENREFFILE */

void UNITPART::UNITDECLARATION(SETOFSYS FSYS, TESTP &UMARKP)
{
	CTP		LCP;
	bool	FOUND;
	LEXSTKREC	LLEXSTK;
	if (options.INMODULE)
		CERROR(182); /* NESTED MODULES !ALLOWED */

	if (CODEINSEG)
	{
		CERROR(399);
		SEGINX=0;
		CURBYTE=0;
	};
//     WITH LLEXSTK
	LLEXSTK.DOLDTOP=TOP;
	LLEXSTK.DOLDLEV=LEVEL;
	LLEXSTK.POLDPROC=CURPROC;
	LLEXSTK.SOLDPROC=NEXTPROC;
	LLEXSTK.DOLDSEG=SEG;
	LLEXSTK.DLLC=LC;
	LLEXSTK.PREVLEXSTACKP=TOS;

	SEG=NEXTSEG;
	NEXTSEG++;
	if (NEXTSEG>MAXSEG)
		CERROR(250);
		NEXTPROC=1;
		PUBLICPROCS=false;
		options.INMODULE=true;
		INSYMBOL();
		if (SY!=SYMBOLS::IDENT)
			CERROR(2);
		else
			FOUND=false;
		LCP=MODPTR;
		while ((LCP!=NULL)&&!FOUND)
		{
			if (LCP->NAME!=ID)
				LCP=LCP->NEXT;
			else
			{ 
				FOUND=true;
				CERROR(101);
			};
			if (!FOUND)
			{
//				NEW(LCP,MODULE);
// WITH LCP^ //
				LCP = new (IDENTIFIER);
				strcpy_s(LCP->NAME,15,ID);
				LCP->IDTYPE=NULL;
				LCP->NEXT=MODPTR;
				LCP->KLASS=MODULE;
				LCP->SEGID=SEG;
				MODPTR=LCP;
			};
		};
		strcpy_s(SEGTABLE[SEG].SEGNAME,1,ID);
//		MARK(UMARKP);
//		NEW(REFLIST);
//		NEW(TOS);
		REFLIST = (REFARRAY*) new REFARRAY;
		TOS = new (LEXSTKREC);
		*TOS=LLEXSTK;
		LEVEL=1;
		if (TOP<DISPLIMIT)
		{
			TOP=TOP +1;
//     WITH DISPLAY[TOP]
			DISPLAY[TOP].FNAME=NULL;
			DISPLAY[TOP].BLCK.FFILE=NULL;
			DISPLAY[TOP].BLCK.FLABEL=NULL;
			DISPLAY[TOP].OCCUR=BLCK;
//};
		if (LCP!=NULL)
			ENTERID(LCP);
	}
	else
		CERROR(250);
	INSYMBOL();
	if (SY==SYMBOLS::SEMICOLON)
		INSYMBOL();
	else
		CERROR(14);
} /*UNITDECLARATION*/

// SEGMENT
UNITPART::UNITPART(SETOFSYS FSYS)
{
	TESTP UMARKP;
	OPENREFFILE();
	SYSCOMM::RESET(&REFFILE,NULL);
	do {
		NREFS=1;
		REFBLK=0;
		if ((SY==SYMBOLS::SEPARATSY))
		{
			SEPPROC=true;
			INSYMBOL();
			if (SY!=SYMBOLS::UNITSY)
				CERROR(24);
		}
		else
		SEPPROC=false;
		UNITDECLARATION(FSYS,UMARKP);
		if (SEPPROC)
			SEGTABLE[SEG].SEGKIND=4;
		else
			SEGTABLE[SEG].SEGKIND=3;
		SEGTABLE[SEG].TEXTADDR=CURBLK;
		WRITETEXT();
		if (SY==SYMBOLS::INTERSY)
			INSYMBOL();
		else
			CERROR(22);
		options.ININTERFACE=true;
		DECLARATIONS(FSYS);
		if (PUBLICPROCS)
		{
			options.ININTERFACE=false;
			if (SY!=SYMBOLS::IMPLESY)
			{
				CERROR(23);
				SKIP(FSYS-STATBEGSYS);
			}
			else
				INSYMBOL();
			BLOCK(FSYS - SET(4,SYMBOLS::SEPARATSY,SYMBOLS::UNITSY,
				SYMBOLS::INTERSY,SYMBOLS::IMPLESY));
			if (REFBLK>0)
			{
				const char *data = (const char*)(&REFLIST[0]);
				if (SYSCOMM::BLOCKWRITE(&REFFILE,data,1,REFBLK)!=1)
					CERROR(402);
			}
			WRITELINKERINFO(true);
		}
		else
		{
			DLINKERINFO=false;
//	 WITH SEGTABLE[SEG]
			SEGTABLE[SEG].CODELENGTH=0;
			SEGTABLE[SEG].DISKADDR  = CURBLK;
			SEGTABLE[SEG].SEGKIND=0;
		}
		SEPPROC=false; /*false WHENEVER !INMODULE*/
		options.INMODULE=false;
		if (SY==SYMBOLS::ENDSY)
			INSYMBOL();
		else
		{
			CERROR(13);
			SKIP(FSYS);
		}
		if (SY!=SYMBOLS::PERIOD)
			if (SY==SYMBOLS::SEMICOLON)
				INSYMBOL();
			else
				CERROR(14);
// WITH TOS^
		TOP=TOS->DOLDTOP;
		LEVEL=TOS->DOLDLEV;
		CURPROC=TOS->POLDPROC;
		NEXTPROC=TOS->SOLDPROC;
		SEG=TOS->DOLDSEG;
		LC=TOS->DLLC;
		TOS=TOS->PREVLEXSTACKP;
		RELEASE(UMARKP);
	}
	while (SET(2,SYMBOLS::UNITSY,SYMBOLS::SEPARATSY).in(SY));
	SYSCOMM::CLOSE(&REFFILE);
} /*UNITPART*/

void LINKERINFO::GLOBALSEARCH(CTP FCP)     
{
	bool NEEDEDBYLINKER;
	NEEDEDBYLINKER=true;
	GLOBALSEARCH0(FCP);

// WITH LIREC
	if (NEEDEDBYLINKER)
	{
		if (SEGTABLE[SEG].SEGKIND==2) /*SEGPROC*/
		if ((LIREC.LITYPE==CONSTDEF)||(LIREC.LITYPE==PUBLICDEF))
			NEEDEDBYLINKER=false;
	}
	if (NEEDEDBYLINKER)
	{
		strcpy_s(LIREC.LINAME,FCP->NAME);
		for (LGTH=1;LGTH<=8;LGTH++)
		{
			GENBYTE(ORD(LIREC.LINAME[LGTH]));
		}
		GENWORD(ORD(LIREC.LITYPE));
		GLOBALSEARCH1(FCP);
	
		if (IC>=1024)
		{
			WRITECODE(false);
			IC=0;
		}
		if (FCP->LLINK!=NULL)
			GLOBALSEARCH(FCP->LLINK);
	
		if (FCP->RLINK!=NULL)
			GLOBALSEARCH(FCP->RLINK);
	}
} /*GLOBALSEARCH*/

void LINKERINFO::GLOBALSEARCH0(CTP FCP)
{
	bool NEEDEDBYLINKER;
	NEEDEDBYLINKER=true;
//	WITH LIREC,FCP^
	switch (FCP->KLASS)
	{
	case TYPES:
		NEEDEDBYLINKER=false;
		break;

	case KONST:
		if ((FCP->IDTYPE->SIZE==1)&& !options.INMODULE)
		{
			LIREC.LITYPE=CONSTDEF;
			LIREC.CONSTANT=FCP->VALUES.IVAL;
		}
		else
			NEEDEDBYLINKER=false;
		break;

	case FORMALVARS:
	case ACTUALVARS:
		if (options.INMODULE)
		{
			if (FCP->PUBLIC)
			{
				LIREC.LITYPE=PUBBLIC;
				LIREC.NWORDS=0;
			}
			else
			{
				LIREC.LITYPE=PRIVVATE;
				if (FCP->KLASS==FORMALVARS)
					LIREC.NWORDS=PTRSIZE;
				else
					LIREC.NWORDS=FCP->IDTYPE->SIZE;
			}
			LIREC.FORMAT=BIG;
		}
		else
		{
			LIREC.LITYPE=PUBLICDEF;
			LIREC.BASEOFFSET=FCP->VADDR;
		}
		break;

	case FIELD:
		NEEDEDBYLINKER=false;
		break;

	case PROC1:
	case FUNC:
		if (FCP->PFDECKIND==DECLARED)
			if (FCP->PFKIND==ACTUAL)
				if (FCP->KLASS==PROC1)
					if (FCP->EXTURNAL)
						if (SEPPROC)
							LIREC.LITYPE=SEPPREF;
						else
							LIREC.LITYPE=EXTPROC;
					else
						if (SEPPROC)
							LIREC.LITYPE=SSEPPROC;
						else
							NEEDEDBYLINKER=false;
				else /*KLASS==FUNC*/
					if (FCP->EXTURNAL)
						if (SEPPROC)
							LIREC.LITYPE=SEPFREF;
						else
							LIREC.LITYPE=EXTFUNC;
					else
						if (SEPPROC)
							LIREC.LITYPE=SSEPFUNC;
						else NEEDEDBYLINKER=false;
			else
				NEEDEDBYLINKER=false;
		else
			NEEDEDBYLINKER=false;
		if (NEEDEDBYLINKER)
		{
			LCP=FCP->NEXT;
			LIREC.NPARAMS=0;
			while (LCP!=NULL)
			{
				// WITH LCP^)
				if (FCP->KLASS==FORMALVARS)
					LIREC.NPARAMS=LIREC.NPARAMS + PTRSIZE;
				else
					if (FCP->KLASS==ACTUALVARS)
						if (LCP->IDTYPE->FORM<=POWER)
							LIREC.NPARAMS=LIREC.NPARAMS + LCP->IDTYPE->SIZE;
						else
							LIREC.NPARAMS=LIREC.NPARAMS + PTRSIZE;
				LCP=LCP->NEXT;

				if (SET(2,SEPPREF,SEPFREF).in(LIREC.LITYPE))
				{
					LIREC.FORMAT=BITE;
					LIREC.NWORDS=LIREC.NPARAMS;
				}
				else
				{
					LIREC.PROCNUM=LCP->PFNAME;
					LIREC.RANGE=NULL;
				}
			} /*PROC,FUNC*/;
		}
		break;

	case MODULE:
		if (!options.INMODULE)
			NEEDEDBYLINKER=false;
		else
			LIREC.LITYPE=MODDULE;
		LIREC.NWORDS=0;
		LIREC.FORMAT=BITE;
		break;
	default:
		break;
	}
}

void LINKERINFO::GLOBALSEARCH1(CTP FCP)
{
	bool NEEDEDBYLINKER;
	NEEDEDBYLINKER=true;
	switch (LIREC.LITYPE)
	{
	case MODDULE:
	case PUBBLIC:
	case PRIVVATE:
	case SEPPREF:
	case SEPFREF:
		GENWORD(ORD(LIREC.FORMAT));
		FIC=IC;
		GENWORD(0);
		GENWORD(LIREC.NWORDS);
		if (LIREC.LITYPE==MODDULE)
			GETREFS(FCP->SEGID,1);
		else
			if (SET(SEPPREF,SEPFREF).in(LIREC.LITYPE))
				GETREFS(-FCP->PFNAME,1);
			else
				GETREFS(FCP->VADDR + 32,FCP->IDTYPE->SIZE);
		break;

	case CONSTDEF:
		GENWORD(LIREC.CONSTANT);
		GENWORD(0);
		GENWORD(0);
		break;

	case PUBLICDEF:
		GENWORD(LIREC.BASEOFFSET);
		GENWORD(0);
		GENWORD(0);
		break;

	case EXTPROC:
	case EXTFUNC:
		GENWORD(LIREC.PROCNUM);
		GENWORD(LIREC.NPARAMS);
		GENWORD(ORD(LIREC.RANGE));
		break;

	case SSEPPROC:
	case SSEPFUNC:
		GENWORD(LIREC.PROCNUM);
		GENWORD(LIREC.NPARAMS);
		GENWORD(ORD(LIREC.RANGE));
		for (LGTH=1;LGTH<=8;LGTH++)
		{
			GENBYTE(ORD(LIREC.LINAME[LGTH]));
			if (LIREC.LITYPE==SSEPPROC)
				GENWORD(ORD(SEPPREF));
			else GENWORD(ORD(SEPFREF));
			GENWORD(ORD(BITE));
			FIC=IC;
			GENWORD(0);
			GENWORD(LIREC.NPARAMS);
			GETREFS(-LIREC.PROCNUM,1);
		}
	}
}

void LINKERINFO::GLOBALSEARCH2(CTP FCP)
{
	bool NEEDEDBYLINKER;
	NEEDEDBYLINKER=true;
}

void LINKERINFO::GLOBALSEARCH3(CTP FCP)
{
	bool NEEDEDBYLINKER;
	NEEDEDBYLINKER=true;
}
