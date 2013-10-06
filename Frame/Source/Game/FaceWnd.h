#pragma once
#include "GuiButton.h"
#include "GuiEdit.h"


class CFaceWnd : public CGuiWnd
{
public:
	CFaceWnd(void);
	~CFaceWnd(void);

protected:
	virtual void OnCreate();
	virtual void OnDrawBackground(int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex) const;
	virtual void OnButtonClicked(int nWindowID);

public:
	CGuiButton	m_OKBtn;
	CGuiButton	m_CancelBtn;
	CGuiEdit	m_FaceEdit;

	BOOL		m_bShowFace;
};

