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
	CD3D10Renderer(int width, int height, bool isWindowed);
	virtual ~CD3D10Renderer();

	/* @copydoc ARenderer::VInitialize */
	virtual bool VInitialize() override;
	/* @copydoc ARenderer::VPreRender */
	virtual void VPreRender() override;
	/* @copydoc ARenderer::VPostRender */
	virtual void VPostRender() override;

	/* @copydoc ARenderer::VCreateVertexBuffer */
	virtual shared_ptr<AVertexGpuBuffer> VCreateVertexBuffer(size_t numVertices, size_t stride, void* pSource, AGpuBuffer::EUsage usage, bool isInSystemMemory) override;
	/* @copydoc ARenderer::VCreateVertexBuffer */
	virtual shared_ptr<AIndexGpuBuffer> VCreateIndexBuffer(size_t numIndices, AIndexGpuBuffer::EIndexType indexType, void* pSource, AGpuBuffer::EUsage usage, bool isInSystemMemory) override;

	virtual shared_ptr<ATexture> VLoadTexture(const std::string& name) override;

	virtual void VRender(const CVertexDeclaration& vertexDeclaration, 
						 shared_ptr<AVertexGpuBuffer> pVertexBuffer,
						 shared_ptr<AIndexGpuBuffer> pIndexBuffer /* = nullptr */);

	virtual void VSetShaderProgram(shared_ptr<AShaderProgram> pShader);

	virtual void VSetWorldTransform(const CMatrix4x4& worldTransform) override;
	virtual void VSetViewTransform(const CMatrix4x4& viewTransform) override;
	virtual void VSetProjectionTransform(const CMatrix4x4& projectionTransform) override;

	virtual void VSetAmbientColour(const TColour& colour) override;
	virtual void VSetBackgroundColour(const TColour& colour) override;

	virtual void VSetFog(EFogType fogType, const TColourValue& colour = TColourValue::s_BLACK, float start = 0.0f, float end = 1.0f, float exponentialDensity = 1.0f) override;
protected:
private:
	DXGI_SAMPLE_DESC CheckMultiSampleLevels();

	virtual void VSetTexture(size_t layer, const shared_ptr<ATexture>& pTexture, bool disable = false) override;
	virtual void VSetTextureLayerFiltering(size_t layer, ETextureSamplerType samplerType, ETextureFilterType filterType) override;
	virtual void VSetTextureLayerBlendState(size_t layer, const TTextureLayerBlendState& blendState) override;
	virtual void VSetTextureAddressModes(size_t layer, const TTextureUVWAddressModes& addressModes) override;
	virtual void VSetTextureBorderColour(size_t layer, const TColourValue& colour) override;

	virtual void VSetVertexDeclaration(const CVertexDeclaration& vertexDeclaration);
	virtual void VSetVertexBuffer(shared_ptr<AVertexGpuBuffer> pVertexBuffer);
	virtual void VSetIndexBuffer(shared_ptr<AIndexGpuBuffer> pIndexBuffer);

	std::vector<D3D10_INPUT_ELEMENT_DESC> GetD3DVertexDeclaration(const CVertexDeclaration& vertexDeclaration);

	// Member Variables
public:
protected:
private:
	ID3D10Device* m_pD3DDevice;
	IDXGISwapChain* m_pSwapChain;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10Texture2D* m_pDepthStencilBuffer;
	ID3D10DepthStencilState* m_pDepthStencilState;
	ID3D10DepthStencilView* m_pDepthStencilView;
	ID3D10RasterizerState* m_pRasterizerState;
	std::vector<ID3D10SamplerState*> m_SamplerStates;
	std::vector<ID3D10ShaderResourceView*> m_Textures;

	D3DXCOLOR m_backgroundColour;
	D3DXCOLOR m_ambientColour;
	D3D10_VIEWPORT m_viewPort;
	
	DXGI_SAMPLE_DESC m_sampleDesc;
	DXGI_SWAP_CHAIN_DESC m_d3dpp;	// Presentation parameters.

	shared_ptr<CD3D10ShaderProgram> m_pBoundVertexShader;
	shared_ptr<CD3D10ShaderProgram> m_pBoundPixelShader;
	shared_ptr<CD3D10ShaderProgram> m_pBoundGeometryShader;

	CVertexDeclaration::EPrimitiveType m_PrimitiveType;
};

}

#endif	// __SLIMD3D10RENDERER_H__