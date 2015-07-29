// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Menu.cpp
// Description	: CMenu implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Menu.h"

// Local Includes

CMenu::CMenu(const std::string& _krstrName, int _iX, int _iY, int _iGap, EAlignment _eAlignment, EDirection _eDirection)
	:AGUIElement(_krstrName, CRect(_iX, _iY, 0, 0)),
	m_iSelection(0),
	m_iOffset(0),
	m_iGap(_iGap),
	m_eAlignment(_eAlignment),
	m_eDirection(_eDirection)
{

}

CMenu::~CMenu()
{

}

bool CMenu::VHandleEvents(const TAppMsg& _krMsg)
{
	if (!IsSelected()) {
		return false;
	}

	switch (_krMsg.m_eMessageType)	{
		case MT_MOUSE_MOVE: {
			if (m_lastMousePosition != _krMsg.m_mousePosition) {
			// Did the mouse actually move?
				m_lastMousePosition = _krMsg.m_mousePosition;

				for (unsigned int i = 0; i < m_guiElements.size(); ++i) {
					if (IsInside(m_guiElements[i]->GetWorldBoundingBox(), _krMsg.m_mousePosition) &&
						m_iSelection != i) {
						SetSelection(i);
					}
				}
			}
			break;
		}
		case MT_GAMEPAD_BUTTON_DOWN: {
			if (m_eDirection == DIR_VERTICAL) {
				if (_krMsg.m_eGamepadButton == GBUTTON_DPAD_DOWN) {
					IncrementSelection();
					return true;
				}
				else if (_krMsg.m_eGamepadButton == GBUTTON_DPAD_UP) {
					DecrementSelection();
					return true;
				}
			}
			else if (m_eDirection == DIR_HORIZONTAL) {
				if (_krMsg.m_eGamepadButton == GBUTTON_DPAD_RIGHT) {
					IncrementSelection();
					return true;
				}
				else if (_krMsg.m_eGamepadButton == GBUTTON_DPAD_LEFT) {
					DecrementSelection();
					return true;
				}
			}
			break;
		}
		case MT_GAMEPAD_L_THUMB_MOVE: {
			if (m_eDirection == DIR_VERTICAL) {
				if (m_lastLeftThumb.GetY() <= 0 && _krMsg.m_gamepadState.m_sThumbLeftY > 0) {
					DecrementSelection();
				}
				else if (m_lastLeftThumb.GetY() >= 0 && _krMsg.m_gamepadState.m_sThumbLeftY < 0) {
					IncrementSelection();
				}
			}
			else if (m_eDirection == DIR_HORIZONTAL) {
				if (m_lastLeftThumb.GetX() <= 0 && _krMsg.m_gamepadState.m_sThumbLeftX > 0) {
					IncrementSelection();
				}
				else if (m_lastLeftThumb.GetX() >= 0 && _krMsg.m_gamepadState.m_sThumbLeftX < 0) {
					DecrementSelection();
				}
			}

			// Store the current left thumb stick state.
			m_lastLeftThumb.SetX(_krMsg.m_gamepadState.m_sThumbLeftX);
			m_lastLeftThumb.SetY(_krMsg.m_gamepadState.m_sThumbLeftY);
			return true;
		}
		case MT_KEYDOWN: {
			if (m_eDirection == DIR_VERTICAL) {
				if (_krMsg.m_uckey == VK_DOWN) {
					IncrementSelection();
					return true;
				}
				else if (_krMsg.m_uckey == VK_UP) {
					DecrementSelection();
					return true;
				}
			}
			else {
				if (_krMsg.m_uckey == VK_LEFT) {
					DecrementSelection();
					return true;
				}
				else if (_krMsg.m_uckey == VK_RIGHT) {
					IncrementSelection();
					return true;
				}
			}

			break;
		}
		default: {
			break;
		}
	}

	/*if (!m_bIsUsingSelection) {
		for (unsigned int i = 0; i < m_buttons.size(); ++i) {
			if (m_buttons[i]) {
				if (m_buttons[i]->GetState() == CButton::STATE_HOVER && i != m_iSelection) {
					m_iSelection = i;
				}
			}
		}
	}*/

	return false;
}

