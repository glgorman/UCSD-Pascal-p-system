
class _CALL
{
public:
//	void VARIABLE();
//	void STRGVAR();
//	void ROUTINE();
//	void NEWSTMT();
	void MOVE();
	void _EXIT();
	void _UNITIO();
	void CONCAT();
	void COPYDELETE();	
	void STR();
	void _CLOSE();
	void _GETPUTETC();
	void SCAN();
	void BLOCKIO();
	void SIZEOF();
	void LOADIDADDR();
	void READ();
	void WRITE();
	void CALLNONSPECIAL();
};

class BODYPART:
	public virtual PASCALSOURCE,
	public virtual COMPILERDATA,
	public virtual PASCALCOMPILER
{
public:
	static void *allocate (void *);
	void MAIN(SETOFSYS FSYS, CTP FPROCP);
	void BODY1();

private:
//	void INSYMBOL();
//	void CERROR(int);  // suppress compiler errors!
//	void GENBYTE(INT_PTR);

protected:
	void LINKERREF(IDCLASS KLASS, int ID, int ADDR);
	void GENLDC(INT_PTR IVAL);
	void GENBIG(INT_PTR IVAL);
	void GEN0(OPRANGE FOP);
	void GEN1(OPRANGE FOP, int FP2);
	void GEN2(OPRANGE FOP, int P1, int FP2);
	void GENNR(NONRESIDENT EXTPROC);
	void ASSIGN(NONRESIDENT EXTPROC);
	void GENJMP(OPRANGE FOP, LBP FLBP);
	void GENFJP(LBP FLBP);
	void GENLABEL(LBP *FLBP);
	void PUTLABEL(LBP FLBP);
	void LOAD();
	void STORE(ATTR &FATTR);
	void LOADADDRESS();
	void SELECTOR(SETOFSYS FSYS, CTP FCP);

	void CALL(SETOFSYS &, CTP);
		void VARIABLE(SETOFSYS &);
		void STRGVAR(SETOFSYS &, bool);
		void ROUTINE(SETOFSYS &, int);
			void EXIT0();
			void NEWSTMT(SETOFSYS &);
			void MOVE(SETOFSYS &, int);
			void UNITIO(SETOFSYS &, int);
			void CONCAT(SETOFSYS &);
			void COPYDELETE(SETOFSYS &, int);	
			void STR(SETOFSYS &);
			void CLOSE(SETOFSYS &);
			void GETPUTETC(SETOFSYS &, int);
			void SCAN(SETOFSYS &FSYS);
			void BLOCKIO(SETOFSYS &FSYS, int);
			void SIZEOF();
			void LOADIDADDR(CTP);
			void READ(SETOFSYS &, int);
			void WRITE(SETOFSYS &, int);
			void CALLNONSPECIAL(SETOFSYS &,CTP);

	void EXPRESSION(SETOFSYS FSYS);
		void FLOATIT(STP &, bool);
		void STRETCHIT(STP &);
		void SIMPLEEXPRESSION(SETOFSYS &);
		void TERM(SETOFSYS &);
		void FACTOR(SETOFSYS &);
		void MAKEPA(STP &STRGFSP, STP);

	void STATEMENT(SETOFSYS FSYS);
//	called from statement
		void ASSIGNMENT(CTP);
		void GOTOSTATEMENT();
		void COMPOUNDSTATEMENT(SETOFSYS &);
		void IFSTATEMENT(SETOFSYS &);
		void CASESTATEMENT(SETOFSYS &FSYS);
		void REPEATSTATEMENT(SETOFSYS &);
		void WHILESTATEMENT(SETOFSYS &);
		void FORSTATEMENT(SETOFSYS &, CTP &);
		void WITHSTATEMENT(SETOFSYS &);
};

class EXPRESSION: public BODYPART
{
public:
	void FLOATIT();
	void STRETCHIT();
	void SIMPLEEXPRESSION();
	void TERM();
	void FACTOR();
	void MAKEPA();
};

class STATEMENT: public BODYPART
{
public:
	void ASSIGNMENT();
	void GOTOSTATEMENT();
	void COMPOUNDSTATEMENT();
	void IFSTATEMENT();
	void CASESTATEMENT();
	void REPEATSTATEMENT();
	void WHILESTATEMENT();
	void FORSTATEMENT();
	void WITHSTATEMENT();
};