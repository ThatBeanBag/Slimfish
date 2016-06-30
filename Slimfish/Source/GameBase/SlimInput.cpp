// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimInput.cpp
// Description	: CSlimInput implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimInput.h"

// Local Includes

namespace Slim {

CInput::CInput()
	:m_KeyStates(static_cast<int>(EKeyCode::MAX), EButtonState::UP),
	m_MouseButtonStates(static_cast<int>(EMouseButton::MAX), EButtonState::UP)
{

}

CInput::~CInput()
{

}

bool CInput::IsMouseButtonDown(EMouseButton mouseButton) const
{
	return IsButtonDown(m_MouseButtonStates[static_cast<int>(mouseButton)]);
}

bool CInput::GetMouseButtonPress(EMouseButton mouseButton) const
{
	return m_MouseButtonStates[static_cast<int>(mouseButton)] == EButtonState::PRESS;
}

bool CInput::GetMouseButtonRelease(EMouseButton mouseButton) const
{
	return m_MouseButtonStates[static_cast<int>(mouseButton)] == EButtonState::RELEASE;
}


const CPoint<>& CInput::GetMousePosition() const
{
	return m_MousePosition;
}

bool CInput::IsKeyDown(EKeyCode key) const
{
	return IsButtonDown(m_KeyStates[static_cast<int>(key)]);
}

bool CInput::GetKeyPress(EKeyCode key) const
{
	return m_KeyStates[static_cast<int>(key)] == EButtonState::PRESS;
}

bool CInput::GetKeyRelease(EKeyCode key) const
{
	return m_KeyStates[static_cast<int>(key)] == EButtonState::RELEASE;
}

void CInput::SetMouseButtonPress(EMouseButton button)
{
	m_MouseButtonStates[static_cast<int>(button)] = EButtonState::PRESS;
}

void CInput::SetMouseButtonRelease(EMouseButton button)
{
	m_MouseButtonStates[static_cast<int>(button)] = EButtonState::RELEASE;
}

void CInput::SetMousePosition(int x, int y)
{
	m_MousePosition = CPoint<>(x, y);
}

void CInput::SetKeyPress(EKeyCode key)
{
	m_KeyStates[static_cast<int>(key)] = EButtonState::PRESS;
}

void CInput::SetKeyRelease(EKeyCode key)
{
	m_KeyStates[static_cast<int>(key)] = EButtonState::RELEASE;
}

void CInput::FlushPerFrameStates()
{
	static auto flushState = [](EButtonState& buttonState) { 
		if (buttonState == EButtonState::PRESS) {
			buttonState = EButtonState::DOWN;
		}
		else if (buttonState == EButtonState::RELEASE) {
			buttonState = EButtonState::UP;
		}
	};

	std::for_each(m_KeyStates.begin(), m_KeyStates.end(), flushState);
	std::for_each(m_MouseButtonStates.begin(), m_MouseButtonStates.end(), flushState);
}

}