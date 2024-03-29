
#include "stdafx.h"
#include "../Frame Lisp/btreetype.h"
#include "insymbol.h"
#include "../Frame Lisp/objects.h"
#include "compilerdata.h"
#include "declarationpart.h"
#include "bodypart.h"
#include "compiler.h"

struct pcode
{
	char *mnemonic;
	int	 opcode;
	pcode (char *str, int op)
	{
		mnemonic=str;
		opcode = op;;
	}
};

pcode pcodes[] =
{
	pcode("CSP",0),
	pcode("INIT",1),
	pcode("ADI",2),
	pcode("ADI",3),
	pcode("",4),
	pcode("",5),
	pcode("TURTLE",6),
	pcode("DVR",7),
	pcode("CHK",8),
	pcode("FLO",9),
	pcode("FLT",10),
	pcode("",11),
	pcode("INT",12),
	pcode("IOR",13),
	pcode("",14),
	pcode("",15),
	pcode("",16),
	pcode("",17),
	pcode("",18),
	pcode("",19),
	pcode("",20),
	pcode("GETSEG",21),
	pcode("RELSEG",22),
	pcode("",23),
	pcode("SQI",24),
	pcode("SQR",25),
	pcode("STO",26),
	pcode("IXS",27),
	pcode("UNI",28),
	pcode("",29),
	pcode("CSP",30),
	pcode("LDCN",31),
	pcode("ADJ",32),
	pcode("",33),
	pcode("",34),
	pcode("IND",35),
	pcode("",36),
	pcode("LAO",37),
	pcode("LCA",38), // load character array?
	pcode("LDO",39),
	pcode("MOV",40),
	pcode("MVB",41),
	pcode("SAS",42),
	pcode("SRO",43),
	pcode("",44),
	pcode("RNP",45),
	pcode("CIP",46),
	pcode("EQU",47),
	pcode("GEQ",48),
	pcode("GRT",49),
	pcode("LDA",50),
	pcode("LDC",51),
	pcode("LEQ",52),
	pcode("LES",53),
	pcode("LOD",54),
	pcode("NEQ",55),
	pcode("STR",56),
	pcode("UJP",57),
	pcode("",58),
	pcode("STP",59),
	pcode("LDM",60),
	pcode("STM",61),
	pcode("",62),
	pcode("STB",63),
	pcode("IXP",64),
	pcode("RBP",65),
	pcode("CBP",66),
	pcode("",67),
	pcode("",68),
	pcode("",69),
	pcode("",70),
	pcode("",71),
	pcode("",72),
	pcode("",73),
	pcode("LDL",74),
	pcode("",75),
	pcode("",76),
	pcode("CXP",77),
	pcode("CLP",78),
	pcode("CGP",79),
	pcode("",80),
	pcode("",81),
	pcode("",82),
	pcode("",84),
	pcode("BPT",85),
	pcode("XIT",86),
	pcode("",87),
	pcode("",88),
	pcode("",89),
	pcode("",90),
	pcode("",91),
	pcode("",92),
	pcode("",93),
	pcode("",94),
	pcode("",95),
//    pcode("",0),
};

void BODYPART::MAIN(const SETOFSYS &FSYS, CTP FPROCP)
{	
// locals
	ADDRRANGE LLC1,EXITIC;
	CTP LCP;
	OPRANGE LOP;
	LABELP LLP;
	int LMIN,LMAX;
	JTABRANGE JTINX;
	int temp;
	int *DUMMYVAR;
	DUMMYVAR = &temp;

	WRITELN(OUTPUT,"BODYPART::MAIN");
#if 0
	FSYS.debug_list();
#endif

	/*for(PRETTY DISPLAY)STACK&&HEAP*/
	if ((options.NOSWAP)&&(STARTINGUP))
	{
		/* BRING IN DECLARATIONPART */
		DECLARATIONPART::MAIN(FSYS);
		EXIT_CODE E("BODYPART");
		throw(E);
	}
	NEXTJTAB=1;
	if (options.NOISY)
	{
		WRITELN(OUTPUT);
#if 0
		if (!options.NOSWAP)/*MUST ADJUST DISPLAY)STACK&&HEAP*/
			SYSCOMM::UNITWRITE(3,DUMMYVAR[-1600],35);
#endif
		DUMMYVAR[0] = MEMAVAIL();
		if (DUMMYVAR[0]<SMALLESTSPACE)
			SMALLESTSPACE=DUMMYVAR[0];
		if (FPROCP!=NULL)
			WRITELN(OUTPUT,FPROCP->NAME," [",DUMMYVAR[0]," words]");
		::WRITE(OUTPUT,"<",SCREENDOTS,">");
	}
	if (FPROCP!=NULL)
	{
		LLC1 = FPROCP->LOCALLC;
		LCP = FPROCP->NEXT;
		while (LCP!=NULL) // with (LCP)
		{
			if (LCP->IDTYPE!=NULL)
			if ((LCP->KLASS==ACTUALVARS))
			if ((LCP->IDTYPE->form() > POWER))
			{ 
				LLC1 = LLC1 - PTRSIZE;
				GEN2(50/*LDA*/,0,LCP->VADDR);
				GEN2(54/*LOD*/,0,LLC1);
				if (PAOFCHAR(LCP->IDTYPE))
				if (LCP->IDTYPE->is_string())
					GEN1(42/*SAS*/,LCP->IDTYPE->MAXLENG);
				else if (LCP->IDTYPE->INXTYPE!=NULL)
				{ 
					GETBOUNDS(LCP->IDTYPE->INXTYPE,LMIN,LMAX);
					GEN1(41/*MVB*/,LMAX - LMIN + 1);
				}
				else;
				else
					GEN1(40/*MOV*/,(int)LCP->IDTYPE->size());
			}
			else
				LLC1 = LLC1 - (int)LCP->IDTYPE->size();
			else if (LCP->KLASS==FORMALVARS)
				LLC1 = LLC1 - PTRSIZE;
			LCP = LCP->NEXT;
		}
	}
	STARTDOTS = SCREENDOTS;
	LCMAX = LC;
	LLP = DISPLAY[TOP].BLCK.FLABEL;
	while (LLP!=NULL)
	{
		GENLABEL(&LLP->CODELBP);
		LLP = LLP->NEXTLAB;
	}
	if (!options.INMODULE)
	if (LEVEL==1)
	{
		LCP = USINGLIST;
		while (LCP!=NULL)
		{
			if (LCP->SEGID>=0)
			{
				GENLDC(LCP->SEGID);
				GEN1(30/*CSP*/,21/*GETSEG*/);             
			}
			LCP = LCP->NEXT;
		}
		if (USERINFO.STUPID)
			GEN2(77/*CXP*/,6/*TURTLE*/,1/*INIT*/);
	}
	LCP = DISPLAY[TOP].BLCK.FFILE;
	while (LCP!=NULL) // with (LCP,IDTYPE)
	{
		GEN2(50/*LDA*/,0,LCP->VADDR);
		GEN2(50/*LDA*/,0,LCP->VADDR+FILESIZE);
		if (LCP->IDTYPE->FILTYPE==NULL)
			GENLDC(-1);
		else if (LCP->IDTYPE==INTRACTVPTR)
			GENLDC(0);
		else if (LCP->IDTYPE->FILTYPE==CHARPTR)
			GENLDC(-2);
		else GENLDC((int)LCP->IDTYPE->FILTYPE->size());
		GEN2(77/*CXP*/,0/*SYS*/,3/*FINIT*/);
		LCP = LCP->NEXT;
	}
	if ((LEVEL==1)&&(!options.SYSCOMP))
		GEN1(85/*BPT*/,SCREENDOTS+1);
	do
	{
		do
		{
			STATEMENT(FSYS+SYMBOLS::SEMICOLON+SYMBOLS::ENDSY);
		}
		while (BNF::STATBEGSYS.in(SY));
		TEST = SY!=SYMBOLS::SEMICOLON;
		if (!TEST)
			INSYMBOL();
	}
	while (!TEST);

	// What to do about infinite loop
	// cycling between error 13 and 17?
	if (SY==SYMBOLS::ENDSY)
		INSYMBOL();
	else if (SY==SYMBOLS::PERIOD)
		return;  // does this fix it?
	else
		CERROR(13);

	EXITIC = IC;
	LCP = DISPLAY[TOP].BLCK.FFILE;
	while (LCP!=NULL) //with (LCP)
	{
		GEN2(50/*LDA*/,0,LCP->VADDR);
		GENLDC(0);
		GEN2(77/*CXP*/,0/*SYS*/,6/*FCLOSE*/);
		LCP = LCP->NEXT;
	}
	if (!options.INMODULE)
	if (LEVEL==1)
	{
		LCP = USINGLIST;
		while (LCP!=NULL)
		{
			if (LCP->SEGID>=0)
			{ 
				GENLDC(LCP->SEGID);
				GEN1(30/*CSP*/,22/*RELSEG*/);
			}
			LCP = LCP->NEXT;
		}
	}
	if (FPROCP==NULL)
		GEN0(86/*XIT*/);
	else
	{
		if (FPROCP->PFLEV==0)
			LOP = 65;/*RBP*/
		else
			LOP = 45;/*RNP*/;
		if (FPROCP->IDTYPE==NULL)
			GEN1(LOP,0);
		else
			GEN1(LOP,(int)FPROCP->IDTYPE->size());
	}
	LLP = DISPLAY[TOP].BLCK.FLABEL;  /* CHECK UNDEFINED LABELS */
	while (LLP!=NULL)
	{
		if (!LLP->CODELBP->DEFINED)
			if (REFLIST!=(void*)MAXADDR)
				CERROR(168);
		LLP = LLP->NEXTLAB;
	}
	JTINX = NEXTJTAB - 1;
	if (ODD(IC))
		IC = IC + 1;
	while (JTINX > 0)
	{ 
		GENWORD(IC-JTAB[JTINX]);
		JTINX = JTINX-1;
	}
	if (FPROCP==NULL)
	{ 
		GENWORD((LCMAX-LCAFTERMARKSTACK)*2);
		GENWORD(0);
	}
	else
	{
		GENWORD((LCMAX-FPROCP->LOCALLC)*2);
		GENWORD((FPROCP->LOCALLC-LCAFTERMARKSTACK)*2);
	}
	GENWORD(IC-EXITIC);
	GENWORD(IC);
	GENBYTE(CURPROC);
	GENBYTE(LEVEL-1);
	if (!CODEINSEG)
	{
		CODEINSEG = true;
		SEGTABLE[SEG].DISKADDR = CURBLK;
	}
	WRITECODE(false);
	SEGINX = SEGINX + IC;
	PROCTABLE[CURPROC] = SEGINX - 2;
}

void BODYPART::LINKERREF(IDCLASS KLASS, int ID, int ADDR)
{
	int ioresult;
#if 0
	PASCALCOMPILER	*m_ptr = (PASCALCOMPILER*) this;
#endif
	PASCALCOMPILER	*m_ptr = reinterpret_cast<PASCALCOMPILER*>(this);

	pascal_file *file = m_ptr->REFFILE;
	REFARRAY *list = m_ptr->REFLIST;
	const char *data = (const char*)(&list[0]);
	if (m_ptr->NREFS>REFSPERBLK)/*WRITE BUFFER*/
	{
		ioresult = SYSCOMM::BLOCKWRITE(file,(unsigned char*)data,1,m_ptr->REFBLK);
		if (ioresult!=1)
			CERROR(402);
		m_ptr->REFBLK++;
		m_ptr->NREFS=1;
	}
	// with	(m_ptr->REFLIST[m_ptr->NREFS])
	REFARRAY &ARR = m_ptr->REFLIST[m_ptr->NREFS];
	if (BNF::VARS.in(KLASS))
		ARR->KEY=ID+32;
	else /*PROC*/
		ARR->KEY=ID;
	ARR->OFFSET=SEGINX+ADDR;
	NREFS=NREFS+1;
}

void BODYPART::GENLDC(int IVAL)
{
	char src[16];
	char *dest = *CODEP;
	if ((IVAL>=0)&&(IVAL<=127))
		GENBYTE(IVAL);
	else
	{
		GENBYTE(51/*LDC*/+148);
		src[0] = IVAL;
		src[1] = 0;
		MOVELEFT((const char*)src,(char*)&(dest[IC]),2);
		IC=IC+2;
	}
} /*GENLDC*/

void BODYPART::GENBIG(int IVAL)
{
	char LOWORDER;
	char src[16];
	char *dest = *CODEP;
	if (IVAL<=127)
		GENBYTE(IVAL);
	else
	{
		src[0] = IVAL;
		src[1] = 0;
		MOVELEFT((const char*)src,(char*)&(dest[IC]),2);
		LOWORDER = dest[IC];
		dest[IC]=(char)(ORD(dest[IC+1])+128);
		dest[IC+1]=LOWORDER;
		IC=IC+2;
	}
} /*GENBIG*/ ;

void BODYPART::GEN0(OPRANGE FOP)
{
	int i;
	//	with(GATTR.CVAL.VALP)
	CSP &P = GATTR.CVAL.VALP;
	GENBYTE(FOP+128);
	if (FOP==38/*LCA*/)
	{
		GENBYTE(P->SLGTH);
		for(i=1;i<=P->SLGTH;i++)
			GENBYTE(ORD(P->SVAL[i]));
	}
} /*GEN0*/ ;

void BODYPART::GEN1(OPRANGE FOP, int FP2)
{
	int	I,J;
	GENBYTE(FOP+128);
	if (FOP==51/*LDC*/)
	{
		if (FP2==2)
			I=REALSIZE;
		else
		{
			I=8;
			while(I>0)
			{
				if (GATTR.CVAL.VALP->SVAL[I]!=0)
					break;
				else
					I=I-1;
			}
			GATTR.TYPTR->resize(I);
			if (I>1)
			{
				GENBYTE(I);
				for(J=I;J>=1;J--)
				{
					GENWORD(GATTR.CVAL.VALP->CSTVAL[J]);
//					GENWORD(GATTR.CST.VALP->CSTVAL[J]);
				}
			}
			else
			{
				IC=IC-1;
				if (I==1)
					GENLDC(GATTR.CVAL.VALP->CSTVAL[1]);
			}
		}
	}
	else if (SET<128>(11, 30/*CSP*/,32/*ADJ*/,45/*RNP*/,
		46/*CIP*/,60/*LDM*/,61/*STM*/,
		65/*RBP*/,66/*CBP*/,78/*CLP*/,
		42/*SAS*/,79/*CGP*/).in(FOP))
		GENBYTE(FP2);
	else if (options.INMODULE&&(SET<128>(3,37/*LAO*/,39/*LDO*/,43/*SRO*/).in(FOP)))
	{
		LINKERREF(ACTUALVARS,FP2,IC);
		GENBYTE(128);
		GENBYTE(0);
	}
	else if (((FOP==74/*LDL*/)||(FOP==39/*LDO*/))
		&&(FP2<=16))
	{
		IC=IC-1;
		if (FOP==39/*LDO*/)
			GENBYTE(231+FP2);
		else
			GENBYTE(215+FP2);
	}
	else if ((FOP==35/*IND*/)&&(FP2<=7))
	{
		IC=IC-1;
		GENBYTE(248+FP2);
	}
	else
		GENBIG(FP2);
} /*GEN1*/

