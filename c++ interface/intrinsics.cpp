
#include "stdafx.h"
#include <vector>
#include "intrinsics.h"

#define PAGESZ (4096)
#define	BLOCKSIZE	(512)

namespace SYSCOMM
{
using namespace std;
	vector<char*>::iterator m_pos;
	vector<char*>::iterator m_end;
	vector<char*> *m_source;
};


// 16MB sounds good for now
// TODO make something reasonable

float PWROFTEN(int arg)
{
	float result = 1;
	int i;
	for (i=0;i<arg;i++)
		result*=10;
	return result;
}

void MARK(void* &PTR)
{
	PTR = 0;
}

void RELEASE(void *ptr)
{
	delete ptr;
}

DWORD MEMAVAIL()
{
	int result;
	result = 1<<24;
	return result;
}

void MOVELEFT(char const *src, char *dest, int count)
{
	memcpy(dest,src,count);
}

int SCAN(int max,bool find,char ch, const char *str)
{
	int i;
	char ch1;
	int pos = 0;
	for (i=0;i<max;i++)
	{
		ch1= str[i];
		if ((ch1=ch)&&(find==true))
		{
			pos=i;
			break;
		}
		else
		if ((ch1!=ch)&&(find==false))
		{
			pos=i;
			break;
		}
	}
	return pos;
}

int ROUND (double arg)
{
	int result;
	result = (int)(arg+0.5000);
	return result;
}

int TRUNC(double d)
{
	int result;
	result = (int)d;
	return result;
}

int ORD(const int &arg)
{
	return arg;
}

bool ODD(const int &arg)
{
	bool result;
	if (arg&0x01)
		result = true;
	else
		result = false;
	return result;
}

void TIME(int &tick1, int &tick2)
{
	tick2 = GetTickCount();
	tick1 = tick2/1000;;
}

s_param::s_param(char arg)
{
	m_type = CHAR1;
	ch = arg;
}
s_param::s_param(unsigned long arg)
{
	m_type = ULONG1;
	ch = arg;
}
s_param::s_param(char* arg)
{
	m_type = CHARPTR1;
	str = arg;
}
s_param::s_param(double arg)
{
	m_type = DOUBLE1;
	d = arg;
}
s_param::s_param(float arg)
{
	m_type = FLOAT1;
	f = arg;
}
s_param::s_param(int arg)
{
	m_type = INT1;
	i = arg;
}
s_param::s_param(LPVOID arg)
{
	m_type = VOID1;
	i = (DWORD)arg;
}
s_param::s_param(size_t arg)
{
	m_type = SIZE1;
	sz = arg;
}

void WRITE(int uid, const s_param &w1)
{
	_WRITE(uid,1,&w1);
}

