// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10Renderer.cpp
// Description	: CSlimD3D10Renderer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D10Renderer.h"

// Local Includes

namespace Slim {

	bool CD3D10Renderer::VInitialize()
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.Windowed

	}

	void CD3D10Renderer::VPreRender()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VPostRender()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	shared_ptr<AVertexGPUBuffer> CD3D10Renderer::VCreateVertexBuffer(size_t stride, size_t numVerts, AGPUBuffer::EUsage usage)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	shared_ptr<AIndexGPUBuffer> CD3D10Renderer::VCreateIndexBuffer(size_t stride, size_t numIndices, AGPUBuffer::EUsage usage)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	shared_ptr<ATexture> CD3D10Renderer::VLoadTexture(const string& name, ATexture::EType textureType, ATexture::EUsage usage)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetWorldTransform(const CMatrix4x4& worldTransform)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetViewTransform(const CMatrix4x4& viewTransform)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetProjectionTransform(const CMatrix4x4& projectionTransform)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetAmbientColour(const TColour& colour)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetBackgroundColour(const TColour& colour)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetFog(EFogType fogType, const TColourValue& colour /*= TColourValue::s_BLACK*/, float start /*= 0.0f*/, float end /*= 1.0f*/, float exponentialDensity /*= 1.0f*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetTexture(size_t layer, const shared_ptr<ATexture>& pTexture, bool disable /*= false*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetTextureLayerFiltering(size_t layer, ETextureSamplerType samplerType, ETextureFilterType filterType)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetTextureLayerBlendState(size_t layer, const TTextureLayerBlendState& blendState)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetTextureAddressModes(size_t layer, const TTextureUVWAddressModes& addressModes)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CD3D10Renderer::VSetTextureBorderColour(size_t layer, const TColourValue& colour)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}

