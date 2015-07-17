#include "StdAfx.h"
#include "VirusClearLog2015.h"

CVirusClearLog2015::CVirusClearLog2015(void)
{
	swprintf_s( m_FileName, MAX_PATH, L"EventLog.db");
	if(m_Log)
	{
		m_Log->DeleteLog();
		delete m_Log;
	}
	m_Log = new CLogFile(L"CVirusClearLog2015.txt");
}

CVirusClearLog2015::~CVirusClearLog2015(void)
{
}

void CVirusClearLog2015::Read( const TCHAR *_path )
{
	if (hDll == NULL)
	{
		return;
	}
	TCHAR p[MAX_PATH];
	swprintf_s(p, MAX_PATH, L"%s%s", _path, m_FileName);

	CVirusClearLog::Read(p);

    //TCHAR型をchar型に変換
	char  path[100];
    WideCharToMultiByte(CP_ACP, 0, p, -1, path, sizeof(path), NULL, NULL);

	// ファイルの存在確認
	if( !boost::filesystem::exists(path) )
	{
		m_Log->Add( L"CVirusClearLog2015: not file", path );
		return;
	}

	sqlite3 *db;
	char *err;
	char **result;
	int row, col;
	int i, j;


	// Open the sqlite log database file
	int a = sqlite3_open(path, &db);
	m_Log->AddResult( L"sqlite3_open", sqlite3_errcode(db)== 0 );
	if(sqlite3_errcode(db)== 0)
	{
		// Get the query result
		//sqlite3_get_table(db, "select * from LOG_MALWARE WHERE (ThreatType != 600 AND ThreatType != 601);", &result, &row, &col, &err);
		sqlite3_get_table(db, "select ID, Time,InfectedFile from LOG_MALWARE WHERE (ThreatType != 600 AND ThreatType != 601) AND Time > 1437008011;", &result, &row, &col, &err);
		m_Log->AddResult( L"sqlite3_get_table", sqlite3_errcode(db)== 0 );
		if(sqlite3_errcode(db))
		{
			m_Log->Add( err );
			//m_Log->Add( sqlite3_errmsg(db) );
		}
		if(err == NULL)
		{
			// Show the result data
			m_Log->Add((boost::format( "row=%1% : col=%2%\n" ) % row % col).str().c_str() );

			if(row)
			{
				for (i=0;i < (col * row +1);)
				{
					for (j=0;j < col;j++)
					{
						if(result[i])
						{
							WCHAR zFilename[MAX_PATH]={};
							MultiByteToWideChar(CP_UTF8, 0, (boost::format( "%s" ) % result[i]).str().c_str(), -1, zFilename, MAX_PATH);

							m_Log->Add(zFilename );
						}
						else
						{
							m_Log->Add(L"(null)");
						}
						i++;
					}
					//printf("\n");
				}
			}
		}
	}
	// Free the memory that sqlite3_get_table() allocated
	sqlite3_free_table(result);
	// Close the database
	sqlite3_close(db);
}

