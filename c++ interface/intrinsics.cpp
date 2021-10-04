
#include "stdafx.h"
#include "intrinsics.h"

#define	BLOCKSIZE	(512)

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

int SCAN(int,char,const char *)
{
	ASSERT(false);
	return 0;
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

debug_param::debug_param(char arg)
{
	m_type = CHAR1;
	ch = arg;
}
debug_param::debug_param(char* arg)
{
	m_type = CHARPTR1;
	str = arg;
}
debug_param::debug_param(double arg)
{
	m_type = DOUBLE1;
	d = arg;
}
debug_param::debug_param(float arg)
{
	m_type = FLOAT1;
	f = arg;
}
debug_param::debug_param(int arg)
{
	m_type = INT1;
	i = arg;
}
debug_param::debug_param(size_t arg)
{
	m_type = SIZE1;
	sz = arg;
}

void WRITE(int uid, const debug_param &w1)
{
	_WRITE(uid,1,&w1);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2)
{
	_WRITE(uid,2,&w1,&w2);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3)
{
	_WRITE(uid,3,&w1,&w2,&w3);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4)
{
	_WRITE(uid,4,&w1,&w2,&w3,&w4);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
	const debug_param &w5)
{
	_WRITE(uid,5,&w1,&w2,&w3,&w4,&w5);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
	const debug_param &w5, const debug_param &w6)
{
	_WRITE(uid,6,&w1,&w2,&w3,&w4,&w5,&w6);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
	const debug_param &w5, const debug_param &w6, const debug_param &w7)
{
	_WRITE(uid,7,&w1,&w2,&w3,&w4,&w5,&w6,&w7);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
	const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8)
{
	_WRITE(uid,8,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
	const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8, const debug_param &w9)
{
	_WRITE(uid,9,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
	const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8,
	const debug_param &w9, const debug_param &w10)
{
	_WRITE(uid,10,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
	const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8,
	const debug_param &w9, const debug_param &w10, const debug_param &w11)
{
	_WRITE(uid,11,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10,&w11);
}

void WRITE(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
	const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8,
	const debug_param &w9, const debug_param &w10, const debug_param &w11, const debug_param &w12)
{
	_WRITE(uid,12,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10,&w11,&w12);
}

void WRITELN(int uid)
{
	_WRITELN(uid,0,NULL);
}

void WRITELN(int uid, const debug_param &w1)
{
	_WRITELN(uid,1,&w1);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2)
{
	_WRITELN(uid,2,&w1,&w2);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3)
{
	_WRITELN(uid,3,&w1,&w2,&w3);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4)
{
	_WRITELN(uid,4,&w1,&w2,&w3,&w4);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
		   const debug_param &w5)
{
	_WRITELN(uid,5,&w1,&w2,&w3,&w4,&w5);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
		   const debug_param &w5, const debug_param &w6)
{
	_WRITELN(uid,6,&w1,&w2,&w3,&w4,&w5,&w6);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
		   const debug_param &w5, const debug_param &w6, const debug_param &w7)
{
	_WRITELN(uid,7,&w1,&w2,&w3,&w4,&w5,&w6,&w7);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
		   const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8)
{
	_WRITELN(uid,8,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
		   const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8, const debug_param &w9)
{
	_WRITELN(uid,9,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
		   const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8,
		   const debug_param &w9, const debug_param &w10)
{
	_WRITELN(uid,10,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3,const debug_param &w4,
		   const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8,
		   const debug_param &w9, const debug_param &w10, const debug_param &w11)
{
	_WRITELN(uid,11,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10,&w11);
}

void WRITELN(int uid, const debug_param &w1, const debug_param &w2, const debug_param &w3, const debug_param &w4,
		   const debug_param &w5, const debug_param &w6, const debug_param &w7, const debug_param &w8,
		   const debug_param &w9, const debug_param &w10, const debug_param &w11, const debug_param &w12)
{
	_WRITELN(uid,12,&w1,&w2,&w3,&w4,&w5,&w6,&w7,&w8,&w9,&w10,&w11,&w12);
}

void _WRITE(int uid, size_t sz,...)
{
	char buffer1[256];
	char buffer2[256];
	const debug_param *val;
	unsigned int i;
	va_list vl;
	va_start(vl,sz);
	i=0;
	memset(buffer1,0,256);
	memset(buffer2,0,256);
	while(i<sz)
	{
		val=va_arg(vl,const debug_param *);
		switch (val->m_type)
		{
		case CHAR1:
			sprintf_s(buffer1,256,"%c",val->ch);
			break;
		case CHARPTR1:
			sprintf_s(buffer1,256,"%s",val->str);
			break;
		case DOUBLE1:
			sprintf_s(buffer1,256,"%lf",val->d);
			break;
		case FLOAT1:
			sprintf_s(buffer1,256,"%f",val->f);
			break;
		case INT1:
			sprintf_s(buffer1,256,"%d",val->i);
			break;
		case SIZE1:
			sprintf_s(buffer1,256,"%s"," ");
			break;
		default:
			break;
		}
		strcat_s(buffer2,256,buffer1);
		i++;
	}
	SYSCOMM::OutputDebugString(buffer2);
	va_end(vl);
}

void _WRITELN(int uid, size_t sz,...)
{
	char buffer1[256];
	char buffer2[256];
	const debug_param *val;
	unsigned int i;
	va_list vl;
	va_start(vl,sz);
	i=0;
	memset(buffer1,0,256);
	memset(buffer2,0,256);
	while(i<sz)
	{
		val=va_arg(vl,const debug_param *);
		switch (val->m_type)
		{
		case CHAR1:
			sprintf_s(buffer1,256,"%c",val->ch);
			break;
		case CHARPTR1:
			sprintf_s(buffer1,256,"%s",val->str);
			break;
		case DOUBLE1:
			sprintf_s(buffer1,256,"%lf",val->d);
			break;
		case FLOAT1:
			sprintf_s(buffer1,256,"%f",val->f);
			break;
		case INT1:
			sprintf_s(buffer1,256,"%d",val->i);
			break;
		case SIZE1:
			sprintf_s(buffer1,256,"%s"," ");
			break;
		default:
			break;
		}
		strcat_s(buffer2,256,buffer1);
		i++;
	}
	strcat_s(buffer2,256,"\n");
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
	Beep(262,300); 
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

int SYSCOMM::CLOSE(FILE *f,bool)
{
 	ASSERT(0);
	return 0;
}

void SYSCOMM::READ(int id, char &a)
{
	a = ' ';
}

void SYSCOMM::OPENNEW(struct _iobuf *,char *)
{

}

bool SYSCOMM::IORESULT(void)
{
	return true;
}

void SYSCOMM::OPENOLD(struct _iobuf *,char *)
{

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

int SYSCOMM::BLOCKREAD(FILE* file, char *buf, int blocks, int offset)
{
	int result;
	memset(buf,0,BLOCKSIZE);
	result = -1;
	return result;
}

int SYSCOMM::BLOCKWRITE(FILE* file, const char *buf, int blocks, int offset)
{
	int result = -1;
	return result;
}
