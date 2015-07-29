// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GUIElement.h
// Description	: AGUIElement declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __WIDGET_H__
#define __WIDGET_H__

// Library Includes

// Local Includes
#include "../Utilities/Rect.h"
#include "../Utilities/Colour.h"
#include "../View/AppMsg.h"

typedef enum {
	ANCHOR_LEFT_TOP = 0,
	ANCHOR_MIDDLE,
	ANCHOR_RIGHT_BOTTOM
} EAnchoring;

struct TAnchor {
	EAnchoring m_eVerticalAnchoring;
	EAnchoring m_eHorizontalAnchoring;
};

/************************************************************************/
/* AGUIElement - Abstract class that defines the interface for the GUI 
/* elements of the game.
/************************************************************************/
class AGUIElement {
	// Member Functions
	friend class CBaseGUI;	// For exclusive access to set owner.
public:
	AGUIElement(const std::string& _krstrName, const CRect& _krBoundingBox);

	// Make AGUIElement abstract.
	virtual ~AGUIElement() = 0;

	void SetName(std::string& _krName);
	bool HandleEvents(const TAppMsg& _krMsg);
	void Update(float _fDeltaTime);
	void Render();
	void SetAlpha(unsigned char _ucAlpha);
	void SetVisibility(bool _bIsVisible);
	void SetSelected(bool _bSelected);
	void SetActive(bool _bIsActive);

	void SetPosition(const CPoint& _krPosition);
	void SetBoundingBox(const CRect& _krBoundingBox);
	void SetAnchor(TAnchor _anchor);
	void SetTargetScreenSize(const CPoint& _krTargetScreenSize);

	const std::string GetName() const;
	const CPoint GetPosition() const;
	const CRect GetBoundingBox() const;
	const CPoint GetWorldPosition();
	const CRect GetWorldBoundingBox();
	unsigned char GetAlpha() const;
	bool IsVisible() const;
	bool IsSelected() const;
	bool IsActive() const;
	const TAnchor GetAnchor() const;
protected:
	CBaseGUI* GetOwner();
private:
	virtual bool VHandleEvents(const TAppMsg& _krMsg) { return false; }
	virtual void VUpdate(float _fDeltaTime) {}
	virtual void VRender() {}
	virtual void VSetAlpha(unsigned char _ucAlpha) {}
	virtual void VSetVisibility(bool _bIsVisible) {}
	virtual void VSetSelected(bool _bIsSelected) {}

	void SetOwner(CBaseGUI* _pBaseGUI);

	// Member Variables
public:
protected:
private:
	std::string m_strName;
	CRect m_boundingBox;
	unsigned char m_ucAlpha;
	TAnchor m_anchor;

	bool m_bIsActive;	// True if the element should be drawn but isn't handling events.
	bool m_bIsVisible;	// True if the element is visible, false if the element shouldn't be displayed and shouldn't handle events.
	bool m_bIsSelected;	// True if the element is selected.

	CBaseGUI* m_pOwner;
	CPoint m_targetScreenSize;
};

#endif	// __WIDGET_H__