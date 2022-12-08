//
// Intrinsic functions needed to support Pascal like
// languages.  Copyright 2021 Gerold Lee Gorman
// Permission to redistribute and make use of this
// software under GNU/MIT license.
//
#include <vector>
using namespace std;

#define KEYBOARD (1)
#define OUTPUT	 (1)
#define CONSOLE  (2)
#define LOCK   (false)
#define YES		(1)
#define HAS_CONSOLE		(YES)

typedef FILE PHYLE;
typedef char *TEXT;

//#define WRITE(DEVICE,COUNT,...) _WRITE(DEVICE,COUNT, __VA_ARGS__)
//#define WRITELN(DEVICE,COUNT,...) _WRITELN(DEVICE,COUNT, __VA_ARGS__)

typedef	enum { CHAR1, CHARPTR1, DOUBLE1, DWORD1, FLOAT1, INT1, SIZE1, ULONG1, VOID1 } PTYPE;

class sandbox
{
public:
	static LPVOID allocate_p(size_t size);
};

struct pascal_error
{
	char *errstr;
	int	 errnum;

	pascal_error() {}
	pascal_error (int id, char *str)
	{
		errnum = id;
		errstr = str;
	}
};


extern pascal_error error_list[];

class pascal_file
{
public:
	int blocks_written;
	int blocks_read;
	vector<char*> *m_source;
	vector<char*>::iterator m_begin;
	vector<char*>::iterator m_pos;	
	char *_tmpfname;

public:
	pascal_file ();
	size_t size ();
	void append (char *);
	char *get_sector (int n);
	void *write_sector (int n, char *);
};

class EXIT_CODE
{
public:
	bool m_edit;
	int	 err;
	char *m_str;

public:
	EXIT_CODE(char*str);
	EXIT_CODE(int n, bool edit);
};

class s_param
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
	s_param (DWORD arg);
	s_param (char arg);
	s_param (char* arg);
	s_param (double arg);
	s_param (float arg);
	s_param (int arg);
	s_param (size_t arg);
	// warning - unsafe
	s_param (LPVOID arg);
};

namespace pascal0
{
struct key_info;
};

namespace SEARCH
{
	pascal0::key_info *get_key_info (int index);
	void RESET_SYMBOLS();
	int IDSEARCH(int pos, char *&str);
};

namespace SYSCOMM
{
using namespace std;

	void LAUNCH_CONSOLE();
	void REWRITE(pascal_file*,char*);
	BOOL WINAPI ConsoleHandler(DWORD);
	void RESET(pascal_file*,char*);
	bool IORESULT(void);
	void OPENNEW(pascal_file *,char *);
	void OPENOLD(pascal_file *,char *);
	void READ(int, char &);
	int CLOSE(pascal_file*,bool lock=false);
	int UNITWRITE (int UNITNUMBER, char *ARRAY, int LENGTH, int BLOCK=0, DWORD MODE=0);
	int BLOCKWRITE(pascal_file *, const unsigned char *param2, int param3,int param4=0);
	int BLOCKREAD(pascal_file *, char *param2, int param3,int &param4);
	void OutputDebugString (const char *str);
};

void READLN(int uid, char *(&));

void _WRITE(int uid, size_t argc, ...);
void WRITE(int uid, const s_param &);
void WRITE(int uid, const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &,const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,
		   const s_param &,const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,
		   const s_param &,const s_param &,const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,
		   const s_param &,const s_param &,const s_param &,const s_param &,const s_param &);
void WRITE(int uid, const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,
		   const s_param &,const s_param &,const s_param &,const s_param &,const s_param &,const s_param &);

void _WRITELN(int uid, size_t argc, ...);

void WRITELN(int uid);
void WRITELN(int uid, const s_param &);
void WRITELN(int uid, const s_param &,const s_param &);
void WRITELN(int uid, const s_param &,const s_param &,const s_param &);
void WRITELN(int uid, const s_param &,const s_param &,const s_param &,const s_param &);
void WRITELN(int uid, const s_param &,const s_param &,const s_param &,const s_param &,
			 const s_param &);
void WRITELN(int uid, const s_param &,const s_param &,const s_param &,const s_param &
			 ,const s_param &,const s_param &);
void WRITELN(int uid, const s_param &,const s_param &,const s_param &,const s_param &,
			 const s_param &,const s_param &,const s_param &);
void WRITELN(int uid, const s_param &,const s_param &,const s_param &,const s_param &,
			 const s_param &,const s_param &,const s_param &,const s_param &);


int SCAN(int,bool,char,const char *(a));
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