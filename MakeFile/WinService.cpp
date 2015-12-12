#include "stdafx.h"
#include "WinService.h"


CWinService::CWinService():
m_manager(::OpenSCManager(NULL, NULL, GENERIC_EXECUTE)),
m_service(nullptr)
{
}


CWinService::~CWinService()
{
	if (m_manager)	CloseServiceHandle(m_manager);
}

CWinService::CWinService(_TCHAR *serviceName):
m_manager(::OpenSCManager(NULL, NULL, GENERIC_EXECUTE)),
m_service(nullptr)
{
	swprintf_s(m_ServiceName, MAX_PATH, serviceName);
}
DWORD CWinService::GetStatus()
{
	if (m_manager == nullptr)	m_manager = ::OpenSCManager(nullptr, nullptr, GENERIC_EXECUTE);
	if (m_manager == nullptr)
	{

		DWORD err = GetLastError();

	}
	SERVICE_STATUS	sStatus;

	::ZeroMemory(&sStatus, sizeof(SERVICE_STATUS));

	m_service = ::OpenService(m_manager, m_ServiceName, SERVICE_QUERY_STATUS);

	bool bRet = ::QueryServiceStatus(m_service, &sStatus);
	CloseServiceHandle(m_service);

	if (bRet == false)	return 0;
	return sStatus.dwCurrentState;
}

void CWinService::GetStatus(DWORD aStatus, _TCHAR *result)
{
	switch (aStatus)
	{
	case 0:
		result = _T("情報取得失敗");
		break;
	case SERVICE_CONTINUE_PENDING:			//0x00000005
		result = _T("The service continue is pending.");
		break;
	case SERVICE_PAUSE_PENDING:			// 0x00000006
		result = _T("The service pause is pending.");
		break;
	case SERVICE_PAUSED:			// 0x00000007
		result = _T("The service is paused.");
		break;
	case SERVICE_RUNNING:			// 0x00000004
		result = _T("The service is running.");
		break;
	case SERVICE_START_PENDING:			// 0x00000002
		result = _T("The service is starting.");
		break;
	case SERVICE_STOP_PENDING:			// 0x00000003
		result = _T("The service is stopping.");
		break;
	case SERVICE_STOPPED: 			// 0x00000001
		result = _T("The service is not running.");
		break;
	}
}

void CWinService::start()
{
	if (GetStatus() == SERVICE_RUNNING)	return;


	// SERVICE_STOPPEDになるまで待機する？
	while (GetStatus() != SERVICE_STOPPED)
	{
		Sleep(100);
	}
	//if (sStatus.dwCurrentState == SERVICE_STOPPED)

	//サービス開始要求
	m_service = ::OpenService(m_manager, m_ServiceName, SERVICE_START | SERVICE_QUERY_STATUS);
	bool bRet = ::StartService(m_service, NULL, NULL);
	::CloseServiceHandle(m_service);

}

void CWinService::stop()
{
	if (GetStatus() == SERVICE_STOPPED)	return;
	while (GetStatus() != SERVICE_RUNNING)
	{
		Sleep(100);
	}

	//サービス停止要求
	m_service = ::OpenService(m_manager, m_ServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
	SERVICE_STATUS status;
	bool bRet = ::ControlService(m_service, SERVICE_CONTROL_STOP, &status);
	::CloseServiceHandle(m_service);
}