void BODYPART::GEN2(OPRANGE FOP, int FP1, int FP2)
{
	if ((FOP==64/*IXP*/)||(FOP==77/*CXP*/))
	{
		GENBYTE(FOP+128);
		GENBYTE(FP1);
		GENBYTE(FP2);
	}
	else if (SET<256>(6,47/*EQU*/,48/*GEQ*/,49/*GRT*/,
		52/*LEQ*/,53/*LES*/,55/*NEQ*/).in(FOP))
		if (FP1==0)
			GEN0(OPRANGE(FOP+20));
		else
		{
			GEN1(FOP,FP1+FP1);
			if (FP1>4)
				GENBIG(FP2);
		}
	else
	{ /*LDA,LOD,STR*/
		if (FP1==0)
			GEN1(FOP+20,FP2);
		else
		{
			GENBYTE(FOP+128);
			GENBYTE(FP1);
			GENBIG(FP2);
		}
	}
} /*GEN2*/

void BODYPART::GENNR(NONRESIDENT EXTPROC)
{
	if (PFNUMOF[EXTPROC]==0)
		ASSIGN(EXTPROC);
	if (SEPPROC)
	{
		GEN1(79/*CGP*/,0);
		LINKERREF(PROC1,-PFNUMOF[EXTPROC],IC-1);
	}
	else
		GEN1(79/*CGP*/,PFNUMOF[EXTPROC]);
}

void BODYPART::ASSIGN(NONRESIDENT EXTPROC)
{
	PROCTABLE[NEXTPROC]=0;
	PFNUMOF[EXTPROC]=NEXTPROC;
	NEXTPROC++;
	if (NEXTPROC>MAXPROCNUM)
		CERROR(193);/*!ENOUGH ROOM for(THIS*/
	CLINKERINFO=true;  /*OPERATION*/
}

void BODYPART::GENJMP(OPRANGE FOP, LBP FLBP)
{
	int DISP;
	// with FLBP
	if (FLBP->DEFINED)
	{
		GENBYTE(FOP+128);
		DISP=FLBP->OCCURIC-IC-1;
		if ((DISP>=0)&&(DISP<=127))
			GENBYTE(DISP);
		else
		{
			if (FLBP->JTABINX==0)
			{
				FLBP->JTABINX=NEXTJTAB;
				if (NEXTJTAB==MAXJTAB)
					CERROR(253);
				else
					NEXTJTAB++;
				JTAB[FLBP->JTABINX]=FLBP->OCCURIC;
			}
			DISP=-FLBP->JTABINX;
			GENBYTE(248-FLBP->JTABINX-FLBP->JTABINX);
		}
	}
	else
	{
		// WATCHOUT FOR ENDIANNESS!!
		char src[16];
		src[0] = FLBP->REFLIST&0xff;
		src[1] = (FLBP->REFLIST&0xff00)>>8;
		MOVELEFT((const char*)src,(char*)(&CODEP[IC]),2);
		if (FOP==57/*UJP*/)
			DISP=IC+4096;
		else
			DISP=IC;
		FLBP->REFLIST=DISP;
		IC=IC+2;
	}
} /*GENJMP*/

void BODYPART::GENFJP(LBP FLBP)
{
	LOAD();
	if (GATTR.TYPTR!=BOOLPTR)
		CERROR(135);
	GENJMP(33/*FJP*/,FLBP);
}

void BODYPART::GENLABEL(LBP *FLBP)
{
	LBP	ptr = new CODELABEL;
	(*FLBP) = ptr;
//	(*FLBP)->DEFINED=false;
	(*FLBP)->REFLIST=MAXADDR;
}

void BODYPART::PUTLABEL(LBP FLBP)
{
	int LREF;
	OPRANGE LOP;
	LREF=FLBP->REFLIST;
	FLBP->DEFINED=true;
	FLBP->OCCURIC=IC;
	FLBP->JTABINX=0;
	while(LREF<MAXADDR)
	{
		if (LREF>=4096)
		{
			LREF=LREF-4096;
			LOP=57;/*UJP*/
		}
		else
			LOP=33;/*FJP*/;
		IC=LREF;
		MOVELEFT((char*)&CODEP[IC],(char*)&LREF,2);
		GENJMP(LOP,FLBP);
	}
	IC=FLBP->OCCURIC;
} /*PUTLABEL*/ ;

void BODYPART::LOAD()
{
	WRITELN (OUTPUT,"bodypart::load");
	int J,M;
	// with GATTR
	if (GATTR.TYPTR==NULL)
		return;
	switch (GATTR.KIND)
	{
	case CST:
		if (GATTR.TYPTR->is_long())
			// with GATTR.CVAL.VALP^
		{
			M=10000;
			GENLDC(GATTR.CVAL.VALP->LONGVAL[1]);
			GENLDC(1);
			for(J=2;J<=GATTR.CVAL.VALP->LLENG;J++)
			{
				if (J==GATTR.CVAL.VALP->LLENG)
					M=TRUNC(PWROFTEN(GATTR.CVAL.VALP->LLAST));
				GENLDC(M);
				GENLDC(1);
				GENLDC(8/*DMP*/);
				GENNR(DECOPS);
				GENLDC(GATTR.CVAL.VALP->LONGVAL[J]);
				GENLDC(1);
				GENLDC(2/*DAD*/);
				GENNR(DECOPS);
			}
		}
		else if ((GATTR.TYPTR->is_scalar())&&(GATTR.TYPTR!=REALPTR))
			GENLDC(GATTR.CVAL.IVAL);
		else if (GATTR.TYPTR==NILPTR)
			GEN0(31/*LDCN*/);
		else if (GATTR.TYPTR==REALPTR)
			GEN1(51/*LDC*/,2);
		else
			GEN1(51/*LDC*/,5);
		break;
	case VARBL:
		switch (GATTR.ACCESS())
		{
		case DRCT:
			if (GATTR.VLEVEL==1)
				GEN1(39/*LDO*/,GATTR.DPLMT);
			else 
				GEN2(54/*LOD*/,LEVEL-GATTR.VLEVEL,GATTR.DPLMT);
			break;
		case INDRCT:
			GEN1(35/*IND*/,GATTR.IDPLMT);
			break;
		case PACKD: 
			GEN0(58/*LDP*/);
			break;
		case MULTI:
			GEN1(60/*LDM*/,(int)GATTR.TYPTR->size());
			break;
		case BYTE1:
			GEN0(62/*LDB*/);
			break;
		case EXPR1:
			// with TYPTR^
			if (((GATTR.TYPTR->is_power())||
				(GATTR.TYPTR->is_long())&&(GATTR.KIND!=CST))
				&&(GATTR.KIND!=EXPR))
				GENLDC((int)GATTR.TYPTR->size());
			GATTR.KIND=EXPR;
			break;
		default:
			break;
		}
	default:
		break;
	}
} /*LOAD*/

void BODYPART::STORE(ATTR &FATTR)
{
	// with FATTR
	WRITELN (OUTPUT,"bodypart::store");

	if (FATTR.TYPTR==NULL)
		return;

	static char *access_types [] = 
	{
		"DRCT",
		"INDRCT",
		"PACKD",
		"MULTI",
		"BYTE1",
	};
	int access = (int)FATTR.ACCESS();
	WRITELN (OUTPUT,"----> access type = <",access_types[access],">");
	switch (FATTR.ACCESS())
	{
		case DRCT:
			if (FATTR.VLEVEL==1)
				GEN1(43/*SRO*/,FATTR.DPLMT);
			else
				GEN2(56/*STR*/,LEVEL-FATTR.VLEVEL,FATTR.DPLMT);
			break;
		case INDRCT:
			if (FATTR.IDPLMT!=0)
				CERROR(400);
			else
				GEN0(26/*STO*/);
			break;
		case PACKD:
			GEN0(59/*STP*/);
			break;
		case MULTI: 
			GEN1(61/*STM*/,(int)FATTR.TYPTR->size());
			break;
		case BYTE1:
			GEN0(63/*STB*/);
			break;
		default:
			break;
	}
} /*STORE*/

void BODYPART::LOADADDRESS()
{
	// with GATTR
	char *svar = NULL;
	WRITELN (OUTPUT,"BODYPART::LOADADDRESS ()");

	if (GATTR.TYPTR==NULL)
		return;

	switch (GATTR.KIND)
	{
		case CST:
			if (STRGTYPE(GATTR.TYPTR))
			{
				svar = &(GATTR.CVAL.VALP->SVAL[1]);
				WRITELN (OUTPUT,"TYPE = STRING: \"",svar,"\"");
				GEN0(38/*LCA*/);
			}
			else
				CERROR(400);
			break;
		case VARBL:
			switch (GATTR.ACCESS())
			{
			case DRCT:
				if (GATTR.VLEVEL==1)
					GEN1(37/*LAO*/,GATTR.DPLMT);
				else
					GEN2(50/*LDA*/,LEVEL-GATTR.VLEVEL,GATTR.DPLMT);
				break;
			case INDRCT:
				if (GATTR.IDPLMT!=0)
					GEN1(34/*INC*/,GATTR.IDPLMT+GATTR.IDPLMT);
				break;
			case PACKD:
				CERROR(103);
				break;
			break;
			}
		default:
			break;
	}
	GATTR.KIND=VARBL;
	GATTR.set_access(INDRCT);
	GATTR.IDPLMT=0;
} /*LOADADDRESS*/ ;


void BODYPART::SELECTOR0 (const SETOFSYS &FSYS, CTP FCP, ATTR &LATTR, CTP &LCP, int &LMIN, int &LMAX)
{
	GATTR.TYPTR=FCP->IDTYPE;
	GATTR.KIND=VARBL;
	WRITELN (OUTPUT,"---bodypart::selector0");

	// with FCP, GATPTR
	switch (FCP->KLASS)
	{
	case ACTUALVARS:
		GATTR.VLEVEL=FCP->VLEV;
		GATTR.DPLMT=FCP->VADDR;
		GATTR.set_access(DRCT);
		if (options.INMODULE)
			if (GATTR.TYPTR!=NULL)
				if ((FCP->VLEV==1)&&(GATTR.TYPTR->form()==RECORDS))
					LOADADDRESS();
		break;

	case FORMALVARS:
		if (FCP->VLEV==1)
			GEN1(39/*LDO*/,FCP->VADDR);
		else
			GEN2(54/*LOD*/,LEVEL-FCP->VLEV,FCP->VADDR);
		GATTR.set_access(INDRCT);
		GATTR.IDPLMT=0;
		break;

	case FIELD:	
		if (DISPLAY[DISX].OCCUR==CREC)
		{
			GATTR.set_access(DRCT);
			GATTR.VLEVEL=DISPLAY[DISX].CREC.CLEV;
			GATTR.DPLMT=DISPLAY[DISX].CREC.CDSPL+FCP->FLDADDR;
		}
		else
		{
			if (LEVEL==1)
				GEN1(39/*LDO*/,DISPLAY[DISX].VREC.VDSPL);
			else
				GEN2(54/*LOD*/,0,DISPLAY[DISX].VREC.VDSPL);
			GATTR.set_access(INDRCT);
			GATTR.IDPLMT=FCP->FLDADDR;
		}
		if (FCP->FISPACKED)
		{
			LOADADDRESS();
			if (((FCP->FLDRBIT==0)||(FCP->FLDRBIT==8))
				&&(FCP->FLDWIDTH==8))
			{
				GATTR.set_access(BYTE1);
				if (FCP->FLDRBIT==8)
					GEN1(34/*INC*/,1);
			}
			else
			{
				GATTR.set_access(PACKD);
				GENLDC(FCP->FLDWIDTH);
				GENLDC(FCP->FLDRBIT);
			}
		}
		break;

	case FUNC:
		if (FCP->PFDECKIND!=DECLARED)
			CERROR(150);
		else if (!FCP->INSCOPE)
			CERROR(103);
		else
		{
			GATTR.set_access(DRCT);
			GATTR.VLEVEL=FCP->PFLEV+1;
			GATTR.DPLMT=LCAFTERMARKSTACK;
		}
		break;

	default:
		break;
	} /*switch /*/

	if (GATTR.TYPTR!=NULL)
		if ((GATTR.TYPTR->form()<=POWER)&&(GATTR.TYPTR->size()>PTRSIZE))
		{
			LOADADDRESS();
			GATTR.set_access(MULTI);
		}

		if (!SETOFSYS(BNF::SELECTSYS+FSYS).in(SY))
		{
			CERROR(59);
			SKIP(BNF::SELECTSYS+FSYS);
		}
}

void BODYPART::SELECTOR1 (const SETOFSYS &FSYS, CTP FCP, ATTR &LATTR, CTP &LCP, int &LMIN, int &LMAX)
{
	WRITELN (OUTPUT,"---bodypart::selector1");
	do {
		LATTR=GATTR;
		if (LATTR.TYPTR!=NULL)
		if (LATTR.TYPTR->form()!=ARRAYS)
		{
			CERROR(138);
			LATTR.TYPTR=NULL; 
		}
		LOADADDRESS();
		INSYMBOL();
		EXPRESSION(FSYS+SETOFSYS(2,SYMBOLS::COMMA,SYMBOLS::RBRACK));
		LOAD();
		if (GATTR.TYPTR!=NULL)
			if (GATTR.TYPTR->form()!=SCALAR)
				CERROR(113);
		if (LATTR.TYPTR!=NULL)
			// with LATTR.TYPTR^
		{
		if (COMPTYPES(GATTR.TYPTR->INXTYPE,GATTR.TYPTR))
		{
			if ((GATTR.TYPTR->INXTYPE!=NULL)&&
				!STRGTYPE(LATTR.TYPTR))
			{
				GETBOUNDS(GATTR.TYPTR->INXTYPE,LMIN,LMAX);
				if (options.RANGECHECK)
				{ 
					GENLDC(LMIN);
					GENLDC(LMAX);
					GEN0(8/*CHK*/);
				}
				if (LMIN!=0)
				{
					GENLDC(abs(LMIN));
					if (LMIN>0)
						GEN0(21/*SBI*/);
					else
						GEN0(2/*ADI*/);
				}  
			}
		}
		else
			CERROR(139);
		// with GATTR
		GATTR.TYPTR=GATTR.TYPTR->AELTYPE;
		GATTR.KIND=VARBL;
		GATTR.set_access(INDRCT);
		GATTR.IDPLMT=0;
		if (GATTR.TYPTR!=NULL)
			if (GATTR.TYPTR->is_packed())
				if (GATTR.TYPTR->ELWIDTH==8)
				{
					GATTR.set_access(BYTE1);
					if (STRGTYPE(LATTR.TYPTR)&&options.RANGECHECK)
						GEN0(27/*IXS*/);
					else
						GEN0(2/*ADI*/);
				}
				else
				{
					GATTR.set_access(PACKD);
					GEN2((OPRANGE)64/*IXP*/,GATTR.TYPTR->ELSPERWD,GATTR.TYPTR->ELWIDTH);
				}
			else
			{
				GEN1(36/*IXA*/,(int)GATTR.TYPTR->size());
				if ((GATTR.TYPTR->form()<=POWER)&&
					(GATTR.TYPTR->size()>PTRSIZE))
					GATTR.set_access(MULTI);
			}
		}
	}
	while(SY==SYMBOLS::COMMA);
}

