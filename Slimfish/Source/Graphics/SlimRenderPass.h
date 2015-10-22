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
#include "SlimRenderQueue.h"

namespace Slim {

// Forward Declaration.
class AShaderProgram;

/** Class representing a single render pass of a material.
@remarks
	Complex techniques require multiple passes thus, passes have a many to one 
	relationship with techniques. The same way techniques have a many to one 
	relationship with materials. 
@par
	Defines everything that is specific to a single render pass. This includes
	texture layers. Shader programs for the programmable pipeline. Blending mode,
	colour writes enabled, rasterizer state descriptors, depth and stencil
	settings etc.
*/
class CRenderPass {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CRenderPass();

	CRenderPass(const CRenderPass& other);
	CRenderPass& operator=(const CRenderPass& other);

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
	CTextureLayer* AddTextureLayer(const CTextureLayer& textureLayer);

	/** Add a new texture layer to the pass by loading a texture and creating a texture layer from it.
		@remarks
			The new texture layer is added to the back of the existing texture layers.
	 	@author Hayden Asplet
	 	@param 
			textureName The name of the texture to load e.g. waterNormalMap.jpg, brickDiffuse.png etc.
	 	@return 
			Reference to the created texture layer for further modification.
	*/
	CTextureLayer* AddTextureLayer(const std::string& textureName);

	/**  Add a new texture layer to the pass.
	 	@author Hayden Asplet
	 	@return Pointer to the new texture layer for modification.
	*/
	CTextureLayer* AddTextureLayer();

	/** Retrieve a texture layer at the index specified.
	 	@author Hayden Asplet
	 	@param layerIndex Index of the layer to get (must be in range of the number of texture layers).
	 	@return Reference to the texture layer for modification and reading.
	*/
	CTextureLayer* GetTextureLayer(size_t layerIndex);

	/** Retrieve all textures layers of the render pass for iteration. @author Hayden Asplet. */
	const std::vector<std::unique_ptr<CTextureLayer>>& GetTextureLayers() const;

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

	/** Retrieves whether the pass has a vertex shader or not. @author Hayden Asplet */
	const bool HasVertexShader() const;
	/** Retrieves whether the pass has a pixel shader or not. @author Hayden Asplet */
	const bool HasPixelShader() const;
	/** Retrieves whether the pass has a geometry shader or not. @author Hayden Asplet */
	const bool HasGeometryShader() const;

	/** Set the blending mode of the pass (the blending factors and operations for colour and alpha).
	 	@author Hayden Asplet
		@param 
			blendingMode Structure describing how a source alpha/colour blends with the destinations
			alpha/colour.
	*/
	void SetBlendingMode(TBlendingMode blendingMode);

	/** Get the blending mode of the pass (the blending factors and operations for colour and alpha).
	 	@author Hayden Asplet
		@return 
			Structure describing how a source alpha/colour blends with the destinations
			alpha/colour.
	*/
	const TBlendingMode& GetBlendingMode() const;

	/** Set the source colour blend factor. @author Hayden Asplet */
	void SetSourceColourBlendFactor(EBlendFactor sourceColourBlendFactor);
	/** Get the source colour blend factor. @author Hayden Asplet */
	const EBlendFactor GetSourceColourBlendFactor() const;
	/** Set the destination colour blend factor. @author Hayden Asplet */
	void SetDestColourBlendFactor(EBlendFactor destColourBlendFactor);
	/** Get the destination colour blend factor. @author Hayden Asplet */
	const EBlendFactor GetDestColourBlendFactor() const;
	/** Set the colour blend operation. @author Hayden Asplet */
	void SetColourBlendOperation(EBlendOperation colourBlendOperation);
	/** Get the colour blend operation. @author Hayden Asplet */
	const EBlendOperation GetColourBlendOperation() const;
	/** Set the source alpha blend factor. @author Hayden Asplet */
	void SetSourceAlphaBlendFactor(EBlendFactor sourceAlphaBlendFactor);
	/** Get the source alpha blend factor. @author Hayden Asplet */
	const EBlendFactor GetSourceAlphaBlendFactor() const;
	/** Set the destination alpha blend factor. @author Hayden Asplet */
	void SetDestAlphaBlendFactor(EBlendFactor destAlphaBlendFactor);
	/** Get the destination alpha blend factor. @author Hayden Asplet */
	const EBlendFactor GetDestAlphaBlendFactor() const;
	/** Set the alpha blend operation. @author Hayden Asplet */
	void SetAlphaBlendOperation(EBlendOperation alphaBlendOperation);
	/** Get the alpha blend operation. @author Hayden Asplet */
	const EBlendOperation GetAlphaBlendOperation() const;

	/** Set the colour writes enabled to describe which colour channels are written to the back buffer.
	 	@author Hayden Asplet
	*/
	void SetColourWritesEnabled(bool red, bool blue, bool green, bool alpha);

	/** Convenience method to set the colour writes enabled for all colour channels at once.
	 	@author Hayden Asplet
	 	@param all True if all colours channels should be written to the back buffer.
	*/
	void SetColourWritesEnabled(bool all);

	/** Get the colour writes enabled that describe which colour channels should be written to the back buffer.
	 	@author Hayden Asplet
	*/
	const TColourWritesEnabled GetColourWritesEnabled();

	// Depth buffer settings.
	
