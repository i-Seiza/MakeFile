// MakeFile.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <string>

#include "TmUtyPPI.h"
#include "Log.h"

#include "sqlite3.h"

#include "VirusClearLog.h"


#define PCCRTN_S_OK MAKE_HRESULT( SEVERITY_SUCCESS,FACILITY_ITF, 0x0200)
#define PCCRTN_E_FAIL MAKE_HRESULT( SEVERITY_ERROR,FACILITY_ITF, 0x0201)
#define PCCRTN_E_INVALIDARG MAKE_HRESULT( SEVERITY_ERROR,FACILITY_ITF, 0x0204)
#define PCCRTN_E_OUTOFMEMORY MAKE_HRESULT( SEVERITY_ERROR,FACILITY_ITF, 0x0205)



std::wstring GetResultComment( HRESULT hr )
{
	std::wstring str;

	switch( hr )
	{
	case PCCRTN_S_OK:
		str = L"success";
		break;
	case PCCRTN_E_FAIL:
		str = L"generic fail";
		break;
	case PCCRTN_E_INVALIDARG:
		str = L"Invalid parameter";
		break;
	case PCCRTN_E_OUTOFMEMORY:
		str = L"out of memory";
		break;
	default:
		str = L"other error";
		break;
	}
	
	return str;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CLog log(true);

	CoInitialize(NULL);

	ITmUtyPPI2* pTPI2 = NULL;
	ITmUtyPPI* pTPI = NULL;

	HRESULT hr = CoCreateInstance( CLSID_TmUtyPPI, NULL, CLSCTX_INPROC_SERVER, IID_ITmUtyPPI2, reinterpret_cast<void**>(&pTPI2) );

	log.Add( L"CoCreateInstance", SUCCEEDED(hr) );

	if (SUCCEEDED(hr))
	{
		
		hr = CoCreateInstance( CLSID_TmUtyPPI, NULL, CLSCTX_INPROC_SERVER, IID_ITmUtyPPI, reinterpret_cast<void**>(&pTPI) );
		if (FAILED(hr))
		{
			CoUninitialize();
			return 0;
		}

		BSTR pVal;
		// プロダクト名称の取得
		hr = pTPI->TmupGetProductName(&pVal);
		log.Add( L"TmupGetProductName", GetResultComment(hr) );
		::SysFreeString(pVal);

		if( SUCCEEDED(hr) )
		{
			log.Add( pVal );
		}

		// OEM プロダクトID の取得
		hr = pTPI->TmupGetOEMProductID(&pVal);
		log.Add( L"TmupGetOEMProductID", GetResultComment(hr) );
		::SysFreeString(pVal);
		if( SUCCEEDED(hr) )
		{
			log.Add( pVal );
		}

		// プロダクトバージョンの取得
		hr = pTPI->TmupGetProductVersion(&pVal);
		log.Add( L"TmupGetProductVersion", GetResultComment(hr) );
		::SysFreeString(pVal);
		if( SUCCEEDED(hr) )
		{
			log.Add( pVal );
		}

		// プロダクトのインストールパスの取得
		hr = pTPI->TmupGetProductPath(&pVal);
		log.Add( L"TmupGetProductPath", GetResultComment(hr) );
		::SysFreeString(pVal);
		if( SUCCEEDED(hr) )
		{
			log.Add( pVal );
		}

		// プロダクトの認証サーバのURL の取得
		hr = pTPI->TmupGetServerURL(&pVal);
		log.Add( L"TmupGetServerURL", GetResultComment(hr) );
		::SysFreeString(pVal);
		if( SUCCEEDED(hr) )
		{
			log.Add( pVal );
		}

		// プロダクトのメイン画面の表示
		hr = pTPI->TmupShowMainWindow(0);
		log.Add( L"TmupShowMainWindow", GetResultComment(hr) );
		::SysFreeString(pVal);
		if( SUCCEEDED(hr) )
		{
			log.Add( pVal );
		}

		// スマートスキャンエージェントパターンバージョンの取得
		hr = pTPI2->TmupGetVsapiPatthernVersion(&pVal);
		log.Add( L"TmupGetVsapiPatthernVersion", GetResultComment(hr) );
		::SysFreeString(pVal);
		if( SUCCEEDED(hr) )
		{
			log.Add( pVal );
		}


		// VSAPI エンジンバージョン
		hr = pTPI2->TmupGetVsapiEngineVersion(&pVal);
		log.Add( L"TmupGetVsapiEngineVersion", GetResultComment(hr) );
		::SysFreeString(pVal);
		if( SUCCEEDED(hr) )
		{
			log.Add( pVal );
		}

		// ログファイルパスの取得
		hr = pTPI2->TmupGetVirusLogFilePath(&pVal);
		log.Add( L"TmupGetVirusLogFilePath", GetResultComment(hr) );
		::SysFreeString(pVal);
		if( SUCCEEDED(hr) )
		{
			log.Add( pVal );
		}
		 

		pTPI->Release();
		pTPI2->Release();
		::CoUninitialize();
	}


	CVirusClearLog vc;
	vc.Read("E:\\desktop\\MakeFile\\debug\\TmEvLog.db");



	return 0;
}

