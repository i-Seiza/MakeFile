#pragma once
#include <boost/lexical_cast.hpp>
#include <string>
#include <boost/regex.hpp>

class CVersion
{
public:
	CVersion(void);
	~CVersion(void);


	template <typename T>	void Set(T value )
	{
		std::wstring wtext = boost::lexical_cast<std::wstring>(value);
		boost::basic_regex<wchar_t>  regex(L"([0-9]*)\\.([0-9]*)\\.([0-9]*)");
		boost::match_results<std::wstring::const_iterator> result;
		if(boost::regex_search(wtext, result, regex) )
		{
			majer = boost::lexical_cast<int>(result.str(1));
			miner = boost::lexical_cast<int>(result.str(2));
			build = boost::lexical_cast<int>(result.str(3));
		}
	}

	template <typename T>	bool operator ==(T value)
	{
		CVersion v;
		v.Set(value);
		if( majer != v.GetMajerVersion() )	return false;
		if( miner != v.GetMinerVersion() )	return false;
		if( build != v.GetBuildVersion() )	return false;
		return true;
	}

	int GetMajerVersion();
	int GetMinerVersion();
	int GetBuildVersion();

private:
	int majer;
	int miner;
	int build;


};
