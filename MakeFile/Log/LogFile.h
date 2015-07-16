#pragma once
#include "log.h"

class CLogFile :
	public CLog
{
private:
	TCHAR	m_LogName[MAX_PATH];


public:
	CLogFile(void);
	CLogFile(TCHAR *_path);
	~CLogFile(void);

	void DeleteLog();
	void Save( std::wstring str );


private:
	void SetDefault( TCHAR *path = NULL );
};
