
#include "stdafx.h"
#include "telnet.h"

using namespace std;

//	Create socket and begin listening on port 23

char SECURITY_WARNING[] = "\r\nSECURITY WARNING! This connection is NOT ENCRYPTED. Please use \
SSH, https, or a suitable VPN relay agent if you are connecting to this server from a remote \
host.  If you are connecting from a WIRELESS device via WEP, or WEP2 please be aware that \
other users sharing the same router interface may be able to intercept communcations.\r\n";

#define DEFAULT_IPADDR "127.0.0.1"
char LOGIN[] = "\r\nlogin:";
char PASSWORD[] = "\r\npassword:";
char ACCESS_DENIED[] = "\r\nACCESS DENIED";
char UNKNOWN_USER[] = "\r\nUnknown User: %s or bad password ******";
char WELCOME_MSG[] = "\r\nWelcome %s to Parallax Debug Terminal.\r\nType --help for help";
char DEFAULT_PROMPT[] = "\r\n";

TelnetServer::TelnetServer ()
{
	LPVOID buff;
	m_iPageSize = get_page_size();
	buff = allocate_buffer (m_iPageSize*3);
	m_txBuffer = buff;
	m_rxBuffer = (LPVOID)((char*)buff+m_iPageSize);
	m_tempBuff = (LPVOID)((char*)m_rxBuffer+m_iPageSize);
}

size_t TelnetServer::get_page_size()
{
	SYSTEM_INFO sSysInfo;
	GetSystemInfo(&sSysInfo);
	DWORD dwPageSize = sSysInfo.dwPageSize;
	return dwPageSize;
}

LPVOID TelnetServer::allocate_buffer(size_t size)
{
	BOOL b_valid;
	LPVOID	p_addr;
	size_t allocation = size;
	p_addr = (LPVOID) VirtualAlloc(NULL,allocation,
    MEM_COMMIT,PAGE_READWRITE|PAGE_NOCACHE);
	ASSERT (p_addr!=NULL);
	b_valid = (BOOL) VirtualLock (p_addr,allocation);
	return p_addr;
}

BOOL TelnetServer::InitInstance ()
{
	int status;
	BOOL _sock = AfxSocketInit (&sockInitInfo);
	long flags = FD_WRITE|FD_ACCEPT;
	status = tcp_socket.Create (23,SOCK_STREAM,flags,_T(DEFAULT_IPADDR));
	if (status==0) {
//		cout << "Failed to Create Socket!!\n";
		status = tcp_socket.GetLastError ();
//		cout << "Error code was " << status << "\n";
	}
	status = tcp_socket.Listen (5);
	if (status!=0)
	{
//		cout << "Listening .... \n";
	}
	else
	{
//		cout << "Failed to Listen!!\n";
		status = tcp_socket.GetLastError ();
	}
//		cout << "Error code was " << status << "\n"; }
	return TRUE;
}

void TelnetServer::WaitForConnection ()
{
	int status;
	SOCKADDR *IpSockAddr = NULL;
	int *lpSockAddrLen = NULL;
	do {
//		cout << "Waiting for connection.\n";
		status = tcp_socket.Accept (tcp_use,IpSockAddr,lpSockAddrLen);
		Sleep (1000);
	}
	while (status==0);
//	cout << "Connected!\n";
//	CComThread::OutputDegugString("\r\nTelnet Session Initiated.");
}

bool TelnetServer::authenticate()
{
	char *message = (char*)m_tempBuff;
	char userid[256];
	char pass[256];
	putText(LOGIN);
	getstr(userid);
	putText(PASSWORD);
	getstr(pass);
	if (strcmp(userid,"guest")==0)
	{
		sprintf_s (message,256,WELCOME_MSG,userid);
		putText(message);
		putText(SECURITY_WARNING);
		return true;
	}
	else
	{
		sprintf_s(message,256,UNKNOWN_USER,userid);
		putText(UNKNOWN_USER);
		putText(ACCESS_DENIED);
	}
	return false;
}

void TelnetServer::putstr (char *buffer)
{
	int status,length;
	length = strlen (buffer);
	status = tcp_use.Send (buffer,length,0);
}

int TelnetServer::getstr (char *buffer)
{
	int status = 0;
	char *buffer1 = (char*) m_rxBuffer;  
	while (status<1) {
	status = tcp_use.Receive (buffer1,256,0);
	}
	if (status>0)
		strncpy_s (buffer,256,buffer1,status);
	buffer [status]='\0';
	return status;
}

void TelnetServer::getLine (char *theLine)
{
	int length, count;
	bool busy = true;
	char theChar = '\0';
	theLine [0] = 0;
	char text [256];
	length = 0;
	while (busy==true) {
		count = getstr (text);
		putstr (text);
		for (int i=0;i<count;i++) {
			if ((theChar=='\n')||(theChar=='\r')) {
				busy=false;
				break; }
			theChar = text [i];
			theLine [length++] = theChar;
		}
	}
	theLine [length] = '\0';
}

void TelnetServer::putText (char *str)
{
	putstr (str);
}