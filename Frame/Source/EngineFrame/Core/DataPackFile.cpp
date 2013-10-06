#include "stdafx.h"
#include "DataPackFile.h"
#include "Crc32.h"
#include "DpkFileDefine.h"

#include "MacroUtility.h"
#include "MemoryCheck.h"


CDataPackFile::CDataPackFile(void)
{
	ZeroMemory(m_szDpkFilePath, sizeof(WCHAR)*MAX_PATH);
	m_pFile=SysFileCreate(SYSFUNCTION_VERSION);
	m_pKey = NULL;
}


CDataPackFile::~CDataPackFile(void)
{
	SAFE_DELETE_ARRAY(m_pKey);
	SAFE_RELEASE(m_pFile);
}


BOOL CDataPackFile::Load(ISysFile* pFile, LPCWSTR szDpkPath, LPCWSTR szDpkPassword)
{
	ASSERT(pFile);
	_tcscpy_s(m_szDpkFilePath, MAX_PATH, szDpkPath);
	
	int nLen = _tcslen(szDpkPassword);
	m_pKey = new WCHAR[nLen+1];
	ZeroMemory(m_pKey, sizeof(WCHAR)*(nLen+1));
	_tcscpy_s(m_pKey, nLen+1, szDpkPassword);

	if(!pFile->Open(szDpkPath, TRUE)) // 读取模式
		return FALSE;

	DWORD dwReadSize = 0;
	int nFileCursor=0;

	SDpkFileHeader DpkFileHead;
	pFile->Read(&DpkFileHead, sizeof(DpkFileHead), &dwReadSize);
	ASSERT(sizeof(DpkFileHead)==dwReadSize);
	nFileCursor+=dwReadSize;

	if(strcmp(DpkFileHead.FileMark, "DPK4"))
		return FALSE; // 文件版本不对

	CCrc32 crc32;
	unsigned long crc = crc32.CalculateCrc32(szDpkPassword, _tcslen(szDpkPassword));
	if (DpkFileHead.PasswordCrc32 != crc)
		return FALSE; // 密码crc32不相同

	SDpkMemItem item;
	while(TRUE)
	{
		ZeroMemory(&item, sizeof(item));

		SDpkFileItem DpkFileItem;
		pFile->Read(&DpkFileItem, sizeof(DpkFileItem), &dwReadSize);
		if (0 == dwReadSize) break;

		ASSERT(sizeof(DpkFileItem)==dwReadSize);
		nFileCursor+=dwReadSize;
		item.nFileCursor = nFileCursor;

		_tcscpy_s(item.szItemPath, DpkFileItem.szItemPath);
		item.ulDataCrc32=DpkFileItem.ulDataCrc32;
		item.dwDataSize=DpkFileItem.dwDataSize;

		nFileCursor+=item.dwDataSize;
		pFile->Seek(nFileCursor);

		m_DpkMemItemList.push_back(item);
	}

	pFile->Close();
	return TRUE;
}

BOOL CDataPackFile::ReadItemData( LPCWSTR szItemPath, LPVOID pItemData, DWORD dwItemSize )
{
	ASSERT(szItemPath[0] != _T('\\'));
	BOOL bRet = TRUE;
	WCHAR szPath[MAX_PATH] = {};
	_stprintf_s(szPath, _T("\\%s"), szItemPath);

	for each(SDpkMemItem elem in m_DpkMemItemList)
	{
		if (_tcscmp(elem.szItemPath, szPath) == 0)
		{
			DWORD dwRealReadSize = 0;

			if (!m_pFile->Open(m_szDpkFilePath, TRUE)) return FALSE;
			
			m_pFile->Seek(elem.nFileCursor);
			m_pFile->Read(pItemData, dwItemSize, &dwRealReadSize);
			ASSERT(dwRealReadSize == dwItemSize);
			
			XorEncode(pItemData, dwItemSize, m_pKey);

			m_pFile->Close();
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

BOOL CDataPackFile::WriteItemData( LPCWSTR szItemPath, LPCVOID pItemData, DWORD dwItemSize )
{
	return FALSE;
}

BOOL CDataPackFile::IsItemExists( LPCWSTR szItemPath, BOOL& bItemExists ) const
{
	ASSERT(szItemPath[0] != _T('\\'));
	BOOL bRet = FALSE;
	WCHAR szPath[MAX_PATH] = {};
	_stprintf_s(szPath, _T("\\%s"), szItemPath);

	for each(SDpkMemItem elem in m_DpkMemItemList)
	{
		if (_tcsstr(elem.szItemPath, szPath)==elem.szItemPath) // 两字符串前半部分相同
		{
			bRet = TRUE;
			break;
		}
	}

	bItemExists = bRet;
	return bRet;
}

BOOL CDataPackFile::GetItemSize( LPCWSTR szItemPath, DWORD& dwItemSize ) const
{
	ASSERT(szItemPath[0] != _T('\\'));
	BOOL bRet = FALSE;
	WCHAR szPath[MAX_PATH] = {};
	_stprintf_s(szPath, _T("\\%s"), szItemPath);
	for each(SDpkMemItem elem in m_DpkMemItemList)
	{
		if (_tcscmp(elem.szItemPath, szPath) == 0)
		{
			dwItemSize = elem.dwDataSize;
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

