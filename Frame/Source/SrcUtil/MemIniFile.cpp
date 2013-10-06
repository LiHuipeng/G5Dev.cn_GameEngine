#include "stdafx.h"
#include "MemIniFile.h"

#include "MacroUtility.h"
#include "MemoryCheck.h"

int StrFind( LPCWSTR pStr, WCHAR chr )
{
	int iSize = _tcslen(pStr);
	for (int i=0; i<iSize; i++)
	{
		if (pStr[i] == chr)
		{
			return i;
		}
	}
	return -1;
}


CMemIniFile::CMemIniFile(void)
{
	m_pIniNode = NULL;
}

CMemIniFile::~CMemIniFile(void)
{
	Destroy();
}

// BOOL CMemIniFile::Load( LPCWSTR strFilePath )
// {
// 	HANDLE hRead;
// 	hRead = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, 
// 		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 	if (hRead == INVALID_HANDLE_VALUE)
// 	{
// 		return FALSE;			
// 	}
// 	//判断是否是Unicode文件
// 	BYTE	strBuf[2]={0};
// 	DWORD   nRead;
// 	ReadFile(hRead,strBuf,2,&nRead,NULL);
// 	if(strBuf[0]!=0xff && strBuf[1]!=0xfe)
// 	{
// 		CloseHandle(hRead);
// 		return FALSE;
// 	}
// 	//读取文件信息
// 	int nSize = GetFileSize(hRead, NULL);
// 	BYTE* pData = new BYTE[nSize];	
// 	ZeroMemory(pData, nSize);
// 	DWORD dwReadSize = 0;
// 	BOOL bRead = ReadFile(hRead, pData, nSize, &dwReadSize, NULL);
// 	
//  	Load((LPCVOID)pData, dwReadSize);
// 
// 	CloseHandle(hRead);
// 	SAFE_DELETE_ARRAY(pData);
// 	return bRead;
// }

BOOL CMemIniFile::Load( LPCVOID pFileData, DWORD dwFileSize )
{
	BOOL bRet = FALSE;
	if (0 == dwFileSize)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = ParseIni((LPCWSTR)pFileData, dwFileSize/sizeof(WCHAR));
	}
	return bRet;
}

BOOL CMemIniFile::ParseIni( LPCWSTR szIniText, DWORD dwSize )
{
	WCHAR szSection[MAX_TEXT_LENGTH] = {};
	WCHAR szKey[MAX_TEXT_LENGTH] = {};
	WCHAR szValue[MAX_TEXT_LENGTH] = {};
	int nBegin = 0;
	int nEnd = 0;

	DWORD dwIndex = 0;
	for (DWORD i=0; i<dwSize-1; i++)
	{
		if (szIniText[i]=='\r' && szIniText[i+1]=='\n')
		{
			i++;
			WCHAR szOneLineText[MAX_TEXT_LENGTH] = {};
			_tcsncpy_s(szOneLineText, MAX_TEXT_LENGTH, szIniText+dwIndex, i-dwIndex);
			dwIndex = i;

			nBegin = StrFind(szOneLineText, '[');
			nEnd = StrFind(szOneLineText, ']');
			if (nBegin!=-1 && nEnd!=-1)		//Section
			{
				ZeroMemory(szSection, MAX_TEXT_LENGTH);
				_tcsncpy_s(szSection, MAX_TEXT_LENGTH, szOneLineText+nBegin+1, nEnd-2);

				CreateNewSectionNode(szSection);
			}
			else	//pair(key, value)
			{
				if (!m_pIniNode)
				{
					return FALSE;
				}

				int len = _tcslen(szSection);
				if (len == 0) continue ;

				ZeroMemory(szKey, sizeof(WCHAR)*MAX_TEXT_LENGTH);
				ZeroMemory(szValue, sizeof(WCHAR)*MAX_TEXT_LENGTH);
				nBegin = StrFind(szOneLineText, '=');
				if (nBegin != -1)
				{
					_tcsncpy_s(szKey, MAX_TEXT_LENGTH, szOneLineText+1, nBegin-1);
					nEnd = StrFind(szOneLineText, '\n');
					_tcsncpy_s(szValue, MAX_TEXT_LENGTH, szOneLineText+nBegin+1, nEnd-2);
					CreateNewKeyNode(szKey, szValue);
				}
			}
		}
	}

	return TRUE;
}