void BODYPART::SELECTOR2 (const SETOFSYS &FSYS, const CTP FCP, ATTR &LATTR, CTP &LCP, int &LMIN, int &LMAX)
{
	// todo: move this to debugging section
	{
		bool packed = FCP->FISPACKED;
		char *b;
		if (packed)
			b = "true";
		else
			b = "false";

//		WRITELN (OUTPUT,"BODYPART::SELECTOR: FCP->NAME = \"",FCP->NAME,"\"");
		::WRITE (OUTPUT,"---bodypart::selector2: ");
		::WRITE(OUTPUT,"FCP->NAME = \"",FCP->NAME,"\", ");
		::WRITE(OUTPUT,"FCP->FISPACKED = <",b,">");
		WRITELN(OUTPUT);
	}

	if (GATTR.TYPTR!=NULL)
	if (GATTR.TYPTR->form()!=RECORDS)
	{
		CERROR(140);
		GATTR.TYPTR=NULL;
	}
	INSYMBOL();
	if (SY==SYMBOLS::IDENT)
	{
		if (GATTR.TYPTR!=NULL)
		{
			SEARCHSECTION(GATTR.TYPTR->FSTFLD,LCP);
			ALPHA tag_name;
			if (LCP!=NULL)
				tag_name = LCP->NAME;
			::WRITE(OUTPUT,"LCP->NAME = \"",tag_name,"\"");
			if (LCP==NULL)
			{
				CERROR(152);
				GATTR.TYPTR=NULL;
			}
			else 
			{
				GATTR.TYPTR=FCP->IDTYPE;
				switch (GATTR.ACCESS())
				{
				case DRCT:
					::WRITE(OUTPUT,"#### ACCESS = <DRCT>, FCP->FLDADDR = <",(int)FCP->FLDADDR,">");
					GATTR.DPLMT=GATTR.DPLMT+FCP->FLDADDR;
					break;

				case INDRCT:
					::WRITE(OUTPUT,"#### ACCESS = <INDRCT>, FCP->FLDADDR = <",(int)FCP->FLDADDR,">");
					GATTR.IDPLMT=GATTR.IDPLMT+FCP->FLDADDR;
					break;

				case MULTI:
				case BYTE1:
				case PACKD:
					CERROR(400);
					break;

				default:
					break;
				} /*switch (ACCESS*/

				if (FCP->FISPACKED)
				{
					LOADADDRESS();
					if (((FCP->FLDRBIT==0)||(FCP->FLDRBIT==8))
						&&(FCP->FLDWIDTH==8))
					{
						GATTR.set_access(BYTE1);
						if (FCP->FLDRBIT==8)
						GEN1(34/*INC*/,1);
					}
					else
					{	
						GATTR.set_access(PACKD);
						GENLDC(FCP->FLDWIDTH);
						GENLDC(FCP->FLDRBIT);
					}
				}

				if (GATTR.TYPTR!=NULL)
					if ((GATTR.TYPTR->form()<=POWER)&&
						(GATTR.TYPTR->size()>PTRSIZE))
					{
						LOADADDRESS();
						GATTR.set_access(MULTI);
					}
				}
			}
			INSYMBOL();
	} /*SY==IDENT*/
	else
		CERROR(2);
	WRITELN(OUTPUT);
}

void BODYPART::SELECTOR3 (const SETOFSYS &FSYS, CTP FCP, ATTR &LATTR, CTP &LCP, int &LMIN, int &LMAX)
{
	::WRITE (OUTPUT,"---bodypart::selector3: ");

	if (GATTR.TYPTR!=NULL) // with GATTR,TYPTR^
		if ((GATTR.TYPTR->is_pointer())||(GATTR.TYPTR->is_file()==true))
		{
			LOAD();
			GATTR.KIND=VARBL;
			GATTR.set_access(INDRCT);
			GATTR.IDPLMT=0;
			if (GATTR.TYPTR->is_pointer())
				GATTR.TYPTR=GATTR.TYPTR->ELTYPE;
			else
			{
				GATTR.TYPTR=GATTR.TYPTR->FILTYPE;
				if (GATTR.TYPTR==NULL)
					CERROR(399);
			}
			if (GATTR.TYPTR!=NULL)
				if ((GATTR.TYPTR->form()<=POWER)&&
					(GATTR.TYPTR->size()>PTRSIZE))
					GATTR.set_access(MULTI);
		}
		else
			CERROR(141);
		INSYMBOL();
}

void BODYPART::SELECTOR(const SETOFSYS &FSYS, const CTP FCP)
{
	ATTR	LATTR;
	CTP		LCP = NULL;
	int LMIN, LMAX;
	GATTR.TYPTR=FCP->IDTYPE;
	GATTR.KIND=VARBL; 
	
	// todo: move this to debugging section
	{
		bool packed = FCP->FISPACKED;
		char *b;
		if (packed)
			b = "true";
		else
			b = "false";

		WRITELN (OUTPUT,"bodypart::selector: fcp->name = \"",FCP->NAME,"\"");
		treesearch::printleaf (FCP,ACTUALVARS);
//		::WRITE(OUTPUT,"BODYPART::SELECTOR2: ");
//		::WRITE(OUTPUT,"FCP->NAME = \"",FCP->NAME,"\", ");
//		::WRITE(OUTPUT,"FCP->FISPACKED = <",b,">");
//		WRITELN(OUTPUT);
	}

	SELECTOR0 (FSYS,FCP,LATTR,LCP,LMIN,LMAX);
	while(BNF::SELECTSYS.in(SY))
	{
		if (SY==SYMBOLS::LBRACK)
		{
			SELECTOR1 (FSYS,FCP,LATTR,LCP,LMIN,LMAX);
			if (SY==SYMBOLS::RBRACK)
				INSYMBOL();
			else
				CERROR(12);
		}
		else if (SY==SYMBOLS::PERIOD)
		{
			SELECTOR2 (FSYS,FCP,LATTR,LCP,LMIN,LMAX);		
		}
		else
		{
			SELECTOR3 (FSYS,FCP,LATTR,LCP,LMIN,LMAX);
		}
		SETOFSYS S0 = FSYS+BNF::SELECTSYS;
//		S0.debug_list ("SELECTOR SO: ");
		if (!(S0).in(SY))
		{
			CERROR(6);
			SKIP(FSYS+BNF::SELECTSYS);
		}
	} /*while /*/
} /*SELECTOR*/

void BODYPART::CALL(const SETOFSYS &FSYS, CTP FCP)
{
	/*CALL*/
	int LKEY;
	bool WASLPARENT;
	if (FCP->PFDECKIND==SPECIAL)
	{
		WASLPARENT=true;
		LKEY=FCP->KEY;
		if (SY==SYMBOLS::LPARENT)
			INSYMBOL();
		else
			if (SET<128>(4,2,4,5,6).in(LKEY))
				WASLPARENT=false;
			else
				CERROR(9);
		if (SET<42>(11,7,8,9,10,11,13,14,25,36,39,42).in(LKEY))
		{
			EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
			LOAD();
		}
		if ((SET<43>(21,12,13,14,15,18,19,21,22,23,27,31,32,34,35,36,37,38,
			40,41,42,43).in(LKEY)))
			ROUTINE(FSYS,LKEY);
		else 
			switch (LKEY)
		{
			case 1:
			case 2:
				READ(FSYS,LKEY,WASLPARENT);
				break;
			case 3:
			case 4:
				WRITE(FSYS,LKEY,WASLPARENT);
				break;
			case 5:
			case 6:/*EOF & EOLN*/
				if (WASLPARENT)
				{
					VARIABLE(FSYS+SYMBOLS::RPARENT);
					LOADADDRESS();
					if (GATTR.TYPTR!=NULL)
						if (GATTR.TYPTR->is_file()==false)
							CERROR(125);
						else if ((GATTR.TYPTR->FILTYPE!=CHARPTR)&&
								(LKEY==6))
							CERROR(399);
				}
				else
					LOADIDADDR(INPUTPTR);
				GENLDC(0);
				GENLDC(0);
				if (LKEY==5)
					GEN2(77/*CXP*/,0/*SYS*/,10/*FEOF*/);
				else GEN2(77/*CXP*/,0/*SYS*/,11/*FEOLN*/);
				GATTR.TYPTR=BOOLPTR;
				/*EOF*/
				break;
			case 7:
			case 8: GENLDC(1); /*PREDSUCC*/
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR->is_scalar())
						if (LKEY==8)
							GEN0(2/*ADI*/);
						else
							GEN0(21/*SBI*/);
					else
					CERROR(115);
				/*PREDSUCC*/
				break;
			case 9: /*ORD*/
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR->form()>=POWER)
						CERROR(125);
				GATTR.TYPTR=INTPTR;
					/*ORD*/ ;
			case 10: /*SQR*/
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR==INTPTR)
						GEN0(24/*SQI*/);
					else
					if (GATTR.TYPTR==REALPTR)
						GEN0(25/*SQR*/);
					else {
						CERROR(125);
						GATTR.TYPTR=INTPTR;
					}
					/*SQR*/
					break;
			case 11: /*ABS*/
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR==INTPTR)
						GEN0(0/*ABI*/);
					else if (GATTR.TYPTR==REALPTR)
						GEN0(1/*ABR*/);
					else {
						CERROR(125);
						GATTR.TYPTR=INTPTR;
					}
						/*ABS*/
				break;
			case 16: /*LENGTH*/
				STRGVAR(FSYS+SYMBOLS::RPARENT,false);
				GEN0(62/*LDB*/);
				GATTR.TYPTR=INTPTR;
				/*LENGTH*/
				break;
			case 17: /*INSERT*/
				STRGVAR(FSYS+SYMBOLS::COMMA,false);
				if (SY==SYMBOLS::COMMA)
					INSYMBOL();
				else
					CERROR(20);
				STRGVAR(FSYS+SYMBOLS::COMMA,true);
				GENLDC(GATTR.TYPTR->MAXLENG);
				if (SY==SYMBOLS::COMMA)
					INSYMBOL();
				else
					CERROR(20);
				EXPRESSION(FSYS+SYMBOLS::RPARENT);
				LOAD();
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR!=INTPTR)CERROR(125);
				GEN2(77/*CXP*/,0/*SYS*/,24/*SINSERT*/);
					/*INSERT*/
				break;
			case 20: /*POS*/
				STRGVAR(FSYS+SYMBOLS::COMMA,false);
				if (SY==SYMBOLS::COMMA)
					INSYMBOL();
				else
					CERROR(20);
				STRGVAR(FSYS+SYMBOLS::RPARENT,false);
				GENLDC(0);
				GENLDC(0);
				GEN2(77/*CXP*/,0/*SYS*/,27/*SPOS*/);
				GATTR.TYPTR=INTPTR;
				/*POS*/
				break;
			case 24: /*IDSEARCH*/
				VARIABLE(FSYS+SYMBOLS::COMMA);
				LOADADDRESS();
				if (SY==SYMBOLS::COMMA)
					INSYMBOL();
				else
					CERROR(20);
				VARIABLE(FSYS+SYMBOLS::RPARENT);
				LOADADDRESS();
				GEN1(30/*CSP*/,7/*IDS*/);
					/*IDSEARCH*/
				break;
			case 25: /*treesearch*/
				if (SY==SYMBOLS::COMMA)
					INSYMBOL();
				else
					CERROR(20);
				VARIABLE(FSYS+SYMBOLS::COMMA);
				LOADADDRESS();
				if (SY==SYMBOLS::COMMA)
					INSYMBOL();
				else
					CERROR(20);
				VARIABLE(FSYS+SYMBOLS::RPARENT);
				LOADADDRESS();
				GATTR.TYPTR=INTPTR;
				GEN1(30/*CSP*/,8/*TRS*/);
					/*treesearch*/
				break;
			case 26: /*TIME*/
				VARIABLE(FSYS+SYMBOLS::COMMA);
				LOADADDRESS();
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR!=INTPTR)
						CERROR(125);
				if (SY==SYMBOLS::COMMA)
					INSYMBOL();
				else
					CERROR(20);
				VARIABLE(FSYS+SYMBOLS::RPARENT);
				LOADADDRESS();
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR!=INTPTR)
						CERROR(125);
				GEN1(30/*CSP*/,9/*TIM*/);
					/*TIME*/
					break;
			case 33:
			case 28:
			case 29:
			case 30:
				/*OPEN,RESET,REWRITE*/
				VARIABLE(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
				LOADADDRESS();
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR->is_file()==false)
						CERROR(125);
				if (SY!=SYMBOLS::COMMA)
					if (LKEY==33)
						GEN2(77/*CXP*/,0/*SYS*/,4/*FRESET*/);
					else
					CERROR(20);
				else
				{
					INSYMBOL();
					STRGVAR(FSYS+SYMBOLS::RPARENT,false);
					if ((LKEY==28)||(LKEY==30))
						GENLDC(0);
					else
						GENLDC(1);
					GENLDC(0);
					GEN2(77/*CXP*/,0/*SYS*/,5/*FOPEN*/);
				}
				if (options.IOCHECK)
					GEN1(30/*CSP*/,0/*IOC*/);
				/*OPEN*/
				break;
			case 39:/*TRUNC*/
				if (GATTR.TYPTR==INTPTR)
				{
					GEN0(10/*FLT*/);
					GATTR.TYPTR=REALPTR;
				}
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR==REALPTR)
						GEN1(30/*CSP*/,23/*TRUNC*/); /*** TEMPORARY --
													 TRUNC WILL BE CSP 14 IN II.0 ***/
					else if (GATTR.TYPTR->is_long())
						{
							GENLDC(INTSIZE);
							GENLDC(0 /*DAJ*/);
							GENNR(DECOPS);
						}
						else CERROR(125);
						 GATTR.TYPTR=INTPTR;
						break;
			default:
				break;
		} /*SPECIAL CASES*/ ;
		if (WASLPARENT)
			if (SY==SYMBOLS::RPARENT)
				INSYMBOL();
			else
				CERROR(4);
	} /*SPECIAL procedujres and FUNCTIONS*/
	else
		CALLNONSPECIAL(FSYS,FCP);
} /*CALL*/

