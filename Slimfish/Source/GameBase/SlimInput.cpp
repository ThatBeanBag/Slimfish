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
{

}

CInput::~CInput()
{

}

bool CInput::IsMouseButtonDown(EMouseButton mouseButton) const
{
	return m_CurrentMouseButtonStates[mouseButton];
}

bool CInput::GetMouseButtonPress(EMouseButton mouseButton) const
{
	return m_CurrentMouseButtonStates[mouseButton] && !m_PreviousMouseButtonStates[mouseButton];
}

bool CInput::GetMouseButtonRelease(EMouseButton mouseButton) const
{
	return !m_CurrentMouseButtonStates[mouseButton] && m_PreviousMouseButtonStates[mouseButton];
}

const CPoint<>& CInput::GetMousePosition() const
{
	return m_MousePosition;
}

bool CInput::IsKeyDown(EKeyCode key) const
{
	return m_CurrentKeyStates[key];
}

bool CInput::GetKeyPress(EKeyCode key) const
{
	return m_CurrentKeyStates[key] && !m_PreviousKeyStates[key];
}

bool CInput::GetKeyRelease(EKeyCode key) const
{
	return !m_CurrentKeyStates[key] && m_PreviousKeyStates[key];
}

void CInput::SetMouseButtonPress(EMouseButton button)
{
	m_CurrentMouseButtonStates[button] = true;
}

void CInput::SetMouseButtonRelease(EMouseButton button)
{
	m_CurrentMouseButtonStates[button] = false;
}

void CInput::SetMousePosition(int x, int y)
{
	m_MousePosition = CPoint<>(x, y);
}

void CInput::SetKeyPress(EKeyCode key)
{
	m_CurrentKeyStates[key] = true;
}

void CInput::SetKeyRelease(EKeyCode key)
{
	m_CurrentKeyStates[key] = false;
}

void CInput::FlushPerFrameStates()
{
	m_PreviousKeyStates = m_CurrentKeyStates;
	m_PreviousMouseButtonStates = m_CurrentMouseButtonStates;
	m_PreviousGamepadButtonStates = m_CurrentGamepadButtonStates;
	m_PreviousGamepadAxisStates = m_CurrentGamepadAxisStates;

}

}