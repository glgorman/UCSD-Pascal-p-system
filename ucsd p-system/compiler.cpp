
#include "stdafx.h"

//#include "../Frame Lisp/symbol_table.h"
#include "../Frame Lisp/btreetype.h"
//#include "../Frame Lisp/node_list.h"
//#include "../Frame Lisp/text_object.h"

#include "compilerdata.h"
#include "declarationpart.h"
#include "bodypart.h"
#include "compiler.h"
#include "unitpart.h"

void COMPILERDATA::ENTERID(CTP FCP)
{
	WRITELN(OUTPUT,"PASCALCOMPILER::ENTERID: ");
	WRITELN(OUTPUT,"FCP->NAME = \"",FCP->NAME,'"');

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
			else
				I=TREESEARCH(LCP1->RLINK,LCP1,FCP->NAME);
		}
		if (I==1)
			LCP1->RLINK=FCP;
		else
			LCP1->LLINK=FCP;
	};
	FCP->LLINK=NULL;
	FCP->RLINK=NULL;

#if 0
	PRINTTREE(LCP);
#endif
} /*ENTERID*/ ;

void PASCALCOMPILER::ENTERID(CTP FCP)
{
	COMPILERDATA::ENTERID(FCP);
}

PASCALCOMPILER::PASCALCOMPILER()
{

}

PASCALCOMPILER::PASCALCOMPILER(INFOREC &arg)
{
//  CRASHES! why?
//	memset (this,0,sizeof(PASCALCOMPILER));
	size_t sz1, sz2, sz3;
	sz1 = sizeof(PASCALSOURCE);
	sz2 = sizeof(COMPILERDATA);
	sz3 = sizeof(PASCALCOMPILER);
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
	WRITELN(OUTPUT,"COMPINIT::ENTSTDTYPES()");
//	NEW(INTPTR,SCALAR,STANDARD);
	structure *INTPTR;
	INTPTR = (structure*) new structure(SCALAR);
	INTPTR->SIZE = INTSIZE;
	INTPTR->SCALKIND=STANDARD;
	m_ptr->INTPTR = INTPTR;

	structure *REALPTR;
	REALPTR = (structure*) new structure(SCALAR);
	REALPTR->SIZE = REALSIZE;
	REALPTR->SCALKIND=STANDARD;
	m_ptr->REALPTR = REALPTR;

	structure *LONGINTPTR;
	LONGINTPTR = (structure*) new structure(SCALAR);
	LONGINTPTR->SIZE = INTSIZE;
	LONGINTPTR->SCALKIND=STANDARD;
	m_ptr->LONGINTPTR = LONGINTPTR;

	m_ptr->CHARPTR = (structure*) new structure(SCALAR);
	m_ptr->CHARPTR->SIZE = CHARSIZE;
	m_ptr->CHARPTR->SCALKIND=STANDARD;

	m_ptr->BOOLPTR = (structure*) new structure(SCALAR);
	m_ptr->BOOLPTR->SIZE = BOOLSIZE;
	m_ptr->BOOLPTR->SCALKIND=DECLARED;

	m_ptr->NILPTR = (structure*) new structure(POINTER);
	m_ptr->NILPTR->SIZE = PTRSIZE;
	m_ptr->NILPTR->ELTYPE=NULL ;
   
	m_ptr->TEXTPTR = (structure*) new structure(FILES);
	m_ptr->TEXTPTR->SIZE = FILESIZE+CHARSIZE;
	m_ptr->TEXTPTR->FILTYPE=m_ptr->CHARPTR;
	
	// NEW(INTRACTVPTR,FILES);
	structure *INTRACTVPTR; 
	INTRACTVPTR = (structure*) new structure(FILES);
	INTRACTVPTR->SIZE=FILESIZE+CHARSIZE;
	// FIXME? weird??
	INTRACTVPTR->FILTYPE = m_ptr->CHARPTR;
	m_ptr->INTRACTVPTR = INTRACTVPTR;
	
	// NEW(STRGPTR,ARRAYS,true,true);
	structure *STRGPTR; 
	STRGPTR = new structure(ARRAYS);
	STRGPTR->SIZE=(DEFSTRGLGTH + CHRSPERWD)/CHRSPERWD;
	STRGPTR->AISPACKD=true;
	STRGPTR->AISSTRNG=true;
	STRGPTR->INXTYPE=INTPTR;
	STRGPTR->ELWIDTH=BITSPERCHR;
	STRGPTR->ELSPERWD=CHRSPERWD;
	// FIXME? weird??
	STRGPTR->AELTYPE=m_ptr->CHARPTR;
	STRGPTR->MAXLENG=DEFSTRGLGTH;
	m_ptr->STRGPTR = STRGPTR;

} /*ENTSTDTYPES*/