void BODYPART::EXIT0()
{
	CTP LCP;
	if (SY==SYMBOLS::IDENT)
	{
		SEARCHID(SETOFIDS(2,PROC1,FUNC),LCP);
		INSYMBOL();
	}
	else if (SY==SYMBOLS::PROGSY)
	{
		LCP=OUTERBLOCK;
		INSYMBOL();
	}
	else LCP=NULL;;
	if (LCP!=NULL)
		if (LCP->PFDECKIND==DECLARED)
		{
			GENLDC(LCP->PFSEG);
			GENLDC(LCP->PFNAME);
			if (options.INMODULE)
			{
				LINKERREF(PROC1,LCP->PFSEG,IC-2);
				if (SEPPROC)
					LINKERREF(PROC1,-LCP->PFNAME,IC-1);
			}
		}
		else
			CERROR(125);
	else
		CERROR(125);
	GEN1(30/*CSP*/,4/*XIT*/);
} /*EXIT*/

void BODYPART::FLOATIT(STP &FSP, bool FORCEFLOAT)
{
	if ((GATTR.TYPTR==REALPTR)||(FSP==REALPTR)||FORCEFLOAT)
	{
		if (GATTR.TYPTR==INTPTR)
		{
			GEN0(10/*FLT*/);
			GATTR.TYPTR=REALPTR;
		}
		if (FSP==INTPTR)
		{
			GEN0(9/*FLO*/);
			FSP=REALPTR;
		}
	}
} /*FLOATIT*/ 

void BODYPART::STRETCHIT(STP &FSP)
{
	if ((FSP->is_long())||(GATTR.TYPTR->is_long()))
	if (GATTR.TYPTR==INTPTR)
	{
	   GENLDC(INTSIZE);
	   GATTR.TYPTR=LONGINTPTR;
	}
	else if (FSP==INTPTR)
	{
		GENLDC(14/*DCV*/);
		GENNR(DECOPS);
		FSP=LONGINTPTR;
	}
} /*STRETCHIT*/

void BODYPART::MAKEPA(STP &STRGFSP, STP PAFSP)
{
	int LMIN,LMAX;
	if (PAFSP->INXTYPE!=NULL)
	{
		GETBOUNDS(PAFSP->INXTYPE,LMIN,LMAX);
		if (LMAX-LMIN+1!=STRGFSP->MAXLENG)
			CERROR(129);
	}
	STRGFSP=PAFSP;
} /*MAKEPA*/ 

void BODYPART::FACTOR(const SETOFSYS &FSYS) 
{
	CTP	LCP;
	bool VARPART,ALLCONST;
	STP	LSP;	
	int HIGHVAL,LOWVAL,LIC,LOP;
	SETOFSYS CSTPART;
#if 0
	FSYS.debug_list("BODYPART::FACTOR FSYS = ");
#endif

	if (!(BNF::FACBEGSYS.in(SY)))
	{
		CERROR(58);
		SKIP(FSYS+BNF::FACBEGSYS);
		GATTR.TYPTR=NULL;
	}
	while(BNF::FACBEGSYS.in(SY))
	{
		 switch (SY)
		 {
		   /*ID*/
			case SYMBOLS::IDENT:
			SEARCHID(SETOFIDS(5,KONST,FORMALVARS,ACTUALVARS,FIELD,FUNC),LCP);
			INSYMBOL();
			 if (LCP->KLASS==FUNC)
			 {
				CALL(FSYS,LCP);
				GATTR.KIND=EXPR;
			 }
			 else if (LCP->KLASS==KONST)
			 {
				GATTR.TYPTR=LCP->IDTYPE;
				GATTR.KIND=CST;
				GATTR.CVAL=LCP->VALUES;
			 }
			 else SELECTOR(FSYS,LCP);
			 if (GATTR.TYPTR!=NULL)
			   if (GATTR.TYPTR->form()==SUBRANGE)
				   GATTR.TYPTR=GATTR.TYPTR->RANGETYPE;
			break;
		
			case SYMBOLS::INTCONST:
				GATTR.TYPTR=INTPTR;
				GATTR.KIND=CST;
				GATTR.CVAL=VAL;
				INSYMBOL();
				break;

		 case SYMBOLS::REALCONST:
			 GATTR.TYPTR=REALPTR;
			 GATTR.KIND=CST;
			 GATTR.CVAL=VAL;
			 INSYMBOL();
			 break;

		 case SYMBOLS::STRINGCONST:
			if (LGTH==1)
				GATTR.TYPTR=CHARPTR;
			else
			{
				LSP = structure::allocate (ARRAYS);
				*LSP=*STRGPTR;
				LSP->MAXLENG=LGTH;
				GATTR.TYPTR=LSP;
			}
			GATTR.KIND=CST;
			GATTR.CVAL=VAL;
			INSYMBOL();
			break;

		 case SYMBOLS::LONGCONST:
			 LSP = structure::allocate (LONGINT);
			*LSP=*LONGINTPTR;
			LSP->resize(DECSIZE(LGTH));
			GATTR.TYPTR=LSP;
			GATTR.KIND=CST;
			GATTR.CVAL=VAL;
			INSYMBOL();

 /*/*/   case SYMBOLS::LPARENT:
			INSYMBOL();
			EXPRESSION(FSYS+SYMBOLS::RPARENT);
			if (SY==SYMBOLS::RPARENT)
				INSYMBOL();
			else
				CERROR(4);
			break;

 /*NOT*/ case SYMBOLS::NOTSY:
			INSYMBOL();
			FACTOR(FSYS);
			if ((GATTR.KIND==CST)&&(GATTR.TYPTR==BOOLPTR))
				 GATTR.CVAL.IVAL=ORD(!ODD(GATTR.CVAL.IVAL));
			else
			{
				LOAD();
				GEN0(19/*NOT*/);
				if (GATTR.TYPTR!=NULL)
				   if (GATTR.TYPTR!=BOOLPTR)
				   {
					   CERROR(135);
					   GATTR.TYPTR=NULL;
				   }
			}
			break;

 /*[*/   case SYMBOLS::LBRACK:
			INSYMBOL();
			CSTPART=SETOFSYS(0);
			VARPART=false;
 //           NEW(LSP,POWER);
			LSP = structure::allocate (POWER);
			 // with LSP^
			LSP->ELSET=NULL;
			LSP->resize(0);
			if (SY==SYMBOLS::RBRACK)
			{
				 // with GATTR
				GATTR.TYPTR=LSP;
				GATTR.KIND=CST;
				INSYMBOL();
			}
			else
			{
			   do {
				   EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::RBRACK+SYMBOLS::COLON);
				   if (GATTR.TYPTR!=NULL)
					 if (GATTR.TYPTR->form()!=SCALAR)
					   { CERROR(136);
						GATTR.TYPTR=NULL;
					}
					 else
					   if (COMPTYPES(LSP->ELSET,GATTR.TYPTR))
						 { ALLCONST=false; LOP=23/*SGS*/;
						   if ((GATTR.KIND==CST)&&
							 (GATTR.CVAL.IVAL<=127))
							 { ALLCONST=true;
							   LOWVAL=GATTR.CVAL.IVAL;
							   HIGHVAL=LOWVAL;
							 }
						   LIC=IC;
						   LOAD();
						   if (SY==SYMBOLS::COLON)
						   {
							   INSYMBOL();
							   LOP=20;/*SRS*/;
							   EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::RBRACK);
							   if (COMPTYPES(LSP->ELSET,GATTR.TYPTR))
								   ;
							   else
							   {
									CERROR(137);
									GATTR.TYPTR = NULL;
							   }
							   if (ALLCONST)
								 if ((GATTR.KIND==CST)&&
								   (GATTR.CVAL.IVAL<=127))
									 HIGHVAL=GATTR.CVAL.IVAL;
								else
								{
									LOAD();
									ALLCONST=false;
								}
							   else
								   LOAD();
							}
							if (ALLCONST)
							 {
								IC=LIC;
								 /*FORGET FIRST CONST*/
								CSTPART=CSTPART+SETOFSYS::range(LOWVAL,HIGHVAL);    
							 }
						   else
							 { GEN0(LOP);
							   if (VARPART)
								   GEN0(28/*UNI*/);
							   else VARPART=true;
							 }
						   LSP->ELSET=GATTR.TYPTR;
						   GATTR.TYPTR=LSP;
						 }
					   else
						   CERROR(137);
				   TEST=SY!=SYMBOLS::COMMA;
				   if (!TEST)
					   INSYMBOL();
				}
				 while(!TEST);
				 if (SY==SYMBOLS::RBRACK)
					 INSYMBOL();
				 else
					 CERROR(12);
				}
			   if (VARPART)
			   {
				 if (CSTPART!=SETOFSYS(0))
				   {
					 SETOFSYS *PVAL = static_cast<SETOFSYS*> (&SCONST->PVAL);
					*PVAL=CSTPART;
					 SCONST->CCLASS=PSET;
					 GATTR.CVAL.VALP=SCONST;
					 GATTR.KIND=CST;
					 LOAD();
					 GEN0(28/*UNI*/);
				   }
				 GATTR.KIND=EXPR;
			   }
			 else
			   {
				 SETOFSYS *PVAL = static_cast<SETOFSYS*> (&SCONST->PVAL);
				 *PVAL=CSTPART;
				 SCONST->CCLASS=PSET;
				 GATTR.CVAL.VALP=SCONST;
				 GATTR.KIND=CST;
			   }
			break;
		   
			default:
			   break;
	   } /*switch /*/
	   if (!(FSYS.in(SY)))
	   {
		   CERROR(6);
		   SKIP(FSYS+BNF::FACBEGSYS);
	   }
	 } /*while /*/
} /*FACTOR*/

void BODYPART::TERM(const SETOFSYS &FSYS)
{ /*TERM*/
	ATTR		LATTR;
//	STP			LSP;
	OPERATOR	LOP;

	FACTOR(FSYS+SYMBOLS::MULOP);
	while(SY==SYMBOLS::MULOP)
	{
	   LOAD();
	   LATTR=GATTR;
	   LOP=OP;
	   INSYMBOL();
	   FACTOR(FSYS+SYMBOLS::MULOP);
		LOAD();
		if ((LATTR.TYPTR!=NULL)&&(GATTR.TYPTR!=NULL))
		switch (LOP)
		{
/***/     case MUL:
			FLOATIT(LATTR.TYPTR,false);
			STRETCHIT(LATTR.TYPTR);
			if ((LATTR.TYPTR==INTPTR)&&(GATTR.TYPTR==INTPTR))
				GEN0(15/*MPI*/);
			else if ((LATTR.TYPTR==REALPTR)&&
					 (GATTR.TYPTR==REALPTR))
				GEN0(16/*MPR*/);
			else if ((GATTR.TYPTR->is_long())&&
					(LATTR.TYPTR->is_long()))
			{
				GENLDC(8/*DMP*/);
				GENNR(DECOPS);
			}
			else if ((LATTR.TYPTR->is_power())
				&&COMPTYPES(LATTR.TYPTR,GATTR.TYPTR))
				GEN0(12/*INT*/);
			else {
				CERROR(134);
				GATTR.TYPTR = NULL;
			}
			break;
/*/*/     case RDIV:
				FLOATIT(LATTR.TYPTR,true);
				if ((LATTR.TYPTR==REALPTR)&&
				   (GATTR.TYPTR==REALPTR))
					GEN0(7/*DVR*/);
				 else {
					 CERROR(134);
					 GATTR.TYPTR=NULL;
				 }
			break;
			/*DIV*/
			case IDIV:
				STRETCHIT(LATTR.TYPTR);
				 if ((LATTR.TYPTR==INTPTR)&&
				   (GATTR.TYPTR==INTPTR))
					GEN0(6/*DVI*/);
				 else if ((LATTR.TYPTR->is_long())&&
					 (GATTR.TYPTR->is_long()))
					 {
						 GENLDC(10/*DDV*/);
						 GENNR(DECOPS);
					}
				   else {
					   CERROR(134);
					   GATTR.TYPTR=NULL;
				   }
			break;
/*MOD*/   case IMOD:
				if ((LATTR.TYPTR==INTPTR)&&
				 (GATTR.TYPTR==INTPTR))
				 GEN0(14/*MOD*/);
			   else {
				   CERROR(134);
				   GATTR.TYPTR=NULL;
				}
			break;
/*AND*/   case ANDOP:
				if ((LATTR.TYPTR==BOOLPTR)&&
				 (GATTR.TYPTR==BOOLPTR))
				 GEN0(4/*AND*/);
			   else { 
				   CERROR(134);
				   GATTR.TYPTR=NULL;
				}
			break;
	   } /*switch /*/
	 else GATTR.TYPTR=NULL;
   } /*while /*/
} /*TERM*/

#if 0
void BODYPART::SIMPLEEXPRESSION(SETOFSYS &FSYS)
{
	ASSERT(false);
}
#endif

void BODYPART::SIMPLEEXPRESSION(const SETOFSYS &FSYS)
{
	ATTR LATTR;
	OPERATOR LOP;
	bool SIGNED;

	/*SIMPLEEXPRESSION*/
	SIGNED=false;
	if ((SY==SYMBOLS::ADDOP)
		&&(SET<128>(2,PLUS,MINUS).in(OP)))
	{
		SIGNED=OP==MINUS;
		INSYMBOL();
	}
	TERM(FSYS+SYMBOLS::ADDOP);
	if (SIGNED)
	{
		LOAD();
		if (GATTR.TYPTR==REALPTR)
				GEN0(18/*NGR*/);
		
		else if (GATTR.TYPTR==INTPTR)
				GEN0(17/*NGI*/);

		else if ((GATTR.TYPTR!=NULL)&&
			(GATTR.TYPTR->is_long()))
		{
			GENLDC(6/*DNG*/);
			GENNR(DECOPS);
		}
		else {
			CERROR(134);
			GATTR.TYPTR=NULL;
		}
	}
	while(SY==SYMBOLS::ADDOP)
	{
		LOAD();
		LATTR=GATTR;
		LOP=OP;
		INSYMBOL();
		TERM(FSYS+SYMBOLS::ADDOP);
		LOAD();
		if ((LATTR.TYPTR!=NULL)&&(GATTR.TYPTR!=NULL))
		switch (LOP){
	/*+*/
		case PLUS:
			FLOATIT(LATTR.TYPTR,false);
			STRETCHIT(LATTR.TYPTR);
			if ((LATTR.TYPTR==INTPTR)&&(GATTR.TYPTR==INTPTR))
				GEN0(2/*ADI*/);
			else if ((LATTR.TYPTR==REALPTR)&&(GATTR.TYPTR==REALPTR))
				GEN0(3/*ADR*/);
			else if ((GATTR.TYPTR->is_long())&&
				(LATTR.TYPTR->is_long()))
			{
				GENLDC(2/*DAD*/);
				GENNR(DECOPS);
			}
			else if ((LATTR.TYPTR->is_power())
					&&COMPTYPES(LATTR.TYPTR,GATTR.TYPTR))
				GEN0(28/*UNI*/);
			else {
				CERROR(134);
				GATTR.TYPTR=NULL;
			}
			break;
	/*-*/
		case MINUS:
			FLOATIT(LATTR.TYPTR,false);
			STRETCHIT(LATTR.TYPTR);
			if ((LATTR.TYPTR==INTPTR)&&(GATTR.TYPTR==INTPTR))
				GEN0(21/*SBI*/);
			else if ((LATTR.TYPTR==REALPTR)
				&&(GATTR.TYPTR==REALPTR))
				GEN0(22/*SBR*/);
			else if ((GATTR.TYPTR->is_long())&&
					(LATTR.TYPTR->is_long()))
			{ 
				GENLDC(4/*DSB*/);
				GENNR(DECOPS);
			}
			else if ((LATTR.TYPTR->is_power())
					&&COMPTYPES(LATTR.TYPTR,GATTR.TYPTR))
				GEN0(5/*Dif /*/);
			else {
				CERROR(134);
				GATTR.TYPTR=NULL;
			}

		break;
		/*OR*/ case OROP:
			if ((LATTR.TYPTR==BOOLPTR)&&(GATTR.TYPTR==BOOLPTR))
				GEN0(13/*IOR*/);
			else {
				CERROR(134);
				GATTR.TYPTR=NULL;
			}
			break;

		default:
			break;
		} /*switch /*/
		else
			GATTR.TYPTR=NULL;
	} /*while */
} /*SIMPLEEXPRESSION*/

