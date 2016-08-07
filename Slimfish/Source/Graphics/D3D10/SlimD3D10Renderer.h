// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10Renderer.h
// Description	: CSlimD3D10Renderer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10RENDERER_H__
#define __SLIMD3D10RENDERER_H__

// Library Includes
#include <d3d10.h>
#include <d3dx10.h>

// Local Includes
#include "../SlimRenderer.h"
#include "SlimD3D10Forward.h"

namespace Slim {

/** Specialization of ARenderer for rendering in DirectX 10.
	@remarks
*/
class CD3D10Renderer :public ARenderer {
	// Member Functions
public:
	/** Default Constructor.
	 	@author Hayden Asplet
	*/
	CD3D10Renderer(int width, int height, bool isWindowed);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	virtual ~CD3D10Renderer();

	/** @copydoc ARenderer::VInitialize */
	virtual bool VInitialize() override;
	/** @copydoc ARenderer::VPreRender */
	virtual void VPreRender() override;
	/** @copydoc ARenderer::VPostRender */
	virtual void VPostRender() override;

	/** @copydoc ARenderer::VCreateVertexBuffer */
	virtual shared_ptr<AVertexGpuBuffer> VCreateVertexBuffer(size_t numVertices, size_t stride, const void* pSource, EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory) override;
	/** @copydoc ARenderer::VCreateVertexBuffer */
	virtual shared_ptr<AIndexGpuBuffer> VCreateIndexBuffer(size_t numIndices, AIndexGpuBuffer::EIndexType indexType, const void* pSource, EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory) override;

	/** @copydoc ARenderer::VCreateShaderProgram */
	virtual shared_ptr<AShaderProgram> VCreateShaderProgram(const std::string& name, EShaderProgramType type, const std::string& entry, const std::string& shaderModel);

	/** @copydoc ARenderer::VLoadTexture */
	virtual shared_ptr<ATexture> VLoadTexture(const std::string& name, ETextureType type, ETextureUsage usage) override;

	/** @copydoc ARenderer::VCreateTexture */
	virtual shared_ptr<ATexture> VCreateTexture(const std::string& name) override;

	/** @copydoc ARenderer::VCreateRenderTexture */
	virtual std::unique_ptr<ARenderTexture> VCreateRenderTexture(const std::string& name, size_t width, size_t height, size_t depth, ETextureType textureType, size_t msaaCount, size_t msaaQuality) override;

	/** @copydoc ARenderer::VCreateRenderTexture */
	virtual std::unique_ptr<ARenderTexture> VCreateRenderTexture(std::shared_ptr<ATexture> pTexture);

	/** @copydoc ARenderer::VRender */
	virtual void VRender(const CVertexDeclaration& vertexDeclaration, EPrimitiveType primitiveType, shared_ptr<AVertexGpuBuffer> pVertexBuffer, shared_ptr<AIndexGpuBuffer> pIndexBuffer = nullptr, size_t countOverride = 0, size_t instances = 1);

	/** @copydoc ARenderer::VSetShaderProgram */
	virtual void VSetShaderProgram(shared_ptr<AShaderProgram> pShader);
	/** @copydoc ARenderer::VDisableShaderProgram */
	virtual void VDisableShaderProgram(EShaderProgramType programType) override;

	/** @copydoc ARenderer::VSetRenderTarget */
	virtual void VSetRenderTargets(std::vector<ARenderTexture*> renderTargets) override;

	/** @copydoc ARenderer::VSetStreamOutTargets */
	virtual void VSetStreamOutTargets(const std::vector<std::shared_ptr<AGpuBuffer> >& buffers) override;

	/** @copydoc ARenderer::VSetStreamOutTarget */
	virtual void VSetStreamOutTarget(const std::shared_ptr<AGpuBuffer>& pBuffer) override;

	/** @copydoc ARenderer::VBeginStreamOutQuery */
	virtual void VBeginStreamOutQuery() override;

	/** @copydoc ARenderer::VEndStreamOutQuery */
	virtual size_t VEndStreamOutQuery() override;

	/** @copydoc ARenderer::VSetWorldTransform */
	virtual void VSetWorldTransform(const CMatrix4x4& worldTransform) override;
	/** @copydoc ARenderer::VSetViewTransform */
	virtual void VSetViewTransform(const CMatrix4x4& viewTransform) override;
	/** @copydoc ARenderer::VSetProjectionTransform */
	virtual void VSetProjectionTransform(const CMatrix4x4& projectionTransform) override;

	/** @copydoc ARenderer::VSetAmbientColour */
	virtual void VSetAmbientColour(const CColour& colour) override;
	/** @copydoc ARenderer::VSetBackgroundColour */
	virtual void VSetBackgroundColour(const CColourValue& colour) override;

	/** @copydoc ARenderer::VSetFog */
	virtual void VSetFog(EFogType fogType, const CColourValue& colour = CColourValue::s_BLACK, float start = 0.0f, float end = 1.0f, float exponentialDensity = 1.0f) override;
	/** @copydoc ARenderer::VSetBlending */
	virtual void VSetBlendingMode(const TBlendingMode& blendingMode) override;

