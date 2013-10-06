#pragma once


#define CANVAS_CX	1280
#define CANVAS_CY	720
#define GAME_UPDATE_DELAY	20


#define LOADING_IMAGE_CX		800
#define LOADING_IMAGE_CY		30


#define DPK_PASSWORD		_T("123")

#ifdef _DEBUG
#define CONFIG_DPK_PATH	_T("Config")
#define RESOURCE_DPK_PATH	_T("Resource")
#else
#define CONFIG_DPK_PATH	_T("Config.dpk")
#define RESOURCE_DPK_PATH	_T("Resource.dpk")
#endif


enum
{
	GUI_FACE_WND = 1, // 这个enum不会用于建立数组，所以没有必要从0开始编号。
	GUI_OK_BTN,
	GUI_CANCEL_BTN,
	GUI_FACE_EDIT,
};

enum GAMERES
{
	FPK_STANDARD,

	IPK_LOADING,
	IPK_FACE,
	IPK_FACE_WND,
	IPK_OK_BTN,
	IPK_CANCEL_BTN,
	IPK_FACE_EDIT,

	ANI_STAR_1,
	ANI_STAR_2,

	GAMERES_COUNT,
};
