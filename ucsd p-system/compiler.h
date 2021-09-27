
#include "stdafx.h"
#include "intrinsics.h"
#include "range.h"
#include "sets.h"

#define SEGMENT /**/
#define	DISPLIMIT	12
#define MAXLEVEL	8
#define AXADDR		28000
#define INTSIZE		1
#define REALSIZE	2
#define BITSPERWD	16
#define CHARSIZE	1
#define BOOLSIZE	1
#define PTRSIZE		1
#define FILESIZE	300
#define NILFILESIZE	40
#define BITSPERCHR	8	
#define CHRSPERWD	2
#define STRINGSIZE	0
#define STRGLGTH	255
#define MAXINT		32767
#define MAXDEC		36
#define DEFSTRGLGTH	80
#define LCAFTERMARKSTACK	1
#define REFSPERBLK	128
#define EOL			13
#define MAXCURSOR	1023
#define MAXCODE		1299
#define MAXJTAB		24
#define MAXSEG		15
#define MAXPROCNUM	149
#define MAXDIRLEN	40

class INFOREC
{
public:
	FILE *WORKSYM, *WORKCODE;
	int ERRSYM,ERRBLK,ERRNUM;
	bool SLOWTERM,STUPID;
	char ALTMODE;
};

class USERPROGRAM
{
public:
	void FILEHANDLER();
	void DEBUGGER();
	void PRINTERROR();
	void INITIALIZE();
	void GETCMD();
	void NOTUSED1();
	void NOTUSED2();
	void NOTUSED3();
	USERPROGRAM();
};

