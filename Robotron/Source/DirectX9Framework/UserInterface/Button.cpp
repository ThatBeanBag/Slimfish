// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Button.cpp
// Description	: CButton implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Button.h"

// Local Includes


CButton::CButton(const std::string& _krstrName, const CRect& _krBoundingBox, shared_ptr<ISprite> _pSprite, bool _bIsToggled)
	:CStaticImage(_krstrName, _krBoundingBox, _pSprite),
	m_masks(STATE_MAX, CRect()),
	m_eButtonState(STATE_OUT),
	m_bIsToggled(_bIsToggled)
{

}

CButton::CButton(const CStaticImage& _krStaticImage, bool _bIsToggled)
	:CStaticImage(_krStaticImage),
	m_masks(STATE_MAX, CRect()),
	m_eButtonState(STATE_OUT),
	m_bIsToggled(_bIsToggled)
{

}

CButton::~CButton()
{

}

void CButton::SetMask(EButtonState _eButtonState, const CRect& _krMask)
{
	if (_eButtonState != STATE_MAX) {
		m_masks[_eButtonState] = _krMask;
	}
}

void CButton::SetState(EButtonState _eButtonState)
{
	m_eButtonState = _eButtonState;
}

CRect CButton::GetMask(EButtonState _eButtonState) const
{
	DEBUG_ASSERT(_eButtonState != STATE_MAX);

	if (m_eButtonState != STATE_MAX) {
		return m_masks[_eButtonState];
	}

	return CRect();
}

CButton::EButtonState CButton::GetState() const
{
	return m_eButtonState;
}

bool CButton::VHandleEvents(const TAppMsg& _krMsg)
{
	if (_krMsg.m_eMessageType == MT_MOUSE_MOVE) {
		if (m_lastMousePosition != _krMsg.m_mousePosition) {
			// Did the mouse actually move?
			m_lastMousePosition = _krMsg.m_mousePosition;
			if (IsInside(GetWorldBoundingBox(), _krMsg.m_mousePosition)) {
				SetSelected(true);
			}
			else {
				SetSelected(false);
			}
		}
	}

	if (IsSelected()) {
		switch (_krMsg.m_eMessageType) {
			case MT_LMB_DOWN: {
				if (IsInside(GetWorldBoundingBox(), _krMsg.m_mousePosition)) {
					Press();
				}

				break;
			}

			case MT_LMB_UP: {
				if (IsInside(GetWorldBoundingBox(), _krMsg.m_mousePosition) && (m_eButtonState == STATE_PRESSED || m_bIsToggled)) {
					Release();
					return true;
				}
				break;
			}
			case MT_GAMEPAD_BUTTON_DOWN: {
				if (_krMsg.m_eGamepadButton == GBUTTON_A) {
					Press();
					return true;
				}
				break;
			}
			case MT_GAMEPAD_BUTTON_UP: {
				if (_krMsg.m_eGamepadButton == GBUTTON_A) {
					Release();
					return true;
				}
				break;
			}

			case MT_KEYDOWN: {
				if (_krMsg.m_uckey == VK_RETURN) {
					Press();
					return true;
				}
				break;
			}
			case MT_KEYUP: {
				if (_krMsg.m_uckey == VK_RETURN) {
					Release();
					return true;
				}
				break;
			}
		default:break;
		}
	}
	/*else if (m_bIsToggled && IsSelected()) {
		switch (_krMsg.m_eMessageType) {
		case MT_LMB_DOWN: {
			if (IsInside(GetWorldBoundingBox(), _krMsg.m_mousePosition)) {
				GetOwner()->CallGUIEventCallback(GUIEVENT_BUTTON_PRESSED, this);

				// Toggle the state.
				m_eButtonState = m_eButtonState == STATE_PRESSED; STATE_HOVER : STATE_PRESSED;

				return true;
			}

			break;
		}

		case MT_LMB_UP: {
			if (IsInside(GetWorldBoundingBox(), _krMsg.m_mousePosition) && m_eButtonState == STATE_PRESSED) {
				// Call the GUI event callback.
				GetOwner()->CallGUIEventCallback(GUIEVENT_BUTTON_RELEASED, this);

				// Toggle the state.
				m_eButtonState = m_eButtonState == STATE_PRESSED; STATE_HOVER: STATE_PRESSED;

				return true;
			}

			break;
		}
		case MT_GAMEPAD_BUTTON_DOWN: {
			if (_krMsg.m_eGamepadButton == GBUTTON_A) {
				// Toggle the state.
				m_eButtonState = m_eButtonState == STATE_PRESSED; STATE_HOVER: STATE_PRESSED;
				GetOwner()->CallGUIEventCallback(GUIEVENT_BUTTON_PRESSED, this);
				return true;
			}

			break;
		}
		case MT_GAMEPAD_BUTTON_UP: {
			if (_krMsg.m_eGamepadButton == GBUTTON_A) {
				// Toggle the state.
				m_eButtonState = m_eButtonState == STATE_PRESSED; STATE_HOVER: STATE_PRESSED;
				GetOwner()->CallGUIEventCallback(GUIEVENT_BUTTON_RELEASED, this);
				return true;
			}

			break;
		}

		case MT_KEYDOWN: {
			if (_krMsg.m_uckey == VK_RETURN) {
				// Toggle the state.
				m_eButtonState = m_eButtonState == STATE_PRESSED; STATE_HOVER: STATE_PRESSED;
				GetOwner()->CallGUIEventCallback(GUIEVENT_BUTTON_PRESSED, this);
				return true;
			}

			break;
		}
		case MT_KEYUP: {
			if (_krMsg.m_uckey == VK_RETURN) {
				// Toggle the state.
				m_eButtonState = m_eButtonState == STATE_PRESSED; STATE_HOVER: STATE_PRESSED;
				GetOwner()->CallGUIEventCallback(GUIEVENT_BUTTON_RELEASED, this);
				return true;
			}

			break;
		}
		default:break;
		}
	}*/

	return false;
}

void CButton::VRender()
{
	DEBUG_ASSERT(m_eButtonState != STATE_MAX);

	CStaticImage::RenderImage(m_masks[m_eButtonState]);
}

void CButton::VSetSelected(bool _bIsSelected)
{
	if (m_bIsToggled && m_eButtonState == STATE_PRESSED) {
		return;
	}

	if (_bIsSelected && m_eButtonState != STATE_PRESSED) {
		m_eButtonState = STATE_HOVER;
	}
	else if(!_bIsSelected) {
		m_eButtonState = STATE_OUT;
	}

}

void CButton::Release()
{
	// We are no longer pressed on the button.
	if (m_bIsToggled) {
		// Toggle the state.
		m_eButtonState = m_eButtonState == STATE_PRESSED ? STATE_HOVER : STATE_PRESSED;
	}
	else {
		m_eButtonState = STATE_HOVER;
	}

	// Call the GUI event callback.
	GetOwner()->CallGUIEventCallback(GUIEVENT_BUTTON_RELEASED, this);
}

void CButton::Press()
{
	// We are no longer pressed on the button.
	if (m_bIsToggled) {
	}
	else {
		m_eButtonState = STATE_PRESSED;
	}

	// Call the GUI event callback.
	GetOwner()->CallGUIEventCallback(GUIEVENT_BUTTON_PRESSED, this);
}