void CMemIniFile::Save( LPCWSTR strFilePath )
{
#ifdef _WIN32

	HANDLE hFile;
	hFile=CreateFile(strFilePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	DWORD dwUHead=0xFEFF;
	DWORD dwWrite=0;
	WriteFile(hFile, &dwUHead, 2, &dwWrite, NULL);
	ASSERT(dwWrite == 2);
	
	SMemIniSectionNode* pTmpSection = m_pIniNode;
	SMemIniKeyNode* pTmpKey = NULL;
	WCHAR szText[MAX_TEXT_LENGTH] = {};
	while (pTmpSection)
	{
		ZeroMemory(szText, sizeof(WCHAR)*MAX_TEXT_LENGTH);
		_stprintf_s(szText, MAX_TEXT_LENGTH, _T("[%s]\r\n"), pTmpSection->szSectionValue);
		WriteFile(hFile, szText, _tcslen(szText)*sizeof(WCHAR), &dwWrite, NULL);
		pTmpKey = pTmpSection->pKeyNode;
		pTmpSection = pTmpSection->pSibling;

		while (pTmpKey)
		{
			ZeroMemory(szText, sizeof(WCHAR)*MAX_TEXT_LENGTH);
			_stprintf_s(szText, MAX_TEXT_LENGTH, _T("%s=%s\r\n"), pTmpKey->szKey, pTmpKey->szValue);
			WriteFile(hFile, szText, _tcslen(szText)*sizeof(WCHAR), &dwWrite, NULL);
			pTmpKey = pTmpKey->pSibling;
		}
	}

	CloseHandle(hFile);

#endif // _WIN32
}

BOOL CMemIniFile::SetValue( LPCWSTR szSection, LPCWSTR szKey, LPCWSTR szValue )	//SetStringValue
{
	//szSection
	SMemIniSectionNode* pTmpSection = LocateSection(szSection);
	if (!pTmpSection)		//m_pIniNode is nullptr
	{
		pTmpSection = CreateNewSectionNode(szSection);
	}
	ASSERT(pTmpSection);

	//pair(szKey, szValue)
	SMemIniKeyNode* pTmpKey = LocateKey(pTmpSection, szKey);
	if (NULL == pTmpKey)
	{
		pTmpKey = CreateNewKeyNode(szKey, szValue);
	}
	ASSERT(pTmpKey);

	return TRUE;
}

BOOL CMemIniFile::SetValue( LPCWSTR szSection, LPCWSTR szKey, int nValue )
{
	WCHAR szValue[MAX_TEXT_LENGTH] = {};
	_stprintf_s(szValue, MAX_TEXT_LENGTH, _T("%d"), nValue);

	return SetValue(szSection, szKey, szValue);
}

//确定在原ini节点中没有该szSection时，建立新节点
SMemIniSectionNode* CMemIniFile::CreateNewSectionNode( LPCWSTR szSection )
{
	SMemIniSectionNode* pNewSection = new SMemIniSectionNode;
	ZeroMemory(pNewSection, sizeof(SMemIniSectionNode));
	_tcsncpy_s(pNewSection->szSectionValue, MAX_TEXT_LENGTH, szSection, _tcslen(szSection));
	pNewSection->pKeyNode = NULL;
	pNewSection->pSibling = NULL;

	SMemIniSectionNode* pTmpSection = m_pIniNode;
	if (pTmpSection)	//m_pIniNode is`t nullptr
	{
		while(pTmpSection->pSibling)
		{
			pTmpSection = pTmpSection->pSibling;
		}
		pTmpSection->pSibling = pNewSection;
	}
	else
	{
		m_pIniNode = pNewSection;
	}

	return pNewSection;
}

SMemIniKeyNode* CMemIniFile::CreateNewKeyNode( LPCWSTR szKey, LPCWSTR szValue )
{

	SMemIniKeyNode* pNewKeyNode = new SMemIniKeyNode;
	ZeroMemory(pNewKeyNode, sizeof(SMemIniKeyNode));
	_tcsncpy_s(pNewKeyNode->szKey, MAX_TEXT_LENGTH, szKey, _tcslen(szKey));
	_tcsncpy_s(pNewKeyNode->szValue, MAX_TEXT_LENGTH, szValue, _tcslen(szValue));
	pNewKeyNode->pSibling = NULL;

	SMemIniSectionNode* pTmpSection = m_pIniNode;
	while(pTmpSection->pSibling)
	{
		pTmpSection = pTmpSection->pSibling;
	}
	SMemIniKeyNode*	pTmpKey = pTmpSection->pKeyNode;
	if (pTmpKey)				//The SectionNode have yet KeyNode
	{
		while(pTmpKey->pSibling)
		{
			pTmpKey = pTmpKey->pSibling;
		}
		pTmpKey->pSibling = pNewKeyNode;
	}
	else
	{
		pTmpSection->pKeyNode = pNewKeyNode;
	}

	return pNewKeyNode;
}

BOOL CMemIniFile::GetValue( LPCWSTR szSection, LPCWSTR szKey, LPWSTR szValue, DWORD dwValueSize )
{
	SMemIniSectionNode* pTmpSection = LocateSection(szSection);
	if (!pTmpSection)
	{
		return FALSE;
	}

	SMemIniKeyNode* pTmpKey = LocateKey(pTmpSection, szKey);
	if (!pTmpKey)
	{
		return FALSE;
	}

	_tcsncpy_s(szValue, dwValueSize, pTmpKey->szValue, MAX_TEXT_LENGTH);
	return TRUE;
}


int CMemIniFile::GetValue( LPCWSTR szSection, LPCWSTR szKey, int nDefault )
{
	WCHAR szValue[MAX_TEXT_LENGTH] = {};
	BOOL b=GetValue(szSection, szKey, szValue, MAX_TEXT_LENGTH);
	if(!b) return nDefault;
	return _ttoi(szValue);
}

SMemIniSectionNode* CMemIniFile::LocateSection( LPCWSTR szSection )
{
	SMemIniSectionNode* pTmpSection = NULL;
	if (!m_pIniNode)		//m_pIniNode is nullptr
	{
		return NULL;
	}
	else
	{
		pTmpSection = m_pIniNode;
		while(pTmpSection)
		{
			if (0 == _tcscmp(pTmpSection->szSectionValue, szSection))	
			{
				break;
			}
			pTmpSection = pTmpSection->pSibling;
		}
	}

	return pTmpSection;
}

SMemIniKeyNode* CMemIniFile::LocateKey( SMemIniSectionNode* pIniSection, LPCWSTR szKey )
{
	ASSERT(pIniSection);
	SMemIniKeyNode* pTmpKey = pIniSection->pKeyNode;
	while(pTmpKey)
	{
		if (0 == _tcscmp(pTmpKey->szKey, szKey))
		{
			break;
		}
		pTmpKey = pTmpKey->pSibling;
	}

	return pTmpKey;
}

void CMemIniFile::Destroy()
{
	SMemIniSectionNode* pTmpSectionTrace = m_pIniNode;
	SMemIniSectionNode* pTmpSection = NULL;
	SMemIniKeyNode*	pTmpKeyTrace = NULL;
	SMemIniKeyNode*	pTmpKey = NULL;

	while(pTmpSectionTrace)
	{
		pTmpSection = pTmpSectionTrace;
		pTmpSectionTrace = pTmpSectionTrace->pSibling;

		pTmpKeyTrace = pTmpSection->pKeyNode;
		while(pTmpKeyTrace)
		{
			pTmpKey = pTmpKeyTrace;
			pTmpKeyTrace = pTmpKeyTrace->pSibling;

			SAFE_DELETE(pTmpKey);
		}
		SAFE_DELETE(pTmpSection);
	}
}

unsigned int CMemIniFile::GetSaveSize() const
{
	unsigned int nIniSize = 0;
	SMemIniSectionNode* pTmpSection = m_pIniNode;
	SMemIniKeyNode* pTmpKey = NULL;
	WCHAR szText[MAX_TEXT_LENGTH];
	while (pTmpSection)
	{
		ZeroMemory(szText, sizeof(WCHAR)*MAX_TEXT_LENGTH);
		_stprintf_s(szText, MAX_TEXT_LENGTH, _T("[%s]\r\n"), pTmpSection->szSectionValue);
		pTmpKey = pTmpSection->pKeyNode;
		pTmpSection = pTmpSection->pSibling;

		nIniSize += _tcslen(szText)*sizeof(WCHAR);

		while (pTmpKey)
		{
			ZeroMemory(szText, sizeof(WCHAR)*MAX_TEXT_LENGTH);
			_stprintf_s(szText, MAX_TEXT_LENGTH, _T("%s=%s\r\n"), pTmpKey->szKey, pTmpKey->szValue);
			pTmpKey = pTmpKey->pSibling;

			nIniSize += _tcslen(szText)*sizeof(WCHAR);
		}

		if (pTmpSection)	//如果不是最后一个片段
		{
			ZeroMemory(szText, sizeof(WCHAR)*MAX_TEXT_LENGTH);
			_stprintf_s(szText, MAX_TEXT_LENGTH, _T("\r\n"));
			nIniSize += _tcslen(szText)*sizeof(WCHAR);
		}
	}

	nIniSize += 2;	//unicode头

	return nIniSize;
}

BOOL CMemIniFile::Save( BYTE* pMem, unsigned int nMemSize )
{
	unsigned int nIniSize = GetSaveSize();
	if (nIniSize > nMemSize)
	{
		return FALSE;
	}

	ZeroMemory(pMem, nMemSize);

	BYTE* pMemTmp = pMem;
	unsigned int nMemIndex = 0;
	unsigned int nLen = 0;

	{
		unsigned char UnicodeMark[2]={0xff,0xfe};
		nLen = 2;
		memcpy_s(pMemTmp+nMemIndex, nMemSize-nMemIndex, UnicodeMark, nLen);
		nMemIndex += nLen;
		ASSERT(nMemIndex < nMemSize);
	}


	SMemIniSectionNode* pTmpSection = m_pIniNode;
	SMemIniKeyNode* pTmpKey = NULL;
	WCHAR szText[MAX_TEXT_LENGTH] = {};
	while (pTmpSection)
	{
		ZeroMemory(szText, sizeof(WCHAR)*MAX_TEXT_LENGTH);
		_stprintf_s(szText, MAX_TEXT_LENGTH, _T("[%s]\r\n"), pTmpSection->szSectionValue);
		pTmpKey = pTmpSection->pKeyNode;
		pTmpSection = pTmpSection->pSibling;

		nLen = _tcslen(szText)*sizeof(WCHAR);
		memcpy_s(pMemTmp+nMemIndex, nMemSize-nMemIndex, szText, nLen);
		nMemIndex += nLen;
		ASSERT(nMemIndex < nMemSize);
	
		while (pTmpKey)
		{
			ZeroMemory(szText, sizeof(WCHAR)*MAX_TEXT_LENGTH);
			_stprintf_s(szText, MAX_TEXT_LENGTH, _T("%s=%s\r\n"), pTmpKey->szKey, pTmpKey->szValue);
			pTmpKey = pTmpKey->pSibling;

			nLen = _tcslen(szText)*sizeof(WCHAR);
			memcpy_s(pMemTmp+nMemIndex, nMemSize-nMemIndex, szText, nLen);
			nMemIndex += nLen;
			ASSERT(nMemIndex <= nMemSize);
		}


		if (pTmpSection)	//如果不是最后一个片段
		{
			ZeroMemory(szText, sizeof(WCHAR)*MAX_TEXT_LENGTH);
			_stprintf_s(szText, MAX_TEXT_LENGTH, _T("\r\n"));
			nLen = _tcslen(szText)*sizeof(WCHAR);
			memcpy_s(pMemTmp+nMemIndex, nMemSize-nMemIndex, szText, nLen);
			nMemIndex += nLen;
			ASSERT(nMemIndex <= nMemSize);
		}
	}

	return TRUE;
}

