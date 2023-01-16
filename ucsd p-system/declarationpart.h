
#if 0
class DECLARATIONS{
protected:
	SYMBOLS::SYMBOL	LSY0;
	bool	NOTDONE;
	int		*DUMMYVAR; 
};
#endif

struct stack_frame
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

class DECLARATIONPART:
	virtual public COMPILERDATA
{
public:
class USESDECLARATION;

private:
	void debug_stack (char *src, stack_frame *ptr);
	void TYP1(const SETOFSYS &FSYS, STP &FSP, ADDRRANGE &FSIZE);
		void ARRAY (const SETOFSYS &FSYS, stack_frame *param);
		bool PACKABLE(stack_frame *param, STP FSP);
		void FIELDLIST(stack_frame *param, const SETOFSYS &FSYS, STP &FRECVAR);
		void ALLOCATE(stack_frame *paarm, CTP FCP);
		void VARIANTLIST(stack_frame *param, const SETOFSYS &FSYS, STP &FRECVAR);
		void USESDECLARATION(bool MAGIC);
		void POINTERTYPE(stack_frame *param);
		void SIMPLETYPE(const SETOFSYS &FSYS, STP &FSP, ADDRRANGE &FSIZE);
		void DYYVARIANTLIST(stack_frame *param);

	void LABELDECLARATION(const SETOFSYS &FSYS);
	void CONSTDECLARATION(const SETOFSYS &FSYS);
	void TYPEDECLARATION(const SETOFSYS &FSYS);
	void VARDECLARATION(const SETOFSYS &FSYS);
	void PROCDECLARATION(const SETOFSYS &,SYMBOLS::SYMBOL,bool,bool&);
	void PARAMETERLIST(const SETOFSYS &, SYMBOLS::SYMBOL &, CTP &, CTP, bool&, bool);

public:
	DECLARATIONPART ();
	DECLARATIONPART (PASCALCOMPILER *);
	virtual ~DECLARATIONPART ();
	void MAIN (const SETOFSYS &FSYS);
};

