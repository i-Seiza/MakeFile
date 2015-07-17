#pragma once

#include <windows.h>
#include "sqlite3.h"
#include "logFile.h"

#include <boost/filesystem.hpp>

// Type define function pointer in sqlite3.dll
typedef int (__cdecl * pOpen)(char *, sqlite3 **);
typedef int (__cdecl * pExec)(sqlite3 *, const char *, sqlite3_callback, void *, char **);
typedef int (__cdecl * pGetTable)(sqlite3 *, const char *, char ***, int *, int *, char **);
typedef int (__cdecl * pFreeTable)(char **);
typedef int (__cdecl * pClose)(sqlite3 *);
typedef int (__cdecl * pErrCode)(sqlite3 *);
typedef int (__cdecl * pErrString)(sqlite3 *);


class CVirusClearLog
{
protected:
	
	HINSTANCE hDll;
	
	// function pointer
	pOpen		sqlite3_open;
	pExec		sqlite3_exec;
	pGetTable	sqlite3_get_table;
	pFreeTable	sqlite3_free_table;
	pClose		sqlite3_close;
	pErrCode	sqlite3_errcode;
	pErrString	sqlite3_errmsg;

	CLogFile *m_Log;
	TCHAR	m_FileName[MAX_PATH];


protected:
	void Initialize();



public:
	CVirusClearLog(void);
	~CVirusClearLog(void);

public:
	virtual void Read( const TCHAR *path );

};
