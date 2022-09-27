
#include "stdafx.h"
#include "new.h"
#include <memory>
#include <vector>

#include "../Frame Lisp/intrinsics.h"
#include "../Frame Lisp/range.h"
#include "../Frame Lisp/sets.h"

using namespace std;

#define IDENTSIZE	63
#define STRGLGTH	255
#define MAXDIRLEN	40
#define MAXINT		32767
#define MAXDEC		36
#define EOL			13
#define MAXCURSOR	8191
#define MAXCODE		2047

extern char *SYMBOL_NAMES1[];
extern char *SYMBOL_NAMES2[];
extern char *ID_NAMES1[];

namespace pascal0
{
struct key_info;
extern key_info operators[];
};

typedef	DWORD BITRANGE;
class identifier;
typedef identifier* CTP;
class structure;
typedef structure*	STP;
typedef char ALPHA[IDENTSIZE+1];
typedef char SYMBUFARRAY[MAXCURSOR+256];
typedef unsigned char CODEARRAY[MAXCODE+1];

int TREESEARCH(const CTP& n1, CTP& n2, ALPHA &str);
void PRINTTREE(const CTP &n1);
void PRINTTREE1(const CTP &n1);

namespace SYMBOLS
{
typedef enum _SYMBOL
{
	IDENT,COMMA,COLON,SEMICOLON,LPARENT,RPARENT,DOSY,TOSY,
	DOWNTOSY,ENDSY,UNTILSY,OFSY,THENSY,ELSESY,BECOMES,LBRACK,
	RBRACK,ARROW,PERIOD,BEGINSY,IFSY,CASESY,REPEATSY,WHILESY,
	FORSY,WITHSY,GOTOSY,LABELSY,CONSTSY,TYPESY,VARSY,PROCSY,
	FUNCSY,PROGSY,FORWARDSY,INTCONST,REALCONST,STRINGCONST,
	NOTSY,MULOP,ADDOP,RELOP,SETSY,PACKEDSY,ARRAYSY,RECORDSY,
	FILESY,OTHERSY,LONGCONST,USESSY,UNITSY,INTERSY,IMPLESY,
	EXTERNLSY,SEPARATSY,COMMENTSY,MAXSYMBOL
}	SYMBOL;
};

typedef enum _OPERATOR
{
	MUL,RDIV,ANDOP,IDIV,IMOD,PLUS,MINUS,OROP,LTOP,LEOP,
	GEOP,GTOP,NEOP,EQOP,INOP,NOOP,MAXOPERATOR,
} OPERATOR;

typedef enum _CSTCLASS
{
	VOID2,
	REEL,
	PSET,
	STRG,
	TRIX,
	LONG1,
	MAXCST,
} CSTCLASS;

#if 0
typedef enum _UNITFILE
{
	WORKCODE,
	SYSLIBRARY,
} UNITFILE;
#endif

class CONSTREC;
typedef CONSTREC*	CSP;
class CONSTREC
{
public:
	CONSTREC (CSTCLASS);

private:
	CONSTREC();

public:
	CSTCLASS	CCLASS;
	int	 LLENG,LLAST;
	union
	{
		_SET_<128>	PVAL;
		char	SLGTH;
		int		LONG;
		int		LONGVAL[10];
		int		CSTVAL[8];
		float	REEL;
		float	RVAL;
		char	SVAL[STRGLGTH];
	};
};

typedef union
{
	int IVAL;
	CSP	VALP;
} VALU;

class PSYMBOL
{
public:
	int index;
	SYMBOLS::SYMBOL	SY;	/*SYMBOL FOUND BY INSYMBOL*/
	OPERATOR	OP;		/*CLASSIFICATION LAST SYMBOL*/
	ALPHA		ID;		/*LAST identifier FOUND*/			
    VALU		VAL;	/*VALUE THEN LAST CONSTANT*/
	char		*str;

	PSYMBOL();
	PSYMBOL &operator = (int);
};

class PASCALCOMPILER;
void WRITELINKERINFO(bool DECSTUFF);

namespace pascal0
{
struct key_info
{
	ALPHA		ID;
	SYMBOLS::SYMBOL		SY;
	OPERATOR	OP;
	key_info() { };
	key_info(char *STR, SYMBOLS::SYMBOL _SY, OPERATOR _OP)
	{
		strcpy_s(ID,IDENTSIZE,STR);
		SY = _SY;
		OP = _OP;
	}
};
};

