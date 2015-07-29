// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9Sprite.h
// Description	: CD3D9Sprite declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __D3D9SPRITE_H__
#define __D3D9SPRITE_H__

// Library Includes

// Local Includes
#include "Sprite.h"

class CD3D9Sprite : public ISprite {
	// Member Functions
public:
	CD3D9Sprite(ID3DXSprite* _pSprite, IDirect3DTexture9* _pTexture);
	virtual ~CD3D9Sprite();

	virtual const CPoint VGetDimensions() const override;
	virtual void VRender(const CRect& _krDestRect, const CRect& _krSourceRect = CRect(), unsigned char _ucAlpha = 255) override;

protected:
private:
	// Member Variables
public:
protected:
private:
	ID3DXSprite* m_pSprite;	// The sprite to draw to. Owned by the renderer.
	IDirect3DTexture9* m_pTexture;	// The texture to draw. Owned by us.
};

#endif	// __D3D9SPRITE_H__