typedef enum _SYMBOL
{
	IDENT,COMMA,COLON,SEMICOLON,LPARENT,RPARENT,DOSY,TOSY,
	DOWNTOSY,ENDSY,UNTILSY,OFSY,THENSY,ELSESY,BECOMES,LBRACK,
	RBRACK,ARROW,PERIOD,BEGINSY,IFSY,CASESY,REPEATSY,WHILESY,
	FORSY,WITHSY,gotoSY,LABELSY,CONSTSY,TYPESY,VARSY,PROCSY,
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

/* set of symbols */


typedef enum  NONRESIDENT
{
	SEEK,FREADREAL,FWRITEREAL,FREADDEC,FWRITEDEC,DECOPS,MAXNONRESIDENT
};

typedef	int NONRESPFLIST[MAXNONRESIDENT];

typedef enum _CSTCLASS
{
	REEL,
	PSET,
	STRG,
	TRIX,
	LONG1,
	MAXCST,
} CSTCLASS;

struct CONSTREC;
typedef CONSTREC*	CSP;
struct CONSTREC
{
	CSTCLASS	CCLASS;
	union
	{
		int		LONG;
		int		*LONGVAL;
//		int		TRIX[8];
		float	REEL;
		float	RVAL;
//				PSET;
		char	SVAL[STRGLGTH];
	};
};

typedef union
{
	int IVAL;
	CSP	VALP;
} VALU;

#if 0
BITRANGE==0..BITSPERWD;
OPRANGE==0..80;
CURSRANGE==0..MAXCURSOR;
PROCRANGE==0..MAXPROCNUM;
LEVRANGE==0..MAXLEVEL;
ADDRRANGE==0..MAXADDR;
JTABRANGE==0..MAXJTAB;
SEGRANGE==0..MAXSEG;
DISPRANGE==0..DISPLIMIT;
#endif


typedef enum  _STRUCTFORM
{
	SCALAR,
	SUBRANGE,
	POINTER,
	LONGINT,
	POWER,
	ARRAYS,
	RECORDS,
	FILES,
	TAGFLD,
	VARIANT2
} STRUCTFORM;

typedef enum  _DECLKIND
{
	STANDARD,
	DECLARED,
	SPECIAL,
} DECLKIND;

typedef	DWORD BITRANGE;
typedef char ALPHA[16];
struct IDENTIFIER;
typedef IDENTIFIER* CTP;

struct STRUCTURE;
typedef STRUCTURE*	STP;
struct STRUCTURE
{
	ADDRRANGE	SIZE;
	STRUCTFORM	FORM;
	union
	{
		union	//SCALAR
		{
			struct 
			{
				DECLKIND SCALKIND;
				union
				{
					CTP	DECLARED;
					CTP	FCONST;
				};
			};
		};
		struct	//SUBRANGE
		{
			STP		RANGETYPE;
			VALU	MIN, MAX;
		};
		struct	//POINTER
		{
			STP	ELTYPE;	
		};
		struct	//POWER
		{
			STP	ELSET;	
		};
		struct //ARRARYS
		{
			STP	AELTYPE;
			STP INXTYPE;
			bool AISPACKD;
			struct
			{
				BITRANGE ELSPERWD;
				BITRANGE ELWIDTH;
				bool AISSTRNG;
				int MAXLENG;
			};
		};
		struct	//RECORDS
		{
			CTP	FSTFLD;
			STP	RECVAR;
		};
		struct	//FILES
		{
			STP	FILTYPE;
		};
		struct	//TAGFLD
		{
			CTP	TAGFIELDP;
			STP	FSTVAR;
		};
		struct //VARIANT
		{
			STP	NXTVAR;
			STP	SUBVAR;
			VALU VARVAL;
		};
	};
};

/*NAMES*/
typedef enum _IDCLASS0
{
	TYPES,
	KONST,
	FORMALVARS,
	ACTUALVARS,
	FIELD,
	PROC1,
	FUNC,
	MODULE,
} IDCLASS;

typedef enum _IDKIND
{
	ACTUAL,
	FORMAL,
} IDKIND;

struct IDENTIFIER
{
	ALPHA		NAME;
	CTP			LLINK;
	CTP			RLINK;
	STP			IDTYPE;
	CTP			NEXT;
	struct
	{
		IDCLASS		KLASS;
		union /*DECLKIND */  
		{
			struct	/*KONST*/
			{
				VALU	VALUES;	
			};
			struct /*FORMALVARS, ACTUALVARS*/
			{
				LEVRANGE	VLEV;
				ADDRRANGE	VADDR;
				bool		PUBLIC;
			};
			struct /*FIELD*/
			{
				ADDRRANGE	FLDADDR;
				bool		FISPACKED;
			};
			struct
			{
				DECLKIND	PFDECKIND;
				union
				{
					struct	/*SPECIAL*/
					{
						int	KEY;	
					};
					struct	/*STANDARD*/
					{
						int	CSPNUM;	
					};
					struct	/*DECLARED*/
					{
						LEVRANGE	PFLEV;
						PROCRANGE	PFNAME;
						SEGRANGE	PFSEG;
						union
						{
							ADDRRANGE	LOCALLC;
							IDKIND		PFKIND;
							bool		FORWDECL;
							bool		EXTURNAL;
							bool		INSCOPE;
						};
					};
					int			MODULE;
				};
			};
		};};
};

typedef enum _WHERE
{
	BLCK,
	CREC,
	VREC,
	REC
} WHERE;

typedef enum _ATTRKIND
{
	CST,
	VARBL,
	EXPR
} ATTRKIND;

typedef enum _VACCESS
{
	DRCT,
	INDRCT,
	PACKD,
	MULTI,
	BYTE1,
} VACCESS;

//SETOFIDS==SET) IDCLASS;

typedef union _ATTR //(TYPTR: STP)
{
  union //(KIND: ATTRKIND)
  {
//	CST:   (CVAL: VALU);
//	VARBL: (union (ACCESS: VACCESS)
//	DRCT:   (VLEVEL: LEVRANGE; DPLMT: ADDRRANGE);
//	INDRCT: (IDPLMT: ADDRRANGE))
	VALU	CST;
	union // (ACCESS: VACCESS)
	{
//	:   (VLEVEL: LEVRANGE; DPLMT: ADDRRANGE);
//	INDRCT: (IDPLMT: ADDRRANGE))
		struct _DRCT
		{
			LEVRANGE VLEVEL;
			ADDRRANGE DPLMT;
		} DRCT;
		ADDRRANGE	INDRCT;
	};
  };
} ATTR;

struct TESTPOINTER;
typedef TESTPOINTER* TESTP;
struct TESTPOINTER
{
	STP ELT1;
	STP	ELT2;
	TESTP LASTTESTP;
};

struct CODELABEL;
typedef CODELABEL*  LBP;
struct CODELABEL
{
	union
	{
	ADDRRANGE REFLIST;
	ADDRRANGE OCCURIC;
	};  
	JTABRANGE JTABINX;
};

struct USERLABEL;
typedef USERLABEL* LABELP;
struct USERLABEL
{
	int		LABVAL;
	LABELP	NEXTLAB;
	LBP		CODELBP;
};

struct _REFVALUE_
{
	int KEY;
	int OFFSET;
};
typedef _REFVALUE_ REFARRAY[REFSPERBLK+1];

typedef char CODEARRAY[MAXCODE+1];
typedef char SYMBUFARRAY[MAXCURSOR+1];
typedef enum _UNITFILE
{
	WORKCODE,
	SYSLIBRARY,
} UNITFILE;

struct SEGDATA
{
	int	DISKADDR;
	int	CODELENGTH;
	ALPHA	SEGNAME;
    int SEGKIND;
    int TEXTADDR;
};

struct DISPLAYDATA
{
	CTP	FNAME;
	union //(OCCUR: WHERE)
	{
		struct //_BLCK
		{
			CTP			FFILE;
			LABELP		FLABEL;
		} BLCK;
		struct //_CREC
		{
			LEVRANGE	CLEV;
			ADDRRANGE	CDSPL;
		} CREC;
		ADDRRANGE	VREC;
		ADDRRANGE	VDSPL;
	};				
};

struct LEXSTKREC
{
	DISPRANGE	DOLDTOP;
	int			DOLDLEV; // 0..MAXLEVEL;
	PROCRANGE	POLDPROC,SOLDPROC;
	SEGRANGE	DOLDSEG;
	ADDRRANGE	DLLC;
    SYMBOL		BFSY;
    CTP			DFPROCP;
	void		*DMARKP;
	bool		ISSEGMENT;
	LEXSTKREC	*PREVLEXSTACKP;
};

class PASCALCOMPILER;
void WRITELINKERINFO(bool DECSTUFF);

class COMPINIT
{
protected:
	PASCALCOMPILER	*m_ptr;

protected:
	void ENTSTDTYPES();
	void ENTSTDNAMES();
	void ENTUNDECL();
	void ENTSPCPROCS();
	void ENTSTDPROCS();
	void INITSCALARS();
	void INITSETS();

public:
	void INIT(PASCALCOMPILER*);
};

class PASCALCOMPILER
{
friend class COMPINIT;

protected:
	/*	PASCALCOMPILER member variables */

	INFOREC		USERINFO;
	CODEARRAY	*CODEP;			/*CODE BUFFER while (! WRITEOUT*/
	SYMBUFARRAY	*SYMBUFP;		/*SYMBOLIC BUFFER...ASCII||CODED*/
    ATTR		GATTR;			/*DESCRIBES CURRENT EXPRESSION*/
	DISPRANGE	TOP;			/*TOP) DISPLAY*/
    ADDRRANGE	LC,IC;			/*LOCATION&& INSTRUCT COUNTERS*/
	bool		TEST;
    SEGRANGE	SEG;			/*CURRENT SEGMENT NO.*/

/*SCANNER GLOBALS...NEXT FOUR VARS*/
/*MUST BE IN THIS ORDER for (IDSEARCH*/
	
	CURSRANGE	SYMCURSOR;		/*CURRENT SCANNING INDEX IN SYMBUFP^*/
	SYMBOL		SY;				/*SYMBOL FOUND BY INSYMBOL*/
	OPERATOR	OP;				/*CLASSif (ICATION) LAST SYMBOL*/
	ALPHA		ID;				/*LAST IDENTif (IER FOUND*/			
    VALU		VAL;			/*VALUE THEN LAST CONSTANT*/
	DISPRANGE	DISX;			/*LEVEL THEN LAST ID SEARCHED*/
	ADDRRANGE	LCMAX;			/*TEMPORARIES LOCATION COUNTER*/

/*SWITCHES:*/

	bool	PRTERR,gotoOK,RANGECHECK,DEBUGGING;
    bool	NOISY,CODEINSEG,IOCHECK,BPTONLINE;
    bool	CLINKERINFO,DLINKERINFO,LIST,TINY,LSEPPROC;
    bool	DP,INCLUDING,USING,NOSWAP,SEPPROC;
    bool	STARTINGUP,INMODULE,ININTERFACE;
	bool	LIBNOTOPEN,SYSCOMP,PUBLICPROCS,GETSTMTLEV;

	/*POINTERS:*/
	STP	INTPTR,REALPTR,LONGINTPTR,
		CHARPTR,BOOLPTR,TEXTPTR,
		NILPTR,INTRACTVPTR,STRGPTR;	/*POINTERS; /*FIXME STANDARD IDS*/

	CTP UTYPPTR,UCSTPTR,UVARPTR,
	UFLDPTR,UPRCPTR,UFCTPTR,        /*POINTERS; /*FIXME* UNDECLARED IDS*/
	MODPTR,INPUTPTR,OUTPUTPTR;
	
	CTP			OUTERBLOCK;
	CTP			FWPTR;
	CTP			USINGLIST;
	TESTP		GLOBTESTP;		/*LAST TESTPOINTER*/
	LEVRANGE	LEVEL;		/*CURRENT STATIC LEVEL*/
	int			BEGSTMTLEV;
	int			STMTLEV;		/*CURRENT STATEMENT NESTING LEVEL*/
	void		*MARKP;			/*for (MARKING HEAP*/
    LEXSTKREC	*TOS;			/*TOP) LEX STACK*/
    DISPRANGE	GLEV;			/*GLOBAL LEVEL) DISPLAY*/
	bool		NEWBLOCK;		/*INDICATES NEED; /*FIXME*//* PUSH LEX STACK*/
    SEGRANGE	NEXTSEG;		/*NEXT SEGMENT #*/
	int			SEGINX;			/*CURRENT INDEX IN SEGMENT*/
	CSP			SCONST;			/*INSYMBOL STRING RESULTS*/
    int			LOWTIME;
	int			LINEINFO;
	int			SCREENDOTS;
	int			STARTDOTS;
	int			SYMBLK;
	int			SMALLESTSPACE;
	CURSRANGE	LINESTART; 
	PROCRANGE	CURPROC;	/*void NUMBER ASSIGNMENT*/
	PROCRANGE	NEXTPROC;
	SETOFSYS	CONSTBEGSYS;
	SETOFSYS	SIMPTYPEBEGSYS;
	SETOFSYS	TYPEBEGSYS;
    SETOFSYS	BLOCKBEGSYS;
	SETOFSYS	SELECTSYS;
	SETOFSYS	FACBEGSYS;
	SETOFSYS	STATBEGSYS;
	SETOFSYS	TYPEDELS;
    SETOFIDS	VARS;

	DISPLAYDATA	DISPLAY[DISPLIMIT+1];
	NONRESPFLIST	PFNUMOF;
	int PROCTABLE[MAXPROCNUM+1];
	SEGDATA	SEGTABLE[MAXSEG+1];
                 
    char		*COMMENT;
	char		SYSTEMLIB[MAXDIRLEN];
	JTABRANGE	NEXTJTAB;
	int JTAB[MAXJTAB];

	FILE	REFFILE;
    int NREFS,REFBLK;
	REFARRAY	*REFLIST;
    int			OLDSYMBLK;
	int			PREVSYMBLK;
    CURSRANGE	OLDSYMCURSOR;
    CURSRANGE	OLDLINESTART;
	CURSRANGE	PREVSYMCURSOR;
	CURSRANGE	PREVLINESTART;
	UNITFILE	USEFILE;
	FILE		INCLFILE,LIBRARY;
    FILE		_LP;
	int			CURBYTE, CURBLK;
    char		DISKBUF[512];

protected:
	void COMPINIT();
	void DECLARATIONPART(SETOFSYS FSYS);
	void BODYPART(SETOFSYS FSYS, CTP FPROCP);
	void WRITELINKERINFO(bool);
	void UNITPART(SETOFSYS);
	void GETNEXTPAGE();
	void PRINTLINE();
	void ENTERID(CTP FCP);
	void INSYMBOL();
	void SEARCHSECTION(CTP FCP, CTP &FCP1);
	void SEARCHID(SETOFIDS FIDCLS, CTP &FCP);
	void GETBOUNDS(STP FSP, int &FMIN, int &FMAX);
	void SKIP(SETOFSYS);
	bool PAOFCHAR(STP FSP);
	bool STRGTYPE(STP FSP);
	int DECSIZE(int I);
	void CONSTANT(SETOFSYS FSYS, STP FSP, VALU FVALU);
	bool COMPTYPES(STP FSP1, STP FSP2);
	void GENBYTE(int FBYTE);
	void GENWORD(int FWORD);
	void WRITETEXT();
	void WRITECODE(bool FORCEBUF);
	void FINISHSEG();
	void BLOCK(SETOFSYS FSYS);
	void FINDFORW(CTP FCP);
	
public:
	PASCALCOMPILER(INFOREC &);
	void COMPILER_MAIN ();
	void CERROR(int ERRORNUM);
};