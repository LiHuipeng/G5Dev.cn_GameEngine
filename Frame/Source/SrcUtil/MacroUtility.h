// MacroUtility.h <huipeng_lhp@163.com 2007-11-06 15:44>
#pragma once

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

// COM
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
