// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11Texture.h
// Description	: CSlimD3D11Texture declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D11TEXTURE_H__
#define __SLIMD3D11TEXTURE_H__

// Library Includes

// Local Includes
#include "../SlimTexture.h"
#include "SlimD3D11Forward.h"

namespace Slim {

/** Specialization of ATexture for DirectX 11.	
*/
class CD3D11Texture : public ATexture {
	// Member Functions
public:
	CD3D11Texture(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pImmediateContext, const std::string& name, 
			ETextureType textureType = ETextureType::TYPE_2D, ETextureUsage usage = ETextureUsage::STATIC);
	~CD3D11Texture();

	virtual void VLoad() override;
	virtual void VLoadRaw(const void* pData, size_t stride) override;
	virtual void VUnload() override;

	virtual const CImage VGetImage() const;

	ID3D11ShaderResourceView* GetD3DTexture();
	ID3D11Resource* GetD3DResource();
protected:
private:
	void CreateRenderTarget1D(const void* pData = nullptr, size_t stride = 0);
	void CreateRenderTarget2D(const void* pData = nullptr, size_t stride = 0);
	void CreateRenderTarget3D(const void* pData = nullptr, size_t stride = 0);

	void CreateShaderResourceView1D(D3D11_TEXTURE1D_DESC desc);
	void CreateShaderResourceView2D(D3D11_TEXTURE2D_DESC desc);
	void CreateShaderResourceView3D(D3D11_TEXTURE3D_DESC desc);

	// Member Variables
public:
protected:
private:
	ID3D11Device* m_pD3DDevice;		// The D3D Device.
	ID3D11DeviceContext* m_pD3DImmediateContext;

	ComPtr<ID3D11Texture1D> m_pTexture1D;	// DirectX 10 1D texture. Only valid if the texture is of type TEXTURE_TYPE_1D.
	ComPtr<ID3D11Texture2D> m_pTexture2D;	// DirectX 10 2D texture. Only valid if the texture is of type TEXTURE_TYPE_2D.
	ComPtr<ID3D11Texture3D> m_pTexture3D;	// DirectX 10 3D texture. Only valid if the texture is of type TEXTURE_TYPE_3D.
	ComPtr<ID3D11Resource> m_pTexture;

	ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;	// View for shaders.
};

}

#endif // __SLIMD3D11TEXTURE_H__