void BODYPART::EXPRESSION(const SETOFSYS &FSYS)
{
	/*EXPRESSION*/
	//	LABEL 1;    /* STRING COMPARE KLUDGE */
	ATTR LATTR;
	OPERATOR LOP;
	int  TYPIND;
	ADDRRANGE LSIZE;
	bool LSTRING,GSTRING;
	int LMIN,LMAX;

	SIMPLEEXPRESSION(FSYS+SYMBOLS::RELOP);
	if (SY!=SYMBOLS::RELOP)
		return;

	LSTRING=(GATTR.KIND==CST)&&
		(STRGTYPE(GATTR.TYPTR)||(GATTR.TYPTR==CHARPTR));
	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR->form()<=POWER)
			LOAD();
		else
			LOADADDRESS();
	LATTR=GATTR;
	LOP=OP;
	INSYMBOL();
	SIMPLEEXPRESSION(FSYS);
	GSTRING=(GATTR.KIND==CST)&&
		(STRGTYPE(GATTR.TYPTR)||(GATTR.TYPTR==CHARPTR));

	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR->form()<=POWER)
			LOAD();
		else
			LOADADDRESS();

	if ((LATTR.TYPTR!=NULL)&&(GATTR.TYPTR!=NULL))
	if (LOP==INOP)
		if (GATTR.TYPTR->is_power())
			if (COMPTYPES(LATTR.TYPTR,GATTR.TYPTR->ELSET))
				GEN0(11/*INN*/);
			else
			{
				CERROR(129);
				GATTR.TYPTR=NULL;
			}
		else
		{
			CERROR(130);
			GATTR.TYPTR=NULL;
		}
	else 
	{
		if (LATTR.TYPTR!=GATTR.TYPTR)
		{
			FLOATIT(LATTR.TYPTR,false);
			STRETCHIT(LATTR.TYPTR);
		}
		if (LSTRING)
		{
			if (PAOFCHAR(GATTR.TYPTR))
				if (!GATTR.TYPTR->is_string())
				{
					GEN0(29/*S2P*/);
					MAKEPA(LATTR.TYPTR,GATTR.TYPTR);
				}
		}
		else if (GSTRING)
		{
			if (PAOFCHAR(LATTR.TYPTR))
				if (!LATTR.TYPTR->is_string())
				{
					GEN0(80/*S1P*/);
					MAKEPA(GATTR.TYPTR,LATTR.TYPTR);
				}
		}
		if ((LSTRING&&STRGTYPE(GATTR.TYPTR))||
			(GSTRING&&STRGTYPE(LATTR.TYPTR)))
			goto retry;

		if (COMPTYPES(LATTR.TYPTR,GATTR.TYPTR))
		{
			LSIZE=(int)LATTR.TYPTR->size(); /*INVALID for LONG INTS*/
			switch (LATTR.TYPTR->form())
			{
			case SCALAR:
				if (LATTR.TYPTR==REALPTR)
					TYPIND=1;
				else if (LATTR.TYPTR==BOOLPTR)
					TYPIND=3;
				else
					TYPIND=0;
				break;
			case POINTER:
				if (SET<128>(2,LTOP,LEOP,GTOP,GEOP).in(LOP))
					CERROR(131);
				TYPIND=0;
				break;
			case LONGINT:
				TYPIND=7;
				break;
			case POWER:
				if (SET<128>(LTOP,GTOP).in(LOP))
					CERROR(132);
				TYPIND=4;
				break;
			case ARRAYS:
				TYPIND=6;
				if (PAOFCHAR(LATTR.TYPTR))
					if (LATTR.TYPTR->is_string())
retry:					TYPIND=2;
					else
					{
						TYPIND=5;
						if (LATTR.TYPTR->INXTYPE!=NULL)
						{
							GETBOUNDS(LATTR.TYPTR->INXTYPE,LMIN,LMAX);
							LSIZE=LMAX-LMIN+1;
						}
					}
				else if (SET<128>(4,LTOP,LEOP,GTOP,GEOP).in(LOP))
					CERROR(131);
				break;
			case RECORDS:
				if (SET<128>(4,LTOP,LEOP,GTOP,GEOP).in(LOP))
					CERROR(131);
				TYPIND=6;
				break;
			case FILES:
				CERROR(133);
				TYPIND=0;
				break;
			default:
				TYPIND=0;
				break;
			}
			if (TYPIND==7)
			{
				GENLDC(ORD(LOP));
				GENLDC(16/*DCMP*/);
				GENNR(DECOPS);
			}
			else switch (LOP)
			{
			case LTOP:
				GEN2(53/*LES*/,TYPIND,LSIZE);
				break;
			case LEOP:
				GEN2(52/*LEQ*/,TYPIND,LSIZE);
				break;
			case GTOP:
				GEN2(49/*GRT*/,TYPIND,LSIZE);
				break;
			case GEOP:
				GEN2(48/*GEQ*/,TYPIND,LSIZE);
				break;
			case NEOP:
				GEN2(55/*NEQ*/,TYPIND,LSIZE);
				break;
			case EQOP:
				GEN2(47/*EQU*/,TYPIND,LSIZE);
				break;
			default:
				break;
			}
		}
		else
			CERROR(129);
	}
	GATTR.TYPTR=BOOLPTR;
	GATTR.KIND=EXPR;
} /*EXPRESSION*/

void BODYPART::LABEL()
{
	DISPRANGE TTOP;
	LABELP LLP; 
	TTOP=TOP;
	while(DISPLAY[TTOP].OCCUR!=BLCK)
		TTOP=TTOP-1;

	LLP=DISPLAY[TTOP].BLCK.FLABEL;
	while(LLP!=NULL)
	{
		if (LLP->LABVAL==VAL.IVAL)
		{
			if (LLP->CODELBP->DEFINED)
				CERROR(165);
			PUTLABEL(LLP->CODELBP);
			goto fubar;
		}
		else
			LLP=LLP->NEXTLAB;
	}
	CERROR(167);

fubar:
	INSYMBOL();
	if (SY==SYMBOLS::COLON)
		INSYMBOL();
	else
		CERROR(5);
}

void BODYPART::STATEMENT(const SETOFSYS &FSYS)
{
	CTP	LCP;
	WRITELN(OUTPUT,"bodypart::statement");

	STMTLEV=STMTLEV+1;
	if (SY==SYMBOLS::INTCONST)/*LABEL*/
	{
		LABEL();	
	}
	if (options.DEBUGGING)
	{
		GEN1(85/*BPT*/,SCREENDOTS+1);
		options.BPTONLINE=true;
	}
	if (!(FSYS+SYMBOLS::IDENT).in(SY))
	{
		CERROR(6);
		SKIP(FSYS);
	}
	SETOFIDS S6 = BNF::VARS+SETOFIDS(3,FIELD,FUNC,PROC1);
	if ((BNF::STATBEGSYS+SYMBOLS::IDENT).in(SY))
	{
#if 0
		MARK(HEAP); /*for(LABEL CLEANUP*/
#endif
		SYMBOLS::SYMBOL SY0 = SY;
		if (SY0==SYMBOLS::IDENT)
			 SEARCHID(S6,LCP);

		INSYMBOL();
		switch (SY0)
		{
			case SYMBOLS::IDENT:
				if (LCP->KLASS==PROC1)
					CALL(FSYS,LCP);
				else
					ASSIGNMENT(FSYS,LCP);
				break;
			
			case SYMBOLS::BEGINSY:         
				COMPOUNDSTATEMENT(FSYS);
				break;;
			
			case SYMBOLS::GOTOSY:
				GOTOSTATEMENT();
				break;

			case SYMBOLS::IFSY:
				IFSTATEMENT(FSYS);
				break;
			
			case SYMBOLS::CASESY:
				CASESTATEMENT(FSYS);
				break;
			
			case SYMBOLS::WHILESY:
				WHILESTATEMENT(FSYS);
				break;
			
			case SYMBOLS::REPEATSY:
				REPEATSTATEMENT(FSYS);
				break;
			
			case SYMBOLS::FORSY:
				FORSTATEMENT(FSYS,LCP);
				break;
			
			case SYMBOLS::WITHSY:
				WITHSTATEMENT(FSYS);
				break;

			default:
				break;
		}
#if 0
		RELEASE(HEAP);
#endif
		if (IC+100>MAXCODE)
		{
			CERROR(253);
			IC=0;
		}
		if (!(SETOFSYS(4,SYMBOLS::SEMICOLON,SYMBOLS::ENDSY,SYMBOLS::ELSESY,SYMBOLS::UNTILSY).in(SY)))
		{
			CERROR(6);
			SKIP(FSYS);
		}
	}
	STMTLEV=STMTLEV-1;
} /*STATEMENT*/

