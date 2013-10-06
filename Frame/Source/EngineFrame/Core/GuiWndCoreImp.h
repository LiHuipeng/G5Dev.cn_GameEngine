#pragma once
#include "IGuiFrame.h"


class CGuiWndCoreImp : public IGuiWndCore
{
public:
	CGuiWndCoreImp(IGuiWndShell* pGuiWndShell);
	~CGuiWndCoreImp(void);

protected:
	virtual void Release();
	virtual void Create(IGuiFrame* pFrame, int cx, int cy);
	virtual void Create(int nWindowID, IGuiWndCore* pParent, int nResIndex);
	virtual void DoRender();

	virtual int  GetWindowID() const;
	virtual void GetWindowRect(RECT* pRect) const; // 相对于Gui容器的坐标
	virtual void GetWindowCanvasRect(RECT* pRect) const; // 相对于win32窗口的坐标
	virtual void SetWindowPos(int x, int y);	// x,y为最终到达的目标点
	virtual BOOL GetCheck() const;
	virtual void SetCheck(BOOL bCheck);

	virtual BOOL DoCursorEvent(const SEngineInputState* pInputState);
	virtual void OnCursorEvent(const SEngineInputState* pInputState);
	virtual void DoUpdate();
	virtual void OnUpdate();
	virtual BOOL IsWindowEnabled() const;
	virtual BOOL IsWindowVisible() const;
	virtual void DoButtonPress(BOOL bDown);

	virtual void AddChild(IGuiWndCore* pChildWnd);
	virtual int  GetChildCount() const;
	virtual IGuiWndCore* GetChild(int nIndex) const;

	virtual IGuiWndCore* GetParent() const;
	virtual IGuiFrame* GetGuiFrame() const;
	virtual IGuiWndShell* GetGuiWndShell();

	virtual void DrawWindow() const;
	virtual void DrawButton() const;
	virtual void DrawEdit() const;

private:
	IGuiWndShell*	m_pWndShell;
	IGuiFrame*		m_pFrame;
	IGuiWndCore*	m_pParent;
	std::vector<IGuiWndCore*>	m_Child;
	int		m_nResIndex;

	int		m_nWindowID;
	RECT	m_WndRect;
	BOOL	m_bWindowEnable;
	BOOL	m_bWindowVisible;
	BOOL	m_bCursorInThisWindow;
	BOOL	m_bButtonPressDown;
	BOOL	m_bCheckBox;
	BOOL	m_bChecked;
};

