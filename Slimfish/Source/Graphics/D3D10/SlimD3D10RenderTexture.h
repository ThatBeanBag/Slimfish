// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10RenderTexture.h
// Description	: CD3D10RenderTexture declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10RENDERTEXTURE_H__
#define __SLIMD3D10RENDERTEXTURE_H__

// Library Includes

// Local Includes
#include "../SlimRenderTexture.h"
#include "SlimD3D10Forward.h"

namespace Slim {

class CD3D10RenderTexture : public ARenderTexture {
	// Member Functions
public:
	CD3D10RenderTexture(ID3D10Device* pDevice, shared_ptr<ATexture> pTexture);

	virtual ~CD3D10RenderTexture();

	ID3D10DepthStencilView* GetDepthStencilView();
	ID3D10RenderTargetView* GetRenderTargetView();
protected:
private:
	// Member Variables
public:
protected:
private:
	ID3D10Device* m_pD3DDevice;

	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10DepthStencilView* m_pDepthStencilView;
};

}

#endif	// __SLIMD3D10RENDERTEXTURE_H__