	/** @copydoc ARenderer::VSetColourWritesEnabled */
	virtual void VSetColourWritesEnabled(const TColourWritesEnabled& colourWritesEnabled) override;
	/** @copydoc ARenderer::VSetStencilBufferSettings */
	virtual void VSetStencilBufferSettings(const TStencilBufferSettings& settings) override;
	/** @copydoc ARenderer::VSetDepthBufferSettings */
	virtual void VSetDepthBufferSettings(bool enabled, bool writeEnabled, EComparisonFunction compareFunction) override;
	/** @copydoc ARenderer::VSetCullingMode */
	virtual void VSetCullingMode(ECullingMode cullingMode) override;
	/** @copydoc ARenderer::VSetFillMode */
	virtual void VSetFillMode(EFillMode fillMode) override;

	virtual void VDrawText(const std::string text, const CPoint<>& position, const CColour& colour) override;
protected:
private:
	// Windows functions that should be in a windows class.
	virtual void VSetWindowed(bool windowed);
	virtual void VOnResize();

	/** Internal method for determining maximum multi-sample levels for the graphics device.
	 	@author Hayden Asplet
	*/
	DXGI_SAMPLE_DESC DetermineMultiSampleLevels();

	/** @copydoc ARenderer::VSetTextureLayerFiltering */
	virtual void VSetTextureLayerFiltering(size_t layer, ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter);
	/** @copydoc ARenderer::VSetTextureLayerFiltering */
	virtual void VSetTexture(size_t layer, const shared_ptr<ATexture>& pTexture, bool disable = false) override;
	/** @copydoc ARenderer::VSetTextureBlendState */
	virtual void VSetTextureLayerBlendState(size_t layer, const TTextureLayerBlendState& blendState) override;
	/** @copydoc ARenderer::VSetTextureAddressModes */
	virtual void VSetTextureAddressModes(size_t layer, const TTextureUVWAddressModes& addressModes) override;
	/** @copydoc ARenderer::VSetTextureBorderColour */
	virtual void VSetTextureBorderColour(size_t layer, const CColourValue& colour) override;

	/** @copydoc ARenderer::VSetVertexDeclaration */
	virtual void VSetVertexDeclaration(const CVertexDeclaration& vertexDeclaration) override;
	/** @copydoc ARenderer::VSetVertexBuffer */
	virtual void VSetVertexBuffer(const shared_ptr<AVertexGpuBuffer>& pVertexBuffer) override;
	/** @copydoc ARenderer::VSetIndexBuffer */
	virtual void VSetIndexBuffer(const shared_ptr<AIndexGpuBuffer>& pIndexBuffer) override;

	virtual void VBuildStates() override;

	/** Internal helper method for creating a sampler state for a texture layer.
	 	@author Hayden Asplet
	*/
	void CreateSamplerState(size_t layer);

	/** Internal helper method for converting a vertex declaration to input element descriptions
		that directX can use.
	 	@author Hayden Asplet
	*/
	std::vector<D3D10_INPUT_ELEMENT_DESC> GetD3DVertexDeclaration(const CVertexDeclaration& vertexDeclaration);

	// Member Variables
public:
protected:
private:
	ComPtr<ID3D10Device> m_pD3DDevice;
	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D10Query> m_pQuery;
	ComPtr<ID3D10RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D10Texture2D> m_pDepthStencilBuffer;
	ComPtr<ID3D10DepthStencilView> m_pDepthStencilView;

	// Blending.
	D3D10_BLEND_DESC m_BlendDesc;
	ComPtr<ID3D10BlendState> m_pBlendState;

	// Rasterizer.
	D3D10_RASTERIZER_DESC m_RasterizerDesc;
	ComPtr<ID3D10RasterizerState> m_pRasterizerState;

	// Depth-stencil buffer.
	D3D10_DEPTH_STENCIL_DESC m_DepthStencilDesc;
	ComPtr<ID3D10DepthStencilState> m_pDepthStencilState;
	size_t m_StencilReferenceValue;

	// Texture layering.
	std::vector<D3D10_SAMPLER_DESC> m_SamplerDescs;
	std::vector<ComPtr<ID3D10SamplerState>> m_SamplerStates;
	std::vector<ID3D10ShaderResourceView*> m_Textures;

	D3DXCOLOR m_BackgroundColour;	// The clear colour.
	D3D10_VIEWPORT m_ViewPort;		// Current view port.
	
	DXGI_SAMPLE_DESC m_SampleDesc;	// The description of the multi-sampling levels currently being used.
	DXGI_SWAP_CHAIN_DESC m_d3dpp;	// Presentation parameters.

	// Bound shaders.
	shared_ptr<CD3D10ShaderProgram> m_pBoundVertexShader;
	shared_ptr<CD3D10ShaderProgram> m_pBoundPixelShader;
	shared_ptr<CD3D10ShaderProgram> m_pBoundGeometryShader;

	std::vector<CD3D10RenderTexture*> m_BoundRenderTargets;
};

}

#endif	// __SLIMD3D10RENDERER_H__