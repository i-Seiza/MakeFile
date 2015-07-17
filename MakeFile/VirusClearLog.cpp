#include "StdAfx.h"
#include "VirusClearLog.h"



CVirusClearLog::CVirusClearLog(void)
{
	m_Log = new CLogFile(L"CVirusClearLog.txt");
	Initialize();
}

CVirusClearLog::~CVirusClearLog(void)
{
	if(m_Log)	delete m_Log;
	m_Log = NULL;

	FreeLibrary(hDll);

}

void CVirusClearLog::Initialize()
{
	CLogFile log(L"CVirusClearLog.txt");
	// Load sqlite3.dll
	hDll = LoadLibrary(L"sqlite3.dll");
	if(m_Log)	m_Log->AddResult( L"CVirusClearLog::Initialize", hDll != NULL );
	if (hDll == NULL)
	{
		return;
	}

	// Get the function address of sqlite3
	sqlite3_open = (pOpen)GetProcAddress(hDll, "sqlite3_open");
	sqlite3_exec = (pExec)GetProcAddress(hDll, "sqlite3_exec");
	sqlite3_get_table = (pGetTable)GetProcAddress(hDll, "sqlite3_get_table");
	sqlite3_free_table = (pFreeTable)GetProcAddress(hDll, "sqlite3_free_table");
	sqlite3_close = (pClose)GetProcAddress(hDll, "sqlite3_close");
	sqlite3_errcode = (pErrCode)GetProcAddress(hDll, "sqlite3_errcode");
	sqlite3_errmsg	= (pErrString)GetProcAddress(hDll, "sqlite3_errmsg");


}


void CVirusClearLog::Read( const TCHAR *_path )
{

    // �t�@�C�����
    WIN32_FIND_DATA findData;

    // �t�@�C������
    FILETIME fileTime;

    // �t�@�C�����擾
    HANDLE hFile = FindFirstFile(_path, &findData);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        // ���s(�s���p�X)
        return;
    }

    // ����

	SYSTEMTIME creationTime;
	SYSTEMTIME lastAccessTime;
	SYSTEMTIME lastWriteTime;

    // �t�@�C�������n���h������
    FindClose(hFile);

    // �쐬����
    FileTimeToLocalFileTime(&findData.ftCreationTime, &fileTime);
    FileTimeToSystemTime(&fileTime, &creationTime);

    // �A�N�Z�X����
    FileTimeToLocalFileTime(&findData.ftLastAccessTime, &fileTime);
    FileTimeToSystemTime(&fileTime, &lastAccessTime);

    // �X�V����
    FileTimeToLocalFileTime(&findData.ftLastWriteTime, &fileTime);
    FileTimeToSystemTime(&fileTime, &lastWriteTime);

	m_Log->Add( L"�쐬����", (boost::format( "%1%-%2%-%3% %4%:%5%:%6%" ) % creationTime.wYear % creationTime.wMonth % creationTime.wDay % creationTime.wHour % creationTime.wMinute % creationTime.wSecond ).str().c_str());
	m_Log->Add( L"�A�N�Z�X����", (boost::format( "%1%-%2%-%3% %4%:%5%:%6%" ) % lastAccessTime.wYear % lastAccessTime.wMonth % lastAccessTime.wDay % lastAccessTime.wHour % lastAccessTime.wMinute % lastAccessTime.wSecond ).str().c_str());
	m_Log->Add( L"�X�V����", (boost::format( "%1%-%2%-%3% %4%:%5%:%6%" ) % lastWriteTime.wYear % lastWriteTime.wMonth % lastWriteTime.wDay % lastWriteTime.wHour % lastWriteTime.wMinute % lastWriteTime.wSecond ).str().c_str());




}