void WRITE(int uid, const s_param &w1, const s_param &w2)
{
	_WRITE(uid,2,&w1,&w2);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3)
{
	_WRITE(uid,3,&w1,&w2,&w3);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4)
{
	_WRITE(uid,4,&w1,&w2,&w3,&w4);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
	const s_param &w5)
{
	_WRITE(uid,5,&w1,&w2,&w3,&w4,&w5);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
	const s_param &w5, const s_param &w6)
{
	_WRITE(uid,6,&w1,&w2,&w3,&w4,&w5,&w6);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
	const s_param &w5, const s_param &w6, const s_param &w7)
{
	_WRITE(uid,7,&w1,&w2,&w3,&w4,&w5,&w6,&w7);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
	const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8)
{
	_WRITE(uid,8,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
	const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8, const s_param &w9)
{
	_WRITE(uid,9,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
	const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8,
	const s_param &w9, const s_param &w10)
{
	_WRITE(uid,10,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
	const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8,
	const s_param &w9, const s_param &w10, const s_param &w11)
{
	_WRITE(uid,11,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10,&w11);
}

void WRITE(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
	const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8,
	const s_param &w9, const s_param &w10, const s_param &w11, const s_param &w12)
{
	_WRITE(uid,12,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10,&w11,&w12);
}

void WRITELN(int uid)
{
	_WRITELN(uid,0,NULL);
}

void WRITELN(int uid, const s_param &w1)
{
	_WRITELN(uid,1,&w1);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2)
{
	_WRITELN(uid,2,&w1,&w2);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3)
{
	_WRITELN(uid,3,&w1,&w2,&w3);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4)
{
	_WRITELN(uid,4,&w1,&w2,&w3,&w4);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
		   const s_param &w5)
{
	_WRITELN(uid,5,&w1,&w2,&w3,&w4,&w5);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
		   const s_param &w5, const s_param &w6)
{
	_WRITELN(uid,6,&w1,&w2,&w3,&w4,&w5,&w6);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
		   const s_param &w5, const s_param &w6, const s_param &w7)
{
	_WRITELN(uid,7,&w1,&w2,&w3,&w4,&w5,&w6,&w7);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
		   const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8)
{
	_WRITELN(uid,8,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
		   const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8, const s_param &w9)
{
	_WRITELN(uid,9,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
		   const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8,
		   const s_param &w9, const s_param &w10)
{
	_WRITELN(uid,10,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3,const s_param &w4,
		   const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8,
		   const s_param &w9, const s_param &w10, const s_param &w11)
{
	_WRITELN(uid,11,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10,&w11);
}

void WRITELN(int uid, const s_param &w1, const s_param &w2, const s_param &w3, const s_param &w4,
		   const s_param &w5, const s_param &w6, const s_param &w7, const s_param &w8,
		   const s_param &w9, const s_param &w10, const s_param &w11, const s_param &w12)
{
	_WRITELN(uid,12,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10,&w11,&w12);
}

void _WRITE(int uid, size_t sz,...)
{
	char buffer1[PAGESZ];
	char buffer2[PAGESZ];
	const s_param *val;
	unsigned int i;
	va_list vl;
	va_start(vl,sz);
	i=0;
	memset(buffer1,0,PAGESZ);
	memset(buffer2,0,PAGESZ);
	while(i<sz)
	{
		val=va_arg(vl,const s_param *);
		switch (val->m_type)
		{
		case CHAR1:
			sprintf_s(buffer1,PAGESZ,"%c",val->ch);
			break;
		case CHARPTR1:
			sprintf_s(buffer1,PAGESZ,"%s",val->str);
			break;
		case DOUBLE1:
			sprintf_s(buffer1,PAGESZ,"%lf",val->d);
			break;
		case FLOAT1:
			sprintf_s(buffer1,PAGESZ,"%f",val->f);
			break;
		case INT1:
			sprintf_s(buffer1,PAGESZ,"%d",val->i);
			break;
		case SIZE1:
			sprintf_s(buffer1,PAGESZ,"%s"," ");
			break;
		case VOID1:
			sprintf_s(buffer1,PAGESZ,"%0x8d",val->i);
			break;
		default:
			break;
		}
		strcat_s(buffer2,PAGESZ,buffer1);
		i++;
	}
	SYSCOMM::OutputDebugString(buffer2);
	va_end(vl);
}

void _WRITELN(int uid, size_t sz,...)
{
	char buffer1[PAGESZ];
	char buffer2[PAGESZ];
	const s_param *val;
	unsigned int i;
	va_list vl;
	va_start(vl,sz);
	i=0;
	memset(buffer1,0,PAGESZ);
	memset(buffer2,0,PAGESZ);
	while(i<sz)
	{
		val=va_arg(vl,const s_param *);
		switch (val->m_type)
		{
		case CHAR1:
			sprintf_s(buffer1,PAGESZ,"%c",val->ch);
			break;
		case CHARPTR1:
			sprintf_s(buffer1,PAGESZ,"%s",val->str);
			break;
		case DOUBLE1:
			sprintf_s(buffer1,PAGESZ,"%lf",val->d);
			break;
		case FLOAT1:
			sprintf_s(buffer1,PAGESZ,"%f",val->f);
			break;
		case INT1:
			sprintf_s(buffer1,PAGESZ,"%d",val->i);
			break;
		case SIZE1:
			sprintf_s(buffer1,PAGESZ,"%s"," ");
			break;
		default:
			break;
		}
		strcat_s(buffer2,PAGESZ,buffer1);
		i++;
	}
	strcat_s(buffer2,PAGESZ,"\n");
	SYSCOMM::OutputDebugString(buffer2);
	va_end(vl);
}


void SYSCOMM::LAUNCH_CONSOLE()
{
#ifdef HAS_CONSOLE
	AllocConsole();
	SetConsoleCtrlHandler(ConsoleHandler,TRUE);
#endif
}

BOOL WINAPI SYSCOMM::ConsoleHandler(DWORD dwEvent)
{
#if 0
	Beep(262,300);
#endif
	if (dwEvent==CTRL_CLOSE_EVENT)
		return true;
	else
		return false;
}

void SYSCOMM::OutputDebugString (const char *str)
{
	WPARAM w;
	LPARAM l;
	CWinApp *pApp;
	pApp = AfxGetApp();
//	CMainFrame *pMain = (CMainFrame*)pApp->m_pMainWnd;
#if 0
//	TODO - put in a system for various debug levels
//	like typedef enum { NONE, LACONIC, MEDIUM, VERBOSE, }
//	debug_level;
	if (m_debug==false)
		return;
#endif
	w = (WPARAM) _strdup(str);
	l = 0;
#ifdef HAS_CONSOLE
	DWORD dwWritten;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(hOut,(LPVOID)str,(DWORD)strlen(str),&dwWritten,NULL);
#endif
//	if (pMain!=NULL)
//	{
//	  pMain->PostMessage(WM_RECIEVE_DEBUG_DATA,w,l);
//	}
//	else
		delete (char*)w;
//	Sleep(1);
}

void SYSCOMM::OPENNEW(struct _iobuf *,char *)
{
	m_pos = (*m_source).begin();
}

void SYSCOMM::REWRITE(struct _iobuf *,char *)
{
	m_pos = (*m_source).begin();
}

void SYSCOMM::RESET(FILE*,char*)
{
	ASSERT(0);
}

int SYSCOMM::CLOSE(FILE *f,bool)
{
// 	ASSERT(0);
	return 0;
}

void READLN(int id, char *(&str))
{
	BOOL bres;
	char ptr[4];
	int n = 0;
	DWORD	sz,mode;
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hIn,&mode);
//	mode &= !ENABLE_LINE_INPUT;
	mode = ENABLE_ECHO_INPUT|ENABLE_LINE_INPUT;
	SetConsoleMode(hIn,mode);
	while (true)
	{
		do
		{
			bres = ReadConsole(hIn,ptr,1,&sz,NULL);
		}
		while (sz==0);
		str[n]=ptr[0];
		n++;
		if (ptr[0]==13)
			break;
	}
	str[n]=0;
}

void SYSCOMM::READ(int id, char &a)
{
	BOOL bres;
	char buffer[64];
	DWORD	sz,mode;
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hIn,&mode);
	mode &= !ENABLE_LINE_INPUT;
	SetConsoleMode(hIn,mode);
	do
	{
		bres = ReadConsole(hIn,buffer,1,&sz,NULL);
	}
	while (sz=0);
	a = buffer[0];
}

#if 0
void SYSCOMM::READ(int id, char &a)
{
	UINT res = AfxMessageBox(_T("INPUT EXPECTED"),IDOK);
	if (res==IDCANCEL)
		a = 'E';
	else
		a = ' ';
}
#endif

bool SYSCOMM::IORESULT(void)
{
	return true;
}

void SYSCOMM::OPENOLD(struct _iobuf *,char *)
{
	m_pos = (*m_source).begin();
}

int SYSCOMM::UNITWRITE (int UNITNUMBER, char *ARRAY, int LENGTH, int BLOCK, DWORD MODE)
{
	int result = -1;
	switch (UNITNUMBER)
	{
	case 3: // Terminal
		if (MODE==0) {
			WRITELN(OUTPUT,ARRAY);
		}
		result = 0;
	default:
		break;
	}
	return result;
}

// a pascal block is 512 bytes, but an Apple II
// sector is 256 bytes.  m_source is based on
// 256 byte sector style granularity.

int SYSCOMM::BLOCKREAD(FILE* file, char *buf, int blocks, int &read)
{
	char *block_ptr;
	read = 0;
	int i;
	int j;
	int result = 0;
	vector<char*>::iterator &iter = m_pos;
//	iter= (*m_source).begin();
	if (iter==m_source->end())
	{
		read=0;
		return 0;
	}
	memset(buf,0,BLOCKSIZE);
	for (i=0;i<blocks;i++)
	{
		for (j=0;j<2;j++)
		{
			block_ptr = (*iter++);
			if (block_ptr==NULL)
				break;
			memcpy(&(buf[BLOCKSIZE*i+(256*j)]),block_ptr,256);
			buf[BLOCKSIZE*i+256*(j+1)]=0;
		}
		if (block_ptr!=NULL)
			result++;
	}
#if 0
	WRITELN(OUTPUT,"SYSCOMM::BLOCKREAD(FILE*, char*, int, &int)");
	WRITELN(OUTPUT,"BLOCK ",read,"\n\"",buf,"\"");
#endif
	return result;
}

int SYSCOMM::BLOCKWRITE(FILE* file, const char *buf, int blocks, int offset)
{
	WRITELN (OUTPUT,"SYSCOMM::BLOCKWRITE offset= ",offset);
//	assume that we wrote a blcok
//	todo - save the blocks in memory so we can have
//	a hex view - or examine with the dissassembler
	int result = 1;
	return result;
}
