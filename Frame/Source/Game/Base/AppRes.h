// huipeng_lhp@163.com create at 2013-09-21
#pragma once

#include "IEngineFrame.h"
#include "IGuiFrame.h"
#include "AppResDefine.h"


struct SResourceLoadTaskItem
{
	GAMERES	GameRes;
	WCHAR	ResourcePath[MAX_PATH];
};


class CGameRes
{
public:
	CGameRes(void);
	~CGameRes(void);
	void Init( HWND hWnd );
	void Destroy();
	void Update();
	void Render();

	void GuiInit();
	void MessageProcess(UINT message, WPARAM wParam, LPARAM lParam);
	void ViewPosToCanvasPos(int ViewX, int ViewY, int &CanvasX, int& CanvasY);
	int  GetResIndex(GAMERES GameRes);

	void AddResourceLoadTask(GAMERES GameRes, LPCWSTR szItemPath); // 用路径来区分资源类型
	void GetResourceLoadProgress(int& nCurrent, int& nTotal);

	void ImagePackCreate(GAMERES GameRes, LPCWSTR szItemPath);
	void FontPackCreate(GAMERES GameRes, LPCWSTR szItemPath);
	void AnimationCreate(GAMERES GameRes, LPCWSTR szItemPath);

	void Draw(LPCWSTR szText, DWORD dwColor, GAMERES GameRes, int x, int y);
	void Draw(int nItemIndex, GAMERES GameRes, int x, int y, int w=0, int h=0, int sx=0, int sy=0, int sw=0, int sh=0);
	void Draw(GAMERES GameRes, int x, int y);
	void Play(GAMERES GameRes, BOOL bLoop=FALSE);
	void Stop(GAMERES GameRes);

protected:
	void LoadNextResource(); // 所有资源加载完成时 return TRUE

private:
	IEngineFrame*	m_pEngine;

	int		m_nResourceDpkIndex;
	int		m_nConfigDpkIndex;
	int		m_ResIndex[GAMERES_COUNT];

	SEngineInputState	m_InputState;
	POINT	m_CursorPos;
	bool	m_bCursorButtonDown;
	short	m_Pov;

	std::queue<SResourceLoadTaskItem>	m_ResourceLoadTaskItemQueue;
	int		m_nResourceTotalCount;
	BOOL	m_bAllResourceLoadOK;
	DWORD	m_dwTotalFrames;

public:
	IGuiFrame*		m_pGui;
};

