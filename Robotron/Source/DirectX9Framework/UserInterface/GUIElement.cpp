// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AGUIElement.cpp
// Description	: AGUIElement implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "GUIElement.h"

// Local Includes

AGUIElement::AGUIElement(const std::string& _krstrName, const CRect& _krBoundingBox)
	:m_strName(_krstrName),
	m_boundingBox(_krBoundingBox),
	m_ucAlpha(g_kcALPHA_OPAQUE),
	m_bIsVisible(true),
	m_bIsActive(true),
	m_bIsSelected(false)
{
	m_anchor.m_eHorizontalAnchoring = ANCHOR_LEFT_TOP;
	m_anchor.m_eVerticalAnchoring = ANCHOR_LEFT_TOP;

	m_bIsActive = true;

	SetTargetScreenSize(CPoint(0, 1080));
}

AGUIElement::~AGUIElement()
{

}

void AGUIElement::SetName(std::string& _krName)
{
	m_strName = _krName;
}

bool AGUIElement::HandleEvents(const TAppMsg& _krMsg)
{
	if (m_bIsVisible) {
		return VHandleEvents(_krMsg);
	}

	return false;
}

void AGUIElement::Update(float _fDeltaTime)
{
	if (m_bIsVisible) {
		VUpdate(_fDeltaTime);
	}
}

void AGUIElement::Render()
{
	if (m_bIsVisible) {
		VRender();
	}
}

void AGUIElement::SetAlpha(unsigned char _ucAlpha)
{
	m_ucAlpha = _ucAlpha;
	VSetAlpha(_ucAlpha);
}

void AGUIElement::SetVisibility(bool _bIsVisible)
{
	m_bIsVisible = _bIsVisible;
	VSetVisibility(_bIsVisible);
}


void AGUIElement::SetSelected(bool _bSelected)
{
	m_bIsSelected = _bSelected;
	VSetSelected(_bSelected);
}

void AGUIElement::SetActive(bool _bIsActive)
{
	m_bIsActive = _bIsActive;

	if (m_bIsActive) {
		SetAlpha(255);
	}
	else {
		SetAlpha(50);
	}
}

void AGUIElement::SetAnchor(TAnchor	_anchor)
{
	m_anchor = _anchor;
}

void AGUIElement::SetTargetScreenSize(const CPoint& _krTargetScreenSize)
{
	m_targetScreenSize = _krTargetScreenSize;
}

void AGUIElement::SetPosition(const CPoint& _krPosition)
{
	m_boundingBox.SetPosition(_krPosition);
}

void AGUIElement::SetBoundingBox(const CRect& _krBoundingBox)
{
	m_boundingBox = _krBoundingBox;
}

const std::string AGUIElement::GetName() const
{
	return m_strName;
}

const CPoint AGUIElement::GetPosition() const
{
	return m_boundingBox.GetPosition();
}

const CRect AGUIElement::GetBoundingBox() const
{
	return m_boundingBox;
}

const CPoint AGUIElement::GetWorldPosition()
{
	CRect worldBoundingBox = GetWorldBoundingBox();

	return worldBoundingBox.GetPosition();
}

const CRect AGUIElement::GetWorldBoundingBox()
{
	int iScreenWidth = g_pApp->GetScreenWidth();
	int iScreenHeight = g_pApp->GetScreenHeight();

	float fScale = static_cast<float>(iScreenHeight) / static_cast<float>(m_targetScreenSize.GetY());
	/*iScreenWidth = static_cast<int>(iScreenWidth * fScale);
	iScreenHeight = static_cast<int>(iScreenHeight * fScale);*/
	fScale = 1.0f;

	CRect worldBoundingBox = m_boundingBox;
	worldBoundingBox.SetWidth(static_cast<int>(worldBoundingBox.GetWidth() * fScale));
	worldBoundingBox.SetHeight(static_cast<int>(worldBoundingBox.GetHeight() * fScale));
	worldBoundingBox.SetX(static_cast<int>(worldBoundingBox.GetX() * fScale));
	worldBoundingBox.SetY(static_cast<int>(worldBoundingBox.GetY() * fScale));

	if (m_anchor.m_eHorizontalAnchoring == ANCHOR_RIGHT_BOTTOM) {
		worldBoundingBox.SetX(iScreenWidth - (worldBoundingBox.GetX() + worldBoundingBox.GetWidth()));
	}

	if (m_anchor.m_eHorizontalAnchoring == ANCHOR_MIDDLE || m_anchor.m_eVerticalAnchoring == ANCHOR_MIDDLE) {
		CPoint screenCentre(static_cast<int>(static_cast<float>(iScreenWidth / 2.0f)),
			static_cast<int>(static_cast<float>(iScreenHeight / 2.0f)));
		CPoint elementCentre = worldBoundingBox.GetPosition() + screenCentre;

		if (m_anchor.m_eHorizontalAnchoring == ANCHOR_MIDDLE) {
			int iHalfWidth = static_cast<int>(static_cast<float>(worldBoundingBox.GetWidth() / 2.0f));
			worldBoundingBox.SetX(elementCentre.GetX() - iHalfWidth);
		}

		if (m_anchor.m_eVerticalAnchoring == ANCHOR_MIDDLE) {
			int iHalfHeight = static_cast<int>(static_cast<float>(worldBoundingBox.GetHeight() / 2.0f));
			worldBoundingBox.SetY(elementCentre.GetY() - iHalfHeight);
		}
	}

	if (m_anchor.m_eVerticalAnchoring == ANCHOR_RIGHT_BOTTOM) {
		worldBoundingBox.SetY(iScreenHeight - (worldBoundingBox.GetY() + worldBoundingBox.GetHeight()));
	}

	return worldBoundingBox;
}

unsigned char AGUIElement::GetAlpha() const
{
	return m_ucAlpha;
}

bool AGUIElement::IsVisible() const
{
	return m_bIsVisible;
}

bool AGUIElement::IsSelected() const
{
	return m_bIsSelected;
}

bool AGUIElement::IsActive() const
{
	return m_bIsActive;
}

const TAnchor AGUIElement::GetAnchor() const
{
	return m_anchor;
}

CBaseGUI* AGUIElement::GetOwner()
{
	return m_pOwner;
}

void AGUIElement::SetOwner(CBaseGUI* _pBaseGUI)
{
	m_pOwner = _pBaseGUI;
}

