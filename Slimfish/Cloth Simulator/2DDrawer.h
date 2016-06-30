// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: 2DDrawer.h
// Description	: C2DDrawer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __2DDRAWER_H__
#define __2DDRAWER_H__

// Library Includes

// Local Includes
#include <Graphics/SlimTexture.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimRenderPass.h>

class C2DDrawer {
	// Member Functions
	struct TQuadVertex {
		CVector3 position;
		CVector2 uv;
	};

public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	C2DDrawer();
	
	/** Destructor
		@author Hayden Asplet
	*/
	~C2DDrawer();

	bool Initialise();

	void Render(const CRect<>& destRect, std::shared_ptr<ATexture> pImage);
protected:
private:
	// Member Variables
public:
protected:
private:
	CVertexDeclaration m_VertexDeclaration;
	CRenderPass m_2DRenderPass;
	CTextureLayer* m_pTextureLayer;
	std::shared_ptr<AVertexGpuBuffer> m_pQuadBuffer;
};

#endif	// __2DDRAWER_H__