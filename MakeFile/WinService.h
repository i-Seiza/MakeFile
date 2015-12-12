#pragma once

#include <winsvc.h>

class CWinService
{
public:
	CWinService();
	CWinService(_TCHAR *serviceName);
	~CWinService();


	SC_HANDLE m_manager;
	SC_HANDLE m_service;

	_TCHAR m_ServiceName[MAX_PATH];

	DWORD GetStatus();
	void GetStatus(DWORD aStatus, _TCHAR *result);

	void start();
	void stop();
};

