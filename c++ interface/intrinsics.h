//
// Intrinsic functions needed to support Pascal like
// languages.  Copyright 2021 Gerold Lee Gorman
// Permission to redistribute and make use of this
// software under GNU/MIT license.
//

#define KEYBOARD (1)
#define OUTPUT	 (1)
#define LOCK   (false)
#define YES		(1)
#define HAS_CONSOLE		(YES)

typedef FILE PHYLE;
typedef char *TEXT;

//#define WRITE(DEVICE,COUNT,...) _WRITE(DEVICE,COUNT, __VA_ARGS__)
//#define WRITELN(DEVICE,COUNT,...) _WRITELN(DEVICE,COUNT, __VA_ARGS__)

typedef	enum { CHAR1, CHARPTR1, DOUBLE1, FLOAT1, INT1, SIZE1 } PTYPE;

class debug_param
{
public:
	PTYPE	m_type;
	union
	{
		char	ch;
		char	*str;
		double	d;
		float	f;
		int		i;
		size_t	sz;
	};
	debug_param (char arg);
	debug_param (char* arg);
	debug_param (double arg);
	debug_param (float arg);
	debug_param (int arg);
	debug_param (size_t arg);
};

namespace SYSCOMM
{
	void LAUNCH_CONSOLE();
	BOOL WINAPI ConsoleHandler(DWORD);
	bool IORESULT(void);
	void OPENNEW(struct _iobuf *,char *);
	void OPENOLD(struct _iobuf *,char *);
	void READ(int, char &);
	int CLOSE(FILE*,bool lock=false);
	int UNITWRITE (int UNITNUMBER, char *ARRAY, int LENGTH, int BLOCK=0, DWORD MODE=0);
	int BLOCKWRITE(FILE* param1, const char *param2, int param3,int param4=0);
	void BLOCKREAD(FILE* param1, char *param2, int param3,int &param4);
	void OutputDebugString (const char *str);
};

void _WRITE(int uid, size_t argc, ...);

void WRITE(int uid, const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,
		   const debug_param &,const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,
		   const debug_param &,const debug_param &,const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,
		   const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &);
void WRITE(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,
		   const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &,const debug_param &);

void _WRITELN(int uid, size_t argc, ...);

void WRITELN(int uid);
void WRITELN(int uid, const debug_param &);
void WRITELN(int uid, const debug_param &,const debug_param &);
void WRITELN(int uid, const debug_param &,const debug_param &,const debug_param &);
void WRITELN(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &);
void WRITELN(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,
			 const debug_param &);
void WRITELN(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &
			 ,const debug_param &,const debug_param &);
void WRITELN(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,
			 const debug_param &,const debug_param &,const debug_param &);
void WRITELN(int uid, const debug_param &,const debug_param &,const debug_param &,const debug_param &,
			 const debug_param &,const debug_param &,const debug_param &,const debug_param &);

namespace SEARCH
{
	void IDSEARCH(int &offset, char *(&a));
};

int SCAN(int,char,const char *(a));
void RELEASE(void*);

float PWROFTEN(int i);
int TRUNC(double);
int ORD(const int &);
bool ODD(const int &);
int ROUND (double arg);
void NEW(void *(&ptr), int sz);
void MARK(void*(&));
void TIME(int &, int &);
void MOVELEFT(const char*,char*,int);
DWORD MEMAVAIL();