#pragma once
#include "EngineResource.h"
#include "ImagePack.h"

class CEngineFrameImp;

class CAnimation : public CEngineResource
{
public:
	int			m_nPlayCurrentAniFrame;

public:
	CAnimation(void);
	virtual ~CAnimation(void);
	virtual BOOL Draw(int nItemIndex, const SEngineDrawParam* pDrawParam) const;
	virtual void Play(BOOL bLoop);
	virtual void Stop();
	virtual void Update();

	BOOL Load(LPCWSTR szItemPath, int nDpkIndex, CEngineFrameImp * pEngine);
	BOOL IsPlaying() const { return m_bIsPlaying; }

private:
	int		m_nPlaySingleFrameUpdateCount;	// ��ǰ֡���´���
	int		m_nAnimationFrameStep;			// ���ڶ���֮֡��Ĳ���
	BOOL	m_bIsPlaying;
	BOOL	m_bLoop;

	CEngineFrameImp*	m_pEngine;
	int		m_nResIndex;
};
