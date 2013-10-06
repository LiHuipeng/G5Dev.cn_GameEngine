#pragma once
#include "GuiWnd.h"


class CGuiButton : public CGuiWnd
{
public:
	CGuiButton(void);
	virtual ~CGuiButton(void);
// 	BOOL GetCheck() const { return m_bChecked; }
// 	void SetCheck(BOOL bCheck) { m_bChecked=bCheck; }

protected:
	virtual void OnDrawBackground(int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex) const;

private:
};
