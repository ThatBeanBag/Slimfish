// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AppMsg.h
// Description	: TAppMsg declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __APPMSG_H__
#define __APPMSG_H__

// Library Includes

// Local Includes
#include "../Controller/GamepadManager.h"

enum EMessageType {
	MT_CHAR,
	MT_KEYDOWN,
	MT_KEYUP,
	MT_MOUSE_MOVE,
	MT_MOUSE_WHEEL,
	MT_LMB_DOWN,
	MT_MMB_DOWN,
	MT_RMB_DOWN,
	MT_LMB_UP,
	MT_MMB_UP,
	MT_RMB_UP,
	MT_GAMEPAD_L_THUMB_MOVE,
	MT_GAMEPAD_R_THUMB_MOVE,
	MT_GAMEPAD_BUTTON_UP,
	MT_GAMEPAD_BUTTON_DOWN,
	MT_GAMEPAD_L_TRIGGER,
	MT_GAMEPAD_R_TRIGGER
};

struct TAppMsg {
	EMessageType m_eMessageType;
	CPoint m_mousePosition;
	unsigned char m_uckey;
	short m_sDeltaMouseWheel;
	TGamepadState m_gamepadState;
	EGamepadButton m_eGamepadButton;
};

#endif	// __APPMSG_H__