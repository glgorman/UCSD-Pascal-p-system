
#include "insymbol.h"

using namespace std;


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
#define DEFSTRGLGTH	80
#define LCAFTERMARKSTACK	1
#define REFSPERBLK	128
#define MAXJTAB		24
#define MAXSEG		15
#define MAXPROCNUM	149

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

typedef enum  _NONRESIDENT_
{
	SEEK,
	FREADREAL,
	FWRITEREAL,
	FREADDEC,
	FWRITEDEC,
	DECOPS,
	MAXNONRESIDENT
} NONRESIDENT;

typedef	int NONRESPFLIST[MAXNONRESIDENT];

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
				BITRANGE	FLDRBIT,FLDWIDTH;
			};
			struct  /*PROC FUNC*/
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
						IDKIND		PFKIND;
						union
						{
							ADDRRANGE	LOCALLC;
							bool		FORWDECL;
							bool		EXTURNAL;
							bool		INSCOPE;
							bool		IMPORTED;
						};
					};
					int			SEGID;//MODULE;
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

//SETOFIDS==SET OF IDCLASS;

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
	short KEY;
	short OFFSET;
};

typedef _REFVALUE_ REFARRAY[REFSPERBLK+1];

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
	WHERE	OCCUR;
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

class COMPILERDATA: protected PASCALSOURCE
{
protected:
/*	PASCALCOMPILER member variables */
	
    ATTR		GATTR;			/*DESCRIBES CURRENT EXPRESSION*/
	DISPRANGE	TOP;			/*TOP) DISPLAY*/
	bool		TEST;
	DISPRANGE	DISX;			/*LEVEL THEN LAST ID SEARCHED*/
	ADDRRANGE	LCMAX;			/*TEMPORARIES LOCATION COUNTER*/

/*SWITCHES:*/

	bool	PRTERR;
    bool	CODEINSEG;
    bool	CLINKERINFO,DLINKERINFO,LSEPPROC;
	bool	SEPPROC;
    bool	STARTINGUP;
	bool	PUBLICPROCS;
	
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
	
	void		*MARKP;			/*for MARKING HEAP*/
    LEXSTKREC	*TOS;			/*TOP OF LEX STACK*/
    DISPRANGE	GLEV;			/*GLOBAL LEVEL IN DISPLAY*/
	bool		NEWBLOCK;		/*INDICATES NEED PUSH LEX STACK*/
    SEGRANGE	NEXTSEG;		/*NEXT SEGMENT #*/
	int			SEGINX;			/*CURRENT INDEX IN SEGMENT*/
	
    int			LOWTIME;
	int			SMALLESTSPACE;
	 
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
                
	JTABRANGE	NEXTJTAB;
	int JTAB[MAXJTAB];

// bodypart
public:
	FILE	REFFILE;
    int NREFS,REFBLK;
	REFARRAY	*REFLIST;

protected:	
	int			CURBYTE;
    char		DISKBUF[512];
};

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

class PASCALCOMPILER: public virtual COMPILERDATA
{
friend class COMPINIT;

protected:
	void COMPINIT();
	void DECLARATIONS(SETOFSYS FSYS);
	void BODYPART(SETOFSYS FSYS, CTP FPROCP);
	void WRITELINKERINFO(bool);
	void UNITPART(SETOFSYS);
	void ENTERID(CTP FCP);
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
	void WRITECODE(bool FORCEBUF);
	void FINISHSEG();
	void BLOCK(SETOFSYS FSYS);
	void FINDFORW(CTP FCP);
	
public:
	PASCALCOMPILER();
	PASCALCOMPILER(INFOREC &);
	int COMPILER_MAIN (LPVOID param);
	static UINT THREAD_ENTRY (LPVOID param);
};
