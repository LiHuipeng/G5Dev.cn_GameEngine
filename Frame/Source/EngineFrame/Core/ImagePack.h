#pragma once
#include "DpkFileDefine.h"
#include "EngineResource.h"
#include "IEngineRender.h"


class CImagePack : public CEngineResource
{
public:
	CImagePack(void);
	virtual ~CImagePack(void);
	virtual BOOL Draw(int nItemIndex, const SEngineDrawParam* pDrawParam) const;
	virtual BOOL GetItemSize(int nItemIndex, SIZE& ItemSize) const;
	virtual int  GetItemCount(void) const;

	BOOL Load(LPCWSTR szPackPath, CEngineResource* pDpkResource, IEngineRender* pRender);

private:
	STileImageInfo*		m_pTileImageInfo;
	int					m_nTileImageCount;
	std::vector<IEngineImage*>	m_ImageVector;
};

