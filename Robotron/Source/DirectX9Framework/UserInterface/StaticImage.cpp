// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: StaticImage.cpp
// Description	: CStaticImage implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "StaticImage.h"

// Local Includes


CStaticImage::CStaticImage(const std::string& _krstrName, const CRect& _krRect, shared_ptr<ISprite> _pSprite)
	:AGUIElement(_krstrName, _krRect),
	m_pSprite(_pSprite)
{
	// Get the sprite dimensions.
	CPoint dimensions = _pSprite->VGetDimensions();

	// Set the source rectangle to the entire image by default.
	m_sourceRect.SetWidth(dimensions.GetX());
	m_sourceRect.SetHeight(dimensions.GetY());
}

CStaticImage::~CStaticImage()
{

}

void CStaticImage::VRender()
{
	if (m_pSprite) {
		m_pSprite->VRender(GetWorldBoundingBox(), m_sourceRect, GetAlpha());
	}
	else {
		DEBUG_WARNING("No sprite to render.");
	}
}

void CStaticImage::SetSourceRect(const CRect& _krRect)
{
	m_sourceRect = _krRect;
}

void CStaticImage::SetImage(shared_ptr<ISprite> _pSprite)
{
	m_pSprite = _pSprite;
}

shared_ptr<ISprite> CStaticImage::GetImage()
{
	return m_pSprite;
}

void CStaticImage::RenderImage(const CRect& _krRect)
{
	SetSourceRect(_krRect);
	CStaticImage::VRender();
}
