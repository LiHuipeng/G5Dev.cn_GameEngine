// huipeng_lhp@163.com create at 2013-09-23
#pragma once

#include "IGuiFrame.h"
#include "MemIniFile.h"


class CGuiFrameImp : public IGuiFrame
{
public:
	CGuiFrameImp();
	~CGuiFrameImp();

protected:
	virtual void Release();
	virtual BOOL Create(IEngineFrame* pEngine, IGuiWndCore* pMainWnd, int nFontIndex, int nDpkIndex);
	virtual void GetEnginePtr(IEngineFrame** pEngine) const;
	virtual void Update(const SEngineInputState* pInputState);
	virtual void Render() const;
	virtual void SwitchEditorMode();
	virtual void SetFocus(IGuiWndCore* pWnd);
	virtual void SetCapture(IGuiWndCore* pWnd);
	virtual BOOL IsEditorModeOn() const;
	virtual DWORD GetTotalFrames() const;
	virtual int  GetFontIndex() const;

protected:
	BOOL LoadIni();
	void LoadIniProc(CMemIniFile* pIniFile, IGuiWndCore* pWnd);
	void SaveIni();
	void SaveIniProc(CMemIniFile* pIniFile, IGuiWndCore* pWnd);

private:
	IEngineFrame*	m_pEngine;
	IGuiWndCore*	m_pMainWnd;
	IGuiWndCore*	m_pCapturedWnd;
	IGuiWndCore*	m_pFocusedWnd;
	BOOL			m_bEditorModeOn;
	int				m_nFontIndex;
	int				m_nDpkIndex;
	DWORD			m_dwTotalFrames;
};
