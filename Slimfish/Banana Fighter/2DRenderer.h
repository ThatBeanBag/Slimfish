// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: 2DRenderer.h
// Description	: C2DRenderer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __2DRENDERER_H__
#define __2DRENDERER_H__

// Library Includes

// Local Includes
#include <Graphics/SlimTexture.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimRenderPass.h>

class C2DRenderer {
	// Member Functions
	/** Vertex structure for rendering textures to the screen.
	@remarks

	*/
	struct TQuadVertex {
		CVector3 position;
		CVector2 uv;
		CColourValue colour;
	};

public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	C2DRenderer();
	
	/** Destructor
		@author Hayden Asplet
	*/
	~C2DRenderer();

	/** Initialise the 2D renderer.
	 	@author Hayden Asplet
	 	@return True if the initialisation was successful, false otherwise.
	*/
	bool Initialise();

	/** Prepare for rendering 2D images.
	 	@author Hayden Asplet
	*/
	void Prepare(); 

	/** Render a texture to the screen.
	 	@author Hayden Asplet
		@param srcRect Rectangle that defines the region from the source texture to draw.
		@param destRect Rectangle that defines the region on the screen to draw the texture to.
	 	@param pImage Pointer to the texture to render.
		@param colour Colour to multiply the image by. Set the alpha to lower the opacity.
	*/
	void Render(const CRect<>& srcRect, const CRect<>& destRect, 
		std::shared_ptr<ATexture> pImage, const CColourValue& colour = CColourValue::s_WHITE);
protected:
private:
	// Member Variables
public:
protected:
private:
	CVertexDeclaration m_VertexDeclaration;
	CRenderPass m_RenderPass;
	CTextureLayer* m_pTextureLayer;
	std::shared_ptr<AVertexGpuBuffer> m_pQuadBuffer;
};

#endif	// __2DRENDERER_H__