	/** Set the depth check of the pass to enabled or disabled. @author Hayden Asplet */
	void SetDepthCheckEnabled(bool depthCheckEnabled);
	/** Get whether the depth check of the pass is enabled or disabled. @author Hayden Asplet */
	const bool GetDepthCheckEnabled() const;
	/** Set writing to the depth buffer to enabled or disabled for the pass. @author Hayden Asplet */
	void SetDepthWriteEnabled(bool depthWriteEnabled);
	/** Get whether the pass will write to the depth buffer or not. @author Hayden Asplet */
	const bool GetDepthWriteEnabled() const;
	/** Set the compare function to use when testing depth for the pass. @author Hayden Asplet */
	void SetDepthCompareFunction(EComparisonFunction compareFunction);
	/** Get the compare function to use when testing depth for the pass. @author Hayden Asplet */
	const EComparisonFunction GetDepthCompareFunction() const;

	// Stencil buffer settings.

	/** Set the stencil buffer settings of the pass. @author Hayden Asplet */
	void SetStencilBufferSettings(const TStencilBufferSettings& settings);
	/** Get the stencil buffer settings of the pass. @author Hayden Asplet */
	const TStencilBufferSettings& GetStencilBufferSettings() const;
	/** Set the stencil check of the pass to enabled or disabled. @author Hayden Asplet */
	void SetStencilCheckEnabled(bool enabled);
	/** Get whether the stencil check of the pass is enabled or disabled. @author Hayden Asplet */
	const bool GetStencilCheckEnabled() const;
	/** Set the compare function to use when testing stencil for the pass. @author Hayden Asplet */
	void SetStencilCompareFunction(EComparisonFunction compareFunction);
	/** Get the compare function to use when testing stencil for the pass. @author Hayden Asplet */
	const EComparisonFunction GetStencilCompareFunction() const;
	/** Set the stencil reference value to perform against when going a stencil test. @author Hayden Asplet */
	void SetStencilReferenceValue(size_t refValue);
	/** Get the stencil reference value to perform against when going a stencil test. @author Hayden Asplet */
	const size_t GetStencilReferenceValue() const;
	/** Set the stencil write mask to identify the portion of the depth-stencil buffer for writing stencil data.
	 	@author Hayden Asplet
	*/
	void SetStencilWriteMask(size_t mask);
	/** Get the stencil write mask that identifies the portion of the depth-stencil buffer for writing stencil data.
		@author Hayden Asplet
	*/
	const size_t GetStencilWriteMask() const;
	/** Set the stencil operation to perform when the stencil test fails. @author Hayden Asplet */
	void SetStencilFailOperation(EStencilOperation operation);
	/** Get the stencil operation to perform when the stencil test fails. @author Hayden Asplet */
	const EStencilOperation GetStencilFailOperation() const;
	/** Set the stencil operation to perform when the stencil passes, but the depth test fails. @author Hayden Asplet */
	void SetStencilDepthFailOperation(EStencilOperation operation);
	/** Get the stencil operation to perform when the stencil passes, but the depth test fails. @author Hayden Asplet */
	const EStencilOperation GetStencilDepthFailOperation() const;
	/** Set the stencil operation to perform when both the stencil and depth tests pass. @author Hayden Asplet */
	void SetStencilPassOperation(EStencilOperation operation);
	/** Get the stencil operation to perform when both the stencil and depth tests pass. @author Hayden Asplet */
	const EStencilOperation GetStencilPassOperation() const;

	// Rasterizer settings.

	/** Set the cull mode of the pass. @author Hayden Asplet */
	void SetCullingMode(ECullingMode cullingMode);
	/** Get the cull mode of the pass. @author Hayden Asplet */
	const ECullingMode GetCullingMode() const;
	/** Set the fill mode of the pass. @author Hayden Asplet */
	void SetFillMode(EFillMode fillMode);
	/** Get the fill mode of the pass. @author Hayden Asplet */
	const EFillMode GetFillMode() const;

	/** Set the render queue group category. @author Hayden Asplet */
	void SetRenderQueueGroupCategory(ERenderQueueGroupCategory renderQueueGroupCategory);
	/** Get the render queue group category. @author Hayden Asplet */
	const ERenderQueueGroupCategory GetRenderQueueGroupCategory() const;

protected:
private:

	// Member Variables
public:
protected:
private:
	// Texture layers.
	std::vector<std::unique_ptr<CTextureLayer>> m_TextureLayers;

	// Shader programs of the pass. 
	// May not all be valid if the pass does not require the programmable pipeline.
	shared_ptr<AShaderProgram> m_pVertexShader;
	shared_ptr<AShaderProgram> m_pPixelShader;
	shared_ptr<AShaderProgram> m_pGeometryShader;

	// Blending and colour writes for the output merger stage.
	TBlendingMode m_BlendingMode;
	TColourWritesEnabled m_ColourWrites;

	// Depth buffer settings
	bool m_DepthCheckEnabled;
	bool m_DepthWriteEnabled;
	EComparisonFunction m_DepthCompareFunction;

	// Stencil buffer settings
	TStencilBufferSettings m_StencilBufferSettings;

	// Rasterizer state descriptors.
	ECullingMode m_CullingMode;
	EFillMode m_FillMode;

	ERenderQueueGroupCategory m_RenderQueueGroupCategory;
};

}

#endif	// __SLIMRENDERPASS_H__