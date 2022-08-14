
class PASCALCOMPILER;

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

class PASCALCOMPILER: public BODYPART
{
friend class COMPILERDATA;
friend class COMPINIT;
friend class DECLARATIONPART;

protected:
	void DECLARATIONS(SETOFSYS FSYS);
	void BODY(const SETOFSYS &FSYS, CTP FPROCP);
	void WRITELINKERINFO(bool);
	void UNITPART(SETOFSYS);
//	void SEARCHSECTION(CTP FCP, CTP &FCP1);
	void SEARCHID(SETOFIDS FIDCLS, CTP &FCP);
	void GETBOUNDS(STP FSP, int &FMIN, int &FMAX);
//	void SKIP(SETOFSYS);
	bool STRGTYPE(STP FSP);
	int DECSIZE(int I);
	void CONSTANT(const SETOFSYS &FSYS, STP FSP, VALU &FVALU);
	bool COMPTYPES(STP FSP1, STP FSP2);
	void GENBYTE(int FBYTE);
	void GENWORD(int FWORD);
	void WRITECODE(bool FORCEBUF);
	void FINISHSEG();
//	void CERROR(int ERRORNUM);

protected:
	void ENTERID(CTP FCP);
	void BLOCK(const SETOFSYS &FSYS);
	void FINDFORW(CTP FCP);
	bool PAOFCHAR(STP FSP);
	
public:
//	void *operator new (size_t,void*);
	static void *allocate (void *);
	PASCALCOMPILER();
	PASCALCOMPILER(INFOREC &);
	void COMPINIT();
	int COMPILER_MAIN (LPVOID param);
	static UINT THREAD_ENTRY (LPVOID param);
};



class DECLARATIONPART::TYP:
	public PASCALCOMPILER
{
public:
class FIELDLIST;

protected:
#if 0
union
{
  struct /*locals*/
  {
	STP			FSP;
	STP			LSP,LSP1,LSP2;
	DISPRANGE	OLDTOP;
	CTP			LCP;
	ADDRRANGE	LSIZE,DISPL;
	int			LMIN,LMAX;
	bool		PACKING;
	BITRANGE	NEXTBIT,NUMBITS;
  };
  char fp[sizeof(stack_frame)];
};
#endif

protected:
	static void debug_stack (stack_frame *ptr);

public:
//	TYP(DECLARATIONPART *);
//	TYP(SETOFSYS, STP&, ADDRRANGE &);
//	void TYP1(const SETOFSYS FSYS, STP &FSP, ADDRRANGE &FSIZE);
//	void ALLOCATE(CTP FCP);
//	void VARIANTLIST();
//	void SIMPLETYPE(SETOFSYS FSYS, STP &FSP, ADDRRANGE &FSIZE);
//	void FIELDLIST(SETOFSYS FSYS, STP &FRECVAR);
//	bool PACKABLE(STP FSP);	
//	void POINTERTYPE();
};
#if 0
class DECLARATIONPART::TYP::FIELDLIST:
	public DECLARATIONPART::TYP
{
protected:
union
{
  struct stack_frame
  {
	CTP LCP,LCP1,NXT,NXT1,LAST;
	STP LSP,LSP1,LSP2,LSP3,LSP4;
	ADDRRANGE MINSIZE,MAXSIZE,LSIZE;
	VALU LVALU1;
	BITRANGE MAXBIT,MINBIT;
	STP	FRECVAR; //passed into constructor 
  } fp;
  struct
  {
	CTP LCP,LCP1,NXT,NXT1,LAST;
	STP LSP,LSP1,LSP2,LSP3,LSP4;
	ADDRRANGE MINSIZE,MAXSIZE,LSIZE;
	VALU LVALU1;
	BITRANGE MAXBIT,MINBIT;
	STP	FRECVAR; //passed into constructor 
  };
};

protected:
	void ALLOCATE(CTP FCP);
	void VARIANTLIST();

public:
	FIELDLIST(SETOFSYS FSYS, STP &FRECVAR);
};
#endif

typedef struct _DCREC_
{
	int DISKADDR;
	int	INTEGERCODELENG;
} DCREC;

class DECLARATIONPART::USESDECLARATION:
	public DECLARATIONPART
{
protected:
	struct _SEGDICT_
	{
		DCREC DANDC[MAXSEG];
		ALPHA SEGNAME[MAXSEG];
		int   SEGKIND[MAXSEG];
		int	  TEXTADDR[MAXSEG];
		int   FILLER[128];
	} SEGDICT;
	bool		FOUND;
	int			BEGADDR;
	CTP			LCP;
	LEXSTKREC	LLEXSTK;
	ALPHA		LNAME;
	OPERATOR	LOP;
	ALPHA		LID;
	bool		MAGIC;

protected:
	USESDECLARATION(const SETOFSYS &, bool MAGIC);
	void GETTEXT(bool &FOUND);
};

#if 0
class DECLARATIONPART::PROCDECLARATION:
	public DECLARATIONPART
{
protected:

	SYMBOLS::SYMBOL	LSY2;
	CTP			LCP,LCP1;
	STP			LSP;
	bool		EXTONLY,FORW;
	ADDRRANGE	LCM;
	LEXSTKREC	LLEXSTK;


protected:
	PROCDECLARATION(const SETOFSYS&,SYMBOLS::SYMBOL FSY, bool SEGDEC, bool &NOTDONE);
	void PARAMETERLIST(SETOFSYS, SYMBOLS::SYMBOL, CTP &, CTP);
};
#endif