void COMPINIT::ENTSTDNAMES()
{
	WRITELN(OUTPUT,"COMPINIT::ENTSTDNAMES()");
	CTP	CP;
	CTP	CP1;
	int I;
	STP &INTPTR = m_ptr->INTPTR;
	STP &REALPTR = m_ptr->REALPTR;
	STP &CHARPTR = m_ptr->CHARPTR;
	STP &BOOLPTR = m_ptr->BOOLPTR;
	STP &STRGPTR = m_ptr->STRGPTR;
	STP &TEXTPTR = m_ptr->TEXTPTR;
	STP &INTRACTVPTR = m_ptr->INTRACTVPTR;
	STP &NILPTR = m_ptr->NILPTR;

	CP = (identifier*) new identifier("INTEGER ",INTPTR,TYPES);
	m_ptr->ENTERID(CP);
	CP = (identifier*) new identifier("REAL    ",REALPTR,TYPES);
	m_ptr->ENTERID(CP);
	CP = (identifier*) new identifier("CHAR    ",CHARPTR,TYPES);
	m_ptr->ENTERID(CP);
	CP = (identifier*) new identifier("BOOLEAN ",BOOLPTR,TYPES);
	m_ptr->ENTERID(CP);
	CP = (identifier*) new identifier("STRING  ",STRGPTR,TYPES);
	m_ptr->ENTERID(CP);
	CP = (identifier*) new identifier("TEXT  ",TEXTPTR,TYPES);
	m_ptr->ENTERID(CP);
	CP = (identifier*) new identifier("INTERACT ",INTRACTVPTR,TYPES);
	m_ptr->ENTERID(CP);
	m_ptr->INPUTPTR = (identifier*) new identifier("INPUT  ",TEXTPTR,FORMALVARS);
	m_ptr->INPUTPTR->VLEV = 0;
	m_ptr->INPUTPTR->VADDR = 2;
	m_ptr->ENTERID(m_ptr->INPUTPTR);
	m_ptr->OUTPUTPTR = (identifier*) new identifier("OUTPUT  ",TEXTPTR,FORMALVARS);
	m_ptr->OUTPUTPTR->IDTYPE = TEXTPTR;
	m_ptr->OUTPUTPTR->VLEV = 0;
	m_ptr->OUTPUTPTR->VADDR = 3;
	m_ptr->ENTERID(m_ptr->OUTPUTPTR);

	CP = (identifier*) new identifier(FORMALVARS);
	strcpy_s(CP->NAME,IDENTSIZE,"KEYBOARD ");
	CP->IDTYPE = TEXTPTR;
	CP->VLEV = 0;
	CP->VADDR = 4;
	m_ptr->ENTERID(CP);
	
	CP1=NULL;
	for (I=0;I<=1;I++)
	{
		CP = (identifier*) new identifier(NULL,BOOLPTR,KONST);
        if (I==0)
			strcpy_s(CP->NAME,IDENTSIZE,"FALSE   ");
		else 
			strcpy_s(CP->NAME,IDENTSIZE,"TRUE    ");
		CP->NEXT=CP1;
		CP->VALUES.IVAL=I;
		m_ptr->ENTERID(CP);
		CP1=CP;
    };
    m_ptr->BOOLPTR->FCONST=CP;
	CP = (identifier*) new identifier("NULL     ",NILPTR,KONST);
	CP->NEXT=NULL;
	CP->VALUES.IVAL=0;
	m_ptr->ENTERID(CP);
	CP = (identifier*) new identifier("MAXINT  ",INTPTR,KONST);
	CP->VALUES.IVAL=MAXINT;
	m_ptr->ENTERID(CP);
} /*ENTSTDNAMES*/ ;

