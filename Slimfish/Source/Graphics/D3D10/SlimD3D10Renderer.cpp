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

	CD3D10Renderer::CD3D10Renderer(int width, int height, bool isWindowed)
		:ARenderer(width, height, isWindowed),
		m_pD3DDevice(nullptr),
		m_pSwapChain(nullptr),
		m_pRenderTargetView(nullptr),
		m_pDepthStencilState(nullptr),
		m_pRasterizerState(nullptr)
	{
		memset(m_ppSamplerStates, 0, sizeof(m_ppSamplerStates));
		memset(m_ppTextures, 0, sizeof(m_ppTextures));
	}

	bool CD3D10Renderer::VInitialize()
	{
		UINT deviceFlags = 0;

#ifdef _DEBUG
		deviceFlags = D3D10_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

		// Create the device.
		HRESULT hResult = D3D10CreateDevice(0,	// Default adapter.
											D3D10_DRIVER_TYPE_HARDWARE,	// Driver type.
											0,							// Software device.
											deviceFlags,				// Device flags.
											D3D10_SDK_VERSION, 
											&m_pD3DDevice);
		if (FAILED(hResult)) {
			throw CRenderingError();
			return false;
		}

		RECT rect;
		GetClientRect(g_pApp->GetHwnd(), &rect);
		m_width = rect.right;
		m_height = rect.bottom;

		ZeroMemory(&m_d3dpp, sizeof(DXGI_SWAP_CHAIN_DESC));
		m_d3dpp.BufferCount = 1;
		m_d3dpp.BufferDesc.Width = m_width;
		m_d3dpp.BufferDesc.Height = m_height;
		m_d3dpp.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_d3dpp.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_d3dpp.Windowed = m_isWindowed;
		m_d3dpp.OutputWindow = g_pApp->GetHwnd();
		m_d3dpp.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

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

