#pragma once

// ウイルスクリアログを読み取る
class CVirusClearLog
{
public:
	CVirusClearLog(void);
	~CVirusClearLog(void);

public:
	void Read( char *path );
};
