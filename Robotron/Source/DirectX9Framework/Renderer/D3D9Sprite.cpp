// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9Sprite.cpp
// Description	: CD3D9Sprite implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "D3D9Sprite.h"

// Local Includes



CD3D9Sprite::CD3D9Sprite(ID3DXSprite* _pSprite, IDirect3DTexture9* _pTexture)
	:m_pSprite(_pSprite),
	m_pTexture(_pTexture)
{

}

CD3D9Sprite::~CD3D9Sprite()
{
	SafeRelease(m_pTexture);
}

void CD3D9Sprite::VRender(const CRect& _krDestRect, const CRect& _krSourceRect, unsigned char _ucAlpha)
{
	// Make sure we have a sprite and texture.
	DEBUG_ASSERT(m_pSprite && m_pTexture);

	if (_krDestRect.GetWidth() <= 0 || _krDestRect.GetHeight() <= 0) {
		DEBUG_WARNING("Can't draw image at a destination rectangle with no width or height.");
		return;
	}

	CPoint dimensions = VGetDimensions();

	// Create the source rect.
	RECT srcRect;
	if (_krSourceRect == CRect()) {
	// Was the default parameter entered?
		// Use the entire source image.
		srcRect.left = 0;
		srcRect.top = 0;
		srcRect.right = dimensions.GetX();
		srcRect.bottom = dimensions.GetY();
	}
	else {
		srcRect.left = _krSourceRect.GetLeft();
		srcRect.top = _krSourceRect.GetTop();
		srcRect.right = _krSourceRect.GetRight();
		srcRect.bottom = _krSourceRect.GetBottom();
	}

	int iSourceWidth = srcRect.right - srcRect.left;
	int iSourceHeight = srcRect.bottom - srcRect.top;

	if (iSourceWidth <= 0 || iSourceHeight <= 0) {
		return;
	}

	// Get the scale factors to scale from source to destination.
	float fScaleX = static_cast<float>(_krDestRect.GetWidth()) / static_cast<float>(iSourceWidth);
	float fScaleY = static_cast<float>(_krDestRect.GetHeight()) / static_cast<float>(iSourceHeight);

	// Build and set a scale matrix to scale the image from source to destination.
	CMatrix4x4 matScale = BuildScale(fScaleX, fScaleY, 1);
	m_pSprite->SetTransform(&matScale);

	// Pre-unscale the position as it will get scaled by the transform.
	CVec3 vec3Position(_krDestRect.GetX() / fScaleX, _krDestRect.GetY() / fScaleY, 0);

	// Draw the texture to the sprite.
	m_pSprite->Draw(m_pTexture, &srcRect, nullptr, &vec3Position, D3DCOLOR_ARGB(_ucAlpha, 255, 255, 255));
}

const CPoint CD3D9Sprite::VGetDimensions() const
{
	if (m_pTexture) {
	// Do we have a texture?
		// Get the surface description.
		D3DSURFACE_DESC d3dDesc;
		m_pTexture->GetLevelDesc(0, &d3dDesc);

		return CPoint(d3dDesc.Width, d3dDesc.Height);
	}
	else {
		DEBUG_WARNING("No texture when getting dimensions.");
	}

	return CPoint(0, 0);
}