void COMPINIT::ENTUNDECL()
{
	WRITELN(OUTPUT,"COMPINIT::ENTUNDECL()");
	m_ptr->UTYPPTR = (identifier*) new identifier(TYPES);
	strcpy_s(m_ptr->UTYPPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UTYPPTR->IDTYPE=NULL;

	m_ptr->UCSTPTR = (identifier*) new identifier(KONST);
	strcpy_s(m_ptr->UCSTPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UCSTPTR->IDTYPE=NULL;
	m_ptr->UCSTPTR->VALUES.IVAL = NULL;
	m_ptr->UCSTPTR->NEXT=NULL;

	m_ptr->UVARPTR = (identifier*) new identifier(ACTUALVARS);
	strcpy_s(m_ptr->UVARPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UVARPTR->IDTYPE=NULL;
	m_ptr->UCSTPTR->NEXT = NULL;
	m_ptr->UCSTPTR->VLEV = 0;
	m_ptr->UCSTPTR->VADDR = 0;

	m_ptr->UFLDPTR = (identifier*) new identifier(FIELD);
	strcpy_s(m_ptr->UFLDPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UFLDPTR->IDTYPE=NULL;
	m_ptr->UFLDPTR->NEXT = NULL;
	m_ptr->UFLDPTR->FLDADDR = 0;

	m_ptr->UPRCPTR = (identifier*) new identifier(PROC1);
	strcpy_s(m_ptr->UPRCPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UPRCPTR->IDTYPE=NULL;
	m_ptr->UPRCPTR->FORWDECL=false;
	m_ptr->UPRCPTR->NEXT = NULL;
	m_ptr->UPRCPTR->INSCOPE=false;
	m_ptr->UPRCPTR->LOCALLC=0;
	m_ptr->UPRCPTR->EXTURNAL=false;
	m_ptr->UPRCPTR->PFDECKIND=DECLARED;
	m_ptr->UPRCPTR->PFKIND=ACTUAL;

	m_ptr->UFCTPTR = (identifier*) new identifier(FUNC);
	strcpy_s(m_ptr->UFCTPTR->NAME,IDENTSIZE,"        ");
	m_ptr->UFCTPTR->IDTYPE=NULL;
	m_ptr->UFCTPTR->NEXT = NULL;
	m_ptr->UFCTPTR->FORWDECL=false;
	m_ptr->UFCTPTR->INSCOPE=false;
	m_ptr->UFCTPTR->LOCALLC=0;
	m_ptr->UFCTPTR->PFLEV=0;
	m_ptr->UFCTPTR->PFNAME=0;
	m_ptr->UFCTPTR->PFSEG=0;
	m_ptr->UFCTPTR->PFDECKIND=DECLARED;
	m_ptr->UFCTPTR->PFKIND=ACTUAL;
} /*ENTUNDECL*/ ;

void COMPINIT::ENTSPCPROCS()
{
	CTP	LCP;
	int	I;
	bool ISFUNC;
	ALPHA	NA[44];
	WRITELN(OUTPUT,"COMPINIT::ENTSPCPROCS()");
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
	SET<128> TSET((15),2,7,8,10,13,17,18,19,20,32,34,35,40,42,43);
	SET<128> FSET((17),5,6,7,8,9,10,11,15,16,19,20,25,36,37,38,39,41);
	for (I=1;I<=43;I++)
	{
		if (m_ptr->options.TINY)
			if (TSET.in(I))
               continue;
		ISFUNC= FSET.in(I);
		IDCLASS idclass;
		if (ISFUNC)
			idclass=FUNC;
		else
			idclass=PROC1;
		LCP = (identifier*) new identifier(idclass);
		LCP->PFDECKIND = SPECIAL;
		strcpy_s(LCP->NAME,IDENTSIZE,NA[I]);
		LCP->NEXT=NULL;
		LCP->IDTYPE=NULL;
		LCP->KEY=I;	
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
	WRITELN(OUTPUT,"COMPINIT::ENTSTDPROCS()");
	
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

	SET<128> SYSCALLS((5),12,13,17,18,19);

	for (i=1;i<20;i++)
	{
		ISPROC = SYSCALLS.in(i);
		switch (i)
		{
		case 1:
			FTYPE=m_ptr->BOOLPTR;;
			PARAM =  (identifier*) new identifier(ACTUALVARS);
			PARAM->IDTYPE=m_ptr->INTPTR;
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
			PARAM =  (identifier*) new identifier(ACTUALVARS);
			PARAM->IDTYPE=m_ptr->REALPTR;
			break;

		case 5:
			FTYPE=m_ptr->REALPTR;
			break;

		case 12:
			FTYPE=NULL;
			PARAM = (identifier*) new identifier(FORMALVARS);
			LSP = (structure*) new structure(POINTER);
			LSP->SIZE=PTRSIZE;
			LSP->ELTYPE=NULL;
			PARAM->IDTYPE=LSP;
			break;

		case 14:
			FTYPE=m_ptr->REALPTR;
			PARAM=NULL;
			break;

		case 15:
			FTYPE=m_ptr->BOOLPTR; 
			PARAM = (identifier*) new identifier(ACTUALVARS);
			PARAM->IDTYPE=m_ptr->INTPTR;
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
		IDCLASS idclass;
		if (ISPROC)
			idclass=PROC1;
		else
			idclass=FUNC;
		LCP = (identifier*) new identifier((char*)NA[i],NULL,idclass);		
		LCP->PFDECKIND=STANDARD;
		LCP->CSPNUM=i + 20;
		
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
	 WRITELN(OUTPUT,"COMPINIT::INITSCALARS()");
	 NONRESIDENT I;
     m_ptr->FWPTR=NULL;
	 m_ptr->MODPTR=NULL;
	 m_ptr->GLOBTESTP=NULL;
     m_ptr->m_src.LINESTART=0;
	 m_ptr->LINEINFO=LCAFTERMARKSTACK;
	 m_ptr->options.LIST=false;
     m_ptr->m_src.SYMBLK=0;
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
	 m_ptr->options.IOCHECK=true;
	 m_ptr->options.DP=true;
     m_ptr->SEGINX=0;
	 m_ptr->NEXTJTAB=1;
	 m_ptr->NEXTPROC=2;
	 m_ptr->CURPROC=1;

     m_ptr->SCONST = new CONSTREC(VOID2);
	 m_ptr->m_src.SYMBUFP = (SYMBUFARRAY*) new SYMBUFARRAY;
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
	 m_ptr->options.NOISY=!m_ptr->USERINFO.SLOWTERM;
	 m_ptr->SEPPROC=false;
     m_ptr->options.NOSWAP=true;
	 m_ptr->options.DEBUGGING=false;
	 m_ptr->options.BPTONLINE=false;
	 m_ptr->options.INMODULE=false;
     m_ptr->options.GOTOOK=false;
	 m_ptr->options.RANGECHECK=true;
	 m_ptr->options.SYSCOMP=false;
	 m_ptr->options.TINY=false;
     m_ptr->CODEINSEG=false;
	 m_ptr->PRTERR=true;
	 m_ptr->options.INCLUDING=false;
	 m_ptr->options.USING=false;
	 I=SEEK;
	 while (I<=DECOPS)
	 {
		 m_ptr->PFNUMOF[I]=0;
		 I = (NONRESIDENT)(I+1);
	 }
	 m_ptr->COMMENT=NULL;
	 m_ptr->options.LIBNOTOPEN=true;
	 m_ptr->options.GETSTMTLEV=true;
	 m_ptr->BEGSTMTLEV=0;
	 m_ptr->STMTLEV=0;
	 /*INITSCALARS*/ ;
}

void COMPINIT::INITSETS()
{
	WRITELN(OUTPUT,"COMPINIT::INITSETS()");
	m_ptr->BNF::CONSTBEGSYS = SETOFSYS((5),SYMBOLS::ADDOP,SYMBOLS::INTCONST,
		SYMBOLS::REALCONST,SYMBOLS::STRINGCONST,SYMBOLS::IDENT);
	m_ptr->BNF::SIMPTYPEBEGSYS = SETOFSYS((1),SYMBOLS::LPARENT) + m_ptr->BNF::CONSTBEGSYS;	
	m_ptr->BNF::TYPEBEGSYS = SETOFSYS((6),SYMBOLS::ARROW,SYMBOLS::PACKEDSY,
		SYMBOLS::ARRAYSY,SYMBOLS::RECORDSY,SYMBOLS::SETSY,SYMBOLS::FILESY)
	+ m_ptr->BNF::SIMPTYPEBEGSYS;	
	m_ptr->BNF::TYPEDELS = SETOFSYS((4),SYMBOLS::ARRAYSY,SYMBOLS::RECORDSY,
		SYMBOLS::SETSY,SYMBOLS::FILESY);	
	m_ptr->BNF::BLOCKBEGSYS = SETOFSYS((9),SYMBOLS::USESSY,SYMBOLS::LABELSY,
		SYMBOLS::CONSTSY,SYMBOLS::TYPESY,SYMBOLS::VARSY,
		SYMBOLS::PROCSY,SYMBOLS::FUNCSY,SYMBOLS::PROGSY,SYMBOLS::IFSY);	
	m_ptr->BNF::SELECTSYS = SETOFSYS((3),SYMBOLS::ARROW,SYMBOLS::PERIOD,SYMBOLS::LBRACK);	
	m_ptr->BNF::FACBEGSYS = SETOFSYS((8),SYMBOLS::INTCONST,SYMBOLS::REALCONST,
		SYMBOLS::LONGCONST,SYMBOLS::STRINGCONST,SYMBOLS::IDENT,
		SYMBOLS::LPARENT,SYMBOLS::LBRACK,SYMBOLS::NOTSY);	
	m_ptr->BNF::STATBEGSYS = SETOFSYS((8),SYMBOLS::BEGINSY,SYMBOLS::GOTOSY,
		SYMBOLS::IFSY,SYMBOLS::WHILESY,SYMBOLS::REPEATSY,
		SYMBOLS::FORSY,SYMBOLS::WITHSY,SYMBOLS::CASESY);	
	m_ptr->BNF::VARS = SETOFIDS((2),FORMALVARS,ACTUALVARS);
} /*INITSETS*/ ;

void COMPINIT::INIT(PASCALCOMPILER *p)
{
	WRITELN(OUTPUT,"COMPINIT::INIT(PASCALCOMPILER *p)");
	int I;
	m_ptr=p;
	INITSCALARS();
	INITSETS();
	m_ptr->LEVEL=0;
	m_ptr->TOP=0;
	if (m_ptr->options.NOISY)
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

	if (m_ptr->options.SYSCOMP)
	{
		 m_ptr->OUTERBLOCK=NULL;
		 m_ptr->SEG=0;
		 m_ptr->NEXTSEG=1;
		 m_ptr->GLEV = -1;
		 m_ptr->BNF::BLOCKBEGSYS=m_ptr->BNF::BLOCKBEGSYS + 
			 SETOFSYS(2,SYMBOLS::UNITSY,SYMBOLS::SEPARATSY);
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
      
	m_ptr->OUTERBLOCK = new identifier(PROC1);
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

   if (m_ptr->SY==SYMBOLS::PROGSY)
   {
	   m_ptr->INSYMBOL();
       if (m_ptr->SY==SYMBOLS::IDENT)
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
	PRINTTREE1(m_ptr->DISPLAY[0].FNAME);
#endif

	   m_ptr->INSYMBOL();
	   SETOFSYS SPECIAL = SETOFSYS(2,SYMBOLS::RPARENT,SYMBOLS::SEMICOLON)
		   +m_ptr->BNF::BLOCKBEGSYS;
	   if (m_ptr->SY==SYMBOLS::LPARENT)
	   {
			do
				m_ptr->INSYMBOL();
			while (!SPECIAL.in(m_ptr->SY));
			if (m_ptr->SY==SYMBOLS::RPARENT)
				m_ptr->INSYMBOL();
			else
				m_ptr->CERROR(4);
	   }
		if (m_ptr->SY==SYMBOLS::SEMICOLON)
			m_ptr->INSYMBOL();
		else
			m_ptr->CERROR(14);
   }
   MARK(m_ptr->MARKP);
   m_ptr->TOS = new LEXSTKREC;
//   // WITH TOS^) {  /*MAKE LEXSTKREC for (OUTERBLOCK*/
//     {
   m_ptr->TOS->PREVLEXSTACKP=NULL;
   m_ptr->TOS->BFSY=SYMBOLS::PERIOD;
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
	bool except;
	char *str_exit;
	except = false;
	try
	{
		DECLARATIONPART::MAIN (FSYS);
		WRITELN(OUTPUT,"DECLARATIONS COMPLETED");
	}
	catch (EXIT_CODE e)
	{
		except = true;
		str_exit = e.m_str;
		WRITELN(OUTPUT,"PASCALCOMPILER::DECLARATIONS(SETOFSYS FSYS)");
		WRITELN(OUTPUT,"CAUGHT EXCEPTION:", str_exit);
		WRITELN(OUTPUT,str_exit);
	}
	if (except==false)
		WRITELN(OUTPUT,"Resuming Compilation");

}

void PASCALCOMPILER::BODY(const SETOFSYS &FSYS, CTP FPROCP)
{
	char *str_exit;
	bool except = false;
	LPVOID ptr1 = (LPVOID) FPROCP;
	WRITELN(OUTPUT,"PASCALCOMPILER::BODYPART FPROCP = ", (int)ptr1);
	BODYPART *ptr2;
	ptr2 = static_cast<BODYPART*>(BODYPART::allocate((void*)this));
	try
	{
		ptr2->MAIN(FSYS,FPROCP);
		WRITELN(OUTPUT,"PASCALCOMPILER::BODYPART - completed");
	}
	catch (EXIT_CODE e)
	{
		except = true;
		str_exit = e.m_str;
		WRITELN(OUTPUT,"PASCALCOMPILER::BODY(const SETOFSYS &FSYS, CTP FPROCP)");
		WRITELN(OUTPUT,"CAUGHT EXCEPTION: <",e.err,"> ... ",str_exit);
		WRITELN(OUTPUT,str_exit);
	}
	if (except==false)
		WRITELN(OUTPUT,"Resuming Compilation");
}

void PASCALCOMPILER::WRITELINKERINFO(bool flag)
{
	LINKERINFO MAGIC;
	MAGIC.WRITELINKERINFO(flag);
}

void PASCALCOMPILER::UNITPART(SETOFSYS)
{
	
}

void COMPILERDATA::SEARCHSECTION(CTP FCP, CTP &FCP1)
{
	if (FCP!=NULL)
		if (TREESEARCH(FCP,FCP1,ID)==0) /*NADA*/
			;
		else
			FCP1=NULL;
	else
		FCP1=NULL;
} /*SEARCHSECTION*/ ;

void COMPILERDATA::SEARCHID(const SETOFIDS &FIDCLS, CTP &FCP)
{
	PASCALCOMPILER *ptr = reinterpret_cast<PASCALCOMPILER*>(this);
	ptr->PASCALCOMPILER::SEARCHID(FIDCLS,FCP);
}

void PASCALCOMPILER::SEARCHID(const SETOFIDS &FIDCLS, CTP &FCP)
{
	CTP LCP, LCP1;
	int status = false;
	WRITELN(OUTPUT);
	::WRITE(OUTPUT,"PASCALCOMPILER::SEARCHID: ");
	WRITELN(OUTPUT,"Searching for: \"",ID,"\" in SETOFIDS ");
	FIDCLS.debug_list ();

	for (DISX=TOP;DISX>=0;DISX--)
	{
		LCP=DISPLAY[DISX].FNAME;
		LCP1 = NULL;
//		PRINTTREE1(LCP);
		if (LCP!=NULL)
		{
			status = (TREESEARCH(LCP,LCP1,ID)==0?true:false);
			if (status==true)
				if (FIDCLS.in(LCP1->KLASS))
					goto found;
				else if (PRTERR)
					PASCALSOURCE::CERROR(103);
				else
					LCP=NULL;
			else
				LCP=NULL;
		}
	}
	if (PRTERR)
	{
		CERROR(104);
#if 0
		ASSERT(false);
#endif
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
	}
	found: FCP=LCP;
#if 1
	if (LCP!=NULL)
		identifier::debug1(LCP,false);
#endif
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
		else if (FSP->FCONST!=NULL)
			FMAX=FSP->FCONST->VALUES.IVAL;
		else FMAX=0;
	}
}
/*GETBOUNDS*/

void PASCALSOURCE::SKIP(const SETOFSYS &FSYS)
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

void PASCALCOMPILER::CONSTANT(const SETOFSYS &FSYS, STP FSP, VALU &FVALU)
{
	int LGTH=0;
	STP	LSP;
	CTP	LCP;
	enum {NONE,POS,NEG} SIGN;
	CSP LVP;

	LSP=NULL;
	FVALU.IVAL=0;
	if (!(BNF::CONSTBEGSYS.in(SY)))
	{
		CERROR(50);
		SKIP(FSYS+BNF::CONSTBEGSYS);
	}
	if (BNF::CONSTBEGSYS.in(SY))
	{
		if (SY==SYMBOLS::STRINGCONST)
		{
			if (LGTH==1)
				LSP=CHARPTR;
			else
			{
				LSP = (structure*) new structure(ARRAYS);
				*LSP=*STRGPTR;
				LSP->AISPACKD=true;
				LSP->AISSTRNG=true;
				LSP->MAXLENG=LGTH;
				LSP->INXTYPE=NULL;
				LVP = (CONSTREC*) new CONSTREC(VOID2);
				*LVP=*VAL.VALP;
				VAL.VALP=LVP;
			}
			FVALU=VAL;
			INSYMBOL();
		}
		else
		{
			SIGN=NONE;
			if ((SY==SYMBOLS::ADDOP)&&
				(SET<128>((2),PLUS,MINUS).in(OP)))
			{
				if (OP==PLUS)
					SIGN=POS;
				else SIGN=NEG;
					INSYMBOL();
			}
			if (SY==SYMBOLS::IDENT)
			{
				SETOFIDS S0((1),KONST);
				SEARCHID(S0,LCP);
				LSP=LCP->IDTYPE;
				FVALU=LCP->VALUES;
				if (SIGN!=NONE)
					if (LSP==INTPTR)
					{
						if (SIGN==NEG)
							FVALU.IVAL=-FVALU.IVAL;
					}
				else if (LSP==REALPTR)
				{
					if (SIGN==NEG)
					{
						LVP = (CONSTREC*) new CONSTREC(REEL);
						LVP->RVAL=-FVALU.VALP->RVAL;
						FVALU.VALP=LVP;
					}
				}
				else if (COMPTYPES(LSP,LONGINTPTR))
				{
					if (SIGN==NEG)
					{
						LVP = (CONSTREC*) new CONSTREC(LONG1);
						LVP->LONGVAL[1]=- FVALU.VALP->LONGVAL[1];
						FVALU.VALP=LVP;
					}
				}
				else
					CERROR(105);
				INSYMBOL();
			}
			else if (SY==SYMBOLS::INTCONST)
			{
				if (SIGN==NEG)
					VAL.IVAL=-VAL.IVAL;
				LSP=INTPTR;
				FVALU=VAL;
				INSYMBOL();
			}
			else if (SY==SYMBOLS::REALCONST)
			{
				if (SIGN==NEG)
					VAL.VALP->RVAL=-VAL.VALP->RVAL;
				LSP=REALPTR;
				FVALU=VAL;
				INSYMBOL();
			}
			else if (SY==SYMBOLS::LONGCONST)
			{
               if (SIGN==NEG) {
				   VAL.VALP->LONGVAL[1]=- VAL.VALP->LONGVAL[1];
				   LSP = (structure*) new structure(LONGINT);
                   LSP->SIZE=DECSIZE(LGTH);
                   FVALU=VAL;
                   INSYMBOL();
                 }
			}
			else
			{
				CERROR(106);
				SKIP(FSYS);
			}
		}
		if (!(FSYS.in(SY)))
		{
			CERROR(6);
			SKIP(FSYS);
		}
	}
	FSP=LSP;
}

bool PASCALCOMPILER::COMPTYPES(STP &FSP1, STP &FSP2)
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
		else if (FSP1->FORM==SUBRANGE)
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
	SETOFIDS S1 = SETOFIDS((2),PROC1,FUNC);
	if (FCP!=NULL)
	{
		if (S1.in(FCP->KLASS))
		if (FCP->PFDECKIND==DECLARED)
		if (FCP->PFKIND==ACTUAL)
		if (FCP->FORWDECL)
		{
			USERINFO.ERRNUM=117;
			WRITELN(OUTPUT);
			::WRITELN(OUTPUT,FCP->NAME," undefined");
		};
		FINDFORW(FCP->RLINK);
		FINDFORW(FCP->LLINK);
	}
} 
/*FINDFORW*/ ;

void PASCALCOMPILER::BLOCK(const SETOFSYS &FSYS)
{
#if 0
	FSYS.debug_list ("FSYS");
#endif

	SETOFSYS S1 = SETOFSYS((2),SYMBOLS::UNITSY,SYMBOLS::SEPARATSY);
	SETOFSYS S2 = SETOFSYS((4),SYMBOLS::UNITSY,
		SYMBOLS::INTERSY,SYMBOLS::IMPLESY,SYMBOLS::ENDSY);
	
	SETOFSYS S3 = FSYS;
	S3 += SYMBOLS::CASESY;
	S3 -= SYMBOLS::ENDSY;
	bool BFSYFOUND;

	WRITELN(OUTPUT,"PASCALCOMPILER::BLOCK");
#if 0
	FSYS.debug_list();
#endif
#if 0
	SY=PERIOD;
#endif
	if ((options.NOSWAP)&&(STARTINGUP))
	{
		BODY(FSYS,NULL);
		return;
	}
	if ((S1.in(SY))&&(!options.INMODULE))
	{
		UNITPART(FSYS+S2);
		if (SY==SYMBOLS::PERIOD)
			return;
	}
	NEWBLOCK=true;
	do {
		if (!NEWBLOCK)
		{
			options.DP=false;
			STMTLEV=0;
			IC=0;
			LINEINFO=0;
			if ((!options.SYSCOMP)||(LEVEL>1))
				FINDFORW(DISPLAY[TOP].FNAME);
			if (options.INMODULE)
				if ((TOS->PREVLEXSTACKP)->DFPROCP==OUTERBLOCK)
					if (SY==SYMBOLS::ENDSY)
					{
						FINISHSEG();
						return;
					}
					else if (SY==SYMBOLS::BEGINSY)
					{
						CERROR(13);
						FINISHSEG();
						return;
					}
			if (SY==SYMBOLS::BEGINSY)
				INSYMBOL();
			else
				CERROR(17);
			do
			{
				BODY(S3, TOS->DFPROCP);
				BFSYFOUND=(SY==TOS->BFSY)||
					(options.INMODULE&&(SY==SYMBOLS::ENDSY));
				if (!BFSYFOUND)
				{
					if (TOS->BFSY==SYMBOLS::SEMICOLON)
						CERROR(14);  /*SEMICOLON EXPECTED*/
					else
						CERROR(6);  /* PERIOD EXPECTED */
					SKIP(FSYS+TOS->BFSY);
					BFSYFOUND=(SY==TOS->BFSY)||
						(options.INMODULE&& (SY==SYMBOLS::ENDSY));
				}
			}
			while (!(BFSYFOUND)||BNF::BLOCKBEGSYS.in(SY));
			if (!BFSYFOUND)
			{
				if (TOS->BFSY==SYMBOLS::SEMICOLON)
					CERROR(14);
				else
					CERROR(6); /*PERIOD EXPECTED*/
				DECLARATIONPART::MAIN(FSYS);
			}
			else //BFSYFOUND
			{
				if (SY==SYMBOLS::SEMICOLON)
					INSYMBOL();
				if (!(SETOFSYS((4),SYMBOLS::BEGINSY,SYMBOLS::PROCSY,
					SYMBOLS::FUNCSY,SYMBOLS::PROGSY).in(SY))&&
				(TOS->BFSY==SYMBOLS::SEMICOLON))
				if (!(options.INMODULE&&(SY==SYMBOLS::ENDSY)))
				{
					CERROR(6);
					SKIP(FSYS);
					DECLARATIONPART::MAIN(FSYS);
				}
				else
					goto fail;
			else
fail:		{
				if (TOS->DFPROCP!=NULL)
					(TOS->DFPROCP)->INSCOPE=false;
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
#if 0
	//NOT USING THE PASCAL HEAP --- YET!
			RELEASE(TOS->DMARKP);
			TOS=TOS->PREVLEXSTACKP;
#endif
			
			NEWBLOCK=SETOFSYS((3),SYMBOLS::PROCSY,SYMBOLS::FUNCSY,
				SYMBOLS::PROGSY).in(SY);
		}
	}
	else
	{
		DECLARATIONS(FSYS);
		if (LEVEL==0)
			if (SETOFSYS((2),SYMBOLS::UNITSY,SYMBOLS::SEPARATSY).in(SY))
			{
				UNITPART(FSYS + SETOFSYS((4),SYMBOLS::UNITSY,
					SYMBOLS::INTERSY,SYMBOLS::IMPLESY,SYMBOLS::ENDSY));
				if (SETOFSYS((3),SYMBOLS::PROCSY,SYMBOLS::FUNCSY,
					SYMBOLS::PROGSY).in(SY))
					DECLARATIONPART::MAIN(FSYS);
			}
	}}
	while (!TOS==NULL);
	FINISHSEG();
}

UINT PASCALCOMPILER::THREAD_ENTRY (LPVOID param)
{
	WRITELN(OUTPUT);
	WRITELN(OUTPUT,"July ",4,", ",1776,"...");
	PASCALCOMPILER	*p = (PASCALCOMPILER*) param;
	try
	{
		p->COMPILER_MAIN(NULL);
	}
	catch (...)
	{
		WRITELN(OUTPUT,"Caught EXCEPTION");
		WRITELN(OUTPUT,"EXIT(PASCALCOMPILER)");
	}
	return 0;
}

int PASCALCOMPILER::COMPILER_MAIN (LPVOID)
{
	/* PASCALCOMPILER */
	char CH;
	int	LGTH = 0;
	TIME(LGTH,LOWTIME);
	SETOFSYS FSYSM = BNF::BLOCKBEGSYS+BNF::STATBEGSYS-SETOFSYS(1,SYMBOLS::CASESY);

	WRITELN(OUTPUT,"PASCALCOMPILER::COMPILER_MAIN");
#if 0
	BNF::BLOCKBEGSYS.debug_list("BLOCKBEGSYS");
	BNF::STATBEGSYS.debug_list("STATBEGSYS");
	FSYSM.debug_list("FSYSM");
#endif

//	BLOCK(BLOCKBEGSYS+STATBEGSYS-SET(1,SYMBOLS::CASESY));
	BLOCK(FSYSM);

	if (SY!=SYMBOLS::PERIOD)
		CERROR(21);
	if (options.LIST)
	{
		SCREENDOTS++;
		(*m_src.SYMBUFP)[m_src.SYMCURSOR]=(char)(EOL);
		CH = GETC();
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
	if (options.NOISY)
		WRITELN(OUTPUT);
	
	::WRITE(OUTPUT,SCREENDOTS," lines");
#if 0
	if (LOWTIME>0)
		WRITE(OUTPUT,", ",(LOWTIME+30)/60," secs, ",
         ROUND((3600.0/LOWTIME)*SCREENDOTS)," lines/min");
#endif	
	if (options.NOISY)
	{
	   WRITELN(OUTPUT);
	   char nbuf[IDENTSIZE];
	   sprintf_s(nbuf,IDENTSIZE,"%d",SMALLESTSPACE);
	   ::WRITE(OUTPUT,"Smallest available space = ",nbuf," words");
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
