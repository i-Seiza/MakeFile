#pragma once
#include "windows.h"

// �f�B�X�N�e�ʂɂ��Ă̏������s��
class CDiskVolume
{
public:
	CDiskVolume();
	bool GetDiskVolumeInfo(TCHAR *_path = NULL);
	DWORD GetBytesPerSector();
	TCHAR *GetPath();

	DWORD GetSectorsPerCluster();
	DWORD GetNumberOfFreeClusters();

	unsigned __int64 GetTotalNumberOfFreeBytes();
	bool IsSupportsSparseFiles();

protected:
private:

	void SetPath( TCHAR *_path );



	TCHAR path[_MAX_PATH];
	TCHAR drive[_MAX_PATH];

	DWORD SectorsPerCluster;     // �N���X�^������̃Z�N�^��
	DWORD BytesPerSector;        // �Z�N�^������̃o�C�g��
	DWORD NumberOfFreeClusters;  // �󂫃N���X�^��
	DWORD TotalNumberOfClusters;  // �S�N���X�^��

	unsigned __int64  FreeBytesAvailable;    // �Ăяo���������p�ł���o�C�g��
	unsigned __int64  TotalNumberOfBytes;    // �f�B�X�N�S�̂̃o�C�g��
	unsigned __int64  TotalNumberOfFreeBytes; // �f�B�X�N�S�̂̋󂫃o�C�g��

	bool bSupportsSparseFiles;	// SparseFile���T�|�[�g���Ă��邩

};