void CMenu::VUpdate(float _fDeltaTime)
{

}

void CMenu::VRender()
{

}

void CMenu::VSetAlpha(unsigned char _ucAlpha)
{
	// Set the alpha of the buttons.
	for (unsigned int i = 0; i < m_guiElements.size(); ++i) {
		if (m_guiElements[i]) {
			m_guiElements[i]->SetAlpha(_ucAlpha);
		}
	}
}

void CMenu::VSetVisibility(bool _bIsVisible)
{
	for (unsigned int i = 0; i < m_guiElements.size(); ++i) {
		if (m_guiElements[i]) {
			m_guiElements[i]->SetVisibility(_bIsVisible);
		}
	}
}

void CMenu::VSetSelected(bool _bIsSelected)
{
	if (!m_guiElements.empty()) {
		if (m_guiElements[m_iSelection]) {
			m_guiElements[m_iSelection]->SetSelected(_bIsSelected);
		}
	}
}

void CMenu::AddGUIElement(AGUIElement* _pElement)
{
	DEBUG_ASSERT(_pElement);

	/*CRect guiElementRect = _pElement->GetBoundingBox();
	CRect menuRect = GetBoundingBox();

	switch (m_eDirection) {
		case DIR_HORIZONTAL: {
			guiElementRect.SetY(menuRect.GetY());
			guiElementRect.SetX(menuRect.GetX() + m_iOffset);
			m_iOffset += guiElementRect.GetWidth() + m_iGap;
			
			if (guiElementRect.GetHeight() > menuRect.GetHeight()) {
				menuRect.SetHeight(guiElementRect.GetHeight());
			}

			menuRect.SetWidth(m_iOffset);

			break;
		}
		case DIR_VERTICAL: {
			guiElementRect.SetX(menuRect.GetX());
			guiElementRect.SetY(menuRect.GetY() + m_iOffset);
			m_iOffset += guiElementRect.GetHeight() + m_iGap;
			
			if (guiElementRect.GetWidth() > menuRect.GetWidth()) {
				menuRect.SetWidth(guiElementRect.GetWidth());
			}

			menuRect.SetHeight(m_iOffset);

			break;
		}
		default: {
			break;
		}
	}

	_pElement->SetBoundingBox(guiElementRect);
	SetBoundingBox(menuRect);*/

	CRect menuRect = GetBoundingBox();
	CRect elementRect = _pElement->GetBoundingBox();

	if (m_eDirection == DIR_VERTICAL) {
		if (elementRect.GetWidth() > menuRect.GetWidth()) {
			menuRect.SetWidth(elementRect.GetWidth());
		}

		menuRect.SetHeight(menuRect.GetHeight() + elementRect.GetHeight() + m_iGap);
	}
	else {
		if (elementRect.GetHeight() > menuRect.GetHeight()) {
			menuRect.SetHeight(elementRect.GetHeight());
		}

		menuRect.SetWidth(menuRect.GetWidth() + elementRect.GetWidth() + m_iGap);
	}

	SetBoundingBox(menuRect);

	_pElement->SetAnchor(GetAnchor());
	_pElement->SetAlpha(GetAlpha());
	m_guiElements.push_back(_pElement);

	m_iOffset = 0;

	if (GetAnchor().m_eHorizontalAnchoring == ANCHOR_MIDDLE) {
		menuRect.SetX(menuRect.GetX() - static_cast<int>(static_cast<float>(menuRect.GetWidth()) / 4.0f));
	}
	
	if (GetAnchor().m_eVerticalAnchoring == ANCHOR_MIDDLE) {
		menuRect.SetY(menuRect.GetY() - static_cast<int>(static_cast<float>(menuRect.GetHeight()) / 4.0f));
	}

	if ((m_eDirection == DIR_VERTICAL && GetAnchor().m_eVerticalAnchoring == ANCHOR_RIGHT_BOTTOM) ||
		(m_eDirection == DIR_HORIZONTAL && GetAnchor().m_eHorizontalAnchoring == ANCHOR_RIGHT_BOTTOM)) {

		for (int i = m_guiElements.size() - 1; i >= 0; --i) {
			CRect elementRect = m_guiElements[i]->GetBoundingBox();

			if (m_eDirection == DIR_VERTICAL) {
				elementRect.SetX(menuRect.GetX());
				elementRect.SetY(menuRect.GetY() + m_iOffset);
				m_iOffset += elementRect.GetHeight() + m_iGap;
			}
			else {
				elementRect.SetY(menuRect.GetY());
				elementRect.SetX(menuRect.GetX() + m_iOffset);
				m_iOffset += elementRect.GetWidth() + m_iGap;
			}

			m_guiElements[i]->SetBoundingBox(elementRect);
		}
	}
	else {
		for (unsigned int i = 0; i < m_guiElements.size(); ++i) {
			CRect elementRect = m_guiElements[i]->GetBoundingBox();

			if (m_eDirection == DIR_VERTICAL) {
				elementRect.SetX(menuRect.GetX());
				elementRect.SetY(menuRect.GetY() + m_iOffset);
				m_iOffset += elementRect.GetHeight() + m_iGap;
			}
			else {
				elementRect.SetY(menuRect.GetY());
				elementRect.SetX(menuRect.GetX() + m_iOffset);
				m_iOffset += elementRect.GetWidth() + m_iGap;
			}

			m_guiElements[i]->SetBoundingBox(elementRect);
		}
	}

	/*if (m_eDirection == DIR_VERTICAL && GetAnchor().m_eVerticalAnchoring == ANCHOR_RIGHT_BOTTOM) {
		m_iOffset = 0;

		for (int i = m_guiElements.size() - 1; i >= 0; --i) {
			CRect elementRect = m_guiElements[i]->GetBoundingBox();
			elementRect.SetX(menuRect.GetX());
			elementRect.SetY(menuRect.GetY() + m_iOffset);
			m_iOffset += elementRect.GetHeight() + m_iGap;
			m_guiElements[i]->SetBoundingBox(elementRect);
		}
	}
	else if (m_eDirection == DIR_HORIZONTAL && GetAnchor().m_eHorizontalAnchoring == ANCHOR_RIGHT_BOTTOM) {
		m_iOffset = 0;

		for (int i = m_guiElements.size() - 1; i >= 0; --i) {
			CRect elementRect = m_guiElements[i]->GetBoundingBox();
			elementRect.SetY(menuRect.GetY());
			elementRect.SetX(menuRect.GetX() + m_iOffset);
			m_iOffset += elementRect.GetWidth() + m_iGap;
			m_guiElements[i]->SetBoundingBox(elementRect);
		}
	}*/

	if (m_guiElements.size() == 1 && IsSelected()) {
		SetSelection(0);
	}
}