void BODYPART::NEWSTMT(const SETOFSYS &FSYS)
{
	STP			LSP,LSP1;
	int			VARTS;
	ADDRRANGE	LSIZE;
	VALU		LVAL;
	WRITELN(OUTPUT,"bodypart::newstmt");

	VARIABLE(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
	LOADADDRESS();
	LSP=NULL;
	LSP1=NULL;
	VARTS=0;
	LSIZE=0;
	if (GATTR.TYPTR!=NULL)
	{
		if (GATTR.TYPTR->is_pointer())
		{
			if (GATTR.TYPTR->ELTYPE!=NULL)
			{
				LSIZE=(int)GATTR.TYPTR->ELTYPE->size();
				if (GATTR.TYPTR->ELTYPE->form()==RECORDS)
					LSP=GATTR.TYPTR->ELTYPE->RECVAR;
			}
		}
		else
			CERROR(116); 
	}	
	while(SY==SYMBOLS::COMMA)
	{
		INSYMBOL();
		CONSTANT(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT,LSP1,LVAL);

		object *debug_obj1;
		object::scalar *debug_obj2;

		if (LSP1->is_scalar())
		{
			debug_obj1 = object::allocate (SCALAR);
			memcpy (debug_obj1,LSP1,sizeof(object::scalar));
			debug_obj2 = reinterpret_cast<object::scalar*>(debug_obj1);
		}

		// error in original source?
		// LSP = LSP1; // added - GLG 2022-09-23
		VARTS=VARTS+1;
		if (LSP==NULL)
			CERROR(158);
		else if (LSP->form()!=TAGFLD)
			CERROR(162);
		else if (LSP->TAGFIELDP!=NULL)
			if (STRGTYPE(LSP1)||(LSP1==REALPTR))
				CERROR(159);
			else 
		if (COMPTYPES(LSP->TAGFIELDP->IDTYPE,LSP1))
		{
			LSP1=LSP->FSTVAR;
			while(LSP1!=NULL)
			{
				// with LSP1^
				if (LSP1->VARVAL.IVAL==LVAL.IVAL)
				{
					LSIZE=(int)LSP1->size();
					LSP=LSP1->SUBVAR;
					goto retry;
				}
				else
					LSP1=LSP1->NXTVAR;    
			}
			LSIZE=(int)LSP->size();
			LSP=NULL;
		}
		else
			CERROR(116);
retry:
		;
	} /*while*/
	GENLDC(LSIZE);
	GEN1(30/*CSP*/,1/*NEW*/);
} /*NEWSTMT*/ ;

void BODYPART::STRGVAR(const SETOFSYS &FSYS, bool MUSTBEVAR)
{
	WRITELN(OUTPUT,"bodypart::strgvar");

	EXPRESSION(FSYS);
	// with GATTR
	if (((GATTR.KIND==CST)&&(GATTR.TYPTR==CHARPTR))
		||STRGTYPE(GATTR.TYPTR))
		if (GATTR.KIND==VARBL)
			LOADADDRESS();
		else
		{
			if (MUSTBEVAR)
				CERROR(154);
			if (GATTR.KIND==CST)
			{
				if (GATTR.TYPTR==CHARPTR)
				{
					// with SCONST^)
					{
						SCONST->CCLASS=STRG;
						SCONST->SLGTH=1;
						SCONST->SVAL[1]=(char)(GATTR.CVAL.IVAL);
					}
					GATTR.CVAL.VALP=SCONST;
//					NEW(TYPTR,ARRAYS,true,true);
					GATTR.TYPTR = structure::allocate (ARRAYS);
					*GATTR.TYPTR = *STRGPTR;
					GATTR.TYPTR->MAXLENG=1;
				}
				LOADADDRESS();
			}
		}
	else
	{
		if (GATTR.TYPTR!=NULL)
			CERROR(125);
		GATTR.TYPTR=STRGPTR;
	}
} /*STRGVAR*/ ;

void BODYPART::ROUTINE(const SETOFSYS &FSYS, int LKEY)
{ /*ROUTINE*/
	switch (LKEY){
		case 12:
			NEWSTMT(FSYS);
			break;
		case 13:
		case 14:
			UNITIO(FSYS,LKEY);
			break;
		case 15:
			CONCAT(FSYS);
			break;
		case 18:
		case 19:
		case 43:
			COPYDELETE(FSYS,LKEY);
			break;
		case 21:
		case 22:
		case 27:
			MOVE(FSYS,LKEY);
			break;
		case 23:
			EXIT0();
			break;
		case 31:
			CLOSE(FSYS);
			break;
		case 32:
		case 34:
		case 35:
		case 40:
			GETPUTETC(FSYS,LKEY);
			break;
		case 36:
			SCAN(FSYS);
			break;
		case 37:
		case 38:
			BLOCKIO(FSYS,LKEY);
			break;
		case 41:
			SIZEOF();
			break;
		case 42:
			STR(FSYS);
			break;
		default:
			break;
	} /*CASES*/
} /*ROUTINE*/ ;

void BODYPART::ASSIGNMENT(const SETOFSYS &FSYS, CTP FCP)
{
	ATTR LATTR;
	bool CSTRING,PAONLEFT;
	int LMIN,LMAX;
	WRITELN(OUTPUT,"bodypart::assignment: ",FCP->NAME);
	   
	SELECTOR(FSYS+SYMBOLS::BECOMES,FCP);
	if (SY!=SYMBOLS::BECOMES)
	{
		CERROR(51);
		return;
	}
	LMAX=0;
	CSTRING=false;
	if (GATTR.TYPTR!=NULL)
		if ((GATTR.ACCESS()==INDRCT)||(GATTR.TYPTR->form()>POWER))
			LOADADDRESS();

	PAONLEFT=PAOFCHAR(GATTR.TYPTR);
	LATTR=GATTR;
	INSYMBOL();
	EXPRESSION(FSYS);
	
	if (GATTR.KIND==CST)
		CSTRING=(GATTR.TYPTR==CHARPTR)||STRGTYPE(GATTR.TYPTR);

//		ASSERT(GATTR.CVAL.VALP!=NULL);
	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR->form()<=POWER)
			LOAD();
		else
			LOADADDRESS();

	if ((LATTR.TYPTR==NULL)||(GATTR.TYPTR==NULL))
		return;

	if (GATTR.TYPTR==INTPTR)
		if (COMPTYPES(REALPTR,LATTR.TYPTR))
		{
			GEN0(10/*FLT*/);
			GATTR.TYPTR=REALPTR;
		}
	if (COMPTYPES(LONGINTPTR,LATTR.TYPTR))
	{
		if (GATTR.TYPTR==INTPTR)
		{
			GENLDC(INTSIZE);
			GATTR.TYPTR=LONGINTPTR;
		}
		if (GATTR.TYPTR->form()!=LONGINT)
		{
			CERROR(129);
			GATTR.TYPTR=LONGINTPTR;
		}
	}
	if (PAONLEFT)
		if (LATTR.TYPTR->is_string())
		{
			if (CSTRING&&(GATTR.TYPTR==CHARPTR))
				GATTR.TYPTR=STRGPTR;
			else
			{
				// mystery string type?
			}
		}
		else if (LATTR.TYPTR->INXTYPE!=NULL)
		{
			GETBOUNDS(LATTR.TYPTR->INXTYPE,LMIN,LMAX);
			LMAX=LMAX-LMIN+1;
			
			if (CSTRING&&(GATTR.TYPTR!=CHARPTR))
			{
				GEN0(80/*S1P*/);
// if we change the != to <= then MAYBE it will be possible 
// to do a simple assignment of a shorter string into a
// regular packed array of char [256 whic is what a Pascal string
// is -- or is it? ]if (LMAX<=GATTR.TYPTR->MAXLENG) GLG 2022-08-21

				if (LMAX<=GATTR.TYPTR->MAXLENG)
					CERROR(129);
				GATTR.TYPTR=LATTR.TYPTR;
			}
		}
		else
			GATTR.TYPTR=LATTR.TYPTR;

	if (COMPTYPES(LATTR.TYPTR,GATTR.TYPTR))
		switch (LATTR.TYPTR->form())
		{
		case SUBRANGE:
		   if (options.RANGECHECK)
		   {
			   GENLDC(LATTR.TYPTR->MIN.IVAL);
			   GENLDC(LATTR.TYPTR->MAX.IVAL);
			   GEN0(8/*CHK*/);
		   }
		   STORE(LATTR);
		   break;

		case POWER:
		   GEN1(32/*ADJ*/,(int)LATTR.TYPTR->size());
		   STORE(LATTR);
		   break;

		case SCALAR:
		case POINTER:
		   STORE(LATTR);
		   break;

		case LONGINT:
		   GENLDC((int)LATTR.TYPTR->size());
		   GENLDC(0/*DAJ*/);
		   GENNR(DECOPS);
		   STORE(LATTR);
		   break;

		case ARRAYS:
		   if (PAONLEFT)
			   if (LATTR.TYPTR->is_string())
				   GEN1(42/*SAS*/,LATTR.TYPTR->MAXLENG);
			   else
				   GEN1(41/*MVB*/,LMAX);
		   else
			   GEN1(40/*MOV*/,(int)LATTR.TYPTR->size());
		   break;
		
		case RECORDS:
		   GEN1(40/*MOV*/,(int)LATTR.TYPTR->size());
		   break;
		
		case FILES:
		   CERROR(146);
		   break;
		
		default:
		   break;
		}
	else
		CERROR(129);
} /*ASSIGNMENT*/

void BODYPART::VARIABLE(const SETOFSYS &FSYS)
{
	WRITELN(OUTPUT,"bodypart::variable");
	WRITELN(OUTPUT,"searching for <\"",ID,"\">");
	CTP LCP;
	if (SY==SYMBOLS::IDENT)
	{
		SEARCHID(BNF::VARS+FIELD,LCP);
		INSYMBOL();
	}
	else {
		CERROR(2);
		LCP=UVARPTR;
	}
	SELECTOR(FSYS,LCP);
} /*VARIABLE*/

void BODYPART::MOVE(const SETOFSYS &FSYS, int LKEY)
{
	VARIABLE(FSYS+SYMBOLS::COMMA);
	LOADADDRESS();
	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	if (LKEY==27)
	{
		EXPRESSION(FSYS+SYMBOLS::COMMA);
		LOAD();
	}
	else
	{
		VARIABLE(FSYS+SYMBOLS::COMMA);
		LOADADDRESS();
	}
	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	EXPRESSION(FSYS+SYMBOLS::RPARENT);
	LOAD();
	if (LKEY==27)
		GEN1(30/*CSP*/,10/*FLC*/);
	else if (LKEY==21)
		GEN1(30/*CSP*/,2/*MVL*/);
	else
	   GEN1(30/*CSP*/,3/*MVR*/);
} /*MOVE*/

void BODYPART::UNITIO(const SETOFSYS &FSYS, int LKEY)
{
	if (GATTR.TYPTR!=INTPTR)
		CERROR(125);
	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	VARIABLE(FSYS+SYMBOLS::COMMA);
	LOADADDRESS();
	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
	LOAD();
	if (GATTR.TYPTR!=INTPTR)
		CERROR(125);
	if (SY==SYMBOLS::COMMA)
	{
		INSYMBOL();
		if (SY==SYMBOLS::COMMA)
			GENLDC(0);
		else
		{
			EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
			LOAD();
			if (GATTR.TYPTR!=INTPTR)
				CERROR(125);
		}
	}
	else
		GENLDC(0);
	if (SY==SYMBOLS::COMMA)
	{
		INSYMBOL();
		EXPRESSION(FSYS+SYMBOLS::RPARENT);
		LOAD();
		if (GATTR.TYPTR!=INTPTR)
			CERROR(125);
	}
	else
		GENLDC(0);
	if (LKEY==13)
		GEN1(30/*CSP*/,5/*URD*/);
	else
		GEN1(30/*CSP*/,6/*UWT*/);
} /*UNITIO*/

void BODYPART::CONCAT(const SETOFSYS &FSYS)
{
	ADDRRANGE	LLC; 
	int		TEMPLGTH;
	
	TEMPLGTH=0;
	LLC=LC;
	LC=LC+(STRGLGTH/CHRSPERWD)+1;
	GENLDC(0); GEN2(56/*STR*/,0,LLC);
	GEN2(50/*LDA*/,0,LLC);
	do {
		STRGVAR(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT,false);
		TEMPLGTH=TEMPLGTH+GATTR.TYPTR->MAXLENG;
		if (TEMPLGTH < STRGLGTH)
			GENLDC(TEMPLGTH);
		else
			GENLDC(STRGLGTH);
		GEN2(77/*CXP*/,0/*SYS*/,23/*SCONCAT*/);
		GEN2(50/*LDA*/,0,LLC);
		TEST=SY!=SYMBOLS::COMMA;
		if (!TEST)
			INSYMBOL();
	}
	while(!TEST);
	if (TEMPLGTH<STRGLGTH)
		LC=LLC+(TEMPLGTH/CHRSPERWD)+1;
	else
		TEMPLGTH=STRGLGTH;
	if (LC>LCMAX)
		LCMAX=LC;
	LC=LLC;
 // with GATTR
	{
//		NEW(TYPTR,ARRAYS,true,true);
		GATTR.TYPTR = structure::allocate (ARRAYS); 
		*GATTR.TYPTR=*STRGPTR;
		GATTR.TYPTR->MAXLENG=TEMPLGTH;
	}
} /*CONCAT*/

void BODYPART::COPYDELETE(const SETOFSYS &FSYS, int LKEY)
{
	ADDRRANGE	LLC;
	STP			LSP;

	if (LKEY==19)
	{
		LLC=LC;
		LC=LC+(STRGLGTH/CHRSPERWD)+1;
	}
	if (LKEY!=43)
	{
		STRGVAR(FSYS+SYMBOLS::COMMA, LKEY==18);
	 if (LKEY==19)
	   {
		   LSP=GATTR.TYPTR;
		   GEN2(50/*LDA*/,0,LLC);
	   }
	 if (SY==SYMBOLS::COMMA)
		 INSYMBOL();
	 else CERROR(20);
   }
	EXPRESSION(FSYS+SYMBOLS::COMMA);
	LOAD();
	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR!=INTPTR)
			CERROR(125);
	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	EXPRESSION(FSYS+SYMBOLS::RPARENT);
	LOAD();
	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR!=INTPTR)
			CERROR(125);
	if (LKEY==19)
	{
	 GEN2(77/*CXP*/,0/*SYS*/,25/*SCOPY*/);
	 GEN2(50/*LDA*/,0,LLC);
	 if (LSP->MAXLENG < STRGLGTH)
	   LC=LLC+(LSP->MAXLENG/CHRSPERWD)+1;
	 if (LC>LCMAX)LCMAX=LC;
	 LC=LLC;
	 GATTR.TYPTR=LSP;
	}
	else if (LKEY==43)
		GEN2(77/*CXP*/,0/*SYS*/,29/*GOTOXY*/);
	else
		GEN2(77/*CXP*/,0/*SYS*/,26/*SDELETE*/);
} /*COPYDELETE*/

void BODYPART::STR(const SETOFSYS &FSYS)
{
	// with GATTR
	if (COMPTYPES(LONGINTPTR,GATTR.TYPTR))
		; // why?
	else if (GATTR.TYPTR==INTPTR)
	{ 
		GENLDC(1);
		GATTR.TYPTR=LONGINTPTR;
	}
	else
		CERROR(125);
	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	STRGVAR(FSYS+SYMBOLS::RPARENT, true);
	if (STRGTYPE(GATTR.TYPTR))
	{
		GENLDC(GATTR.TYPTR->MAXLENG);
		GENLDC(12/*DSTR*/);
		GENNR(DECOPS);
	}
	else CERROR(116);
} /*STR*/

void BODYPART::CLOSE(const SETOFSYS &FSYS)
{
	VARIABLE(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
	LOADADDRESS();
	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR->is_file()==false)
			CERROR(125);

	if (SY==SYMBOLS::COMMA)
	{
		INSYMBOL();
		if (SY==SYMBOLS::IDENT)
		{
			if (ID=="NORMAL  ")
				GENLDC(0);
			else if (ID=="LOCK    ")
				GENLDC(1);
			else if (ID=="PURGE   ")
				GENLDC(2);
			else if (ID=="CRUNCH  ")
				GENLDC(3);
			else
				CERROR(2);
			INSYMBOL();
		}
		else
			CERROR(2);
	}
	else GENLDC(0);
	GEN2(77/*CXP*/,0/*SYS*/,6/*FCLOSE*/);
	if (options.IOCHECK)
		GEN1(30/*CSP*/,0/*IOC*/);
} /*CLOSE*/

void BODYPART::GETPUTETC(const SETOFSYS &FSYS, int LKEY)
{
	VARIABLE(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
	LOADADDRESS();
	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR->is_file()==false)
			CERROR(125);
		else
			if (GATTR.TYPTR->FILTYPE==NULL)
				CERROR(399);
	switch (LKEY)
	{
	case 32:
		if (SY==SYMBOLS::COMMA)
		{
			INSYMBOL(); EXPRESSION(FSYS+SYMBOLS::RPARENT);
			LOAD();
			if (GATTR.TYPTR!=INTPTR)
				CERROR(125);
		}
		else CERROR(125);
		GENNR(SEEK);
		break;
	case 34:
		GEN2(77/*CXP*/,0/*SYS*/,7/*FGET*/);
		break;
	case 35:
		GEN2(77/*CXP*/,0/*SYS*/,8/*FPUT*/);
		break;
	case 40:
		if (GATTR.TYPTR!=NULL)
			if (GATTR.TYPTR->FILTYPE!=CHARPTR)
				CERROR(399);
		GENLDC(12);
		GENLDC(0);
		GEN2(77/*CXP*/,0/*SYS*/,17/*WRC*/);
		break;
	default:
		break;
	} /*switch /*/
	if (options.IOCHECK)
		GEN1(30/*CSP*/,0/*IOC*/);
} /*GETPUTETC*/ 

void BODYPART::SCAN(const SETOFSYS &FSYS)
{
	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR!=INTPTR)
			CERROR(125);

	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	if (SY==SYMBOLS::RELOP)
	{
		if (OP==EQOP)
			GENLDC(0);
		else if (OP==NEOP)
			GENLDC(1);
		else
			CERROR(125);
		INSYMBOL();
	}
	else
		CERROR(125);
	EXPRESSION(FSYS+SYMBOLS::COMMA);
	LOAD();
	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR!=CHARPTR)
			CERROR(125);
	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	VARIABLE(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
	LOADADDRESS();
	if (SY==SYMBOLS::COMMA)
	{
		INSYMBOL();
		EXPRESSION(FSYS+SYMBOLS::RPARENT);
		LOAD();
	}
	else
		GENLDC(0);
	GEN1(30/*CSP*/,11/*SCN*/);
	GATTR.TYPTR=INTPTR;
} /*SCAN*/ 

