// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GamepadManager.cpp
// Description	: CGamepadManager implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes
#include <limits.h>

// This Include
#include "GamepadManager.h"

// Local Includes


CGamepad::CGamepad(EGamepadIndex _eIndex)
	:m_eIndex(_eIndex)
{
	DEBUG_ASSERT(_eIndex != GINDEX_MAX);
	ZeroMemory(&m_gamepadState, sizeof(TGamepadState));
	ZeroMemory(&m_xInputState, sizeof(XINPUT_STATE));

	m_gamepadState.m_eGamepadIndex = m_eIndex;
}

CGamepad::~CGamepad()
{
	// Turn off the vibration.
	SetVibration();
}

void CGamepad::Poll()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(m_eIndex, &state) == ERROR_SUCCESS) {
		if (state.dwPacketNumber != m_xInputState.dwPacketNumber) {
		// Has the state been changed?
			// Create and send a message.
			TAppMsg appMsg;

			m_gamepadState.m_sThumbLeftX = state.Gamepad.sThumbLX;
			m_gamepadState.m_sThumbLeftY = state.Gamepad.sThumbLY;
			m_gamepadState.m_sThumbRightX = state.Gamepad.sThumbRX;
			m_gamepadState.m_sThumbRightY = state.Gamepad.sThumbRY;
			m_gamepadState.m_ucLeftTrigger = state.Gamepad.bLeftTrigger;
			m_gamepadState.m_ucRightTrigger = state.Gamepad.bRightTrigger;

			if (m_gamepadState.m_sThumbLeftX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				m_gamepadState.m_sThumbLeftX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
				m_gamepadState.m_sThumbLeftX = 0;
			}

			if (m_gamepadState.m_sThumbLeftY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				m_gamepadState.m_sThumbLeftY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
				m_gamepadState.m_sThumbLeftY = 0;
			}

			if (m_gamepadState.m_sThumbRightX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				m_gamepadState.m_sThumbRightX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
				m_gamepadState.m_sThumbRightX = 0;
			}

			if (m_gamepadState.m_sThumbRightY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				m_gamepadState.m_sThumbRightY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
				m_gamepadState.m_sThumbRightY = 0;
			}

			appMsg.m_gamepadState = m_gamepadState;

			if (state.Gamepad.bLeftTrigger != m_xInputState.Gamepad.bLeftTrigger) {
				// Has the left trigger been changed?
				appMsg.m_eMessageType = MT_GAMEPAD_L_TRIGGER;
				g_pApp->GetGame()->OnMsgProc(appMsg);
			}

			if (state.Gamepad.bRightTrigger != m_xInputState.Gamepad.bRightTrigger) {
				// Has the right trigger been changed?
				appMsg.m_eMessageType = MT_GAMEPAD_R_TRIGGER;
				g_pApp->GetGame()->OnMsgProc(appMsg);
			}

			if (state.Gamepad.sThumbLX != m_xInputState.Gamepad.sThumbLX ||
				state.Gamepad.sThumbLY != m_xInputState.Gamepad.sThumbLY) {
				// Has the left thumb stick been moved?
				appMsg.m_eMessageType = MT_GAMEPAD_L_THUMB_MOVE;
				g_pApp->GetGame()->OnMsgProc(appMsg);
			}

			if (state.Gamepad.sThumbRX != m_xInputState.Gamepad.sThumbRX ||
				state.Gamepad.sThumbRY != m_xInputState.Gamepad.sThumbRY) {
				// Has the right thumb stick been moved?
				appMsg.m_eMessageType = MT_GAMEPAD_R_THUMB_MOVE;
				g_pApp->GetGame()->OnMsgProc(appMsg);
			}

			if (state.Gamepad.wButtons != m_xInputState.Gamepad.wButtons) {
				// Has a button state changed?
				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0) {
					// Has the A button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_A;
					m_gamepadState.m_bButtons[GBUTTON_A] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0) {
					// Has the A button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_A;
					m_gamepadState.m_bButtons[GBUTTON_A] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) {
					// Has the X button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_X;
					m_gamepadState.m_bButtons[GBUTTON_X] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0) {
					// Has the X button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_X;
					m_gamepadState.m_bButtons[GBUTTON_X] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
					// Has the B button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_B;
					m_gamepadState.m_bButtons[GBUTTON_B] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0) {
					// Has the B button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_B;
					m_gamepadState.m_bButtons[GBUTTON_B] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0) {
					// Has the Y button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_Y;
					m_gamepadState.m_bButtons[GBUTTON_Y] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0) {
					// Has the Y button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_Y;
					m_gamepadState.m_bButtons[GBUTTON_Y] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) == 0) {
					// Has the Left Shoulder button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_LEFT_SHOULDER;
					m_gamepadState.m_bButtons[GBUTTON_LEFT_SHOULDER] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0) {
					// Has the Left Shoulder button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_LEFT_SHOULDER;
					m_gamepadState.m_bButtons[GBUTTON_LEFT_SHOULDER] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) == 0) {
					// Has the Right Shoulder button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_RIGHT_SHOULDER;
					m_gamepadState.m_bButtons[GBUTTON_RIGHT_SHOULDER] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0) {
					// Has the Right Shoulder button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_RIGHT_SHOULDER;
					m_gamepadState.m_bButtons[GBUTTON_RIGHT_SHOULDER] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) == 0) {
					// Has the DPad Down button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_DPAD_DOWN;
					m_gamepadState.m_bButtons[GBUTTON_DPAD_DOWN] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0) {
					// Has the DPad Down button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_DPAD_DOWN;
					m_gamepadState.m_bButtons[GBUTTON_DPAD_DOWN] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) == 0) {
					// Has the DPad Up button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_DPAD_UP;
					m_gamepadState.m_bButtons[GBUTTON_DPAD_UP] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0) {
					// Has the DPad Up button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_DPAD_UP;
					m_gamepadState.m_bButtons[GBUTTON_DPAD_UP] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) == 0) {
					// Has the DPad Left button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_DPAD_LEFT;
					m_gamepadState.m_bButtons[GBUTTON_DPAD_LEFT] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0) {
					// Has the DPad Left button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_DPAD_LEFT;
					m_gamepadState.m_bButtons[GBUTTON_DPAD_LEFT] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) == 0) {
					// Has the DPad Right button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_DPAD_RIGHT;
					m_gamepadState.m_bButtons[GBUTTON_DPAD_RIGHT] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0) {
					// Has the DPad Right button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_DPAD_RIGHT;
					m_gamepadState.m_bButtons[GBUTTON_DPAD_RIGHT] = true;
				}

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_START) == 0) {
					// Has the DPad Right button been pressed?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_DOWN;
					appMsg.m_eGamepadButton = GBUTTON_START;
					m_gamepadState.m_bButtons[GBUTTON_START] = false;
				}
				else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_START) == 0 && (m_xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0) {
					// Has the DPad Right button been released?
					appMsg.m_eMessageType = MT_GAMEPAD_BUTTON_UP;
					appMsg.m_eGamepadButton = GBUTTON_START;
					m_gamepadState.m_bButtons[GBUTTON_START] = true;
				}

				g_pApp->GetGame()->OnMsgProc(appMsg);
			}

			// Update the state.
			m_xInputState = state;
		}
	}
}

