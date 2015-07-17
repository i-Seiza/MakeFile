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

    // ファイル情報
    WIN32_FIND_DATA findData;

    // ファイル時間
    FILETIME fileTime;

    // ファイル情報取得
    HANDLE hFile = FindFirstFile(_path, &findData);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        // 失敗(不正パス)
        return;
    }

    // 成功

	SYSTEMTIME creationTime;
	SYSTEMTIME lastAccessTime;
	SYSTEMTIME lastWriteTime;

    // ファイル検索ハンドル閉じる
    FindClose(hFile);

    // 作成日時
    FileTimeToLocalFileTime(&findData.ftCreationTime, &fileTime);
    FileTimeToSystemTime(&fileTime, &creationTime);

    // アクセス日時
    FileTimeToLocalFileTime(&findData.ftLastAccessTime, &fileTime);
    FileTimeToSystemTime(&fileTime, &lastAccessTime);

    // 更新日時
    FileTimeToLocalFileTime(&findData.ftLastWriteTime, &fileTime);
    FileTimeToSystemTime(&fileTime, &lastWriteTime);

	m_Log->Add( L"作成日時", (boost::format( "%1%-%2%-%3% %4%:%5%:%6%" ) % creationTime.wYear % creationTime.wMonth % creationTime.wDay % creationTime.wHour % creationTime.wMinute % creationTime.wSecond ).str().c_str());
	m_Log->Add( L"アクセス日時", (boost::format( "%1%-%2%-%3% %4%:%5%:%6%" ) % lastAccessTime.wYear % lastAccessTime.wMonth % lastAccessTime.wDay % lastAccessTime.wHour % lastAccessTime.wMinute % lastAccessTime.wSecond ).str().c_str());
	m_Log->Add( L"更新日時", (boost::format( "%1%-%2%-%3% %4%:%5%:%6%" ) % lastWriteTime.wYear % lastWriteTime.wMonth % lastWriteTime.wDay % lastWriteTime.wHour % lastWriteTime.wMinute % lastWriteTime.wSecond ).str().c_str());




}

