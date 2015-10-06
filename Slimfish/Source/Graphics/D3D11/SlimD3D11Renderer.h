// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11Renderer.h
// Description	: CSlimD3D11Renderer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D11RENDERER_H__
#define __SLIMD3D11RENDERER_H__

// Library Includes
//#include <D3D11.h>
//#include <D3DX11.h>

// Local Includes
#include "../SlimRenderer.h"
#include "SlimD3D11Forward.h"

namespace Slim {

/** Specialization of ARenderer for rendering in DirectX 10.
	@remarks
*/
class CD3D11Renderer :public ARenderer {
	// Member Functions
public:
	/** Default Constructor.
	 	@author Hayden Asplet
	*/
	CD3D11Renderer(int width, int height, bool isWindowed);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	virtual ~CD3D11Renderer();

	/* @copydoc ARenderer::VInitialize */
	virtual bool VInitialize() override;
	/* @copydoc ARenderer::VPreRender */
	virtual void VPreRender() override;
	/* @copydoc ARenderer::VPostRender */
	virtual void VPostRender() override;

	/* @copydoc ARenderer::VCreateVertexBuffer */
	virtual shared_ptr<AVertexGpuBuffer> VCreateVertexBuffer(size_t numVertices, size_t stride, const void* pSource, EGpuBufferUsage usage, bool isInSystemMemory) override;
	/* @copydoc ARenderer::VCreateVertexBuffer */
	virtual shared_ptr<AIndexGpuBuffer> VCreateIndexBuffer(size_t numIndices, AIndexGpuBuffer::EIndexType indexType, const void* pSource, EGpuBufferUsage usage, bool isInSystemMemory) override;

	/* @copydoc ARenderer::VCreateShaderProgram */
	virtual shared_ptr<AShaderProgram> VCreateShaderProgram(const std::string& name, EShaderProgramType type, const std::string& entry, const std::string& shaderModel);

	/* @copydoc ARenderer::VLoadTexture */
	virtual shared_ptr<ATexture> VLoadTexture(const std::string& name, ETextureType type, ETextureUsage usage) override;

	/* @copydoc ARenderer::VCreateRenderTexture */
	virtual std::unique_ptr<ARenderTexture> VCreateRenderTexture(const std::string& name, size_t width, size_t height, size_t msaaCount, size_t msaaQuality, ETextureType textureType /* = ETextureType::TYPE_2D */) override;

	/* @copydoc ARenderer::VRender */
	virtual void VRender(const CVertexDeclaration& vertexDeclaration,
						 EPrimitiveType primitiveType, 
						 shared_ptr<AVertexGpuBuffer> pVertexBuffer,
						 shared_ptr<AIndexGpuBuffer> pIndexBuffer = nullptr);

	/* @copydoc ARenderer::VSetShaderProgram */
	virtual void VSetShaderProgram(shared_ptr<AShaderProgram> pShader);
	/* @copydoc ARenderer::VDisableShaderProgram */
	virtual void VDisableShaderProgram(EShaderProgramType programType) override;

	/* @copydoc ARenderer::VSetRenderTarget */
	virtual void VSetRenderTarget(ARenderTexture* pRenderTarget) override;

	/* @copydoc ARenderer::VSetWorldTransform */
	virtual void VSetWorldTransform(const CMatrix4x4& worldTransform) override;
	/* @copydoc ARenderer::VSetViewTransform */
	virtual void VSetViewTransform(const CMatrix4x4& viewTransform) override;
	/* @copydoc ARenderer::VSetProjectionTransform */
	virtual void VSetProjectionTransform(const CMatrix4x4& projectionTransform) override;

	/* @copydoc ARenderer::VSetAmbientColour */
	virtual void VSetAmbientColour(const CColour& colour) override;
	/* @copydoc ARenderer::VSetBackgroundColour */
	virtual void VSetBackgroundColour(const CColourValue& colour) override;

	/* @copydoc ARenderer::VSetFog */
	virtual void VSetFog(EFogType fogType, const CColourValue& colour = CColourValue::s_BLACK, float start = 0.0f, float end = 1.0f, float exponentialDensity = 1.0f) override;
	/* @copydoc ARenderer::VSetBlending */
	virtual void VSetBlendingMode(const TBlendingMode& blendingMode) override;

