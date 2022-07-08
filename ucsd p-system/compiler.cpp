
#include "stdafx.h"
#include "compiler.h"
#include "unitpart.h"
#include "declarationpart.h"

PASCALCOMPILER::PASCALCOMPILER()
{
//	TODO - this is the default constructor
//	which is called from DECLARATION part,
//	which means that a compilar object may
//	already exist - so follow the appropriate
//	trail of bread crumbs to where ever
//	we really need to be?
}

PASCALCOMPILER::PASCALCOMPILER(INFOREC &arg)
{
	USERINFO = arg;
	SEARCH::RESET_SYMBOLS();
}

SEGMENT void USERPROGRAM()
{
	AfxMessageBox (_T("NO CODE"),IDOK);
}  

SEGMENT void USERPROGRAM::FILEHANDLER()
{
	// do nothing
}

SEGMENT void USERPROGRAM::DEBUGGER()
{
	// do nothing
}

SEGMENT void USERPROGRAM::PRINTERROR()
{
	// do nothing
}

SEGMENT void USERPROGRAM::INITIALIZE()
{
	// do nothing
}

SEGMENT void USERPROGRAM::GETCMD()
{
	// do nothing
}

SEGMENT void USERPROGRAM::NOTUSED1()
{
	// do nothing 
}

SEGMENT void USERPROGRAM::NOTUSED2()
{
	// do nothing
}

SEGMENT void USERPROGRAM::NOTUSED3()
{
	// do nothing
}

SEGMENT	USERPROGRAM::USERPROGRAM()
{
	// do nothing
}

void PASCALCOMPILER::COMPINIT()
{
	::COMPINIT INSTANCE;
	INSTANCE.INIT(this);
}

void COMPINIT::ENTSTDTYPES()
{
//	WRITELN(OUTPUT,"COMPINIT::ENTSTDTYPES()");
//	NEW(INTPTR,SCALAR,STANDARD);
	m_ptr->INTPTR = (STRUCTURE*) new STRUCTURE;
	m_ptr->INTPTR->SIZE = INTSIZE;
	m_ptr->INTPTR->FORM=SCALAR;
	m_ptr->INTPTR->SCALKIND=STANDARD;

	m_ptr->REALPTR = (STRUCTURE*) new STRUCTURE;
	m_ptr->REALPTR->SIZE = REALSIZE;
	m_ptr->REALPTR->FORM=SCALAR;
	m_ptr->REALPTR->SCALKIND=STANDARD;

	m_ptr->LONGINTPTR = (STRUCTURE*) new STRUCTURE;
	m_ptr->LONGINTPTR->SIZE = INTSIZE;
	m_ptr->LONGINTPTR->FORM=SCALAR;
	m_ptr->LONGINTPTR->SCALKIND=STANDARD;

	m_ptr->CHARPTR = (STRUCTURE*) new STRUCTURE;
	m_ptr->CHARPTR->SIZE = CHARSIZE;
	m_ptr->CHARPTR->FORM=SCALAR;
	m_ptr->CHARPTR->SCALKIND=STANDARD;

	m_ptr->BOOLPTR = (STRUCTURE*) new STRUCTURE;
	m_ptr->BOOLPTR->SIZE = BOOLSIZE;
	m_ptr->BOOLPTR->FORM=SCALAR;
	m_ptr->BOOLPTR->SCALKIND=DECLARED;

	m_ptr->NILPTR = (STRUCTURE*) new STRUCTURE;
	m_ptr->NILPTR->SIZE = PTRSIZE;
	m_ptr->NILPTR->FORM=POINTER;
	m_ptr->NILPTR->ELTYPE=NULL ;
   
	m_ptr->TEXTPTR = (STRUCTURE*) new STRUCTURE;
	m_ptr->TEXTPTR->SIZE = FILESIZE+CHARSIZE;
	m_ptr->TEXTPTR->FORM=FILES;
	m_ptr->TEXTPTR->FILTYPE=m_ptr->CHARPTR;
	
#if 0
	NEW(INTRACTVPTR,FILES);
	// WITH INTRACTVPTR^)
	SIZE=FILESIZE+CHARSIZE;
	FORM=FILES;
	FILTYPE=CHARPTR;
	
	NEW(STRGPTR,ARRAYS,true,true);
     // WITH STRGPTR^)
	FORM=ARRAYS;
	SIZE=(DEFSTRGLGTH + CHRSPERWD) DIV CHRSPERWD;
	AISPACKD=true;
	AISSTRNG=true;
	INXTYPE=INTPTR;
	ELWIDTH=BITSPERCHR;
	ELSPERWD=CHRSPERWD;
	AELTYPE=CHARPTR;
	MAXLENG=DEFSTRGLGTH;
#endif
} /*ENTSTDTYPES*/

void COMPINIT::ENTSTDNAMES()
{
//	WRITELN(OUTPUT,"COMPINIT::ENTSTDNAMES()");
	CTP	CP;
	CTP	CP1;
	int I;
	STP &INTPTR = m_ptr->INTPTR;
	STP &REALPTR = m_ptr->REALPTR;
	STP &CHARPTR = m_ptr->CHARPTR;
	STP &BOOLPTR = m_ptr->BOOLPTR;
	STP &STRGPTR = m_ptr->STRGPTR;
	STP &TEXTPTR = m_ptr->TEXTPTR;

	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"INTEGER ");
	CP->IDTYPE = INTPTR;
	CP->KLASS = TYPES;
	m_ptr->ENTERID(CP);

	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"REAL    ");
	CP->IDTYPE = REALPTR;
	CP->KLASS = TYPES;
	m_ptr->ENTERID(CP);

	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"CHAR    ");
	CP->IDTYPE = CHARPTR;
	CP->KLASS = TYPES;
	m_ptr->ENTERID(CP);

	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"BOOLEAM ");
	CP->IDTYPE = BOOLPTR;
	CP->KLASS = TYPES;
	m_ptr->ENTERID(CP);

	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"STRING  ");
	CP->IDTYPE = STRGPTR;
	CP->KLASS = TYPES;
	m_ptr->ENTERID(CP);

	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"TEXT  ");
	CP->IDTYPE = TEXTPTR;
	CP->KLASS = TYPES;
	m_ptr->ENTERID(CP);

	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"INTERACT ");
	CP->IDTYPE = m_ptr->INTRACTVPTR;
	CP->KLASS = TYPES;
	m_ptr->ENTERID(CP);
	
	m_ptr->INPUTPTR = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(m_ptr->INPUTPTR->NAME,IDENTSIZE,"INPUT  ");
	m_ptr->INPUTPTR->IDTYPE = TEXTPTR;
	m_ptr->INPUTPTR->KLASS = FORMALVARS;
	m_ptr->INPUTPTR->VLEV = 0;
	m_ptr->INPUTPTR->VADDR = 2;
	m_ptr->ENTERID(m_ptr->INPUTPTR);

	m_ptr->OUTPUTPTR = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(m_ptr->OUTPUTPTR->NAME,IDENTSIZE,"OUTPUT  ");
	m_ptr->OUTPUTPTR->IDTYPE = TEXTPTR;
	m_ptr->OUTPUTPTR->KLASS = FORMALVARS;
	m_ptr->OUTPUTPTR->VLEV = 0;
	m_ptr->OUTPUTPTR->VADDR = 3;
	m_ptr->ENTERID(m_ptr->OUTPUTPTR);

	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"KEYBOARD ");
	CP->IDTYPE = TEXTPTR;
	CP->KLASS = FORMALVARS;
	CP->VLEV = 0;
	CP->VADDR = 4;
	m_ptr->ENTERID(CP);
	
	CP1=NULL;
	for (I=0;I<=1;I++)
	{
		CP = (IDENTIFIER*) new IDENTIFIER;
        CP->IDTYPE=BOOLPTR;
        if (I==0)
			strcpy_s(CP->NAME,IDENTSIZE,"FALSE   ");
		else 
			strcpy_s(CP->NAME,IDENTSIZE,"TRUE    ");
		CP->NEXT=CP1;
		CP->VALUES.IVAL=I;
		CP->KLASS=KONST;
		m_ptr->ENTERID(CP);
		CP1=CP;
    };
    m_ptr->BOOLPTR->FCONST=CP;

	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"NULL     ");
	CP->IDTYPE=m_ptr->NILPTR;
	CP->NEXT=NULL;
	CP->VALUES.IVAL=0;
	CP->KLASS=KONST;
	m_ptr->ENTERID(CP);
	
	CP = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(CP->NAME,IDENTSIZE,"MAXINT  ");
	CP->IDTYPE=INTPTR;
	CP->KLASS=KONST;
	CP->VALUES.IVAL=MAXINT;
	m_ptr->ENTERID(CP);
} /*ENTSTDNAMES*/ ;

