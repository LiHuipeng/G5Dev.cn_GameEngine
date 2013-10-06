#pragma once

class CSound;

__interface IEngineSound
{
	virtual void    Release() = 0;
		
	virtual BOOL    Init(HWND hWnd) = 0;
	virtual CSound* SoundCreate(LPCWSTR szFileName, BOOL bStreaming) = 0;
	virtual void	SoundRelease(CSound* pSound) = 0;
	virtual void    SoundPlay(CSound* pSound, BOOL bLoop) = 0;
	virtual void    SoundStop(CSound* pSound) = 0;
};


#ifdef SOUND_EXPORTS
#define SOUND_API __declspec(dllexport)
#else
#define SOUND_API __declspec(dllimport)
#endif

SOUND_API IEngineSound* EngineSoundCreate();