	/* @copydoc ARenderer::VSetColourWritesEnabled */
	virtual void VSetColourWritesEnabled(const TColourWritesEnabled& colourWritesEnabled) override;
	/* @copydoc ARenderer::VSetStencilBufferSettings */
	virtual void VSetStencilBufferSettings(const TStencilBufferSettings& settings) override;
	/* @copydoc ARenderer::VSetDepthBufferSettings */
	virtual void VSetDepthBufferSettings(bool enabled, bool writeEnabled, EComparisonFunction compareFunction) override;
	/* @copydoc ARenderer::VSetCullingMode */
	virtual void VSetCullingMode(ECullingMode cullingMode) override;
	/* @copydoc ARenderer::VSetFillMode */
	virtual void VSetFillMode(EFillMode fillMode) override;

	virtual void VDrawText(const std::string text, const CPoint& position, const CColour& colour) override;
protected:
private:
	// Windows functions that should be in a windows class.
	virtual void VSetWindowed(bool windowed);
	virtual void VOnResize();

	/** Internal method for determining maximum multi-sample levels for the graphics device.
	 	@author Hayden Asplet
	*/
	DXGI_SAMPLE_DESC DetermineMultiSampleLevels();

	/* @copydoc ARenderer::VSetTextureLayerFiltering */
	virtual void VSetTextureLayerFiltering(size_t layer, ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter);
	/* @copydoc ARenderer::VSetTextureLayerFiltering */
	virtual void VSetTexture(size_t layer, const shared_ptr<ATexture>& pTexture, bool disable = false) override;
	/* @copydoc ARenderer::VSetTextureBlendState */
	virtual void VSetTextureLayerBlendState(size_t layer, const TTextureLayerBlendState& blendState) override;
	/* @copydoc ARenderer::VSetTextureAddressModes */
	virtual void VSetTextureAddressModes(size_t layer, const TTextureUVWAddressModes& addressModes) override;
	/* @copydoc ARenderer::VSetTextureBorderColour */
	virtual void VSetTextureBorderColour(size_t layer, const CColourValue& colour) override;

	/* @copydoc ARenderer::VSetVertexDeclaration */
	virtual void VSetVertexDeclaration(const CVertexDeclaration& vertexDeclaration);
	/* @copydoc ARenderer::VSetVertexBuffer */
	virtual void VSetVertexBuffer(shared_ptr<AVertexGpuBuffer> pVertexBuffer);
	/* @copydoc ARenderer::VSetIndexBuffer */
	virtual void VSetIndexBuffer(shared_ptr<AIndexGpuBuffer> pIndexBuffer);

	/** Internal helper method for creating a sampler state for a texture layer.
	 	@author Hayden Asplet
	*/
	void CreateSamplerState(size_t layer);

	/** Internal helper method for converting a vertex declaration to input element descriptions
		that directX can use.
	 	@author Hayden Asplet
	*/
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DVertexDeclaration(const CVertexDeclaration& vertexDeclaration);


	// Member Variables
public:
protected:
private:
	ComPtr<ID3D11Device> m_pD3DDevice;
	ComPtr<ID3D11DeviceContext> m_pImmediateContext;
	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	// Blending.
	D3D11_BLEND_DESC m_BlendDesc;
	ComPtr<ID3D11BlendState> m_pBlendState;

	// Rasterizer.
	D3D11_RASTERIZER_DESC m_RasterizerDesc;
	ComPtr<ID3D11RasterizerState> m_pRasterizerState;

	// Depth-stencil buffer.
	D3D11_DEPTH_STENCIL_DESC m_DepthStencilDesc;
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	size_t m_StencilReferenceValue;

	// Texture layering.
	std::vector<D3D11_SAMPLER_DESC> m_SamplerDescs;
	std::vector<ComPtr<ID3D11SamplerState> > m_SamplerStates;
	std::vector<ID3D11ShaderResourceView*> m_Textures;

	float m_pBackgroundColour[4];	// The clear colour.
	D3D11_VIEWPORT m_ViewPort;		// Current view port.
	
	DXGI_SAMPLE_DESC m_SampleDesc;	// The description of the multi-sampling levels currently being used.
	DXGI_SWAP_CHAIN_DESC m_d3dpp;	// Presentation parameters.

	// Bound shaders.
	shared_ptr<CD3D11ShaderProgram> m_pBoundVertexShader;
	shared_ptr<CD3D11ShaderProgram> m_pBoundPixelShader;
	shared_ptr<CD3D11ShaderProgram> m_pBoundGeometryShader;

	CD3D11RenderTexture* m_pBoundRenderTarget;
};

}

#endif	// __SLIMD3D11RENDERER_H__