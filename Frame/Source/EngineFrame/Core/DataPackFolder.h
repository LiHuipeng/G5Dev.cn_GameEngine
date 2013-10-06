#pragma once
#include "EngineResource.h"
#include "ISysFunction.h"
#include "ISysFunction.h"


class CDataPackFolder : public CEngineResource
{
public:
	CDataPackFolder();
	~CDataPackFolder();
	BOOL Open(ISysFile* pFile, LPCWSTR szDpkPath);

	virtual BOOL ReadItemData(LPCWSTR szItemPath, LPVOID pItemData, DWORD dwItemSize);
	virtual BOOL WriteItemData(LPCWSTR szItemPath, LPCVOID pItemData, DWORD dwItemSize);
	virtual BOOL IsItemExists(LPCWSTR szItemPath, BOOL& bItemExists) const;
	virtual BOOL GetItemSize(LPCWSTR szItemPath, DWORD& dwItemSize) const;

private:
	WCHAR		m_szRootPath[MAX_PATH];
	ISysFile*	m_pFile;
};

