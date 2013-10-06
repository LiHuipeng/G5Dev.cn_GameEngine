#pragma once
#include "IGuiFrame.h"


class CGuiWnd : public IGuiWndShell
{
public:
	CGuiWnd();
	virtual ~CGuiWnd();

public:
	IGuiWndCore*	m_pCore;

// IGuiWndShell implement
protected:
	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDrawForeground(int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex) const;
	virtual void OnDrawBackground(int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex) const;
	virtual void OnButtonClicked(int nWindowID);

// IGuiWndCore implement
public:
	virtual void Create(int nWindowID, IGuiWndCore* pParent, int nResIndex);
	virtual void SetWindowPos(int x, int y);
};