class INFOREC
{
public:
	pascal_file *WORKSYM, *WORKCODE;
	int ERRSYM,ERRBLK,ERRNUM;
	bool SLOWTERM,STUPID;
	char ALTMODE;
};

class PASCALSOURCE
{
private:
	vector<PSYMBOL> m_symbols;
	
public:
	bTreeType<PSYMBOL> *m_ptree;

	/*SCANNER GLOBALS...NEXT FOUR VARS*/
	/*MUST BE IN THIS ORDER FOR IDSEARCH*/

struct // for eventual refactoring
{
	SYMBOLS::SYMBOL	SY;			/*SYMBOL FOUND BY INSYMBOL*/
	OPERATOR	OP;				/*CLASSIFICATION LAST SYMBOL*/
	ALPHA		ID;				/*LAST identifier FOUND*/			
    VALU		VAL;			/*VALUE THEN LAST CONSTANT*/
};

struct
{
	SYMBUFARRAY	*SYMBUFP;		/*SYMBOLIC BUFFER...ASCII||CODED*/
	CURSRANGE	SYMCURSOR;		/*CURRENT SCANNING INDEX IN SYMBUFP^*/
	CURSRANGE	OLDSYMCURSOR;
	CURSRANGE	PREVSYMCURSOR;
	CURSRANGE	LINESTART;
	CURSRANGE	PREVLINESTART;
    CURSRANGE	OLDLINESTART;
	int			SYMBLK;
	int			OLDSYMBLK;
	int			PREVSYMBLK;
} m_src;

	CODEARRAY	*CODEP;			/*CODE BUFFER WHILE WRITEOUT*/
	CSP			SCONST;			/*INSYMBOL STRING RESULTS*/

protected:
struct // _options 
{
	bool	INMODULE,USING,INCLUDING,ININTERFACE;
	bool	DP,NOISY,LIST,GETSTMTLEV,BPTONLINE;
	bool	NOSWAP,GOTOOK,DEBUGGING,IOCHECK;
	bool	TINY,RANGECHECK,SYSCOMP,LIBNOTOPEN;
} options;

	int		LGTH;
	int		STARTDOTS;
	int		SCREENDOTS;
	int		LINEINFO;
	int		BEGSTMTLEV;
	int		STMTLEV;		/*CURRENT STATEMENT NESTING LEVEL*/
	int		CURBLK;
	 
	char		SYSTEMLIB[MAXDIRLEN];
	char		*COMMENT;
	pascal_file		*_LP;
	pascal_file		*LIBRARY;
	pascal_file		*INCLFILE;
	pascal_file		*USEFILE;
	pascal_file		*WORKCODE;
	pascal_file		*UNITFILE;
	pascal_file		*SYSLIBRARY;

	INFOREC		USERINFO;
	ADDRRANGE	LC,IC;			/*LOCATION AND INSTRUCT COUNTERS*/
	SEGRANGE	SEG;			/*CURRENT SEGMENT NO.*/
	LEVRANGE	LEVEL;		/*CURRENT STATIC LEVEL*/
	PROCRANGE	CURPROC;	/*void NUMBER ASSIGNMENT*/
	PROCRANGE	NEXTPROC;

private:
	void CHECK();
	void NUMBER();
	void STRING();
	int CREATE_SYMLIST(LPVOID);
	void DEBUG_SY (const PSYMBOL &p, SYMBOLS::SYMBOL start, SYMBOLS::SYMBOL stop);
	void SCANSTRING(char *STRG, int MAXLENG, char);
	char PEEK();
	char PEEK(int i);
	void GETSYMBOL();
	void GETIDENT();
	bool GETOPERATOR();

protected:
	char GETC();
	void CERROR(int ERRORNUM);
	void PRINTLINE();
	void WRITETEXT();
	void GETNEXTPAGE();
	void PARSEOPTION(char);
	void SKIP(const SETOFSYS &FSYS);

public:
	static void *allocate(void*);
	void build_tree ();
	void COMMENTER(char STOPPER);
	void INSYMBOL();
	int SYMBOL_DUMP (LPVOID);
	void SOURCE_DUMP ();
};

