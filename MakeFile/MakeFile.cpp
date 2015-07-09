// MakeFile.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "FileIO.h"



int _tmain(int argc, _TCHAR* argv[])
{
	::MessageBox(NULL, L"", L"", MB_OK);

	CFileIO io;
	HANDLE hFile = NULL;

	hFile = CreateFile(
		//L"\\\\.\\E:",
		L"\\\\.\\PhysicalDrive0",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH,
		NULL
		);
	if (hFile==INVALID_HANDLE_VALUE)	return 0;

	DWORD	dwRet;
	DWORD	dwLen = 4096;		//十分なバッファー領域を確保
	BYTE *pcbData = new BYTE[dwLen];
	if(pcbData == NULL)
	{
		::CloseHandle(hFile);
		return	false;
	}
	ZeroMemory(pcbData,dwLen);

	STORAGE_PROPERTY_QUERY		sQuery;

	sQuery.PropertyId	= StorageDeviceProperty;
	sQuery.QueryType	= PropertyStandardQuery;
	sQuery.AdditionalParameters[0] = NULL;
	bool b = ::DeviceIoControl(hFile,IOCTL_STORAGE_QUERY_PROPERTY,&sQuery,sizeof(STORAGE_PROPERTY_QUERY),pcbData,dwLen,&dwRet,NULL);


	STORAGE_DEVICE_DESCRIPTOR*	pDescriptor;
	pDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pcbData;
	delete(pcbData);

	DRIVE_LAYOUT_INFORMATION_EX *driveLayoutInfo = new DRIVE_LAYOUT_INFORMATION_EX[4096];
	
	DWORD BytesReturned;
	OVERLAPPED  Overlapped;
	b = DeviceIoControl(
		hFile,
		IOCTL_DISK_GET_DRIVE_LAYOUT_EX, // dwIoControlCode
		NULL,                           // lpInBuffer
		0,                              // nInBufferSize
		driveLayoutInfo,           // output buffer
		sizeof(*driveLayoutInfo)*4096,         // size of output buffer
		&BytesReturned,      // number of bytes returned
		NULL );  // OVERLAPPED structure


	printf("PartitionCount: %d\n",driveLayoutInfo->PartitionCount);
	printf("PartitionStyle: %d\n",driveLayoutInfo->PartitionStyle);
	printf("--------------\n");
	for (int i = 0; i < driveLayoutInfo->PartitionCount; i++)
	{
		printf("パーティション番号:%d\n", i);
		printf("%d\n", driveLayoutInfo->PartitionEntry[i].PartitionStyle);

	}
	

	io.FileClose(&hFile);

	return 0;
}

