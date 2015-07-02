#include "stdafx.h"
#include "DiskVolume.h"
#include "Log.h"



CDiskVolume::CDiskVolume()
{
	SetPath(L"E:\\dummy.txt");
}

void CDiskVolume::SetPath(TCHAR *_path)
{
	swprintf_s(path, MAX_PATH, _path );
	_wsplitpath_s( path, drive, MAX_PATH, NULL, 0, NULL, 0, NULL, 0 );
	wcscat(drive, L"\\");
}

// 指定されたドライブの情報を取得する
bool CDiskVolume::GetDiskVolumeInfo(TCHAR *_path)
{
	if (_path != NULL)
	{
		SetPath(_path);
	}

	// ダミーファイルを削除する
	DeleteFile(path);

	// ログ
	CLog log;

	// 取得処理
	BOOL bSuccess = GetDiskFreeSpaceEx(
		drive,
		(PULARGE_INTEGER)&FreeBytesAvailable,    // 呼び出し側が利用できるバイト数
		(PULARGE_INTEGER)&TotalNumberOfBytes,    // ディスク全体のバイト数
		(PULARGE_INTEGER)&TotalNumberOfFreeBytes // ディスク全体の空きバイト数
		);
	log.AddResult(L"ディスク容量取得", bSuccess != 0);

	if (bSuccess == 0)
	{
		return false;
	}

	log.Add(L"\tディスクチェックするドライブ", drive);
	log.Add(L"\t呼び出し側が利用できるバイト数", FreeBytesAvailable);
	log.Add(L"\tディスク全体のバイト数", TotalNumberOfBytes);
	log.Add(L"\tディスク全体の空きバイト数",TotalNumberOfFreeBytes);
	log.Add(L"----------");

	bSuccess = GetDiskFreeSpace(
		drive,					  // ルートパス
		&SectorsPerCluster,     // クラスタ当たりのセクタ数
		&BytesPerSector,        // セクタ当たりのバイト数
		&NumberOfFreeClusters,  // 空きクラスタ数
		&TotalNumberOfClusters  // 全クラスタ数
		);

	log.AddResult(L"GetDiskFreeSpace", bSuccess != 0);
	log.Add(L"\tクラスタ当たりのセクタ数", SectorsPerCluster);
	log.Add(L"\tセクタ当たりのバイト数", BytesPerSector);
	log.Add(L"\t空きクラスタ数",NumberOfFreeClusters);
	log.Add(L"\t全クラスタ数",TotalNumberOfClusters);
	log.Add(L"----------");




	if (bSuccess == 0)
	{
		return false;
	}


	TCHAR VolumeNameBuffer[_MAX_PATH];        // ボリューム名バッファ
	DWORD VolumeSerialNumber;     // ボリュームのシリアル番号
	DWORD MaximumComponentLength; // ファイル名の最大の長さ
	DWORD FileSystemFlags;        // ファイルシステムのオプション
	TCHAR FileSystemNameBuffer[_MAX_PATH];    // ファイルシステム名を格納するバッファ


	// 指定されたルートディレクトリのファイルシステムとボリュームに関する情報を取得します。
	bSuccess = GetVolumeInformation(
		drive,           // ルートディレクトリ
		VolumeNameBuffer,        // ボリューム名バッファ
		sizeof(VolumeNameBuffer),            // 名前バッファのサイズ
		&VolumeSerialNumber,     // ボリュームのシリアル番号
		&MaximumComponentLength, // ファイル名の最大の長さ
		&FileSystemFlags,        // ファイルシステムのオプション
		FileSystemNameBuffer,    // ファイルシステム名を格納するバッファ
		sizeof(FileSystemNameBuffer)	// ファイルシステム名を格納するバッファのサイズ
		);
	log.AddResult(L"GetVolumeInformation", bSuccess != 0);
	log.Add(L"\tボリューム名",VolumeNameBuffer);
	log.Add(L"\tボリュームのシリアル番号", VolumeSerialNumber);
	log.Add(L"\tファイル名の最大の長さ",MaximumComponentLength);
	log.Add(L"\tファイルシステムのオプション",FileSystemFlags);
	log.Add(L"\tファイルシステム名", FileSystemNameBuffer);
	log.Add(L"----------");

	if (bSuccess == 0)
	{
		return false;
	}

	bSupportsSparseFiles = FileSystemFlags & FILE_SUPPORTS_SPARSE_FILES;


	return true;
}


TCHAR * CDiskVolume::GetPath()
{
	return path;
}



TCHAR * CDiskVolume::GetDrive()
{
	return drive;
}

DWORD CDiskVolume::GetBytesPerSector()
{
	return BytesPerSector;
}

DWORD CDiskVolume::GetSectorsPerCluster()
{
	return SectorsPerCluster;
}


DWORD CDiskVolume::GetNumberOfFreeClusters()
{
	return NumberOfFreeClusters;
}

unsigned __int64 CDiskVolume::GetTotalNumberOfBytes()
{
	return TotalNumberOfBytes;
}

unsigned __int64 CDiskVolume::GetTotalNumberOfFreeBytes()
{
	return TotalNumberOfFreeBytes;
}

bool CDiskVolume::IsSupportsSparseFiles()
{
	return bSupportsSparseFiles;
}

