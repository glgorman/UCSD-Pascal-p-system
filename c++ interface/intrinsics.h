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

class deug_param
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
	deug_param (char arg);
	deug_param (char* arg);
	deug_param (double arg);
	deug_param (float arg);
	deug_param (int arg);
	deug_param (size_t arg);
};

void _WRITE(int uid, size_t argc, ...);

void WRITE(int uid, deug_param);
void WRITE(int uid, deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param,deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,
		   deug_param,deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,
		   deug_param,deug_param,deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,
		   deug_param,deug_param,deug_param,deug_param,deug_param);
void WRITE(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,
		   deug_param,deug_param,deug_param,deug_param,deug_param,deug_param);

void _WRITELN(int uid, size_t argc, ...);

void WRITELN(int uid);
void WRITELN(int uid, deug_param);
void WRITELN(int uid, deug_param,deug_param);
void WRITELN(int uid, deug_param,deug_param,deug_param);
void WRITELN(int uid, deug_param,deug_param,deug_param,deug_param);
void WRITELN(int uid, deug_param,deug_param,deug_param,deug_param,deug_param);
void WRITELN(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param);
void WRITELN(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param);
void WRITELN(int uid, deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param,deug_param);

int SCAN(int,char,const char *);
void RELEASE(void*);
int TRUNC(double);
int ORD(const int &);
bool ODD(const int &);
int ROUND (double arg);
void NEW(void *(&ptr), int sz);
void READ(int, char &);
void MARK(void*(&));
void TIME(int &, int &);
int CLOSE(FILE*,bool lock=false);
int UNITWRITE (int UNITNUMBER, char *ARRAY, int LENGTH, int BLOCK=0, DWORD MODE=0);
int BLOCKWRITE(FILE* param1, const char *param2, int param3,int param4=0);
int BLOCKREAD(FILE* param1, char *param2, int param3,int param4=0);
void MOVELEFT(const char*,char*,int);
DWORD MEMAVAIL();