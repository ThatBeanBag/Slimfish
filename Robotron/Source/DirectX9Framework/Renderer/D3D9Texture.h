// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9Texture.h
// Description	: CD3D9Texture declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __D3D9TEXTURE_H__
#define __D3D9TEXTURE_H__

// Library Includes

// Local Includes
#include "Texture.h"

class CD3D9Texture : public ITexture {
	// Member Functions
public:
	CD3D9Texture(IDirect3DDevice9* _pD3DDevice, IDirect3DTexture9* _pTexture);
	virtual ~CD3D9Texture();

	virtual void VSetToRenderer(ETextureStage _eTextureStage) override;
	virtual void VSetTranslation(float _fX, float _fY) override;

protected:
private:
	// Member Variables
public:
protected:
	IDirect3DDevice9* m_pDevice;
	IDirect3DTexture9* m_pTexture;

	float m_fX;
	float m_fY;
private:
};

#endif	// __D3D9TEXTURE_H__