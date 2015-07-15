#include "StdAfx.h"
#include "Log.h"

#define _LOG_FILE_NAME L"log.txt"

/////////////////////////
// コンストラクタ
CLog::CLog(void)
{
	_tsetlocale(LC_ALL, _T(""));
	SetWriteFile(false);
}
CLog::CLog(bool bFile)
{
	_tsetlocale(LC_ALL, _T(""));
	SetWriteFile(bFile);
	RefreshLogFile();
}

CLog::~CLog(void)
{

}

void CLog::SetWriteFile( bool bStatus )
{
	m_bWriteFile = bStatus;
}

void CLog::RefreshLogFile()
{
	if(m_bWriteFile)
	{
		FILE *fp = NULL;
		_wfopen_s( &fp, _LOG_FILE_NAME, L"w" );
		if(fp)
		{
			fclose(fp);
		}
	}
}

/////////////////////////
// 表示
void CLog::Log(std::wstring str)
{
	str += L"\n";
	::OutputDebugString(str.c_str());
	wprintf(str.c_str());

	if(m_bWriteFile)
	{
		FILE *fp = NULL;
		_wfopen_s( &fp, _LOG_FILE_NAME, L"a" );
		if(fp)
		{
			fputws( str.c_str(), fp );
			fclose(fp);
		}
	}
}



/////////////////////////
// 結果表示
void CLog::AddLastError()
{
	Add(GetLastError());
}

void CLog::AddResult( std::wstring sTitle, bool bSuccess )
{
	std::wstring wtext = sTitle + L":\t";
	if (bSuccess)
	{
		wtext += L"Success";
	}
	else
	{
		wtext += (L"failed(" + boost::lexical_cast<std::wstring>(GetLastError()) + L")");
	}

	Add(wtext);
}

void CLog::Add( std::wstring sTitle, GUID guid )
{
	TCHAR strGuid[40];
	int ret = StringFromGUID2(guid, strGuid, sizeof(strGuid));
	if(ret)	Add(sTitle, strGuid);
	else	Add(L"GUIDの出力に失敗");

}

void CLog::Add(std::wstring sTitle, TCHAR *value )
{
	if(value)		Add(sTitle ,std::wstring(value));
	else			Add(sTitle ,L"不明");
}