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

/** 
	@remarks
		
*/
class CInput {
	// Member Functions
	enum class EButtonState {
		// The button is up.
		UP,
		// The button is down.
		DOWN,
		// The button was released this frame.
		RELEASE,
		// The button was pressed down this frame.
		PRESS,
	};

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
	const CPoint& GetMousePosition() const;

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

	/** Flushes the per frame states of the key codes.
		@note This is to be called internally every tick. Do NOT call it yourself.
	 	@author Hayden Asplet
	*/
	void FlushPerFrameStates();
protected:
private:

	static bool IsButtonDown(EButtonState buttonState) { return buttonState == EButtonState::DOWN ||
																buttonState == EButtonState::PRESS; }

	// Member Variables
public:
protected:
private:
	std::vector<EButtonState> m_KeyStates;	// Keeps track of a keys per frame state (whether it was pressed or released this frame or not).
	std::vector<EButtonState> m_MouseButtonStates;
	CPoint m_MousePosition;
};

}

#endif // __SLIMINPUT_H__