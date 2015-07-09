#include "stdafx.h"
#include "initguid.h"
#include <vds.h>
#include <stdio.h>

#pragma comment( lib, "ole32.lib" )
#pragma comment( lib, "rpcrt4.lib" )

#define _SafeRelease(x) {if (NULL != x) { x->Release(); x = NULL; } }
#define _VAR "----------------------\n"

void EnumerateDisks(IVdsPack* pPack)
{
	HRESULT hResult;
	ULONG ulFetched = 0;
	IUnknown* ppObjUnk ;
	IEnumVdsObject* pEnumVdsObject = NULL;  
	IVdsDisk* pVdsDisk = NULL;
	IVdsDisk2* pVdsDisk2 = NULL;
	IVdsAdvancedDisk* pVdsAdvancedDisk = NULL;

	if (pPack == 0)
		return;

	hResult = pPack->QueryDisks(&pEnumVdsObject);

	if (pEnumVdsObject == 0)
		return;

	while( true)
	{
		if (!pEnumVdsObject)
			break;

		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);
		if (ulFetched == 0) break;

		hResult = ppObjUnk->QueryInterface(IID_IVdsDisk, (void**)&pVdsDisk);

		VDS_DISK_PROP diskProp;
		pVdsDisk->GetProperties(&diskProp);

		printf(_VAR);
		wprintf(L"disk: %d\n", diskProp.status);
		wprintf(L"disk: %s\n", diskProp.pwszAdaptorName);
		wprintf(L"disk: %s\n", diskProp.pwszDevicePath);
		wprintf(L"disk: %s\n", diskProp.pwszFriendlyName);
		wprintf(L"disk: %s\n", diskProp.pwszName);
		wprintf(L"disk: %d\n", diskProp.dwDeviceType);
		wprintf(L"disk: %d\n", diskProp.dwMediaType);
		wprintf(L"disk: %d\n", diskProp.dwSignature);
		wprintf(L"disk: %d\n", diskProp.PartitionStyle);
		wprintf(L"disk: %d\n", diskProp.ReserveMode);
		wprintf(L"disk: %d\n", diskProp.ulFlags);
		printf(_VAR);

		VDS_PARTITION_PROP * pPropArray = NULL;
		LONG pNumberOfPartitions = 0;

		hResult = ppObjUnk->QueryInterface(IID_IVdsAdvancedDisk, (void**)&pVdsAdvancedDisk);        
		pVdsAdvancedDisk->QueryPartitions(&pPropArray, &pNumberOfPartitions);

		VDS_PARTITION_PROP * tmp = pPropArray;
		for (int i = 0; i < pNumberOfPartitions; ++i)
		{
			printf(_VAR);
			printf("Number: %d\n", tmp->ulPartitionNumber);
			printf("Style : %d\n", tmp->PartitionStyle);
			printf("Flags : %d\n", tmp->ulFlags);
			printf("Offset: %ull\n", tmp->ullOffset);
			printf("Size: %ull\n", tmp->ullSize);
			printf("MBR type: %d\n", tmp->Mbr.partitionType);
			printf("MBR type: %d\n", tmp->Mbr.bootIndicator);
			printf("MBR type: %d\n", tmp->Mbr.recognizedPartition);
			printf("MBR type: %d\n", tmp->Mbr.hiddenSectors);
			printf(_VAR);

			++tmp;
		}
		CoTaskMemFree(pPropArray);
	}
}

