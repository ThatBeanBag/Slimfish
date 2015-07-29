// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GamepadManager.h
// Description	: CGamepadManager declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __GAMEPADMANAGER_H__
#define __GAMEPADMANAGER_H__

// Library Includes

// Local Includes

enum EGamepadIndex {
	GINDEX_ONE = 0,
	GINDEX_TWO = 1,
	GINDEX_THREE = 2,
	GINDEX_FOUR = 3,
	GINDEX_MAX = 4
};

enum EGamepadButton {
	GBUTTON_A,
	GBUTTON_X,
	GBUTTON_Y,
	GBUTTON_B,
	GBUTTON_LEFT_SHOULDER,
	GBUTTON_RIGHT_SHOULDER,
	GBUTTON_DPAD_DOWN,
	GBUTTON_DPAD_UP,
	GBUTTON_DPAD_LEFT,
	GBUTTON_DPAD_RIGHT,
	GBUTTON_START,
	GBUTTON_BACK,
	GBUTTON_MAX
};

enum EGamepadTrigger {
	GTRIGGER_LEFT,
	GTRIGGER_RIGHT
};

enum EGamepadThumbstick {
	GTHUMB_LEFT,
	GTHUMB_RIGHT
};

enum EDPadDirection {
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT
};

struct TGamepadState {
	EGamepadIndex m_eGamepadIndex;
	unsigned char m_ucLeftTrigger;
	unsigned char m_ucRightTrigger;
	short m_sThumbLeftX;
	short m_sThumbLeftY;
	short m_sThumbRightX;
	short m_sThumbRightY;
	bool m_bButtons[GBUTTON_MAX];
};

class CGamepad {
	friend class CGamepadManager;

	// Member Functions
public:
	CGamepad(EGamepadIndex _eIndex);
	~CGamepad();

	void Poll();
	void Update(float _fDeltaTime);

	bool IsConnected() const;
	void SetVibration(float fLeftMotor = 0.0f, float fRightMotor = 0.0f, float _fVibrationTimeSeconds = 0.0f);
protected:
private:

	// Member Variables
public:
protected:
private:
	EGamepadIndex m_eIndex;
	TGamepadState m_gamepadState;
	XINPUT_STATE m_xInputState;

	float m_fVibrateTime;
	bool m_bIsUsingVibrationTimer;
};

class CGamepadManager {
	// Member Functions
public:
	CGamepadManager();
	~CGamepadManager();

	void PollGamepads();
	void Update(float _fDeltaTime);

	const CGamepad& GetGamepad(EGamepadIndex _eGameIndex) const;
	bool IsConnected(EGamepadIndex _eGameIndex);
protected:
private:
	// Member Variables
public:
protected:
private:
	std::vector<CGamepad> m_gamepads;
};

#endif	// __GAMEPADMANAGER_H__