void CGamepad::Update(float _fDeltaTime)
{
	if (m_bIsUsingVibrationTimer) {
		m_fVibrateTime -= _fDeltaTime;

		if (m_fVibrateTime <= 0) {
			SetVibration();
			m_fVibrateTime = 0.0f;
			m_bIsUsingVibrationTimer = false;
		}
	}
}

bool CGamepad::IsConnected() const
{
	XINPUT_STATE xInputState;
	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));

	DWORD dwResult = XInputGetState(m_eIndex, &xInputState);

	return dwResult == ERROR_SUCCESS;
}

void CGamepad::SetVibration(float fLeftMotor, float fRightMotor, float _fVibrationTimeSeconds)
{
	if (_fVibrationTimeSeconds != 0.0f) {
		m_fVibrateTime = _fVibrationTimeSeconds;
		m_bIsUsingVibrationTimer = true;
	}
	else {
		m_fVibrateTime = 0.0f;
		m_bIsUsingVibrationTimer = false;
	}

	XINPUT_VIBRATION xInputVibration;
	ZeroMemory(&xInputVibration, sizeof(XINPUT_VIBRATION));

	int iLeftVibration = static_cast<int>(fLeftMotor * static_cast<float>(USHRT_MAX));
	int iRightVibration = static_cast<int>(fRightMotor * static_cast<float>(USHRT_MAX));

	xInputVibration.wLeftMotorSpeed = iLeftVibration;
	xInputVibration.wRightMotorSpeed = iRightVibration;

	XInputSetState(m_eIndex, &xInputVibration);
}

CGamepadManager::CGamepadManager()
	:m_gamepads(GINDEX_MAX, CGamepad(GINDEX_ONE))
{
	for (unsigned int i = 0; i < m_gamepads.size(); ++i) {
		m_gamepads[i].m_eIndex = static_cast<EGamepadIndex>(i);
		m_gamepads[i].m_gamepadState.m_eGamepadIndex = static_cast<EGamepadIndex>(i);
	}
}

CGamepadManager::~CGamepadManager()
{

}

void CGamepadManager::PollGamepads()
{
	for (unsigned int i = 0; i < m_gamepads.size(); ++i) {
		m_gamepads[i].Poll();
	}
}

void CGamepadManager::Update(float _fDeltaTime)
{
	for (unsigned int i = 0; i < m_gamepads.size(); ++i) {
		m_gamepads[i].Update(_fDeltaTime);
	}
}

const CGamepad& CGamepadManager::GetGamepad(EGamepadIndex _eGameIndex) const
{
	DEBUG_ASSERT(_eGameIndex != GINDEX_MAX);
	return m_gamepads[_eGameIndex];
}

bool CGamepadManager::IsConnected(EGamepadIndex _eGameIndex)
{
	DEBUG_ASSERT(_eGameIndex != GINDEX_MAX);
	return m_gamepads[_eGameIndex].IsConnected();
}
