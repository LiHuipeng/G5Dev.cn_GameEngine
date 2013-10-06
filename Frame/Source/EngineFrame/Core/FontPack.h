#pragma once
#include "EngineResource.h"

const int FONT_COUNT = 0xffff;

#pragma pack(1)

struct SFontCharInfo
{
	char image;
	unsigned short x;
	unsigned short y;
};

#pragma pack()


class CFontPack : public CEngineResource
{
public:
	CFontPack(void);
	virtual ~CFontPack(void);
	virtual BOOL Draw(LPCWSTR szText, const SEngineDrawParam* pDrawParam) const;
	BOOL Load(LPCWSTR szItemPath, int nDpkIndex, IEngineFrame * pEngine);

private:
	IEngineFrame*	m_pEngine;
	SFontCharInfo*	m_pFontInfo;
	unsigned char	m_nFontWidth;
	unsigned char	m_nFontHeight;	
	int m_nResIndex;
};

