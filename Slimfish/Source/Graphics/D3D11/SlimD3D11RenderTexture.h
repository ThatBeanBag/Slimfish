// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11RenderTexture.h
// Description	: CD3D11RenderTexture declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D11RENDERTEXTURE_H__
#define __SLIMD3D11RENDERTEXTURE_H__

// Library Includes

// Local Includes
#include "../SlimRenderTexture.h"
#include "SlimD3D11Forward.h"

namespace Slim {

class CD3D11RenderTexture : public ARenderTexture {
	// Member Functions
public:
	CD3D11RenderTexture(ID3D11Device* pDevice, shared_ptr<ATexture> pTexture);

	virtual ~CD3D11RenderTexture();

	ID3D11DepthStencilView* GetDepthStencilView();
	ID3D11RenderTargetView* GetRenderTargetView();
protected:
private:

	// Member Variables
public:
protected:
private:
	ID3D11Device* m_pD3DDevice;

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
};

}

#endif	// __SLIMD3D11RENDERTEXTURE_H__