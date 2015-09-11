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
	:m_KeyStates(static_cast<int>(EKeyCode::MAX), false),
	m_PerFrameKeyStates(static_cast<int>(EKeyCode::MAX), EKeyState::NONE)
{

}

CInput::~CInput()
{

}

bool CInput::IsKeyDown(EKeyCode key) const
{
	return m_KeyStates[static_cast<int>(key)];
}

bool CInput::GetKeyPress(EKeyCode key) const
{
	return m_PerFrameKeyStates[static_cast<int>(key)] == EKeyState::PRESS;
}

bool CInput::GetKeyRelease(EKeyCode key) const
{
	return m_PerFrameKeyStates[static_cast<int>(key)] == EKeyState::RELEASE;
}

void CInput::SetKeyPressed(EKeyCode key)
{
	int index = static_cast<int>(key);
	m_KeyStates[index] = true;
	m_PerFrameKeyStates[index] = EKeyState::PRESS;
}

void CInput::SetKeyReleased(EKeyCode key)
{
	int index = static_cast<int>(key);
	m_KeyStates[index] = false;
	m_PerFrameKeyStates[index] = EKeyState::RELEASE;
}

void CInput::FlushPerFrameStates()
{
	fill(m_PerFrameKeyStates.begin(), m_PerFrameKeyStates.end(), EKeyState::NONE);
}

}