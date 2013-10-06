#pragma once
#include "FaceWnd.h"


class CMainWnd : public CGuiWnd
{
public:
	CMainWnd(void);
	virtual ~CMainWnd(void);

protected:
	virtual void OnCreate();
	virtual void OnDrawBackground(int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex) const;

public:
	CFaceWnd		m_FaceWnd;
};

extern CMainWnd g_MainWnd;
