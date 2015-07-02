#include "StdAfx.h"
#include "Log.h"

/////////////////////////
// �R���X�g���N�^
CLog::CLog(void)
{

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
