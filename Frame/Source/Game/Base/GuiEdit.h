#pragma once
#include "GuiWnd.h"


class CGuiEdit : public CGuiWnd
{
public:
	CGuiEdit(void);
	virtual ~CGuiEdit(void);

protected:
	virtual void OnDrawBackground(int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex) const;

private:
};
