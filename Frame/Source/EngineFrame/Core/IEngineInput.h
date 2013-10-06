// huipeng_lhp@163.com create at 2013-09-28

#pragma once

enum
{
	ENGINE_KEY_HOME,
	ENGINE_KEY_A,		// confirm
	ENGINE_KEY_B,		// back
	ENGINE_KEY_X,
	ENGINE_KEY_Y,
	ENGINE_KEY_LB,
	ENGINE_KEY_RB,
	ENGINE_KEY_LT,
	ENGINE_KEY_RT,
	ENGINE_KEY_BACK,
	ENGINE_KEY_START,
	MAX_ENGINE_KEY,
};

#define MAX_ENGINE_CURSOR		5
#define MAX_ENGINE_BUTTON		MAX_ENGINE_CURSOR

struct SEngineInputStateStep
{
	short	Pov; // point of view
	POINT	CursorPos[MAX_ENGINE_CURSOR];
	bool	CursorBtn[MAX_ENGINE_BUTTON]; // IOS 系统中也支持小写的bool
	bool	Key[MAX_ENGINE_KEY];
};

struct SEngineInputState
{
	SEngineInputStateStep pre;
	SEngineInputStateStep cur;
};


inline BOOL CheckButtonDownEvent(const SEngineInputState* pInputState, int nButtonIndex)
{
	ASSERT(nButtonIndex>=0 && nButtonIndex<MAX_ENGINE_BUTTON);
	if(!pInputState->pre.CursorBtn[nButtonIndex] && pInputState->cur.CursorBtn[nButtonIndex])
		return TRUE;
	else
		return FALSE;
}

inline BOOL CheckPovClickedEvent(const SEngineInputState* pInputState, int Pov)
{
	if(pInputState->pre.Pov<0 && pInputState->cur.Pov==Pov)
		return TRUE;
	else
		return FALSE;
}
