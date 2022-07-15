
#include <afxsock.h>

class TelnetServer
{
	WSAData sockInitInfo;
	CAsyncSocket tcp_socket,tcp_use;
	size_t		m_iPageSize;
	size_t		m_pages_allocated;
	LPVOID		m_txBuffer;
	LPVOID		m_rxBuffer;
	LPVOID		m_tempBuff;

private:
	size_t get_page_size();
	LPVOID allocate_buffer(size_t size);
	void putstr (char *);
	
public:
	TelnetServer ();
	BOOL InitInstance ();
	void WaitForConnection ();
	bool authenticate();
	int getstr (char *);
	void getLine (char *str);
	void putText (char *str);
};