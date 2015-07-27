#include "StdAfx.h"
#include "LogFile.h"

#define _LOG_FILE_NAME L"log.txt"


////////////////////////////
CLogFile::CLogFile(void)
{
	SetDefault();
}

CLogFile::CLogFile(TCHAR *_path)
{
	SetDefault(_path);
}


CLogFile::~CLogFile(void)
{
}

////////////////////////////
void CLogFile::SetDefault( TCHAR *path )
{
	_tsetlocale(LC_ALL, _T(""));
	swprintf_s( m_LogName, MAX_PATH, path == NULL ? _LOG_FILE_NAME : path );
}

////////////////////////////
void CLogFile::DeleteLog()
{
	::DeleteFile(m_LogName);
}

void CLogFile::Log( std::wstring str )
{
	CLog::Log(str);

	FILE *fp = NULL;
	_wfopen_s( &fp, m_LogName, L"a" );
	if(fp)
	{
		fputws( str.c_str(), fp );
		fclose(fp);
	}
}