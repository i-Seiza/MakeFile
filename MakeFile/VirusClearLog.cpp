#include "StdAfx.h"
#include "VirusClearLog.h"
#include <windows.h>
#include "sqlite3.h"


#include "log.h"

// Type define function pointer in sqlite3.dll
typedef int (__cdecl * pOpen)(char *, sqlite3 **);
typedef int (__cdecl * pExec)(sqlite3 *, const char *, sqlite3_callback, void *, char **);
typedef int (__cdecl * pGetTable)(sqlite3 *, const char *, char ***, int *, int *, char **);
typedef int (__cdecl * pFreeTable)(char **);
typedef int (__cdecl * pClose)(sqlite3 *);


CVirusClearLog::CVirusClearLog(void)
{
}

CVirusClearLog::~CVirusClearLog(void)
{
}

void CVirusClearLog::Read( char *path )
{
	CLog log(true);


	HINSTANCE hDll;
	// function pointer
	pOpen sqlite3_open;
	pExec sqlite3_exec;
	pGetTable sqlite3_get_table;
	pFreeTable sqlite3_free_table;
	pClose sqlite3_close;

	// Load sqlite3.dll
	hDll = LoadLibrary(L"sqlite3.dll");
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

	sqlite3 *db;
	char *err;
	char **result;
	int row, col;
	int i, j;


	// Open the sqlite log database file
	int a = sqlite3_open(path, &db);
	// Get the query result
//	sqlite3_get_table(db, "select * from LOG_MALWARE;", &result, &row, &col, &err);
	sqlite3_get_table(db, "select * from LogData;", &result, &row, &col, &err);
	// Show the result data
	log.Add((boost::format( "row=%1% : col=%2%\n" ) % row % col).str().c_str() );
	for (i=0;i < (col * row +1);)
	{
		for (j=0;j < col;j++)
		{
			log.Add((boost::format( " %s" ) % result[i]).str().c_str() );
			i++;
		}
		//printf("\n");
	}
	// Free the memory that sqlite3_get_table() allocated
	sqlite3_free_table(result);
	// Close the database
	sqlite3_close(db);
	FreeLibrary(hDll);


}