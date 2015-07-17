#include "StdAfx.h"
#include "Version.h"

CVersion::CVersion(void)
{
}

CVersion::~CVersion(void)
{
}

int CVersion::GetMajerVersion()
{
	return majer;
}
int CVersion::GetMinerVersion()
{
	return miner;
}
int CVersion::GetBuildVersion()
{
	return build;
}
