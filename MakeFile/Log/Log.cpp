#include "StdAfx.h"
#include "Log.h"


/////////////////////////
// �R���X�g���N�^
CLog::CLog(void)
{
	_tsetlocale(LC_ALL, _T(""));
}


CLog::~CLog(void)
{

}


/////////////////////////
// �\��
void CLog::Log(std::wstring str)
{
	str += L"\n";
	::OutputDebugString(str.c_str());
	wprintf(str.c_str());
}



/////////////////////////
// ���ʕ\��
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
	else	Add(L"GUID�̏o�͂Ɏ��s");

}

void CLog::Add(std::wstring sTitle, TCHAR *value )
{
	if(value)		Add(sTitle ,std::wstring(value));
	else			Add(sTitle ,L"�s��");
}

