
#include "stdafx.h"
#include "new.h"
#include <memory>
#include <vector>

//#include "../Frame Lisp/symbol_table.h"
//#include "../Frame Lisp/btreetype.h"
//#include "../Frame Lisp/node_list.h"
//#include "../Frame Lisp/text_object.h"

#include "../Frame Lisp/intrinsics.h"
#include "../Frame Lisp/range.h"
#include "../Frame Lisp/sets.h"

using namespace std;

#define IDENTSIZE	16
#define STRGLGTH	255
#define MAXDIRLEN	40
#define MAXINT		32767
#define MAXDEC		36
#define EOL			13
#define MAXCURSOR	1023
#define MAXCODE		1299

extern char *SYMBOL_NAMES1[];
extern char *SYMBOL_NAMES2[];

namespace pascal0
{
struct key_info;
extern key_info operators[];
};

typedef	DWORD BITRANGE;
struct IDENTIFIER;
typedef IDENTIFIER* CTP;
struct STRUCTURE;
typedef STRUCTURE*	STP;
typedef char ALPHA[IDENTSIZE];
typedef char SYMBUFARRAY[MAXCURSOR+1];
typedef char CODEARRAY[MAXCODE+1];

int TREESEARCH(const CTP& n1, CTP& n2, ALPHA &str);
void PRINTTREE(const CTP &n1);

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
	REEL,
	PSET,
	STRG,
	TRIX,
	LONG1,
	MAXCST,
} CSTCLASS;

typedef enum _UNITFILE
{
	WORKCODE,
	SYSLIBRARY,
} UNITFILE;

struct CONSTREC;
typedef CONSTREC*	CSP;
struct CONSTREC
{
	CSTCLASS	CCLASS;
	int	 LLENG,LLAST;
	union
	{
		char	SLGTH;
		int		LONG;
		int		LONGVAL[10];
		int		CSTVAL[8];
		float	REEL;
		float	RVAL;
//				PSET;
		char	SVAL[STRGLGTH];
		int		PVAL[4]; // should be SET[0..127];
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
	ALPHA		ID;		/*LAST IDENTIFIER FOUND*/			
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
		strcpy_s(ID,16,STR);
		SY = _SY;
		OP = _OP;
	}
};
};

class INFOREC
{
public:
	FILE *WORKSYM, *WORKCODE;
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
	ALPHA		ID;				/*LAST IDENTIFIER FOUND*/			
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
	int		SYMBLK;
	int		OLDSYMBLK;
	int		PREVSYMBLK;
	int		CURBLK;
	 
	char		SYSTEMLIB[MAXDIRLEN];
	char		*COMMENT;
	FILE		_LP;
	FILE		LIBRARY;
	FILE		INCLFILE;
	UNITFILE	USEFILE;

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
	void SKIP(SETOFSYS FSYS);

public:
	static void *allocate(void*);
	void build_tree ();
	void COMMENTER(char STOPPER);
	void INSYMBOL();
	int SYMBOL_DUMP (LPVOID);
	void SOURCE_DUMP ();
};