void BODYPART::BLOCKIO(const SETOFSYS &FSYS, int LKEY)
{
	VARIABLE(FSYS+SYMBOLS::COMMA);
	LOADADDRESS();
	if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR->is_file()==false)
			CERROR(125);
		else if (GATTR.TYPTR->FILTYPE!=NULL)
				CERROR(399);
	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	VARIABLE(FSYS+SYMBOLS::COMMA);
	LOADADDRESS();
	if (SY==SYMBOLS::COMMA)
		INSYMBOL();
	else
		CERROR(20);
	EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
	LOAD();
	if (GATTR.TYPTR!=INTPTR)
		CERROR(125);
	if (SY==SYMBOLS::COMMA)
	{
		INSYMBOL();
		EXPRESSION(FSYS+SYMBOLS::RPARENT);
		LOAD();
		if (GATTR.TYPTR!=INTPTR)
			CERROR(125);
	}
	else GENLDC(-1);
	if (LKEY==37)
		GENLDC(1);
	else GENLDC(0);
	GENLDC(0);
	GENLDC(0);
	GEN2(77/*CXP*/,0/*SYS*/,28/*BLOCKIO*/);
	if (options.IOCHECK)
		GEN1(30/*CSP*/,0/*IOC*/);
	GATTR.TYPTR=INTPTR;
} /*BLOCKIO*/ 

void BODYPART::SIZEOF()
{
	CTP LCP;
	if (SY==SYMBOLS::IDENT)
	{
		SEARCHID(BNF::VARS+TYPES+FIELD,LCP);
		INSYMBOL();
		if (LCP->IDTYPE!=NULL)
			GENLDC((int)LCP->IDTYPE->size()*CHRSPERWD);
	}
	GATTR.TYPTR=INTPTR;
} /*SIZEOF*/

void BODYPART::LOADIDADDR(CTP FCP)
{
	// with FCP^
	if (FCP->KLASS==ACTUALVARS)
		if (FCP->VLEV==1)
			GEN1(37/*LAO*/,FCP->VADDR);
		else GEN2(50/*LDA*/,LEVEL-FCP->VLEV,FCP->VADDR);
	else  /*FORMALVARS*/
		if (FCP->VLEV==1)
			GEN1(39/*LDO*/,FCP->VADDR);
		else
			GEN2(54/*LOD*/,LEVEL-FCP->VLEV,FCP->VADDR);
} /*LOADIDADDR*/ 


void BODYPART::READ(const SETOFSYS &FSYS, int LKEY, bool param)
{
	CTP	FILEPTR,LCP;
	bool WASLPARENT = param;

	FILEPTR=INPUTPTR;
	if ((SY==SYMBOLS::IDENT)&&WASLPARENT)
	{
		SEARCHID(BNF::VARS+FIELD,LCP);
		if (LCP->IDTYPE!=NULL)
			if (LCP->IDTYPE->is_file()==true)
				if (LCP->IDTYPE->FILTYPE==CHARPTR)
				{
					INSYMBOL();
					FILEPTR=LCP;
					if (!(SETOFSYS(2,SYMBOLS::COMMA,SYMBOLS::RPARENT).in(SY)))
						CERROR(20);
					if (SY==SYMBOLS::COMMA)
						INSYMBOL();
				}
	}
	else if (WASLPARENT)
		CERROR(2);
	if (WASLPARENT&&(SY!=SYMBOLS::RPARENT))
	{
		do {
			LOADIDADDR(FILEPTR);
			VARIABLE(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
			if (GATTR.ACCESS()==BYTE1)
				CERROR(103);
			LOADADDRESS();
			if (GATTR.TYPTR!=NULL)
				if (COMPTYPES(INTPTR,GATTR.TYPTR))
					GEN2(77/*CXP*/,0/*SYS*/,12/*FRDI*/);
				else if (COMPTYPES(REALPTR,GATTR.TYPTR))
				GENNR(FREADREAL);
				else if (COMPTYPES(LONGINTPTR,GATTR.TYPTR))
				{
					GENLDC((int)GATTR.TYPTR->size());
					GENNR(FREADDEC);
				}
				else if (COMPTYPES(CHARPTR,GATTR.TYPTR))
					GEN2(77/*CXP*/,0/*SYS*/,16/*FRDC*/);
				else if (STRGTYPE(GATTR.TYPTR))
				{
					GENLDC(GATTR.TYPTR->MAXLENG);
					GEN2(77/*CXP*/,0/*SYS*/,18/*FRDS*/);
				}
				else
					CERROR(125);
				if (options.IOCHECK)
					GEN1(30/*CSP*/,0/*IOC*/);
				TEST=(SY!=SYMBOLS::COMMA);
				if (!TEST)
					INSYMBOL();
		}
		while(!TEST);
	}
	if (LKEY==2)
	{
		LOADIDADDR(FILEPTR);
		GEN2(77/*CXP*/,0/*SYS*/,21/*FRLN*/);
		if (options.IOCHECK)
			GEN1(30/*CSP*/,0/*IOC*/);
	}
} /*READ*/

void BODYPART::WRITE(const SETOFSYS &FSYS, int LKEY, bool params)
{
	STP		LSP;
	bool	DEFAULT;
	CTP		FILEPTR,LCP;
	int		LMIN,LMAX;

	bool WASLPARENT=params;
	FILEPTR=OUTPUTPTR;
	if ((SY==SYMBOLS::IDENT)&&WASLPARENT)
	{
		SEARCHID(BNF::VARS+FIELD+KONST+FUNC,LCP);
		if (LCP->IDTYPE!=NULL)
			if (LCP->IDTYPE->is_file()==true)
				if (LCP->IDTYPE->FILTYPE==CHARPTR)
				{
					INSYMBOL();
					FILEPTR=LCP;
					if (!(SETOFSYS(2,SYMBOLS::COMMA,SYMBOLS::RPARENT).in(SY)))
						CERROR(20);
					if (SY==SYMBOLS::COMMA)
						INSYMBOL();
				}
	}
	if (WASLPARENT&&(SY!=SYMBOLS::RPARENT))do
	{
		LOADIDADDR(FILEPTR);
		EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::COLON+SYMBOLS::RPARENT);
		LSP=GATTR.TYPTR;
		if (LSP!=NULL)
		{
			if (LSP->form()>LONGINT)
				LOADADDRESS();
			else
			{
				LOAD();
				if (LSP->is_long())
				{
					GENLDC(DECSIZE(MAXDEC));
					GENLDC(0/*DAJ*/);
					GENNR(DECOPS);
				}
			}
		}
		if (SY==SYMBOLS::COLON)
		{
			INSYMBOL();
			EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::COLON+SYMBOLS::RPARENT);
			if (GATTR.TYPTR!=NULL)
				if (GATTR.TYPTR!=INTPTR)
					CERROR(20);
			LOAD();
			DEFAULT=false;
		}
		else DEFAULT=true;
		if (LSP==INTPTR)
		{
			if (DEFAULT)
				GENLDC(0);
			GEN2(77/*CXP*/,0/*SYS*/,13/*FWRI*/);
		}
		else if (LSP==REALPTR)
		{
			if (DEFAULT)GENLDC(0);
			if (SY==SYMBOLS::COLON)
			{
				INSYMBOL();
				EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
				LOAD();
				if (GATTR.TYPTR!=NULL)
					if (GATTR.TYPTR!=INTPTR)
						CERROR(125);
			}
			else
				GENLDC(0);
			GENNR(FWRITEREAL);
		}
		else if (COMPTYPES(LSP,LONGINTPTR))
		{
			if (DEFAULT)
				GENLDC(0);
			GENNR(FWRITEDEC);
		}
		else if (LSP==CHARPTR)
		{
			if (DEFAULT)
				GENLDC(0);
			GEN2(77/*CXP*/,0/*SYS*/,17/*FWRC*/);
		}
		else if (STRGTYPE(LSP))
		{
			if (DEFAULT)
				GENLDC(0);
			GEN2(77/*CXP*/,0/*SYS*/,19/*FWRS*/);
		}
		else if (PAOFCHAR(LSP))
		{
			LMAX=0;
			if (LSP->INXTYPE!=NULL)
			{
				GETBOUNDS(LSP->INXTYPE,LMIN,LMAX);
				LMAX=LMAX-LMIN+1;
			}
			if (DEFAULT)
				GENLDC(LMAX);
			GENLDC(LMAX);
			GEN2(77/*CXP*/,0/*SYS*/,20/*FWRB*/);
		}
		else
			CERROR(125);
		if (options.IOCHECK)
			GEN1(30/*CSP*/,0/*IOC*/);
		TEST=(SY!=SYMBOLS::COMMA);
		if (!TEST)
			INSYMBOL();
	}
	while(!TEST);
	if (LKEY==4)/*WRITELN*/
	{
		LOADIDADDR(FILEPTR);
		GEN2(77/*CXP*/,0/*SYS*/,22/*FWLN*/);
		if (options.IOCHECK)
			GEN1(30/*CSP*/,0/*IOC*/);
	}
} /*WRITE*/

void BODYPART::CALLNONSPECIAL(const SETOFSYS &FSYS, CTP FCP)
{
//	LABEL 1;
	CTP NXT;
	STP LSP;
	bool LB;
	int LMIN,LMAX;

	// with FCP^
	NXT=FCP->NEXT;
	if (FCP->PFDECKIND==DECLARED)
		if (FCP->PFKIND!=ACTUAL)
			CERROR((int)400);

	if (SY==SYMBOLS::LPARENT)
	{
		do {
			if (NXT==NULL)
				CERROR(126);
			INSYMBOL();
			EXPRESSION(FSYS+SYMBOLS::COMMA+SYMBOLS::RPARENT);
			if ((GATTR.TYPTR!=NULL)&&(NXT!=NULL))
			{
				LSP=NXT->IDTYPE;
			if ((NXT->KLASS==FORMALVARS)||(LSP!=NULL))
			{
				if (NXT->KLASS==ACTUALVARS)
					if (GATTR.TYPTR->form()<=POWER)
					{ LB=(GATTR.TYPTR==CHARPTR)
					&&(GATTR.KIND==CST);
				LOAD();
				if (LSP->is_power())
					GEN1(32/*ADJ*/,(int)LSP->size());
				else if (LSP->is_long())
				{
					if (GATTR.TYPTR==INTPTR)
					{ 
						GENLDC(INTSIZE);
						GATTR.TYPTR=LONGINTPTR;
					}
					GENLDC((int)LSP->size());
					GENLDC(0/*DAJ*/);
					GENNR(DECOPS);
				}
				else if ((LSP->form()==SUBRANGE)
							&&options.RANGECHECK)
				{
					GENLDC(LSP->MIN.IVAL);
					GENLDC(LSP->MAX.IVAL);
					GEN0(8/*CHK*/);
				}
				else if ((GATTR.TYPTR==INTPTR)&&
							COMPTYPES(LSP,REALPTR))
				{
					GEN0(10/*FLT*/);
					GATTR.TYPTR=REALPTR;
				}
				else if (LB&&STRGTYPE(LSP))
					GATTR.TYPTR=STRGPTR;
				}
				else /*FORM>POWER*/
				{
					LB=STRGTYPE(GATTR.TYPTR)
					&&(GATTR.KIND==CST);
					LOADADDRESS();
					if (LB&&PAOFCHAR(LSP))
					if (LSP->is_string()==false)
					{
						GEN0(80/*S1P*/);
						if (LSP->INXTYPE!=NULL)
						{
							GETBOUNDS(LSP->INXTYPE,LMIN,LMAX);
							if (LMAX-LMIN+1 !=
							GATTR.TYPTR->MAXLENG)
							CERROR(142);
						}
						GATTR.TYPTR=LSP;
					}
				} /*KLASS==FORMALVARS*/
				else if (GATTR.KIND==VARBL)
				{
					if (GATTR.ACCESS()==BYTE1)
						CERROR(103);
						LOADADDRESS();
						if (LSP!=NULL)
							if (SET<128>(2,POWER,LONGINT).in(LSP->form()))
								if (GATTR.TYPTR->size()!=LSP->size())
									CERROR(142);
				}
				else
					CERROR(154);
				if (!COMPTYPES(LSP,GATTR.TYPTR))
					CERROR(142);
			}
		}
		if (NXT!=NULL)
			NXT=NXT->NEXT;
	}
	while(SY==SYMBOLS::COMMA);
	if (SY==SYMBOLS::RPARENT)
		INSYMBOL();
	else
		CERROR(4);
	} /*LPARENT*/
	if (NXT!=NULL)
		CERROR(126);

	if (FCP->PFDECKIND==DECLARED)
	{
		if (FCP->KLASS==FUNC)
		{
			GENLDC(0);
			GENLDC(0);
		}
		if (options.INMODULE)
			if (SEPPROC)
			if ((FCP->PFSEG==SEG)&&(FCP->PFLEV==1))
			{
				GEN1(79/*CGP*/,0);
				LINKERREF(PROC1,-FCP->PFNAME,IC-1);
			}
			else if (FCP->PFLEV==0)
				GEN2(77/*CXP*/,FCP->PFSEG,FCP->PFNAME);
			else
				CERROR(405); /*CALL !ALLOWED IN SEP PROC*/
			else if (FCP->IMPORTED)
			{
				GEN2(77/*CXP*/,0,FCP->PFNAME);
				LINKERREF(PROC1,FCP->PFSEG,IC-2);
			}
			else goto retry;
		else
retry:
		if (FCP->PFSEG!=SEG)
			GEN2(77/*CXP*/,FCP->PFSEG,FCP->PFNAME);
		else if (FCP->PFLEV==0)
			GEN1(66/*CBP*/,FCP->PFNAME);
		else if (FCP->PFLEV==LEVEL)
			GEN1(78/*CLP*/,FCP->PFNAME);
		else if (FCP->PFLEV==1)
			GEN1(79/*CGP*/,FCP->PFNAME);
		else
			GEN1(46/*CIP*/,FCP->PFNAME);
	}
	else if (FCP->CSPNUM==23)
		GEN1(30,40); /* TEMP I.5 TRANSLATION --
					MEM WILL BE CSP 23 IN II.0  */
	else if ((FCP->CSPNUM!=21)&&(FCP->CSPNUM!=22))
		GEN1(30/*CSP*/,FCP->CSPNUM);
	GATTR.TYPTR=FCP->IDTYPE;

} /*CALLNONSPECIAL*/ ;

void BODYPART::GOTOSTATEMENT()
{
	LABELP		LLP;
	bool		FOUND;
	DISPRANGE	TTOP;

#if 0
	if (!options.GOTOOK)
		CERROR(6);
#endif

	if (SY==SYMBOLS::INTCONST)
	{
		FOUND=false;
		TTOP=TOP;
		while(DISPLAY[TTOP].OCCUR!=BLCK)	
			TTOP=TTOP-1;
		LLP=DISPLAY[TTOP].BLCK.FLABEL;
		while((LLP!=NULL)&&!FOUND)
		{
			if (LLP->LABVAL==VAL.IVAL)
			{
				FOUND=true;
				GENJMP(57/*UJP*/,LLP->CODELBP);
			}
			else
				LLP=LLP->NEXTLAB;
		}
		if (!FOUND)
			CERROR(167);
		INSYMBOL();
	}
	else
		CERROR(15);
} /*GOTOSTATEMENT*/

