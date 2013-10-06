#pragma once

#define MAX_TEXT_LENGTH 100

struct SMemIniKeyNode			//存储key值对结构
{
	WCHAR szKey[MAX_TEXT_LENGTH];
	WCHAR szValue[MAX_TEXT_LENGTH];
	SMemIniKeyNode* pSibling;
};

struct SMemIniSectionNode		//存储section值
{
	WCHAR szSectionValue[MAX_TEXT_LENGTH];
	SMemIniSectionNode* pSibling;
	SMemIniKeyNode* pKeyNode;
};

class CMemIniFile
{
public:
	CMemIniFile(void);
	~CMemIniFile(void);	

	BOOL Load(LPCVOID pFileData, DWORD dwFileSize);
	void Save( LPCWSTR strFilePath );
	BOOL SetValue(LPCWSTR szSection, LPCWSTR szKey, LPCWSTR szValue);
	BOOL SetValue(LPCWSTR szSection, LPCWSTR szKey, int nValue);
	BOOL GetValue(LPCWSTR szSection, LPCWSTR szKey, LPWSTR szValue, DWORD dwValueSize);
	int  GetValue(LPCWSTR szSection, LPCWSTR szKey, int nDefault);
	unsigned int GetSaveSize() const; 
	BOOL Save(BYTE* pMem, unsigned int nMemSize);

protected:
	void Destroy();
	BOOL ParseIni( LPCWSTR szIniText, DWORD dwSize );
	SMemIniSectionNode*	CreateNewSectionNode(LPCWSTR szSection);
	SMemIniKeyNode*	CreateNewKeyNode(LPCWSTR szKey, LPCWSTR szValue);
	SMemIniSectionNode* LocateSection(LPCWSTR szSection);
	SMemIniKeyNode*	LocateKey(SMemIniSectionNode* pIniSection, LPCWSTR szKey);

private:
	SMemIniSectionNode*	m_pIniNode;
};