bool CMenu::RemoveElement(const std::string& _krElementName)
{
	for (auto iter = m_guiElements.begin(); iter != m_guiElements.end(); ++iter) {
		if ((*iter)->GetName() == _krElementName) {
			m_guiElements.erase(iter);
			return true;
		}
	}
	return false;
}

void CMenu::ClearElements()
{
	m_guiElements.clear();
}

void CMenu::IncrementSelection()
{
	int iNewSelection = m_iSelection + 1;
	SetSelection(iNewSelection);
}

void CMenu::DecrementSelection()
{
	int iNewSelection = m_iSelection - 1;
	SetSelection(iNewSelection);
}

void CMenu::SetSelection(int _iSelection)
{
	if (m_guiElements.empty()) {
	// No need to make a selection if there's no buttons.
		m_iSelection = 0;
		return;
	}

	// Set the previous selection to false.
	if (m_guiElements[m_iSelection]) {
		m_guiElements[m_iSelection]->SetSelected(false);
	}

	m_iSelection = _iSelection;

	// Wrap the current selection.
	if (m_iSelection >= static_cast<int>(m_guiElements.size())) {
		m_iSelection = 0;
	}
	else if (m_iSelection < 0) {
		m_iSelection = static_cast<int>(m_guiElements.size()) - 1;
	}

	// Set the new selection.
	if (m_guiElements[m_iSelection]) {
		m_guiElements[m_iSelection]->SetSelected(true);
	}
}