void COMPINIT::ENTUNDECL()
{
//	WRITELN(OUTPUT,"COMPINIT::ENTUNDECL()");
	m_ptr->UTYPPTR = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(m_ptr->UTYPPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UTYPPTR->IDTYPE=NULL;
	m_ptr->UTYPPTR->KLASS=TYPES;

	m_ptr->UCSTPTR = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(m_ptr->UCSTPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UCSTPTR->IDTYPE=NULL;
	m_ptr->UCSTPTR->KLASS=KONST;
	m_ptr->UCSTPTR->VALUES.IVAL = NULL;
	m_ptr->UCSTPTR->NEXT=NULL;

	m_ptr->UVARPTR = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(m_ptr->UVARPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UVARPTR->IDTYPE=NULL;
	m_ptr->UCSTPTR->NEXT = NULL;
	m_ptr->UCSTPTR->VLEV = 0;
	m_ptr->UCSTPTR->VADDR = 0;
	m_ptr->UCSTPTR->KLASS=ACTUALVARS;

	m_ptr->UFLDPTR = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(m_ptr->UFLDPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UFLDPTR->IDTYPE=NULL;
	m_ptr->UFLDPTR->NEXT = NULL;
	m_ptr->UFLDPTR->FLDADDR = 0;
	m_ptr->UFLDPTR->KLASS=FIELD;

	m_ptr->UPRCPTR = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(m_ptr->UPRCPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UPRCPTR->IDTYPE=NULL;
	m_ptr->UPRCPTR->FORWDECL=false;
	m_ptr->UPRCPTR->NEXT = NULL;
	m_ptr->UPRCPTR->INSCOPE=false;
	m_ptr->UPRCPTR->LOCALLC=0;
	m_ptr->UPRCPTR->EXTURNAL=false;
	m_ptr->UPRCPTR->KLASS=PROC1;
	m_ptr->UPRCPTR->PFDECKIND=DECLARED;
	m_ptr->UPRCPTR->PFKIND=ACTUAL;

	m_ptr->UFCTPTR = (IDENTIFIER*) new IDENTIFIER;
	strcpy_s(m_ptr->UFCTPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UFCTPTR->IDTYPE=NULL;
	m_ptr->UFCTPTR->NEXT = NULL;
	m_ptr->UFCTPTR->FORWDECL=false;
	m_ptr->UFCTPTR->INSCOPE=false;
	m_ptr->UFCTPTR->LOCALLC=0;
	m_ptr->UFCTPTR->PFLEV=0;
	m_ptr->UFCTPTR->PFNAME=0;
	m_ptr->UFCTPTR->PFSEG=0;
	m_ptr->UFCTPTR->KLASS=FUNC;
	m_ptr->UFCTPTR->PFDECKIND=DECLARED;
	m_ptr->UFCTPTR->PFKIND=ACTUAL;
} /*ENTUNDECL*/ ;

void COMPINIT::ENTSPCPROCS()
{
	CTP	LCP;
	int	I;
	bool ISFUNC;
	ALPHA	NA[44];
//	WRITELN(OUTPUT,"COMPINIT::ENTSPCPROCS()");
	strcpy_s(NA[ 1],IDENTSIZE,"READ      ");
	strcpy_s(NA[ 2],IDENTSIZE,"READLN    ");
	strcpy_s(NA[ 3],IDENTSIZE,"WRITE     ");
	strcpy_s(NA[ 4],IDENTSIZE,"WRITELN   ");
	strcpy_s(NA[ 5],IDENTSIZE,"EOF       ");
	strcpy_s(NA[ 6],IDENTSIZE,"EOLN      ");
	strcpy_s(NA[ 7],IDENTSIZE,"PRED      ");
	strcpy_s(NA[ 8],IDENTSIZE,"SUCC      ");
	strcpy_s(NA[ 9],IDENTSIZE,"ORD       ");
	strcpy_s(NA[10],IDENTSIZE,"SQR       ");
	strcpy_s(NA[11],IDENTSIZE,"ABS       ");
	strcpy_s(NA[12],IDENTSIZE,"NEW       ");
	strcpy_s(NA[13],IDENTSIZE,"UNITREAD  ");
	strcpy_s(NA[14],IDENTSIZE,"UNITWRIT  ");
	strcpy_s(NA[15],IDENTSIZE,"CONCAT    ");
	strcpy_s(NA[16],IDENTSIZE,"LENGTH    ");
	strcpy_s(NA[17],IDENTSIZE,"INSERT    ");
	strcpy_s(NA[18],IDENTSIZE,"DELETE    ");
	strcpy_s(NA[19],IDENTSIZE,"COPY      ");
	strcpy_s(NA[20],IDENTSIZE,"POS       ");
	strcpy_s(NA[21],IDENTSIZE,"MOVELEFT  ");
	strcpy_s(NA[22],IDENTSIZE,"MOVERIGHT ");
	strcpy_s(NA[23],IDENTSIZE,"EXIT      ");
	strcpy_s(NA[24],IDENTSIZE,"IDSEARCH  ");
	strcpy_s(NA[25],IDENTSIZE,"TREESEARCH");
	strcpy_s(NA[26],IDENTSIZE,"TIME      ");
	strcpy_s(NA[27],IDENTSIZE,"FILLCHAR  ");
	strcpy_s(NA[28],IDENTSIZE,"OPENNEW   ");
	strcpy_s(NA[29],IDENTSIZE,"OPENOLD   ");
	strcpy_s(NA[30],IDENTSIZE,"REWRITE   ");
	strcpy_s(NA[31],IDENTSIZE,"CLOSE     ");
	strcpy_s(NA[32],IDENTSIZE,"SEEK      ");
	strcpy_s(NA[33],IDENTSIZE,"RESET     ");
	strcpy_s(NA[34],IDENTSIZE,"GET       ");
	strcpy_s(NA[35],IDENTSIZE,"PUT       ");
	strcpy_s(NA[36],IDENTSIZE,"SCAN      ");
	strcpy_s(NA[37],IDENTSIZE,"BLOCKREAD");
	strcpy_s(NA[38],IDENTSIZE,"BLOCKWRITE");
	strcpy_s(NA[39],IDENTSIZE,"TRUNC     ");
	strcpy_s(NA[40],IDENTSIZE,"PAGE      ");
	strcpy_s(NA[41],IDENTSIZE,"SIZEOF    ");
	strcpy_s(NA[42],IDENTSIZE,"STR       ");
	strcpy_s(NA[43],IDENTSIZE,"GOTOXY    ");
	SET TSET = SET((15),2,7,8,10,13,17,18,19,20,32,34,35,40,42,43);
	SET FSET = SET((17),5,6,7,8,9,10,11,15,16,19,20,25,36,37,38,39,41);
	for (I=1;I<=43;I++)
	{
		if (m_ptr->TINY)
			if (TSET.in(I))
               continue;
		ISFUNC= FSET.in(I);
		LCP = (IDENTIFIER*) new IDENTIFIER;
		LCP->PFDECKIND = SPECIAL;
		strcpy_s(LCP->NAME,IDENTSIZE,NA[I]);
		LCP->NEXT=NULL;
		LCP->IDTYPE=NULL;
		LCP->KEY=I;
		if (ISFUNC)
			LCP->KLASS=FUNC;
		else
			LCP->KLASS=PROC1;
         m_ptr->ENTERID(LCP);
	}
} /*ENTSPCPROCS*/

void COMPINIT::ENTSTDPROCS()
{
	CTP		LCP,PARAM;
	STP		LSP,FTYPE;
	int		i;
	bool	ISPROC;
	ALPHA	NA[20];
//	WRITELN(OUTPUT,"COMPINIT::ENTSTDPROCS()");
	
	const int SZ = IDENTSIZE;
	strcpy_s(NA[ 1],IDENTSIZE,"ODD     ");
	strcpy_s(NA[ 2],IDENTSIZE,"CHR     ");
	strcpy_s(NA[ 3],IDENTSIZE,"MEMAVAIL");
	strcpy_s(NA[ 4],IDENTSIZE,"ROUND   ");
	strcpy_s(NA[ 5],IDENTSIZE,"SIN     ");
	strcpy_s(NA[ 6],IDENTSIZE,"COS     ");
	strcpy_s(NA[ 7],IDENTSIZE,"LOG     ");
	strcpy_s(NA[ 8],IDENTSIZE,"ATAN    ");
	strcpy_s(NA[ 9],IDENTSIZE,"LN      ");
	strcpy_s(NA[10],IDENTSIZE,"EXP     ");
	strcpy_s(NA[11],IDENTSIZE,"SQRT    ");
	strcpy_s(NA[12],IDENTSIZE,"MARK    ");
	strcpy_s(NA[13],IDENTSIZE,"RELEASE ");
	strcpy_s(NA[14],IDENTSIZE,"IORESULT");
	strcpy_s(NA[15],IDENTSIZE,"UNITBUSY");
	strcpy_s(NA[16],IDENTSIZE,"PWROFTEN");
	strcpy_s(NA[17],IDENTSIZE,"UNITWAIT");
	strcpy_s(NA[18],IDENTSIZE,"UNITCLEA");
	strcpy_s(NA[19],IDENTSIZE,"HALT    ");

	SET SYSCALLS = SET((5),12,13,17,18,19);

	for (i=1;i<20;i++)
	{
		ISPROC = SYSCALLS.in(i);
		switch (i)
		{
		case 1:
			FTYPE=m_ptr->BOOLPTR;;
			PARAM =  (IDENTIFIER*) new IDENTIFIER;
			PARAM->IDTYPE=m_ptr->INTPTR;
			PARAM->KLASS=ACTUALVARS;
			break;

		case 2:
			FTYPE=m_ptr->CHARPTR;
			break;

		case 3:
			FTYPE=m_ptr->INTPTR;
			PARAM=NULL;
			break;

		case 4:
			FTYPE=m_ptr->INTPTR;
			PARAM =  (IDENTIFIER*) new IDENTIFIER;
			PARAM->IDTYPE=m_ptr->REALPTR;
			PARAM->KLASS=ACTUALVARS;
			break;

		case 5:
			FTYPE=m_ptr->REALPTR;
			break;

		case 12:
			FTYPE=NULL;
			PARAM = (IDENTIFIER*) new IDENTIFIER;
			LSP = (STRUCTURE*) new STRUCTURE;
			LSP->SIZE=PTRSIZE;
			LSP->FORM=POINTER;
			LSP->ELTYPE=NULL;
			PARAM->IDTYPE=LSP;
			PARAM->KLASS=FORMALVARS;
			break;

		case 14:
			FTYPE=m_ptr->REALPTR;
			PARAM=NULL;
			break;

		case 15:
			FTYPE=m_ptr->BOOLPTR; 
			PARAM = (IDENTIFIER*) new IDENTIFIER;
			PARAM->IDTYPE=m_ptr->INTPTR;
			PARAM->KLASS=ACTUALVARS;
			break;

		case 16:
			FTYPE=m_ptr->REALPTR;
			break;

		case 17:
			FTYPE=NULL;
			break;
		
		case 19:
			FTYPE=NULL;
			PARAM=NULL;
			break;

		 default:
			break;
		}

		if (ISPROC)
			LCP = (IDENTIFIER*) new IDENTIFIER;
		else
			LCP = (IDENTIFIER*) new IDENTIFIER;
		
		strcpy_s(LCP->NAME,IDENTSIZE,NA[i]);
		LCP->PFDECKIND=STANDARD;
		LCP->CSPNUM=i + 20;
		if (ISPROC)
			LCP->KLASS=PROC1;
		else
			LCP->KLASS=FUNC;
		if (PARAM!=NULL)
			PARAM->NEXT=NULL;
		LCP->IDTYPE=FTYPE;
		LCP->NEXT=PARAM;
		m_ptr->ENTERID(LCP);
	}

}
/*ENTSTDPROCS*/ ;

void COMPINIT::INITSCALARS()
{
//	 WRITELN(OUTPUT,"COMPINIT::INITSCALARS()");
	 NONRESIDENT I;
     m_ptr->FWPTR=NULL;
	 m_ptr->MODPTR=NULL;
	 m_ptr->GLOBTESTP=NULL;
     m_ptr->LINESTART=0;
	 m_ptr->LINEINFO=LCAFTERMARKSTACK;
	 m_ptr->LIST=false;
     m_ptr->SYMBLK=2;
	 m_ptr->SCREENDOTS=0;
	 m_ptr->STARTDOTS=0;
     for (m_ptr->SEG=0;m_ptr->SEG<=MAXSEG;m_ptr->SEG++)
	 {
		m_ptr->SEGTABLE[m_ptr->SEG].DISKADDR=0;
		m_ptr->SEGTABLE[m_ptr->SEG].CODELENGTH=0;
		strcpy_s(m_ptr->SEGTABLE[m_ptr->SEG].SEGNAME,IDENTSIZE,"        ");
		m_ptr->SEGTABLE[m_ptr->SEG].SEGKIND=0;
		m_ptr->SEGTABLE[m_ptr->SEG].TEXTADDR=0;
	 }
     m_ptr->USINGLIST=NULL;
     if (m_ptr->USERINFO.STUPID)
		 strcpy_s(m_ptr->SYSTEMLIB,MAXDIRLEN,"*SYSTEM.PASCAL");
     else
		strcpy_s(m_ptr->SYSTEMLIB,MAXDIRLEN,"*SYSTEM.LIBRARY");

     m_ptr->LC=LCAFTERMARKSTACK;
	 m_ptr->IOCHECK=true;
	 m_ptr->DP=true;
     m_ptr->SEGINX=0;
	 m_ptr->NEXTJTAB=1;
	 m_ptr->NEXTPROC=2;
	 m_ptr->CURPROC=1;

     m_ptr->SCONST = new CONSTREC;
	 m_ptr->SYMBUFP = (SYMBUFARRAY*) new SYMBUFARRAY;
	 m_ptr->CODEP = (CODEARRAY*) new CODEARRAY;
	 memset(m_ptr->CODEP,0xff,MAXCODE+1);

     m_ptr->CLINKERINFO=false;
	 m_ptr->DLINKERINFO=false;
     m_ptr->SEG=1;
	 m_ptr->NEXTSEG=10;
	 m_ptr->CURBLK=1;
	 m_ptr->CURBYTE=0;
	 m_ptr->LSEPPROC=false;
     m_ptr->STARTINGUP=true;
	 m_ptr->NOISY=!m_ptr->USERINFO.SLOWTERM;
	 m_ptr->SEPPROC=false;
     m_ptr->NOSWAP=true;
	 m_ptr->DEBUGGING=false;
	 m_ptr->BPTONLINE=false;
	 m_ptr->INMODULE=false;
     m_ptr->GOTOOK=false;
	 m_ptr->RANGECHECK=true;
	 m_ptr->SYSCOMP=false;
	 m_ptr->TINY=false;
     m_ptr->CODEINSEG=false;
	 m_ptr->PRTERR=true;
	 m_ptr->INCLUDING=false;
	 m_ptr->USING=false;
	 I=SEEK;
	 while (I<=DECOPS)
	 {
		 m_ptr->PFNUMOF[I]=0;
		 I = (NONRESIDENT)(I+1);
	 }
	 m_ptr->COMMENT=NULL;
	 m_ptr->LIBNOTOPEN=true;
	 m_ptr->GETSTMTLEV=true;
	 m_ptr->BEGSTMTLEV=0;
	 m_ptr->STMTLEV=0;
	 /*INITSCALARS*/ ;
}

void COMPINIT::INITSETS()
{
//	WRITELN(OUTPUT,"COMPINIT::INITSETS()");
	m_ptr->CONSTBEGSYS = SET((5),ADDOP,INTCONST,REALCONST,STRINGCONST,IDENT);
	m_ptr->SIMPTYPEBEGSYS = SET((1),LPARENT) + m_ptr->CONSTBEGSYS;	
	m_ptr->TYPEBEGSYS = SET((6),ARROW,PACKEDSY,ARRAYSY,RECORDSY,SETSY,FILESY)
	+ m_ptr->SIMPTYPEBEGSYS;	
	m_ptr->TYPEDELS = SET((4),ARRAYSY,RECORDSY,SETSY,FILESY);	
	m_ptr->BLOCKBEGSYS = SET((9),USESSY,LABELSY,CONSTSY,TYPESY,VARSY,
		PROCSY,FUNCSY,PROGSY,IFSY);	
	m_ptr->SELECTSYS = SET((3),ARROW,PERIOD,LBRACK);	
	m_ptr->FACBEGSYS = SET((8),INTCONST,REALCONST,LONGCONST,STRINGCONST,IDENT,
		LPARENT,LBRACK,NOTSY);	
	m_ptr->STATBEGSYS = SET((8),BEGINSY,GOTOSY,IFSY,WHILESY,REPEATSY,FORSY,WITHSY,CASESY);	
	m_ptr->VARS = SET((2),FORMALVARS,ACTUALVARS);
} /*INITSETS*/ ;

void COMPINIT::INIT(PASCALCOMPILER *p)
{
//	WRITELN(OUTPUT,"COMPINIT::INIT(PASCALCOMPILER *p)");
	int I;
	m_ptr=p;
	INITSCALARS();
	INITSETS();
	m_ptr->LEVEL=0;
	m_ptr->TOP=0;
	if (m_ptr->NOISY)
	{
       for (I=1;I<=7;I++)
	   { 
		   WRITELN(OUTPUT);
		   WRITELN(OUTPUT,"PASCAL Compiler [I.5] (Unit Compiler)");
		   WRITE(OUTPUT,"<   0>");
	   }
	}
	m_ptr->DISPLAY[0].FNAME=NULL;
	m_ptr->DISPLAY[0].BLCK.FFILE=NULL;
	m_ptr->DISPLAY[0].BLCK.FLABEL=NULL;
	m_ptr->DISPLAY[0].OCCUR = BLCK;
	m_ptr->SMALLESTSPACE=MEMAVAIL();

	m_ptr->GETNEXTPAGE();
	m_ptr->INSYMBOL();

	ENTSTDTYPES();
	ENTSTDNAMES();
	ENTUNDECL();
	ENTSPCPROCS();
	ENTSTDPROCS();

	if (m_ptr->SYSCOMP)
	{
		 m_ptr->OUTERBLOCK=NULL;
		 m_ptr->SEG=0;
		 m_ptr->NEXTSEG=1;
		 m_ptr->GLEV = -1;
		 m_ptr->BLOCKBEGSYS=m_ptr->BLOCKBEGSYS + SET(2,UNITSY,SEPARATSY);
	}
	else {
		m_ptr->TOP=1;
		m_ptr->LEVEL=1;
		m_ptr->DISPLAY[1].FNAME=NULL;
		m_ptr->DISPLAY[1].BLCK.FFILE=NULL;
		m_ptr->DISPLAY[1].BLCK.FLABEL=NULL;
		m_ptr->DISPLAY[1].OCCUR = BLCK;
	};
	m_ptr->LC=m_ptr->LC+2;
	m_ptr->GLEV=3; /*KEEP STACK STRAIGHT FOR NOW*/
      
	m_ptr->OUTERBLOCK = new IDENTIFIER;
	m_ptr->OUTERBLOCK->KLASS=PROC1;
	m_ptr->OUTERBLOCK->PFDECKIND=DECLARED;
	m_ptr->OUTERBLOCK->PFKIND=ACTUAL;

	m_ptr->OUTERBLOCK->NEXT=NULL;
	m_ptr->OUTERBLOCK->LOCALLC=m_ptr->LC;
    strcpy_s(m_ptr->OUTERBLOCK->NAME,IDENTSIZE,"PROGRAM ");
	m_ptr->OUTERBLOCK->IDTYPE=NULL;
	m_ptr->OUTERBLOCK->PFLEV=0;
	m_ptr->OUTERBLOCK->PFNAME=1;
	m_ptr->OUTERBLOCK->PFSEG=m_ptr->SEG;
	
	m_ptr->OUTERBLOCK->FORWDECL=false;
	m_ptr->OUTERBLOCK->EXTURNAL=false;
	m_ptr->OUTERBLOCK->INSCOPE=true;

   if (m_ptr->SY==PROGSY)
   {
	   m_ptr->INSYMBOL();
       if (m_ptr->SY==IDENT)
	   {
		   strcpy_s(m_ptr->SEGTABLE[m_ptr->SEG].SEGNAME,16,m_ptr->ID);
           if (m_ptr->OUTERBLOCK!=NULL)
		   {
				strcpy_s(m_ptr->OUTERBLOCK->NAME,16,m_ptr->ID);
				m_ptr->ENTERID(m_ptr->OUTERBLOCK); /*ALLOWS EXIT ON PROGRAM NAME*/
           }
	   }
       else
		   m_ptr->CERROR(2);

//#define DEBUG_TREE true
#ifdef DEBUG_TREE
	PRINTTREE(m_ptr->DISPLAY[0].FNAME);
#endif

	   m_ptr->INSYMBOL();
	   SETOFSYS SPECIAL;
	   SPECIAL = SET(2,RPARENT,SEMICOLON)+m_ptr->BLOCKBEGSYS;
	   if (m_ptr->SY==LPARENT)
	   {
			do
				m_ptr->INSYMBOL();
			while (!SPECIAL.in(m_ptr->SY));
			if (m_ptr->SY==RPARENT)
				m_ptr->INSYMBOL();
			else
				m_ptr->CERROR(4);
	   }
		if (m_ptr->SY==SEMICOLON)
			m_ptr->INSYMBOL();
		else
			m_ptr->CERROR(14);
   }
   MARK(m_ptr->MARKP);
   m_ptr->TOS = new LEXSTKREC;
//   // WITH TOS^) {  /*MAKE LEXSTKREC for (OUTERBLOCK*/
//     {
   m_ptr->TOS->PREVLEXSTACKP=NULL;
   m_ptr->TOS->BFSY=PERIOD;
   m_ptr->TOS->DFPROCP=m_ptr->OUTERBLOCK;
   m_ptr->TOS->DLLC=m_ptr->LC;
   m_ptr->TOS->DOLDLEV=m_ptr->LEVEL;
   m_ptr->TOS->DOLDTOP=m_ptr->TOP;
   m_ptr->TOS->POLDPROC=m_ptr->CURPROC;
   m_ptr->TOS->ISSEGMENT=false;
   m_ptr->TOS->DMARKP=m_ptr->MARKP;

} /*COMPINIT*/

void PASCALCOMPILER::DECLARATIONS(SETOFSYS FSYS)
{
	DECLARATIONPART MAGIC(FSYS);
}

void PASCALCOMPILER::BODYPART(SETOFSYS FSYS, CTP FPROCP)
{
	ASSERT(0);
}

void PASCALCOMPILER::WRITELINKERINFO(bool flag)
{
	LINKERINFO MAGIC;
	MAGIC.WRITELINKERINFO(flag);
}

void PASCALCOMPILER::UNITPART(SETOFSYS)
{
	
}


void PASCALSOURCE::PRINTLINE()
{
	char	DORLEV,STARORC;
	int LENG;
	char A[100];
	STARORC=':';
	if (DP)
		DORLEV='D';
	else
		DORLEV=(char)((BEGSTMTLEV%10) + ORD('0'));

   if (BPTONLINE)
	   STARORC='*';
   WRITE(OUTPUT,_LP._tmpfname,SCREENDOTS/*6**/,
	   (int)SEG/*4*/,
	   (int)CURPROC,/*5,*/
	   STARORC,
	   DORLEV,
	   LINEINFO/*,6*/," ");

   LENG=SYMCURSOR-LINESTART;
   if (LENG>100)
	   LENG=100;
   MOVELEFT((const char*)&(SYMBUFP[LINESTART]),A,LENG);
   if (A[0]==(char)(16/*DLE*/))
   {
       if (A[1]>' ')
		   WRITE(OUTPUT,0,_LP._tmpfname," ",ORD(A[1])-ORD(' '));
       LENG=LENG-2;
       MOVELEFT((const char*)&A[2],A,LENG);
   };
   A[LENG-1]=char(EOL);
   WRITE(OUTPUT,0,_LP._tmpfname,&A[0],LENG);
   if ((USERINFO.ERRBLK==SYMBLK)&&(USERINFO.ERRSYM>LINESTART))
	   WRITELN(OUTPUT,0,_LP._tmpfname,">>>>>> Error # ",USERINFO.ERRNUM);

} /*PRINTLINE*/ ;

void PASCALCOMPILER::ENTERID(CTP FCP)
{
#if 0
	WRITELN(OUTPUT,"PASCALCOMPILER::ENTERID(CTP FCP)");
	WRITELN(OUTPUT,"FCP->NAME = \"",FCP->NAME,'"');
#endif

	CTP LCP,LCP1;
	int I;
	LCP=DISPLAY[TOP].FNAME;
	if (LCP==NULL)
		DISPLAY[TOP].FNAME=FCP;
	else
	{
		I=TREESEARCH(LCP,LCP1,FCP->NAME);
		while (I==0)
		{
			CERROR(101);
			if (LCP1->RLINK==NULL)
				I=1;
			else I=TREESEARCH(LCP1->RLINK,LCP1,FCP->NAME);
		};
		if (I==1)
			LCP1->RLINK=FCP;
		else
			LCP1->LLINK=FCP;
	};
	FCP->LLINK=NULL;
	FCP->RLINK=NULL;
//	PRINTTREE(LCP);
} /*ENTERID*/ ;

void PASCALCOMPILER::SEARCHSECTION(CTP FCP, CTP &FCP1)
{
	if (FCP!=NULL)
		if (TREESEARCH(FCP,FCP1,ID)==0) /*NADA*/
			;
		else
			FCP1=NULL;
	else
		FCP1=NULL;
} /*SEARCHSECTION*/ ;

void PASCALCOMPILER::SEARCHID(SETOFIDS FIDCLS, CTP &FCP)
{
	CTP LCP;
	WRITELN(OUTPUT,"PASCALCOMPILER::SEARCHID(SETOFIDS FIDCLS, CTP &FCP)");
	for (DISX=TOP;DISX>0;DISX--)
	{
		LCP=DISPLAY[DISX].FNAME;
		if (LCP!=NULL)
			if (TREESEARCH(LCP,LCP,ID)==0)
				if (FIDCLS.in(LCP->KLASS))
					goto found;
				else
				if (PRTERR)
					PASCALCOMPILER::CERROR(103);
				else LCP=NULL;
			else LCP=NULL;
		};
		if (PRTERR)
		{
			CERROR(104);
			if (FIDCLS.in(TYPES))
				LCP=UTYPPTR;
			else if (FIDCLS.in(ACTUALVARS))
				LCP=UVARPTR;
			else if (FIDCLS.in(FIELD))
				LCP=UFLDPTR;
			else if (FIDCLS.in(KONST))
				LCP=UCSTPTR;
			else if (FIDCLS.in(PROC1))
				LCP=UPRCPTR;
			else
				LCP=UFCTPTR;
		};
	found: FCP=LCP;
} /*SEARCHID*/ ;

void PASCALCOMPILER::GETBOUNDS(STP FSP, int &FMIN, int &FMAX)
{
	if (FSP->FORM==SUBRANGE)
	{ 
		FMIN=FSP->MIN.IVAL;
		FMAX=FSP->MAX.IVAL;
	}
	else
	{ 
		FMIN=0;
		if (FSP==CHARPTR)
			FMAX=255;
		else
			if (FSP->FCONST!=NULL)
				FMAX=FSP->FCONST->VALUES.IVAL;
			else FMAX=0;
	}
}
/*GETBOUNDS*/

void PASCALCOMPILER::SKIP(SETOFSYS FSYS)
{
	WRITELN(OUTPUT,"PSCALCOMPILER::SKIP ... ");
	while (!FSYS.in(SY))
		INSYMBOL();
}

bool PASCALCOMPILER::PAOFCHAR(STP FSP)
{ 
	bool result=false;
	if (FSP!=NULL)
	if (FSP->FORM==ARRAYS)
		result=FSP->AISPACKD&&(FSP->AELTYPE==CHARPTR);
	return result;
}

bool PASCALCOMPILER::STRGTYPE(STP FSP)
{
	bool result=false;
	if (PAOFCHAR(FSP))
		result=FSP->AISSTRNG;
	return result;
}

int PASCALCOMPILER::DECSIZE(int I)
{
	int result =(TRUNC(I*3.321)+1+BITSPERWD)/BITSPERWD;
	return result;
}

void PASCALCOMPILER::CONSTANT(SETOFSYS FSYS, STP FSP, VALU FVALU)
{
	int LGTH=0;
	STP	LSP;
	CTP	LCP;
	enum {NONE,POS,NEG} SIGN;
	CSP LVP;
	LSP=NULL;
	FVALU.IVAL=0;
	if (!(CONSTBEGSYS.in(SY)))
	{
		CERROR(50);
		SKIP(FSYS+CONSTBEGSYS);
	};
	if (CONSTBEGSYS.in(SY))
	{
		if (SY==STRINGCONST)
		{
			if (LGTH==1)
				LSP=CHARPTR;
			else
			{
				LSP = (STRUCTURE*) new STRUCTURE;
				*LSP=*STRGPTR;
				LSP->MAXLENG=LGTH;
				LSP->INXTYPE=NULL;
				LVP = (CONSTREC*) new CONSTREC;
				*LVP=*VAL.VALP;
				VAL.VALP=LVP;
			};
			FVALU=VAL;
			INSYMBOL();
		}
		else
		{
			SIGN=NONE;
			if ((SY==ADDOP)&&(SET((2),PLUS,MINUS).in(OP)))
			{
				if (OP==PLUS)
					SIGN=POS;
				else SIGN=NEG;
					INSYMBOL();
			};
			if (SY==IDENT)
			{
				SETOFIDS S0((1),KONST);
				SEARCHID(S0,LCP);
				LSP=LCP->IDTYPE;
				FVALU=LCP->VALUES;
			};
			if (SIGN!=NONE)
			{
				if (LSP==INTPTR)
				{
					if (SIGN==NEG)
						FVALU.IVAL=-FVALU.IVAL;
				}
				else if (LSP==REALPTR)
				{
					if (SIGN==NEG)
					{
						LVP = (CONSTREC*) new CONSTREC;
						LVP->CCLASS=REEL;
						LVP->RVAL=-FVALU.VALP->RVAL;
						FVALU.VALP=LVP;
                   }
				}
				else if (COMPTYPES(LSP,LONGINTPTR))
				{
                   if (SIGN==NEG)
				   {
					   LVP = (CONSTREC*) new CONSTREC;
					   LVP->CCLASS=LONG1;
					   LVP->LONGVAL[1]=- FVALU.VALP->LONGVAL[1];
					   FVALU.VALP=LVP;
				   }
				}
				else
					CERROR(105);
				INSYMBOL();
			}
			else if (SY==INTCONST)
			{
				if (SIGN==NEG)
					VAL.IVAL=-VAL.IVAL;
				LSP=INTPTR;
				FVALU=VAL;
				INSYMBOL();
			}
			else if (SY==REALCONST)
			{
				if (SIGN==NEG)
					VAL.VALP->RVAL=-VAL.VALP->RVAL;
				LSP=REALPTR;
				FVALU=VAL;
				INSYMBOL();
			}
			else if (SY==LONGCONST)
			{
               if (SIGN==NEG) {
				   VAL.VALP->LONGVAL[1]=- VAL.VALP->LONGVAL[1];
				   LSP = (STRUCTURE*) new STRUCTURE;
                   LSP->SIZE=DECSIZE(LGTH);
                   LSP->FORM=LONGINT;
                   FVALU=VAL;
                   INSYMBOL();
                 }
			}
			else {
				CERROR(106);
				SKIP(FSYS);
			}
		}
		if (!(FSYS.in(SY)))
		{
			CERROR(6);
			SKIP(FSYS);
		}
	};
	FSP=LSP;
}

bool PASCALCOMPILER::COMPTYPES(STP FSP1, STP FSP2)
{
	bool result;
	bool COMP;
	CTP	NXT1,NXT2;
	TESTP LTESTP1,LTESTP2;

	if ((FSP1==FSP2)||(FSP1==NULL)||(FSP2==NULL))
		result=true;
	else if (FSP1->FORM==FSP2->FORM)
		switch (FSP1->FORM)
		{
			case SCALAR:
				result=false;
				break;
			case SUBRANGE:
				result=COMPTYPES(FSP1->RANGETYPE,FSP2->RANGETYPE);
				break;
			case POINTER:
				COMP=false;
				LTESTP1=GLOBTESTP;
				LTESTP2=GLOBTESTP;
				while (LTESTP1!=NULL)
				{
					if ((LTESTP1->ELT1==FSP1->ELTYPE)&&
					(LTESTP1->ELT2==FSP2->ELTYPE))
					{
						COMP=true;
						LTESTP1=LTESTP1->LASTTESTP;
					};
					if (!COMP)
					{
						LTESTP1 = (TESTPOINTER*) new TESTPOINTER;
						LTESTP1->ELT1=FSP1->ELTYPE;
						LTESTP1->ELT2=FSP2->ELTYPE;
						LTESTP1->LASTTESTP=GLOBTESTP;
					};
					GLOBTESTP=LTESTP1;
					COMP=COMPTYPES(FSP1->ELTYPE,FSP2->ELTYPE);
				};
				result=COMP;
				GLOBTESTP=LTESTP2;
				break;
			case LONGINT:
				result=true;
				break;
			case POWER:
				result=COMPTYPES(FSP1->ELSET,FSP2->ELSET);
				break;
			case ARRAYS:
				COMP=COMPTYPES(FSP1->AELTYPE,FSP2->AELTYPE)
					&& (FSP1->AISPACKD==FSP2->AISPACKD);
				if (COMP&& FSP1->AISPACKD)
				COMP=(FSP1->ELSPERWD==FSP2->ELSPERWD)
                    && (FSP1->ELWIDTH==FSP2->ELWIDTH)
                    && (FSP1->AISSTRNG==FSP2->AISSTRNG);
				if (COMP&& !STRGTYPE(FSP1))
				COMP=(FSP1->SIZE==FSP2->SIZE);
				result=COMP;
				break;
		case RECORDS:
			NXT1=FSP1->FSTFLD;
			NXT2=FSP2->FSTFLD;
			 COMP=true;
			 while ((NXT1!=NULL)&&(NXT2!=NULL)&&COMP)
			 {
				result = COMPTYPES(NXT1->IDTYPE,NXT2->IDTYPE);
				NXT1=NXT1->NEXT;
				NXT2=NXT2->NEXT;
			};
			result=COMP&&(NXT1==NULL)&&(NXT2==NULL)
			   &&(FSP1->RECVAR==NULL)&&(FSP2->RECVAR==NULL);
			break;
		case FILES:
			result=COMPTYPES(FSP1->FILTYPE,FSP2->FILTYPE);
			break;		
		default:
			break;
		}
		else /*FSP1->FORM!=FSP2->FORM*/
			if (FSP1->FORM==SUBRANGE)
				result=COMPTYPES(FSP1->RANGETYPE,FSP2);
			else if (FSP2->FORM==SUBRANGE)
				result=COMPTYPES(FSP1,FSP2->RANGETYPE);
			else
				result=false;
	return result;
}

void PASCALCOMPILER::GENBYTE(int FWORD)
{
	union
	{
		char	bytes[2];
		WORD	word;
	} PCODE;
	PCODE.word = FWORD;
	(*CODEP)[IC] = PCODE.bytes[0];
	IC++;
}

void PASCALCOMPILER::GENWORD(int FWORD)
{
	if (ODD(IC))
		IC++;
	
// warning - check for ENDIANNESS in the
// output!
	union
	{
		DWORD	pinst;
		WORD	word[2];
		char	bytes[4];
	} PCODE;
	
	PCODE.word[0] = FWORD;
	(*CODEP)[IC] = PCODE.bytes[0];
	IC++;
	(*CODEP)[IC] = PCODE.bytes[1];
	IC++;
}

void PASCALCOMPILER::WRITECODE(bool FORCEBUF)
{
	/*FORCEBUF: bool*/;
	int CODEINX,LIC,I;
	CODEINX=0;
	LIC=IC;
	do {
       I=512-CURBYTE;
       if (I>LIC)
		   I=LIC;
       MOVELEFT((char*)(&CODEP[CODEINX]),(char*)(&DISKBUF[CURBYTE]),I);
       CODEINX=CODEINX+I;
       CURBYTE=CURBYTE+I;
       if ((CURBYTE==512)||FORCEBUF)
	   {
		   if (USERINFO.ERRNUM==0)
			   if (SYSCOMM::BLOCKWRITE(USERINFO.WORKCODE,*CODEP,CURBLK)!=1)
				CERROR(402);
           CURBLK=CURBLK+1;
		   CURBYTE=0;
	   }
	   LIC=LIC-I;
   }
   while (LIC!=0);
}

void PASCALCOMPILER::FINISHSEG()
{
	int i;
	IC=0;
	for (i=NEXTPROC-1;i>=1;i--)
	{
       if (PROCTABLE[i]==0)
         GENWORD(0);
       else
         GENWORD(SEGINX+IC-PROCTABLE[i]);
	}
	GENBYTE(SEG);
	GENBYTE(NEXTPROC-1);
	SEGTABLE[SEG].CODELENGTH=SEGINX+IC;
	WRITECODE(true);
	SEGINX=0;
	CODEINSEG=false;
}

void PASCALCOMPILER::FINDFORW(CTP FCP)
{
	SET S1 = SET((2),PROC1,FUNC);
	if (FCP!=NULL)
	{
		if (S1.in(FCP->KLASS))
		if (FCP->PFDECKIND==DECLARED)
			if (FCP->PFKIND==ACTUAL)
				if (FCP->FORWDECL)
				{
					USERINFO.ERRNUM=117;
					WRITELN(OUTPUT);
					WRITE(OUTPUT,FCP->NAME," undefined");
				};
		FINDFORW(FCP->RLINK); FINDFORW(FCP->LLINK);
	}
} 
/*FINDFORW*/ ;

void PASCALCOMPILER::BLOCK(SETOFSYS FSYS)
{
	SET S1 = SET((2),UNITSY,SEPARATSY);
	SETOFSYS S2;
	S2 = S2 + (SETOFSYS)SET((4),UNITSY,INTERSY,IMPLESY,ENDSY);
	bool BFSYFOUND;
	if ((NOSWAP)&&(STARTINGUP))
	{
		BODYPART(FSYS,NULL);
		return;
	}
	if ((S1.in(SY))&&(!INMODULE))
	{
		UNITPART(FSYS+S2);
		if (SY==PERIOD)
			return;
	};
	NEWBLOCK=TRUE;
	do {
		if (!NEWBLOCK)
		{
			DP=false;
			STMTLEV=0;
			IC=0;
			LINEINFO=0;
			if ((!SYSCOMP)||(LEVEL>1))
				FINDFORW(DISPLAY[TOP].FNAME);
			if (INMODULE)
				if ((TOS->PREVLEXSTACKP)->DFPROCP==OUTERBLOCK)
					if (SY==ENDSY)
					{
						FINISHSEG();
						return;
					}
					else if (SY==BEGINSY)
					{
						CERROR(13);
						FINISHSEG();
						return;
					}
			if (SY==BEGINSY)
				INSYMBOL();
			else
				CERROR(17);
			do
			{
				BODYPART(FSYS + CASESY - ENDSY, TOS->DFPROCP);
				BFSYFOUND=(BEGINSY==TOS->BFSY)||(INMODULE&&(SY==ENDSY));
				if (!BFSYFOUND)
				{
					if (TOS->BFSY==SEMICOLON)
						CERROR(14);  /*SEMICOLON EXPECTED*/
					else
						CERROR(6);  /* PERIOD EXPECTED */
					SKIP(FSYS+TOS->BFSY);
					BFSYFOUND=(SY==TOS->BFSY)||(INMODULE&& (SY==ENDSY));
				}
			}
			while (!(BFSYFOUND)||BLOCKBEGSYS.in(SY));
			if (!BFSYFOUND)
			{
				if (TOS->BFSY==SEMICOLON)
					CERROR(14);
				else
					CERROR(6); /*PERIOD EXPECTED*/
				DECLARATIONPART(FSYS);
			}
			else //BFSYFOUND
			{
				if (SY==SEMICOLON)
					INSYMBOL();
				if (!(SET((4),BEGINSY,PROCSY,FUNCSY,PROGSY).in(SY))&&
				(TOS->BFSY==SEMICOLON))
				if (!(INMODULE&&(SY==ENDSY)))
				{
					CERROR(6);
					SKIP(FSYS);
					DECLARATIONPART(FSYS);
				}
				else
					goto fail;
			else
fail:		{
				if (TOS->DFPROCP!=NULL)
					(TOS->DFPROCP)->INSCOPE=FALSE;
				if (TOS->ISSEGMENT)
				{
					if (CODEINSEG)
						FINISHSEG();
					if (DLINKERINFO&&(LEVEL==1))
					{
						SEGTABLE[SEG].SEGKIND=2;
						WRITELINKERINFO(true);
					}
					else if (CLINKERINFO)
					{
						SEGTABLE[SEG].SEGKIND=2;
						WRITELINKERINFO(false);
					};
					NEXTPROC=TOS->SOLDPROC;
					SEG=TOS->DOLDSEG;
				};
				LEVEL=TOS->DOLDLEV;
				TOP=TOS->DOLDTOP;
				LC=TOS->DLLC;
				CURPROC=TOS->POLDPROC;
			};
			RELEASE(TOS->DMARKP);
			TOS=TOS->PREVLEXSTACKP;
			NEWBLOCK=SET((3),PROCSY,FUNCSY,PROGSY).in(SY);
		}
	}
	else
	{
		DECLARATIONS(FSYS);
		if (LEVEL==0)
			if (SET((2),UNITSY,SEPARATSY).in(SY))
			{
				UNITPART(FSYS + SET((4),UNITSY,INTERSY,IMPLESY,ENDSY));
				if (SET((3),PROCSY,FUNCSY,PROGSY).in(SY))
					DECLARATIONPART(FSYS);
			}
	}}
	while (!TOS==NULL);
	FINISHSEG();
}

UINT PASCALCOMPILER::THREAD_ENTRY (LPVOID param)
{
	WRITELN(OUTPUT,"July ",4,", ",1776,"...");
	PASCALCOMPILER	*p = (PASCALCOMPILER*) param;
	p->PASCALCOMPILER::COMPINIT();
	p->SYMBOL_DUMP (NULL);
	p->COMPILER_MAIN(NULL);
	return 0;
}

int PASCALCOMPILER::COMPILER_MAIN (LPVOID)
{
	/* PASCALCOMPILER */
	
	int	LGTH = 0;
	TIME(LGTH,LOWTIME);
#if 0
	// fixme!!
	BLOCK(BLOCKBEGSYS+STATBEGSYS-WTF[CASESY]);
#endif
	if (SY!=PERIOD)
		CERROR(21);
	if (LIST)
	{
		SCREENDOTS++;
		(*SYMBUFP)[SYMCURSOR]=(char)(EOL);
		SYMCURSOR++;
		PRINTLINE();
	};
	USERINFO.ERRBLK=0;
	TIME(LGTH,STARTDOTS);
	LOWTIME=STARTDOTS-LOWTIME;
	SYSCOMM::UNITWRITE(3,".......",7);
	if (DLINKERINFO||CLINKERINFO)
	{
		SEGTABLE[(int)SEG].SEGKIND=1;
		WRITELINKERINFO(true);
	};
	SYSCOMM::CLOSE(&_LP,LOCK);
	if (NOISY)
		WRITELN(OUTPUT);
	
	WRITE(OUTPUT,SCREENDOTS," lines");
#if 0
	if (LOWTIME>0)
		WRITE(OUTPUT,", ",(LOWTIME+30)/60," secs, ",
         ROUND((3600.0/LOWTIME)*SCREENDOTS)," lines/min");
#endif	
	if (NOISY)
	{
	   WRITELN(OUTPUT);
	   char nbuf[16];
	   sprintf_s(nbuf,16,"%d",SMALLESTSPACE);
       WRITE(OUTPUT,"Smallest available space = ",nbuf," words");
	   WRITELN(OUTPUT); 
	}
	IC=0;
	for (SEG=0;SEG<=MAXSEG;SEG++)		
	{ 
		GENWORD(SEGTABLE[SEG].DISKADDR);
		GENWORD(SEGTABLE[SEG].CODELENGTH);
	}
	for (SEG=0;SEG<=MAXSEG;SEG++)
	{
		for (LGTH=1;LGTH<=8;LGTH++)
			GENBYTE(ORD(SEGTABLE[SEG].SEGNAME[LGTH]));
	}
	for (SEG=0;SEG<=MAXSEG;SEG++)
	   GENWORD(SEGTABLE[SEG].SEGKIND);
	
	for (SEG=0;SEG<=MAXSEG;SEG++)
	   GENWORD(SEGTABLE[SEG].TEXTADDR);
	
	for (LGTH=1;LGTH<=80;LGTH++)
     if (COMMENT!=NULL)
		 GENBYTE(ORD(COMMENT[LGTH]));
	 else
		GENBYTE(0);
	 
	for (LGTH=1;LGTH<=256-8*(MAXSEG+1)-40;LGTH++)
		GENWORD(0);
	
	CURBLK=0;
	CURBYTE=0;
	WRITECODE(true);
	return 0;
}
