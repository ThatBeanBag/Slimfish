
// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EditBox.cpp
// Description	: CEditBox implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EditBox.h"

// Local Includes

const float CEditBox::s_kfFLASH_CURSOR_THRESHOLD = 0.5f;

CEditBox::CEditBox(const CStaticImage& _krStaticImage, const CRect& _krTextBox, int _iMaxCharacters, int _iFontHeight, TColour _colour)
	:CStaticImage(_krStaticImage),
	m_textBox(_krTextBox),
	m_iMaxCharacters(_iMaxCharacters),
	m_iFontHeight(_iFontHeight),
	m_colour(_colour),
	m_bIsCursorShown(false),
	m_bIsActive(true),
	m_fFlashCursorTimer(0.0f)
{

}

CEditBox::~CEditBox()
{

}

const std::string CEditBox::GetUserText() const
{
	return m_strUserText;
}

bool CEditBox::VHandleEvents(const TAppMsg& _krMsg)
{
	switch(_krMsg.m_eMessageType) {
		case MT_CHAR: {
			if (m_bIsActive) {
				switch (_krMsg.m_uckey) {
					case ' ': {
						// Eat spaces. User names and server names can't have spaces.
						return false;
					}
					case VK_ESCAPE: {
						return false;
					}
					case VK_BACK: {
						if (!m_strUserText.empty()) {
							m_strUserText.pop_back();
						}

						return true;
					}
					case VK_RETURN: {
						// TODO: call a delegate.
						return true;
					}
					default: {
						if (m_strUserText.size() < static_cast<unsigned int>(m_iMaxCharacters)) {
							m_strUserText.push_back(_krMsg.m_uckey);
						}

						return true;
					}
				}
			}
			break;
		}
		case MT_LMB_DOWN: {
			if (IsInside(GetWorldBoundingBox(), _krMsg.m_mousePosition)) {
				m_bIsActive = true;
				m_bIsCursorShown = true;
			}
			else {
				m_bIsActive = false;
				m_bIsCursorShown = false;
			}

			break;
		}
	}

	return false;
}

void CEditBox::VRender()
{
	CStaticImage::RenderImage(CRect());
	g_pApp->GetRenderer()->VEnd2DDraw();
	g_pApp->GetRenderer()->VBegin2DDraw();

	if (m_bIsCursorShown && static_cast<int>(m_strUserText.size()) < m_iMaxCharacters) {
		m_strUserText.push_back('_');
	}
	else {
		m_strUserText.push_back(' ');
	}

	// The text box is relative to the bounding box.
	CRect worldTextBox = GetWorldBoundingBox();
	worldTextBox.SetX(worldTextBox.GetX() + m_textBox.GetX());
	worldTextBox.SetY(worldTextBox.GetY() + m_textBox.GetY());

	g_pApp->GetRenderer()->VDrawText(m_strUserText.c_str(), worldTextBox, m_iFontHeight, DT_SINGLELINE | DT_VCENTER, m_colour);
	m_strUserText.pop_back();
}

void CEditBox::VUpdate(float _fDeltaTime)
{
	if (m_bIsActive) {
		m_fFlashCursorTimer += _fDeltaTime;

		if (m_fFlashCursorTimer >= s_kfFLASH_CURSOR_THRESHOLD) {
			m_fFlashCursorTimer = 0.0f;
			m_bIsCursorShown = !m_bIsCursorShown;
		}
	}

}

void CEditBox::VSetAlpha(unsigned char _ucAlpha)
{
	m_colour.m_a = _ucAlpha;
}