#if 0
void BODYPART::COMPOUNDSTATEMENT()
{
	ASSERT(false);
}
#endif

void BODYPART::COMPOUNDSTATEMENT(const SETOFSYS &FSYS)
{
	do	{
		do
			STATEMENT(FSYS+SYMBOLS::SEMICOLON+SYMBOLS::ENDSY);
		while(BNF::STATBEGSYS.in(SY));
		TEST=SY!=SYMBOLS::SEMICOLON;
		if (!TEST)
			INSYMBOL();
	}
	while(!TEST);
	if (SY==SYMBOLS::ENDSY)
		INSYMBOL();
	else
		CERROR(13);
} /*COMPOUNDSTATEMENET*/

#if 0
void BODYPART::IFSTATEMENT()
{
	ASSERT(false);
}
#endif

void BODYPART::IFSTATEMENT(const SETOFSYS &FSYS)
{
	LBP		LCIX1,LCIX2;
	int		LIC;
	bool	CONDCOMPILE,NOTHENCLAUSE;

	CONDCOMPILE=false;
	EXPRESSION(FSYS+SYMBOLS::THENSY);
	if ((GATTR.KIND==CST))
		if ((GATTR.TYPTR==BOOLPTR))
		{
			CONDCOMPILE=true;
			NOTHENCLAUSE=!ODD(GATTR.CVAL.IVAL);
			LIC=IC;
		}
	if (!CONDCOMPILE)
	{
		GENLABEL(&LCIX1);
		GENFJP(LCIX1);
	}
	if (SY==SYMBOLS::THENSY)
		INSYMBOL();
	else
		CERROR(52);
	STATEMENT(FSYS+SYMBOLS::ELSESY);
	if (CONDCOMPILE)
		if (NOTHENCLAUSE)
			IC=LIC;
		else LIC=IC;
	if (SY==SYMBOLS::ELSESY)
	{
		if (!CONDCOMPILE)
		{
			GENLABEL(&LCIX2);
			GENJMP(57/*UJP*/,LCIX2);
			PUTLABEL(LCIX1);
		}
		INSYMBOL();
		STATEMENT(FSYS);
		if (CONDCOMPILE)
		{
			if (!NOTHENCLAUSE)
				IC=LIC;
		}
		else
			PUTLABEL(LCIX2);
	}
	else if (!CONDCOMPILE)
		PUTLABEL(LCIX1);
} /*if (STATEMENT*/

#if 0
void BODYPART::CASESTATEMENT()
{
	ASSERT(false);
}
#endif

void BODYPART::CASESTATEMENT(const SETOFSYS &FSYS)
{ 
	struct CASEINFO;
	typedef CASEINFO*	CIP;
	struct CASEINFO
	{
		CIP	NEXT;
		int CSSTART;
		int	CSLAB;
	};

	STP		LSP,LSP1;
	CIP		FSTPTR,LPT1,LPT2,LPT3;
	VALU	LVAL;
	LBP		LADDR, LCIX;
	int		NULSTMT, LMIN, LMAX;
	LSP1 = 0;

	EXPRESSION(FSYS+SYMBOLS::OFSY+SYMBOLS::COMMA+SYMBOLS::COLON);
	LOAD();
	GENLABEL(&LCIX);
	GENJMP(57/*UJP*/,LCIX);
	LSP=GATTR.TYPTR;
	if (LSP!=NULL)
		if ((LSP->form()!=SCALAR)||(LSP==REALPTR))
		{
			CERROR(144);
			LSP=NULL;
		}
	if (SY==SYMBOLS::OFSY)
		INSYMBOL();
	else
		CERROR(8);
	FSTPTR=NULL;
	GENLABEL(&LADDR);

	do {
		LPT3=NULL;
		do {
			CONSTANT(FSYS+SYMBOLS::COMMA+SYMBOLS::COLON,LSP1,LVAL);
			if (LSP!=NULL)
				if (COMPTYPES(LSP,LSP1))
				{
					LPT1=FSTPTR;
					LPT2=NULL;
					while(LPT1!=NULL)
					{
						if (LPT1->CSLAB<=LVAL.IVAL)
						{
							if (LPT1->CSLAB==LVAL.IVAL)
								CERROR(156);
							goto retry;
						}
						LPT2=LPT1;
						LPT1=LPT1->NEXT;
					}
retry:       //  NEW(LPT3);
					LPT3 = new CASEINFO;
					LPT3->NEXT=LPT1;
					LPT3->CSLAB=LVAL.IVAL;
					LPT3->CSSTART=IC;
					if (LPT2==NULL)
						FSTPTR=LPT3;
					else
						LPT2->NEXT=LPT3;
				}
				else
					CERROR(147);
			TEST=SY!=SYMBOLS::COMMA;
			if (!TEST)
				INSYMBOL();
		}
		while(!TEST);

		if (SY==SYMBOLS::COLON)
			INSYMBOL();
		else
			CERROR(5);
		do
		STATEMENT(FSYS+SYMBOLS::SEMICOLON);
		while(BNF::STATBEGSYS.in(SY));

		if (LPT3!=NULL)
			GENJMP(57/*UJP*/,LADDR);
		TEST=SY!=SYMBOLS::SEMICOLON;
		if (!TEST)
			INSYMBOL();
	}
	while(!(TEST||(SY==SYMBOLS::ENDSY)));
	PUTLABEL(LCIX);

	if (FSTPTR!=NULL)
	{
		LMAX=FSTPTR->CSLAB;
		LPT1=FSTPTR;
		FSTPTR=NULL;
		do
		{
			LPT2=LPT1->NEXT;
			LPT1->NEXT=FSTPTR;
			FSTPTR=LPT1;LPT1=LPT2;
		}
		while(LPT1!=NULL);
		LMIN=FSTPTR->CSLAB;
		GEN0(44/*XJP*/);
		GENWORD(LMIN);
		GENWORD(LMAX);
		NULSTMT=IC;
		GENJMP(57/*UJP*/,LADDR);
		do // with FSTPTR^
		{
			while(FSTPTR->CSLAB>LMIN)
			{
				GENWORD(IC-NULSTMT);
				LMIN++;
			}
			GENWORD(IC-FSTPTR->CSSTART);
			FSTPTR=FSTPTR->NEXT;
			LMIN++;
		}
		while(FSTPTR!=NULL);
		PUTLABEL(LADDR);
	}
	if (SY==SYMBOLS::ENDSY)
		INSYMBOL();
	else
		CERROR(13);

} /*CASESTATEMENT*/

#if 0
void BODYPART::REPEATSTATEMENT()
{
	ASSERT(false);
}
#endif

void BODYPART::REPEATSTATEMENT(const SETOFSYS &FSYS)
{ 
	LBP LADDR;
	// generate label for loop address
	GENLABEL(&LADDR);
	PUTLABEL(LADDR);
	do {
		do
			STATEMENT(FSYS+SYMBOLS::SEMICOLON+SYMBOLS::UNTILSY);
		while(BNF::STATBEGSYS.in(SY));
		TEST=(SY!=SYMBOLS::SEMICOLON);
		if (!TEST)
			INSYMBOL();
	}
	while(!TEST);
	if (SY==SYMBOLS::UNTILSY)
	{
		INSYMBOL();
	//	generate the code for the until
	//	expression
		EXPRESSION(FSYS);
	//	generate the jump if we repeat 
		GENFJP(LADDR);
	}
	else
		CERROR(53);
} /*REPEATSTATEMENT*/ 

#if 0
void BODYPART::WHILESTATEMENT()
{
	ASSERT(false);
}
#endif

void BODYPART::WHILESTATEMENT(const SETOFSYS &FSYS)
{
	LBP LADDR, LCIX;
	GENLABEL(&LADDR);
	PUTLABEL(LADDR);
	EXPRESSION(FSYS+SYMBOLS::DOSY);
	GENLABEL(&LCIX);
	GENFJP(LCIX);
	if (SY==SYMBOLS::DOSY)
		INSYMBOL();
	else
		CERROR(54);
	STATEMENT(FSYS);
	GENJMP(57/*UJP*/,LADDR);
	PUTLABEL(LCIX);
} /*WHILESTATEMENT*/

#if 0
void BODYPART::FORSTATEMENT()
{
	ASSERT(false);
}
#endif

void BODYPART::FORSTATEMENT(const SETOFSYS &FSYS, CTP &LCP)
{
	ATTR LATTR;
	SYMBOLS::SYMBOL LSY1;
	LBP LCIX, LADDR;

	if (SY==SYMBOLS::IDENT)
	{
		SEARCHID(BNF::VARS,LCP);
		{
			LATTR.TYPTR=LCP->IDTYPE;
			LATTR.KIND=VARBL;
			if (LCP->KLASS==ACTUALVARS)
			{
				LATTR.set_access(DRCT);
				LATTR.VLEVEL=LCP->VLEV;
				LATTR.DPLMT=LCP->VADDR;
			}
			else
			{
				CERROR(155);
				LATTR.TYPTR=NULL;
			}
		}
		if (LATTR.TYPTR!=NULL)
			if ((LATTR.TYPTR->form()>SUBRANGE)
				||COMPTYPES(REALPTR,LATTR.TYPTR))
			{
				CERROR(143);
				LATTR.TYPTR=NULL;
			}
		INSYMBOL();
	}
	else
	{
		CERROR(2);
		SKIP(FSYS+SYMBOLS::BECOMES+SYMBOLS::TOSY+SYMBOLS::DOWNTOSY+SYMBOLS::DOSY);
	}	
	if (SY==SYMBOLS::BECOMES)
	{
		INSYMBOL();
		EXPRESSION(FSYS+SYMBOLS::TOSY+SYMBOLS::DOWNTOSY+SYMBOLS::DOSY);
		if (GATTR.TYPTR!=NULL)
			if (GATTR.TYPTR->form()!=SCALAR)
				CERROR(144);
			else if (COMPTYPES(LATTR.TYPTR,GATTR.TYPTR))
			{
				LOAD();
				if (LATTR.TYPTR!=NULL)
					if ((LATTR.TYPTR->form()==SUBRANGE)&&options.RANGECHECK)
					{
						GENLDC(LATTR.TYPTR->MIN.IVAL);
						GENLDC(LATTR.TYPTR->MAX.IVAL);
						GEN0(8/*CHK*/);
					}
				STORE(LATTR);
			}
			else
				CERROR(145);
	}
	else
	{
		CERROR(51);
		SKIP(FSYS+SYMBOLS::TOSY+SYMBOLS::DOWNTOSY+SYMBOLS::DOSY);
	}
	GENLABEL(&LADDR);
	if (SET<128>(2,SYMBOLS::TOSY,SYMBOLS::DOWNTOSY).in(SY))
	{
		LSY1=SY;
		INSYMBOL();
		EXPRESSION(FSYS+SYMBOLS::DOSY);
		if (GATTR.TYPTR!=NULL)
			if (GATTR.TYPTR->form()!=SCALAR)
				CERROR(144);
			else if (COMPTYPES(LATTR.TYPTR,GATTR.TYPTR))
			{
				LOAD();
				if (LATTR.TYPTR!=NULL)
					if ((LATTR.TYPTR->form()==SUBRANGE)&&options.RANGECHECK)
					{
						GENLDC(LATTR.TYPTR->MIN.IVAL);
						GENLDC(LATTR.TYPTR->MAX.IVAL);
						GEN0(8/*CHK*/);
					}
					GEN2(56/*STR*/,0,LC);
					PUTLABEL(LADDR);
					GATTR=LATTR; LOAD();
					GEN2(54/*LOD*/,0,LC);
					LC=LC+INTSIZE;
					if (LC>LCMAX)
						LCMAX=LC;
					if (LSY1==SYMBOLS::TOSY)
						GEN2(52/*LEQ*/,0,INTSIZE);
					else
						GEN2(48/*GEQ*/,0,INTSIZE);
			}
			else
				CERROR(145);
	}
	else
	{
		CERROR(55);
		SKIP(FSYS+SYMBOLS::DOSY);
	}
	GENLABEL(&LCIX);
	GENJMP(33/*FJP*/,LCIX);
	if (SY==SYMBOLS::DOSY)
		INSYMBOL();
	else
		CERROR(54);
	STATEMENT(FSYS);
	GATTR=LATTR;
	LOAD();
	GENLDC(1);
	if (LSY1==SYMBOLS::TOSY)
		GEN0(2/*ADI*/);
	else
		GEN0(21/*SBI*/);
	STORE(LATTR);
	GENJMP(57/*UJP*/,LADDR);
	PUTLABEL(LCIX);
	LC=LC-INTSIZE;
} /*FORSTATEMENT*/

#if 0
void BODYPART::WITHSTATEMENT()
{
	ASSERT(false);
}
#endif

void BODYPART::WITHSTATEMENT(const SETOFSYS &FSYS)
{ 
	CTP LCP;
	DISPRANGE LCNT1,LCNT2;
	LCNT1=0; LCNT2=0;
	WRITELN(OUTPUT,"BODYPART::WITHSTATEMENT");
	WRITELN(OUTPUT,"searching for <\"",ID,"\">");;
	do {
		if (SY==SYMBOLS::IDENT)
		{
			SEARCHID(BNF::VARS+FIELD,LCP);
			INSYMBOL();
		}
		else
		{
			CERROR(2);
			LCP=UVARPTR;
		}
		SELECTOR(FSYS+SYMBOLS::COMMA+SYMBOLS::DOSY,LCP);
		if (GATTR.TYPTR!=NULL)
		if (GATTR.TYPTR->form()==RECORDS)
			if (TOP < DISPLIMIT)
			{
				TOP=TOP+1;
				LCNT1=LCNT1+1;
				DISPLAY[TOP].FNAME=GATTR.TYPTR->FSTFLD;
				if (GATTR.ACCESS()==DRCT)
				{ 
					DISPLAY[TOP].OCCUR=CREC;
					DISPLAY[TOP].CREC.CLEV=GATTR.VLEVEL;
					DISPLAY[TOP].CREC.CDSPL=GATTR.DPLMT;
				}
				else
				{
					LOADADDRESS();
					GEN2(56/*STR*/,0,LC);
					DISPLAY[TOP].OCCUR=VREC;
					DISPLAY[TOP].VREC.VDSPL=LC;
					LC=LC+PTRSIZE;
					LCNT2=LCNT2+PTRSIZE;
					if (LC>LCMAX)
						LCMAX=LC;
				}
			}
			else
				CERROR(250);
		else
			CERROR(140);
		TEST=SY!=SYMBOLS::COMMA;
		if (!TEST)
			INSYMBOL();
	}
	while(!TEST);
	if (SY==SYMBOLS::DOSY)
		INSYMBOL();
	else
		CERROR(54);
	STATEMENT(FSYS);
	TOP=TOP-LCNT1;
	LC=LC-LCNT2;
} /*WITHSTATEMENT*/
 