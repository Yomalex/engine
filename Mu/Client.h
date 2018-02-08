#pragma once
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

class CClient
{
public:
	CClient();
	~CClient();

	bool Connect(const TCHAR * lpHost, WORD wPort);
private:
	WSAData wsaData;
	SOCKET sCon;
	DWORD dwBytesRecv;
	DWORD dwBytesSend;
};

