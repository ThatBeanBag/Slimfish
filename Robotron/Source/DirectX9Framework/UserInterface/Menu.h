// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Menu.h
// Description	: CMenu declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MENU_H__
#define __MENU_H__

// Library Includes

// Local Includes
#include "Button.h"

class CMenu : public AGUIElement {
	typedef std::vector<AGUIElement*> TGUIElements;
public:
	enum EDirection {
		DIR_VERTICAL,
		DIR_HORIZONTAL
	};

	enum EAlignment {
		ALIGN_LEFT,		// Also top alignment.
		ALIGN_RIGHT,	// Also bottom alignment.
		ALIGN_MIDDLE
	};

	// Member Functions
public:
	CMenu(const std::string& _krstrName,
		  int _iX, int _iY, int _iGap = 0, 
		  EAlignment _eAlignment = ALIGN_LEFT, 
		  EDirection _eDirection = DIR_VERTICAL);
	virtual ~CMenu();

	void AddGUIElement(AGUIElement* _pElement);
	bool RemoveElement(const std::string& _krElementName);
	void ClearElements();
protected:
private:
	virtual bool VHandleEvents(const TAppMsg& _krMsg) override;
	virtual void VUpdate(float _fDeltaTime) override;
	virtual void VRender() override;
	virtual void VSetAlpha(unsigned char _ucAlpha) override;
	virtual void VSetVisibility(bool _bIsVisible) override;
	virtual void VSetSelected(bool _bIsSelected) override;

	void IncrementSelection();
	void DecrementSelection();
	void SetSelection(int _iSelection);

	// Member Variables
public:
protected:
private:
	int m_iSelection;
	int m_iOffset;
	int m_iGap;
	EDirection m_eDirection;
	EAlignment m_eAlignment;

	TGUIElements m_guiElements;

	CPoint m_lastLeftThumb;
	CPoint m_lastMousePosition;
};

#endif	// __MENU_H__