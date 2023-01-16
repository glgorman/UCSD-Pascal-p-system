
using namespace std;

#define SEGMENT /**/
#define INTSIZE		1
#define REALSIZE	2
#define BITSPERWD	16
#define CHARSIZE	1
#define BOOLSIZE	1
#define PTRSIZE		1
#define FILESIZE	300
#define NILFILESIZE	40
#define CHRSPERWD	2
#define DEFSTRGLGTH	80
#define LCAFTERMARKSTACK	1
#define REFSPERBLK	128

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

class RECORD
{
protected:
	ADDRRANGE	m_size;
	STRUCTFORM	m_form;	

public:
	union
	{
		struct //SCALAR
		{
			DECLKIND SCALKIND;
			union
			{
				CTP	DECLARED;
				CTP	FCONST;
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
			BITRANGE ELSPERWD;
			BITRANGE ELWIDTH;
			bool AISSTRNG;
			int MAXLENG;
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

class structure: public RECORD
{
protected:
	void *operator new (size_t,void*);
	structure (STRUCTFORM);

public:
	static structure *allocate (STRUCTFORM);
	static structure *allocate (STRUCTFORM,size_t);
	static structure *allocate (STRUCTFORM form, DECLKIND type, size_t sz1);
	
	inline void resize (size_t sz)
	{
		m_size = (int) sz;
	}
	inline size_t size()
	{
		size_t sz = m_size;
		return m_size;
	}
	inline STRUCTFORM form()
	{
		return m_form;
	}
	inline bool is_scalar()
	{
		bool result;
		if (form()==SCALAR)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_long()
	{
		bool result;
		if (form()==LONGINT)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_pointer()
	{
		bool result;
		if (form()==POINTER)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_power()
	{
		bool result;
		if (form()==POWER)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_file()
	{
		bool result;
		if (form()==FILES)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_packed()
	{
		return AISPACKD; 
	}
	inline void set_packed(bool val)
	{
		AISPACKD=val;
	}
	inline void set_string(bool val)
	{
		AISSTRNG=val;
	}
	inline bool is_string()
	{
		return AISSTRNG;
	}	
};

typedef enum _IDKIND
{
	ACTUAL,
	FORMAL,
} IDKIND;

class pascal_data
{
public:
	bool FISPACKED;
	DECLKIND PFDECKIND;
	IDKIND PFKIND;
	union/*DECLKIND */
	{
		struct/*KONST*/
		{
			VALU VALUES;
		};
		struct/*FORMALVARS, ACTUALVARS*/
		{
			LEVRANGE VLEV;
			ADDRRANGE VADDR;
			bool PUBLIC;
		};
		struct/*FIELD*/
		{
			ADDRRANGE FLDADDR;
//			bool FISPACKED;
			BITRANGE FLDRBIT,FLDWIDTH;
		};
		struct/*PROC FUNC*/
		{
//			DECLKIND PFDECKIND;
			union
			{
				struct/*SPECIAL*/
				{
					int KEY;
				};
				struct/*STANDARD*/
				{
					int CSPNUM;
				};
				struct/*DECLARED*/
				{ 
					LEVRANGE PFLEV;
					PROCRANGE PFNAME;
					SEGRANGE PFSEG;
//					IDKIND PFKIND;
					union
					{
						ADDRRANGE LOCALLC;
						bool FORWDECL;
						bool EXTURNAL;
						bool INSCOPE;
						bool IMPORTED;
					};
				};
				int SEGID;//MODULE;
			};
		};
	};
};

class identifier: 
	public pascal_data,
	public bNodeType<identifier>
{
public:
	ALPHA NAME;
	IDCLASS KLASS;
	STP IDTYPE;
	CTP NEXT;
	
protected:
	void *operator new (size_t,void*);
	identifier();
	
public:
	static identifier *allocate ();
	static identifier *allocate (IDCLASS);
	static identifier *allocate (char*,STP,IDCLASS);
	static void debug1 (identifier *stp, bool);

public:
	CTP LLINK()
	{
		return static_cast<CTP>(branch1);
	}
	CTP RLINK()
	{
		return static_cast<CTP>(branch2);
	}
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
	EXPR1,
} VACCESS;

//SETOFIDS==SET OF IDCLASS;

class ATTR
{
private:
	VACCESS access;

public:
	STP TYPTR;
	ATTRKIND KIND;
	VALU CVAL;
	LEVRANGE VLEVEL;
	ADDRRANGE DPLMT;
	ADDRRANGE IDPLMT;
	
	ATTR()
	{
		memset(this,0,sizeof(ATTR));
	}
	void set_access(VACCESS a)
	{
		access = a; 
	}
	VACCESS ACCESS()
	{
		VACCESS result = access;
		return result;
	}
};

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
	bool DEFINED;
	union
	{
		ADDRRANGE REFLIST;	// false
		struct				// true
		{
			ADDRRANGE OCCURIC;
			JTABRANGE JTABINX;
		};
	};  
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
		struct // _VREC;
		{
			ADDRRANGE	VDSPL;
		} VREC;
	};				
};

struct LEXSTKREC
{
	DISPRANGE	DOLDTOP;
	int			DOLDLEV; // 0..MAXLEVEL;
	PROCRANGE	POLDPROC,SOLDPROC;
	SEGRANGE	DOLDSEG;
	ADDRRANGE	DLLC;
    SYMBOLS::SYMBOL		BFSY;
    CTP			DFPROCP;
	void		*DMARKP;
	bool		ISSEGMENT;
	LEXSTKREC	*PREVLEXSTACKP;
};

class BNF
{
public:
	SETOFSYS	CONSTBEGSYS;
	SETOFSYS	SIMPTYPEBEGSYS;
	SETOFSYS	TYPEBEGSYS;
    SETOFSYS	BLOCKBEGSYS;
	SETOFSYS	SELECTSYS;
	SETOFSYS	FACBEGSYS;
	SETOFSYS	STATBEGSYS;
	SETOFSYS	TYPEDELS;
    SETOFIDS	VARS;
};

class structures
{
friend class structure;
public:
	static void debug1 (structure *stp);

protected:
	static bool	m_bTracing;
	structures ()
	{
		m_bTracing = false;
	}
	void tron ()
	{
		m_bTracing = true;
	}
	void troff ()
	{
		m_bTracing = false;
	}

protected:
	STP	INTPTR;
	STP REALPTR;
	STP LONGINTPTR;
	STP CHARPTR;
	STP BOOLPTR;
	STP TEXTPTR;
	STP NILPTR;
	STP INTRACTVPTR;
	STP STRGPTR;
};

class identifiers
{
friend class identifier;

protected:
	static bool	m_bTracing;
	identifiers ()
	{
		m_bTracing = false;
	}
	void tron ()
	{
		m_bTracing = true;
	}
	void troff ()
	{
		m_bTracing = false;
	}
	void symbol_dump ()
	{
//		treesearch::printtree("identifiers::UTYPPTR",UTYPPTR,false);
//		treesearch::printtree("identifiers::UCSTPTR",UCSTPTR,false);
//		treesearch::printtree("identifiers::UVARPTR",UVARPTR,false);
//		treesearch::printtree("identifiers::UFLDPTR",UFLDPTR,false);
//		treesearch::printtree("identifiers::MODPTR",MODPTR,false);
//		treesearch::printtree("identifiers::INPUTPTR",INPUTPTR,false);
//		treesearch::printtree("identifiers::OUTPUTPTR",OUTPUTPTR,false);
//		treesearch::printtree("identifiers::OUTERBLOCK",OUTERBLOCK,false);
//		treesearch::printtree("identifiers::FWPTR",FWPTR,false);
//		treesearch::printtree("identifiers::USINGLIST",USINGLIST,false);
	}

protected:
	CTP UTYPPTR;
	CTP UCSTPTR;
	CTP UVARPTR;
	CTP	UFLDPTR;
	CTP UPRCPTR;
	CTP UFCTPTR;       
	CTP	MODPTR;
	CTP INPUTPTR;
	CTP OUTPUTPTR;
	CTP	OUTERBLOCK;
	CTP	FWPTR;
	CTP	USINGLIST;
};

class COMPILERDATA:
	public PASCALSOURCE,
	public BNF,
	public structures,
	public identifiers
{
public:
	static void *allocate(void*);
	ATTR		GATTR;			/*DESCRIBES CURRENT EXPRESSION*/
	DISPRANGE	TOP;			/*TOP OF DISPLAY*/
	DISPRANGE	DISX;			/*LEVEL THEN LAST ID SEARCHED*/
	ADDRRANGE	LCMAX;			/*TEMPORARIES LOCATION COUNTER*/

protected:
/*	PASCALCOMPILER member variables */
/*SWITCHES:*/

	bool	TEST;
	bool	PRTERR;
    bool	CODEINSEG;
    bool	CLINKERINFO,DLINKERINFO,LSEPPROC;
	bool	SEPPROC;
    bool	STARTINGUP;
	bool	PUBLICPROCS;
	
	TESTP		GLOBTESTP;		/*LAST TESTPOINTER*/
	
	void		*MARKP;			/*for MARKING HEAP*/
    LEXSTKREC	*TOS;			/*TOP OF LEX STACK*/
    DISPRANGE	GLEV;			/*GLOBAL LEVEL IN DISPLAY*/
	bool		NEWBLOCK;		/*INDICATES NEED PUSH LEX STACK*/
    SEGRANGE	NEXTSEG;		/*NEXT SEGMENT #*/
	int			SEGINX;			/*CURRENT INDEX IN SEGMENT*/
	
    int			LOWTIME;
	int			SMALLESTSPACE;

	DISPLAYDATA	DISPLAY[DISPLIMIT+1];
	NONRESPFLIST	PFNUMOF;
	int PROCTABLE[MAXPROCNUM+1];
	SEGDATA	SEGTABLE[MAXSEG+1];
                
	JTABRANGE	NEXTJTAB;
	int JTAB[MAXJTAB];

// bodypart
public:
	pascal_file	*REFFILE;
    int NREFS,REFBLK;
	REFARRAY	*REFLIST;

protected:	
	int				CURBYTE;
    unsigned char	DISKBUF[512];

// declared in INSYMBOL, but called from
// declaration part, etc.  Needs access to
// COMPILERDATA for debugging
	void CERROR(int ERRORNUM);

protected:
	// declared in PASCALCOMPILER, but called from
	// declaration part, etc.
	virtual void ENTERID(CTP FCP) = 0;
	virtual void SEARCHID(const SETOFIDS &FIDCLS, CTP &FCP) = 0;
	void SEARCHSECTION(CTP FCP, CTP &FCP1);

protected: // fuctions called from DECLARATIONPART
//	virtual void CERROR(int ERRORNUM) = 0;
	virtual void CONSTANT(const SETOFSYS &FSYS, STP &FSP, VALU &FVALU) = 0;
	virtual void BLOCK(const SETOFSYS &FSYS) = 0;
	virtual void GETBOUNDS(STP FSP, int &FMIN, int &FMAX) = 0;
	virtual bool STRGTYPE(STP FSP) = 0;
	virtual int DECSIZE(int I) = 0;
	virtual bool COMPTYPES(const STP FSP1, const STP FSP2) = 0;

protected:	// functions called from BODYPART
	virtual void GENBYTE(int FBYTE) = 0;
	virtual void GENWORD(int FWORD) = 0;
	virtual void WRITECODE(bool FORCEBUF) = 0;
	virtual bool PAOFCHAR(STP FSP) = 0;
};
