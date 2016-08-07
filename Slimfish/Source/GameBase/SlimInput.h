// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimInput.h
// Description	: CSlimInput declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMINPUT_H__
#define __SLIMINPUT_H__

// Library Includes

// Local Includes
#include "../Utilities/SlimEnumIndexedArray.h"

namespace Slim {

#undef DELETE

/** List of key codes corresponding to keys on a standard U.S. keyboard.
*/
enum class EKeyCode {
	// Numbers.
	NUM_0 = 0, NUM_1 = 1, NUM_2 = 2, NUM_3 = 3, NUM_4 = 4, NUM_5 = 5, NUM_6 = 6, NUM_7 = 7, NUM_8 = 8, NUM_9 = 9,
	// Number pad.
	NUM_PAD_0, NUM_PAD_1, NUM_PAD_2, NUM_PAD_3, NUM_PAD_4, NUM_PAD_5, NUM_PAD_6, NUM_PAD_7, NUM_PAD_8, NUM_PAD_9,
	// Letters of the alphabet.
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	// F Keys.
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	// Arrow keys.
	LEFT_ARROW, RIGHT_ARROW, UP_ARROW, DOWN_ARROW,
	// Virtual key codes.
	LEFT_SHIFT, RIGHT_SHIFT,
	LEFT_CONTROL, RIGHT_CONTROL,
	LEFT_ALT, RIGHT_ALT,
	BACK_SPACE,
	TAB,
	CLEAR,
	RETURN,
	ENTER,
	CAPS_LOCK,
	ESCAPE,
	SPACE,
	INSERT,
	DELETE,
	HOME,
	END,
	PAGE_UP,
	PAGE_DOWN,
	PRINT_SCREEN,
	PAUSE,
	// The '`~' key.
	TELDA,
	NUM_LOCK,
	SCROLL_LOCK,
	// The '=+' key.
	EQUALS,
	// The '-_' key.
	HYPHEN,
	NUM_PAD_MINUS, 
	NUM_PAD_PLUS, 
	NUM_PAD_DIVIDE, 
	NUM_PAD_MULTIPLY,
	// The ',<' key.
	COMMA,
	// The '.>' key.
	PERIOD,
	// The '/?' key.
	FORWARD_SLASH,
	// The '\|' key.
	BACK_SLASH,
	// The ';:'
	SEMI_COLON,
	// The ''"' key.
	QUOTE,
	// The '[{' key.
	LEFT_BRACKET,
	// The ']}' key.
	RIGHT_BRACKET,
	// Describe the maximum key code value. Not to be used as an actual key code.
	MAX
};

/** List of mouse buttons.
*/
enum class EMouseButton {
	// Left mouse click.
	LEFT,
	// The middle mouse button (scroll wheel).
	MIDDLE,
	// Right mouse click.
	RIGHT,
	// First extra button on the mouse.
	EXTRA_1,
	// Second extra button on the mouse.
	EXTRA_2,
	// Describe the maximum key code value. Not to be used as an actual key code.
	MAX
};

/** List of gamepads.
	@remarks
		Use this to refer to a particular gamepad currently only four are supported.
*/
enum class EGamepadIndex {
	// The main gamepad.
	GAMEPAD_0,
	// The second gamepad.
	GAMEPAD_1,
	// The third gamepad.
	GAMEPAD_2,
	// The fourth gamepad.
	GAMEPAD_3,
	// Don't use this, it's used to retrieve the maximum number of gamepads.
	MAX
};

/** List of gamepad buttons
	@remarks
		Represents a binary button on a gamepad which can either be in a pressed or
		released state.
*/
enum class EGamepadButton {
	GAMEPAD_A,
	GAMEPAD_B,
	GAMEPAD_X,
	GAMEPAD_Y,
	GAMEPAD_START,
	GAMEPAD_BACK,
	GAMEPAD_LEFT_BUMPER,
	GAMEPAD_RIGHT_BUMPER,
	GAMEPAD_DPAD_UP,
	GAMEPAD_DPAD_DOWN,
	GAMEPAD_DPAD_LEFT,
	GAMEPAD_DPAD_RIGHT,
	// Left stick clicked in.
	GAMEPAD_LEFT_STICK,
	// Right stick clicked in.
	GAMEPAD_RIGHT_STICK,
	GAMEPAD_LEFT_TRIGGER,
	GAMEPAD_RIGHT_TRIGGER,
	GAMEPAD_LEFT_STICK_UP,
	GAMEPAD_LEFT_STICK_DOWN,
	GAMEPAD_LEFT_STICK_LEFT,
	GAMEPAD_LEFT_STICK_RIGHT,
	GAMEPAD_RIGHT_STICK_UP,
	GAMEPAD_RIGHT_STICK_DOWN,
	GAMEPAD_RIGHT_STICK_LEFT,
	GAMEPAD_RIGHT_STICK_RIGHT,
	// Don't use this, it's used to retrieve the maximum number of gamepad buttons.
	MAX
};

/** List of gamepad axis.
	@remarks
		Represents an axis on a controller that is a associated with a value.
*/
enum class EGamepadAxis {
	// Horizontal component of the left stick, goes from -1 to 1.
	GAMEPAD_LEFT_STICK_X,
	// Vertical component of the left stick, goes from -1 to 1.
	GAMEPAD_LEFT_STICK_Y,
	// Horizontal component of the right stick, goes from -1 to 1.
	GAMEPAD_RIGHT_STICK_X,
	// Vertical component of the right stick, goes from -1 to 1.
	GAMEPAD_RIGHT_STICK_Y,
	// Left trigger, goes from 0 to 1.
	GAMEPAD_LEFT_TRIGGER,
	// Right trigger, goes from 0 to 1.
	GAMEPAD_RIGHT_TRIGGER,
	// Don't use this, it's used to retrieve the maximum number of gamepad axis.
	MAX
};

/** 
	@remarks
		
*/
class CInput {
	using TGamepadButtonStateList = CEnumIndexedArray<bool, EGamepadButton>;
	using TGamepadAxisStateList = CEnumIndexedArray<float, EGamepadAxis>;

