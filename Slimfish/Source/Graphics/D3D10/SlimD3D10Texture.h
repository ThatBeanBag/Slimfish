// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10Texture.h
// Description	: CSlimD3D10Texture declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10TEXTURE_H__
#define __SLIMD3D10TEXTURE_H__

// Library Includes

// Local Includes
#include "../SlimTexture.h"
#include "SlimD3D10Forward.h"

namespace Slim {

class CD3D10Texture : public ATexture {
	// Member Functions
public:
	CD3D10Texture(ID3D10Device* pD3DDevice, const std::string& name, ETextureType textureType, ETextureUsage usage);
	~CD3D10Texture();

	virtual void VLoad() override;
	virtual void VUnload() override;

	virtual const CImage VGetImage() const;

	ID3D10ShaderResourceView* GetD3DTexture();
	ID3D10Resource* GetD3DResource();
protected:
private:
	void CreateRenderTarget1D();
	void CreateRenderTarget2D();
	void CreateRenderTarget3D();

	void CreateShaderResourceView1D(D3D10_TEXTURE1D_DESC desc);
	void CreateShaderResourceView2D(D3D10_TEXTURE2D_DESC desc);
	void CreateShaderResourceView3D(D3D10_TEXTURE3D_DESC desc);

	// Member Variables
public:
protected:
private:
	ID3D10Device* m_pD3DDevice;		// The D3D Device.

	ID3D10Texture1D* m_pTexture1D;	// DirectX 10 1D texture. Only valid if the texture is of type TEXTURE_TYPE_1D.
	ID3D10Texture2D* m_pTexture2D;	// DirectX 10 2D texture. Only valid if the texture is of type TEXTURE_TYPE_2D.
	ID3D10Texture3D* m_pTexture3D;	// DirectX 10 3D texture. Only valid if the texture is of type TEXTURE_TYPE_3D.
	ID3D10Resource* m_pTexture;		// Actual texture handle.

	ID3D10ShaderResourceView* m_pShaderResourceView;	// View for shaders.
};

}

#endif // __SLIMD3D10TEXTURE_H__