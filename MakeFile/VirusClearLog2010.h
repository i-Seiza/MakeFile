#pragma once

#include "virusclearlog.h"


// ウイルスクリアログを読み取る
class CVirusClearLog2010 :
	public CVirusClearLog
{
public:
	CVirusClearLog2010(void);
	~CVirusClearLog2010(void);

public:
	void Read( const TCHAR *path );
};