	// Member Functions
public:
	/** Constructor.
	 	@author Hayden Asplet
	*/
	CInput();

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CInput();

	/** Retrieve whether a mouse button is down or not.
	 	@author Hayden Asplet
	 	@return True if the button is pressed down.
	*/
	bool IsMouseButtonDown(EMouseButton mouseButton) const;

	/** Get whether a mouse button was pressed down this frame or not.
		@author Hayden Asplet
		@return True if the mouse button was pressed down this frame.
	*/
	bool GetMouseButtonPress(EMouseButton mouseButton) const;

	/** Get whether a mouse button was released this frame or not.
	 	@author Hayden Asplet
	 	@return True if the mouse button was released this frame.
	*/
	bool GetMouseButtonRelease(EMouseButton mouseButton) const;

	/** Get the mouse position as a point on the screen.
	 	@author Hayden Asplet
	*/
	const CPoint<>& GetMousePosition() const;

	/** Get whether a key is currently pressed down or not.
	 	@author Hayden Asplet
	 	@return True if the key is currently pressed in.
	*/
	bool IsKeyDown(EKeyCode key) const;

	/** Retrieve whether a key has been pressed this frame or not.
	 	@author Hayden Asplet
		return True if the key code was pressed down this frame.
	*/
	bool GetKeyPress(EKeyCode key) const;

	/** Retrieve whether a key has been released this frame or not.
	 	@author Hayden Asplet
	 	@return True if the key code was released this frame.
	*/
	bool GetKeyRelease(EKeyCode key) const;

	/** Set the state of a mouse button to pressed.
		@note This is to be called internally. Do NOT call it yourself.
	 	@author Hayden Asplet
	*/
	void SetMouseButtonPress(EMouseButton button);

	/** Set the state of a mouse button to released.
		@note This is to be called internally. Do NOT call it yourself.
	 	@author Hayden Asplet
	*/
	void SetMouseButtonRelease(EMouseButton button);

	/** Set the mouse position.
		@note This is to be called internally. Do NOT call it yourself.
	 	@author Hayden Asplet
	*/
	void SetMousePosition(int x, int y);

	/** Set the state of a key to pressed.
		@note This is to be called internally. Do NOT call it yourself.
	 	@author Hayden Asplet
	*/
	void SetKeyPress(EKeyCode key);

	/** Set the state of a key to released.
		@note This is to be called internally. Do NOT call it yourself.
	 	@author Hayden Asplet
	*/
	void SetKeyRelease(EKeyCode key);

	// Gamepad input

	inline bool GetGamepadButtonPress(EGamepadIndex index, EGamepadButton button) const
	{
		return m_CurrentGamepadButtonStates[index][button] && !m_PreviousGamepadButtonStates[index][button];
	}

	inline bool GetGamepadButtonDown(EGamepadIndex index, EGamepadButton button) const
	{
		return m_CurrentGamepadButtonStates[index][button];
	}

	inline bool GetGamepadButtonRelease(EGamepadIndex index, EGamepadButton button) const
	{
		return !m_CurrentGamepadButtonStates[index][button] && m_PreviousGamepadButtonStates[index][button];
	}

	inline float GetGamepadAxis(EGamepadIndex index, EGamepadAxis axis, bool raw = false) const
	{
		auto axisState = m_CurrentGamepadAxisStates[index][axis];
		if (!raw && std::fabs(axisState) < m_GamepadDeadZone) {
			axisState = 0.0f;
		}

		return axisState;
	}

	/** Flushes the per frame states of the key codes.
		@note This is to be called internally every tick. Do NOT call it yourself.
	 	@author Hayden Asplet
	*/
	void FlushPerFrameStates();
protected:
private:

	// Member Variables
public:
protected:
private:
	CEnumIndexedArray<bool, EKeyCode> m_CurrentKeyStates;
	CEnumIndexedArray<bool, EKeyCode> m_PreviousKeyStates;

	CEnumIndexedArray<bool, EMouseButton> m_CurrentMouseButtonStates;
	CEnumIndexedArray<bool, EMouseButton> m_PreviousMouseButtonStates;

	CEnumIndexedArray<TGamepadButtonStateList, EGamepadIndex> m_CurrentGamepadButtonStates;
	CEnumIndexedArray<TGamepadButtonStateList, EGamepadIndex> m_PreviousGamepadButtonStates;

	CEnumIndexedArray<TGamepadAxisStateList, EGamepadIndex> m_CurrentGamepadAxisStates;
	CEnumIndexedArray<TGamepadAxisStateList, EGamepadIndex> m_PreviousGamepadAxisStates;

	float m_GamepadDeadZone = { 0.3f };

	CPoint<> m_MousePosition;
};

}

#endif // __SLIMINPUT_H__