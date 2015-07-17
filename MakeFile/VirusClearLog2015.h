#pragma once
#include "virusclearlog.h"

class CVirusClearLog2015 :
	public CVirusClearLog
{
public:
	CVirusClearLog2015(void);
	~CVirusClearLog2015(void);

public:
	void Read( const TCHAR *path );

};
