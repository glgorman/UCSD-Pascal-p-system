
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
	void SEARCHID(const SETOFIDS &FIDCLS, CTP &FCP);
	bool STRGTYPE(STP FSP);
	int DECSIZE(int I);
	void CONSTANT(const SETOFSYS &FSYS, STP &FSP, VALU &FVALU);
	bool COMPTYPES(STP const FSP1, const STP FSP2);
	void GETBOUNDS(STP FSP, int &FMIN, int &FMAX);
	void GENBYTE(int FBYTE);
	void GENWORD(int FWORD);
	void WRITECODE(bool FORCEBUF);
	void FINISHSEG();

protected:
	void ENTERID(CTP FCP);
	void BLOCK(const SETOFSYS &FSYS);
	void FINDFORW(CTP FCP);
	bool PAOFCHAR(STP FSP);
	
public:
	PASCALCOMPILER();
	PASCALCOMPILER(INFOREC &);
	void *operator new (size_t,void*);
	static PASCALCOMPILER *allocate (INFOREC &);
	
	void COMPINIT();
	int COMPILER_MAIN (LPVOID param);
	static UINT THREAD_ENTRY (LPVOID param);
};

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
