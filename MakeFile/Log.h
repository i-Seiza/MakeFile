#pragma once
#include "Windows.h"
#include <string>
#include <boost/lexical_cast.hpp>

//template <typename T>
class CLog
{
public:
	CLog(void);
	~CLog(void);

	template <typename T>	void Add(std::wstring sTitle, T value )
	{
		std::wstring wtext = boost::lexical_cast<std::wstring>(value);
		Log(sTitle + L":\t" + wtext);
	}
	template <typename T>	void Add(T value)
	{
		std::wstring wtext = boost::lexical_cast<std::wstring>(value);
		Log(wtext);
	}

	void AddLastError();
	void AddResult( std::wstring sTitle, bool bSuccess );

private:
	void Log(std::wstring str);
};