void EnumerateVolumes(IVdsPack* pPack)
{

	HRESULT hResult;
	ULONG ulFetched = 0;
	IUnknown* ppObjUnk ;
	IEnumVdsObject* pEnumVdsObject = NULL;  
	IVdsVolume* pVdsVolume = NULL;
	IVdsVolumeMF *VolumeMF= NULL;

	if (pPack == 0)
		return;

	hResult = pPack->QueryVolumes(&pEnumVdsObject);

	if (pEnumVdsObject == 0)
		return;

	while(true)
	{
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);
		if (ulFetched == 0) break;

		hResult = ppObjUnk->QueryInterface(IID_IVdsVolume, (void**)&pVdsVolume);
		if ( FAILED(hResult) ) {
			continue;
		}


		VDS_VOLUME_PROP volProp;
		pVdsVolume->GetProperties(&volProp);
		printf(_VAR);
		printf("Vol name  : %S\n", volProp.pwszName);
		printf("Vol health: %d\n", volProp.health);
		printf(_VAR);
		
		hResult= pVdsVolume->QueryInterface(IID_IVdsVolumeMF, (void**)&VolumeMF); 
		if ( FAILED(hResult) ) {
			continue;
		}

		VDS_FILE_SYSTEM_PROP pFileSystemProp;
		IVdsAsync *iv;
		VolumeMF->GetFileSystemProperties(&pFileSystemProp);
		hResult= VolumeMF->Format(volProp.RecommendedFileSystemType, volProp.pwszName, volProp.ullSize, true, TRUE, TRUE, &iv);
		CoTaskMemFree(&pFileSystemProp);



	}
}

void EnumeratePacks(IVdsSwProvider* pProvider)
{
	HRESULT hResult;
	ULONG ulFetched = 0;
	IUnknown* ppObjUnk ;
	IEnumVdsObject* pEnumVdsObject = NULL;  
	IVdsPack* pVdsPack = NULL;

	if (pProvider == 0)
		return;

	hResult = pProvider->QueryPacks(&pEnumVdsObject);

	if (pEnumVdsObject == 0)
		return;

	while( true)
	{
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);
		if (ulFetched == 0) break;

		hResult = ppObjUnk->QueryInterface(IID_IVdsPack, (void**)&pVdsPack);
		VDS_PACK_PROP packProp;
		pVdsPack->GetProperties(&packProp);
		if (packProp.status == VDS_PS_ONLINE) {
			printf(_VAR);
			printf("Pack name  : %S\n", packProp.pwszName);
			printf("Pack status: %d\n", packProp.status);
			printf("Pack flags : %d\n", packProp.ulFlags);
			printf(_VAR);
			EnumerateDisks(pVdsPack);
			EnumerateVolumes(pVdsPack);
		}
	}
}


void EnumerateSoftwareProviders(IVdsService* pService)
{
	HRESULT hResult;
	ULONG ulFetched = 0;
	IUnknown* ppObjUnk ;
	IEnumVdsObject* pEnumVdsObject = NULL;
	IVdsSwProvider* pVdsSwProvider = NULL;

	hResult = pService->QueryProviders(VDS_QUERY_SOFTWARE_PROVIDERS, &pEnumVdsObject);
	while( true)
	{
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);
		if (ulFetched == 0) break;

		hResult = ppObjUnk->QueryInterface(IID_IVdsSwProvider,(void**)&pVdsSwProvider);
		EnumeratePacks(pVdsSwProvider);
	}
}


int __cdecl main(void) 
{
	::MessageBox(NULL, L"", L"", MB_OK);




	//////////////////////////////////////////////////////////////////
	HRESULT hResult;
	IVdsService* pService = NULL;
	IVdsServiceLoader *pLoader = NULL;
	//Launch the VDS Service
	hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// Initialize COM security
	CoInitializeSecurity(
		NULL,       // Allow *all* VSS writers to communicate back!
		-1,        // Default COM authentication service
		NULL,       // Default COM authorization service
		NULL,       // reserved parameter
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY, // Strongest COM authentication level
		RPC_C_IMP_LEVEL_IMPERSONATE,  // Minimal impersonation abilities 
		NULL,       // Default COM authentication settings
		EOAC_NONE,      // No special options
		NULL       // Reserved parameter
		);

	if( SUCCEEDED(hResult) )
	{
		hResult = CoCreateInstance( 
			CLSID_VdsLoader,
			NULL,
			CLSCTX_LOCAL_SERVER,
			IID_IVdsServiceLoader,
			(void**) &pLoader
			);

		//if succeeded load VDS on local machine
		if( SUCCEEDED(hResult) )
			pLoader->LoadService(NULL, &pService);
		//Done with Loader now release VDS Loader interface
		_SafeRelease(pLoader);

		if( SUCCEEDED(hResult) )
		{
			hResult = pService->WaitForServiceReady();
			if ( SUCCEEDED(hResult) )
			{
				EnumerateSoftwareProviders(pService);

				return 0;       
			}
		}
	}


	return -1;
}