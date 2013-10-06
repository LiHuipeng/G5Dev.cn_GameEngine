#pragma once
#include "IEngineFrame.h"
#include "IEngineInput.h"


__interface IGuiFrame;


__interface IGuiWndShell
{
	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDrawForeground(int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex) const = 0;
	virtual void OnDrawBackground(int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex) const = 0;
	virtual void OnButtonClicked(int nWindowID) = 0;
};


__interface IGuiWndCore
{
	virtual void Release() = 0;
	virtual void Create(IGuiFrame* pFrame, int cx, int cy) = 0;
	virtual void Create(int nWindowID, IGuiWndCore* pParent, int nResIndex) = 0;
	virtual void DoRender() = 0;

	virtual int  GetWindowID() const = 0;
	virtual void GetWindowRect(RECT* pRect) const = 0; // 相对于Gui容器的坐标
	virtual void GetWindowCanvasRect(RECT* pRect) const = 0; // 相对于win32窗口的坐标
	virtual void SetWindowPos(int x, int y) = 0;	// x,y为最终到达的目标点

	virtual BOOL DoCursorEvent(const SEngineInputState* pInputState) = 0;
	virtual void OnCursorEvent(const SEngineInputState* pInputState) = 0;
	virtual void DoUpdate() = 0;
	virtual void OnUpdate() = 0;
	virtual BOOL IsWindowEnabled() const = 0;
	virtual BOOL IsWindowVisible() const = 0;
	virtual void DoButtonPress(BOOL bDown) = 0;

	virtual void AddChild(IGuiWndCore* pChildWnd) = 0;
	virtual int  GetChildCount() const = 0;
	virtual IGuiWndCore* GetChild(int nIndex) const = 0;

	virtual IGuiWndCore* GetParent() const = 0;
	virtual IGuiFrame* GetGuiFrame() const = 0;
	virtual IGuiWndShell* GetGuiWndShell() = 0;

	virtual void DrawWindow() const = 0;
	virtual void DrawButton() const = 0;
	virtual void DrawEdit() const = 0;
};


__interface IGuiFrame
{
	virtual void Release() = 0;
	virtual BOOL Create(IEngineFrame* pEngine, IGuiWndCore* pMainWnd, int nFontIndex, int nDpkIndex) = 0;
	virtual void GetEnginePtr(IEngineFrame** pEngine) const = 0;
	virtual void Update(const SEngineInputState* pInputState) = 0;
	virtual void Render() const = 0;
	virtual void SwitchEditorMode() = 0;
	virtual void SetFocus(IGuiWndCore* pWnd) = 0;
	virtual void SetCapture(IGuiWndCore* pWnd) = 0;
	virtual BOOL IsEditorModeOn() const = 0;
	virtual DWORD GetTotalFrames() const = 0;
	virtual int  GetFontIndex() const = 0;
};


ENGINEFRAME_API IGuiFrame*   GuiFrameCreate(DWORD dwVersion);
ENGINEFRAME_API IGuiWndCore* GuiWndCoreCreate(DWORD dwVersion, IGuiWndShell* pGuiWndShell);
