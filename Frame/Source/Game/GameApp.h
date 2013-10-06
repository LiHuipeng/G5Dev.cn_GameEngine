// huipeng_lhp@163.com create at 2013-09-16
#pragma once

#include "resource.h"

#include "AppRes.h"


class CGame
{
public:
	CGame(void);
	void Init(HWND hWnd);
	void Destroy();
	void Update();
	void Render();
	void OnKeyDown(WPARAM wParam, LPARAM lParam);

protected:
	

private:

public:
	CGameRes	m_Res;
};

extern CGame g_Game;
