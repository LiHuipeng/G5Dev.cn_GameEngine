// 文件由 huipeng_lhp@163.com 创建于 2013-09-18
// 本文件内容只服务于win32环境

#include "stdafx.h"
#include "DataPackFolder.h"

#include "MacroUtility.h"
#include "MemoryCheck.h"


CDataPackFolder::CDataPackFolder()
{
	ZeroMemory(m_szRootPath, sizeof(m_szRootPath));
	m_pFile=SysFileCreate(SYSFUNCTION_VERSION);
}

CDataPackFolder::~CDataPackFolder()
{
	SAFE_RELEASE(m_pFile);
}

BOOL CDataPackFolder::Open( ISysFile* pFile, LPCWSTR szDpkPath )
{
	if(!pFile->IsFileExists(szDpkPath)) return FALSE;

	_tcscpy_s(m_szRootPath, szDpkPath);

	return TRUE;
}

BOOL CDataPackFolder::ReadItemData(LPCWSTR szItemPath, LPVOID pItemData, DWORD dwItemSize)
{
	ASSERT(szItemPath[0]!=_T('\\'));
	WCHAR path[MAX_PATH];
	_stprintf_s(path, _T("%s\\%s"), m_szRootPath, szItemPath);
	if(!m_pFile->Open(path, TRUE)) return FALSE;

	ASSERT(pItemData);
	ASSERT(dwItemSize);
	DWORD dwBytesRead;
	m_pFile->Read(pItemData, dwItemSize, &dwBytesRead);
	ASSERT(dwItemSize==dwBytesRead);

	m_pFile->Close();
	return TRUE;
}

BOOL CDataPackFolder::WriteItemData(LPCWSTR szItemPath, LPCVOID pItemData, DWORD dwItemSize)
{
	ASSERT(szItemPath[0]!=_T('\\'));
	WCHAR path[MAX_PATH];
	_stprintf_s(path, _T("%s\\%s"), m_szRootPath, szItemPath);
	if(!m_pFile->Open(path, FALSE)) return FALSE;

	ASSERT(pItemData);
	ASSERT(dwItemSize);
	DWORD dwBytesWrite;
	m_pFile->Write(pItemData, dwItemSize, &dwBytesWrite);
	ASSERT(dwItemSize==dwBytesWrite);

	m_pFile->Close();
	return TRUE;
}

BOOL CDataPackFolder::IsItemExists(LPCWSTR szItemPath, BOOL& bItemExists) const
{
	ASSERT(szItemPath[0]!=_T('\\'));
	WCHAR path[MAX_PATH];
	_stprintf_s(path, _T("%s\\%s"), m_szRootPath, szItemPath);
	bItemExists=m_pFile->IsFileExists(path);
	return TRUE;
}

BOOL CDataPackFolder::GetItemSize( LPCWSTR szItemPath, DWORD& dwItemSize ) const
{
	ASSERT(szItemPath[0]!=_T('\\'));
	WCHAR path[MAX_PATH];
	_stprintf_s(path, _T("%s\\%s"), m_szRootPath, szItemPath);
	if(!m_pFile->Open(path, TRUE)) return FALSE;
	dwItemSize=m_pFile->Size();
	m_pFile->Close();
	return TRUE;
}
