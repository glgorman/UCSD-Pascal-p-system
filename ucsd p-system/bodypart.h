
class BODYPART:
	public DECLARATIONPART
{
public:
	void MAIN(const SETOFSYS &FSYS, CTP FPROCP);

protected:
	static void *allocate (void *);

private:
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
	void SELECTOR(const SETOFSYS &FSYS, CTP FCP);

	void CALL(const SETOFSYS &, CTP);
//	nested procedures within call
		void VARIABLE(const SETOFSYS &);
		void STRGVAR(const SETOFSYS &, bool);
		void ROUTINE(const SETOFSYS &, int);
			void NEWSTMT(const SETOFSYS &);
		void MOVE(const SETOFSYS &, int);
		void EXIT0();
		void UNITIO(const SETOFSYS &, int);
		void CONCAT(const SETOFSYS &);
		void COPYDELETE(const SETOFSYS &, int);	
		void STR(const SETOFSYS &);
		void CLOSE(const SETOFSYS &);
		void GETPUTETC(const SETOFSYS &, int);
		void SCAN(const SETOFSYS &FSYS);
		void BLOCKIO(const SETOFSYS &FSYS, int);
		void SIZEOF();
	
	void LOADIDADDR(CTP);
	void READ(const SETOFSYS &, int, bool param);
	void WRITE(const SETOFSYS &, int, bool params);
	void CALLNONSPECIAL(const SETOFSYS &,CTP);

	void EXPRESSION(const SETOFSYS &);
//	nested procedures within expression
		void FLOATIT(STP &, bool);
		void STRETCHIT(STP &);
		void SIMPLEEXPRESSION(const SETOFSYS &);
		void TERM(const SETOFSYS &);
		void FACTOR(const SETOFSYS &);
		void MAKEPA(STP &STRGFSP, STP);

	void STATEMENT(const SETOFSYS &FSYS);
//	called from statement
		void LABEL();
		void ASSIGNMENT(const SETOFSYS &, CTP);
		void GOTOSTATEMENT();
		void COMPOUNDSTATEMENT(const SETOFSYS &);
		void IFSTATEMENT(const SETOFSYS &);
		void CASESTATEMENT(const SETOFSYS &FSYS);
		void REPEATSTATEMENT(const SETOFSYS &);
		void WHILESTATEMENT(const SETOFSYS &);
		void FORSTATEMENT(const SETOFSYS &, CTP &);
		void WITHSTATEMENT(const SETOFSYS &);
};

#if 0
class _CALL
{
protected:
	void VARIABLE(const SETOFSYS &);
	void STRGVAR(const SETOFSYS &, bool);
	void ROUTINE(const SETOFSYS &, int);
		void NEWSTMT(const SETOFSYS &);
	void MOVE(const SETOFSYS &, int);
	void EXIT0();
	void UNITIO(const SETOFSYS &, int);
	void CONCAT(const SETOFSYS &);
	void COPYDELETE(const SETOFSYS &, int);	
	void STR(const SETOFSYS &);
	void CLOSE(const SETOFSYS &);
	void GETPUTETC(const SETOFSYS &, int);
	void SCAN(const SETOFSYS &FSYS);
	void BLOCKIO(const SETOFSYS &FSYS, int);
	void SIZEOF();
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
#endif