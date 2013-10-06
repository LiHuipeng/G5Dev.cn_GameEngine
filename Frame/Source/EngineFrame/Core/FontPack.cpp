#include "stdafx.h"
#include "FontPack.h"

#include "MacroUtility.h"


CFontPack::CFontPack(void)
{
	m_pEngine=NULL;
	m_nResIndex=-1;
	m_nFontWidth=0;
	m_nFontHeight=0;
	m_pFontInfo=NULL;
	m_pFontInfo=new SFontCharInfo[FONT_COUNT];
	ZeroMemory(m_pFontInfo, sizeof(SFontCharInfo)*FONT_COUNT);
}


CFontPack::~CFontPack(void)
{
	SAFE_DELETE_ARRAY(m_pFontInfo);

	ASSERT(m_pEngine);
	m_pEngine->ResourceRelease(m_nResIndex);
	m_nResIndex=-1;
}


BOOL CFontPack::Load( LPCWSTR szItemPath, int nDpkIndex, IEngineFrame * pEngine )
{
	ASSERT(pEngine);
	m_pEngine=pEngine;
	if(!m_pEngine->ImagePackCreate(m_nResIndex, szItemPath, nDpkIndex)) return FALSE;

	// 获得 Font.bin 文件正文
	WCHAR path[MAX_PATH];
	_stprintf_s(path, _T("%s\\Font.bin"), szItemPath);
	DWORD dwItemSize=0;
	if(!pEngine->ResourceGetItemSize(nDpkIndex, path, dwItemSize)) return FALSE;
	BYTE* pItemData=new BYTE[dwItemSize];
	if(!pEngine->ResourceReadItemData(nDpkIndex, path, pItemData, dwItemSize)) { delete[] pItemData; return FALSE; }

	// 读取 Font.bin 内容
	m_nFontWidth=*pItemData;
	m_nFontHeight=*(pItemData+1);
	memcpy(m_pFontInfo, pItemData+2, dwItemSize-2);

	SAFE_DELETE_ARRAY(pItemData);
	return TRUE;
}


BOOL CFontPack::Draw( LPCWSTR szText, const SEngineDrawParam* pDrawParam ) const
{
	int nTextCount=_tcslen(szText);
	int w=pDrawParam->w;
	int h=pDrawParam->h;
	if(!w) w=m_nFontWidth*nTextCount;
	if(!h) h=m_nFontHeight;

	int nPosX = 0;
	int nPosY = 0;
	int nIndex = 0;
	SEngineDrawParam dp;
	while (szText[nIndex])
	{
		ZeroMemory(&dp, sizeof(SEngineDrawParam));

		if (nPosX+m_nFontWidth > w)
		{
			nPosX = 0;
			nPosY += m_nFontHeight;
			if (nPosY+m_nFontHeight > h)
			{
				return FALSE;
			}
		}

		//Draw
		dp.flag=pDrawParam->flag;
		dp.color=pDrawParam->color;
		dp.x = nPosX+pDrawParam->x;
		dp.y = nPosY+pDrawParam->y;
		dp.w = m_nFontWidth;
		dp.h = m_nFontHeight;
		dp.sx = m_pFontInfo[szText[nIndex]].x;
		dp.sy = m_pFontInfo[szText[nIndex]].y;
		dp.sw = m_nFontWidth;
		dp.sh = m_nFontHeight;
		m_pEngine->ResourceDraw(m_nResIndex, m_pFontInfo[szText[nIndex]].image, &dp);

		if(szText[nIndex]<127)
			nPosX += m_nFontWidth/2;	// 半角字
		else
			nPosX += m_nFontWidth;		// 全角字
		nIndex++;
	}

	return TRUE;
}
