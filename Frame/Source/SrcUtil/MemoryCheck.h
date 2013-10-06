#if !defined(_MEMCHECK_H__0AE7BC4F_D215_46b7_8748_ED1D4AF459A7__INCLUDED_)
#define _MEMCHECK_H__0AE7BC4F_D215_46b7_8748_ED1D4AF459A7__INCLUDED_

#pragma once

#include <crtdbg.h>

#ifdef _DEBUG

#ifndef DEBUG_NEW
#define new			DEBUG_NEW
#endif

#ifndef DEBUG_NEW
#define DEBUG_NEW   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif


inline void EnableMemLeakCheck()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
}

#endif // !defined(_MEMCHECK_H__0AE7BC4F_D215_46b7_8748_ED1D4AF459A7__INCLUDED_)