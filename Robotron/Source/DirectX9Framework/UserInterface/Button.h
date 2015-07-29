// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Button.h
// Description	: CButton declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __BUTTON_H__
#define __BUTTON_H__

// Library Includes

// Local Includes
#include "..\3rdParty\fastdelegate\FastDelegate.h"
#include "StaticImage.h"

typedef fastdelegate::FastDelegate0<> TButtonDelegate;

/************************************************************************/
/* CButton - The main GUI element
/*
/* Buttons have 3 states:
/* STATE_OUT - The mouse cursor is outside of the button's bounding rectangle.
/* STATE_HOVER - The mouse cursor is over the button's bounding rectangle.
/* STATE_PRESSED - The use has clicked on the button.
/*
/* Each button is assigned a portion of the button's sprite.
/* The TMasks are the source rectangles assigned to each state this helps
/* to change the appearance of button according to it's current state.
/*
/* When a button is clicked on, a GUI event is trigger which will call
/* the GUI event callback of the owner GUI. 
/************************************************************************/
class CButton : public CStaticImage {
	typedef std::vector<CRect> TMasks;
public:

	enum EButtonState {
		STATE_OUT,
		STATE_HOVER,
		STATE_PRESSED,
		STATE_MAX
	};

	// Member Functions
public:
	CButton(const CStaticImage& _krStaticImage, bool _bIsToggled = false);
	CButton(const std::string& _krstrName, const CRect& _krBoundingBox, shared_ptr<ISprite> _pSprite, bool _bIsToggled = false);
	virtual ~CButton();

	void SetMask(EButtonState _eButtonState, const CRect& _krMask);
	void SetState(EButtonState _eButtonState);
	CRect GetMask(EButtonState eButtonState) const;
	EButtonState GetState() const;
protected:
private:
	virtual bool VHandleEvents(const TAppMsg& _krMsg) override;
	virtual void VRender() override;
	virtual void VSetSelected(bool _bIsSelected) override;

	void Release();
	void Press();

	// Member Variables
public:
protected:
private:
	TMasks m_masks;				// The masks of the source image for the various states of the button.
	EButtonState m_eButtonState;// The button's state (out, hover or pressed).
	CPoint m_lastMousePosition;
	bool m_bIsToggled;
};

#endif	// __BUTTON_H__