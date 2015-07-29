// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9NormalMap.h
// Description	: CD3D9NormalMap declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __D3D9NORMALMAP_H__
#define __D3D9NORMALMAP_H__

// Library Includes

// Local Includes
#include "NormalMap.h"

class CD3D9NormalMap : public INormalMap {
	// Member Functions
public:
	CD3D9NormalMap(IDirect3DDevice9* _pD3DDevice, IDirect3DTexture9* _pTexture);
	virtual ~CD3D9NormalMap();

	virtual void VSetToRenderer(ETextureStage _eTextureStage) override;
	virtual void SetLight(const TLight& _krLight) override;
	virtual void VSetTranslation(float _fX, float _fY) override;
protected:
private:
	// Member Variables
public:
protected:
private:
	float m_fX;
	float m_fY;

	TLight m_light;
	IDirect3DDevice9* m_pDevice;
	IDirect3DTexture9* m_pTexture;
};

#endif	// __D3D9NORMALMAP_H__