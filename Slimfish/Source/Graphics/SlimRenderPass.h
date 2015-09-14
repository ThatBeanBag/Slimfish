// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRenderPass.h
// Description	: CRenderPass declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMRENDERPASS_H__
#define __SLIMRENDERPASS_H__

// Library Includes

// Local Includes
#include "SlimTypes.h"
#include "SlimTextureLayer.h"

namespace Slim {

// Forward Declaration.
class AShaderProgram;

/** Class representing a single render pass of a material.
@remarks
	Complex techniques require multiple passes thus, passes have
	a many to one relationship with techniques. The same way techniques have 
	a many to one relationship with materials. 
*/
class CRenderPass {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CRenderPass();

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CRenderPass();

	/** Add a new texture layer to the pass.
		@remarks
			The texture layer is added to the back of the existing texture layers.
	 	@author Hayden Asplet
	 	@param textureLayer Texture layer to add.
	*/
	void AddTextureLayer(const CTextureLayer& textureLayer);

	/** Add a new texture layer to the pass by loading a texture and creating a texture layer from it.
		@remarks
			The new texture layer is added to the back of the existing texture layers.
	 	@author Hayden Asplet
	 	@param 
			textureName The name of the texture to load e.g. waterNormalMap.jpg, brickDiffuse.png etc.
	 	@return 
			Reference to the created texture layer for further modification.
	*/
	CTextureLayer& AddTextureLayer(const std::string& textureName);

	/** Retrieve a texture layer at the index specified.
	 	@author Hayden Asplet
	 	@param layerIndex Index of the layer to get (must be in range of the number of texture layers).
	 	@return Reference to the texture layer for modification and reading.
	*/
	CTextureLayer& GetTextureLayer(size_t layerIndex);

	/** Remove a texture layer at an index.
	 	@author Hayden Asplet
	 	@param layerIndex Index of the layer to remove (must be in range of the number of texture layers).
	*/
	void RemoveTextureLayer(size_t layerIndex);

	/** Remove all texture layers. @author Hayden Asplet */
	void ClearTextureLayers();

	/** Retrieve the number of texture layers of the render pass. @author Hayden Asplet */
	size_t GetNumTextureLayers() const;

	/** Set the vertex shader of the pass. @author Hayden Asplet */
	void SetVertexShader(shared_ptr<AShaderProgram> shader);

	/** Get the vertex shader of the pass (not valid if the pass does not have a vertex shader).
	 	@author Hayden Asplet
	 	@return Pointer to the vertex shader or null if the pass does not have a vertex shader.
	*/
	const shared_ptr<AShaderProgram> GetVertexShader() const;

	/** Set the pixel shader of the pass. @author Hayden Asplet */
	void SetPixelShader(shared_ptr<AShaderProgram> shader);

	/** Get the pixel shader of the pass (not valid if the pass does not have pixel shader).
		@author Hayden Asplet
		@return Pointer to the pixel shader or null if the pass does not have a pixel shader.
	*/
	const shared_ptr<AShaderProgram> GetPixelShader() const;

	/** Set the geometry shader of the pass. @author Hayden Asplet */
	void SetGeometryShader(shared_ptr<AShaderProgram> shader);

	/** Get the geometry shader of the pass (not valid if the pass does not have a geometry shader).
		@author Hayden Asplet
		@return Pointer to the vertex shader or null if the pass does not have a geometry shader.
	*/
	const shared_ptr<AShaderProgram> GetGeometryShader() const;

	/** Set the cull mode of the pass. @author Hayden Asplet */
	void SetCullMode(ECullingMode cullingMode);
	/** Get the cull mode of the pass. @author Hayden Asplet */
	const ECullingMode GetCullMode() const;
	/** Set the fill mode of the pass. @author Hayden Asplet */
	void SetFillMode(EFillMode fillMode);
	/** Get the fill mode of the pass. @author Hayden Asplet */
	const EFillMode GetFillMode() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	// Texture layers.
	std::vector<CTextureLayer> m_TextureLayers;

	// Shader programs of the pass. 
	// May not all be valid if the pass does not require the programmable pipeline.
	shared_ptr<AShaderProgram> m_pVertexShader;
	shared_ptr<AShaderProgram> m_pPixelShader;
	shared_ptr<AShaderProgram> m_pGeometryShader;

	// Colour blending.
	EBlendFactor m_SourceColourBlendFactor;
	EBlendFactor m_DestColourBlendFactor;
	EBlendOperation m_ColourBlendOperation;
	// Alpha blending.
	EBlendFactor m_SourceAlphaBlendFactor;
	EBlendFactor m_DestAlphaBlendFactor;
	EBlendOperation m_AlphaBlendOperation;

	ECullingMode m_CullMode;
	EFillMode m_FillMode;
};

}

#endif	// __SLIMRENDERPASS_H__