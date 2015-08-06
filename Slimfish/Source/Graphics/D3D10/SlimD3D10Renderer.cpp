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

	CD3D10Renderer::~CD3D10Renderer()
	{
		SafeRelease(m_pRenderTargetView);
		SafeRelease(m_pDepthStencilView);
		SafeRelease(m_pSwapChain);
		SafeRelease(m_pDepthStencilBuffer);
		SafeRelease(m_pD3DDevice);
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

		// Check MSAA count and quality.
		CheckMultiSampleLevels();

		// Get the width and height of the client rectangle.
		RECT rect;
		GetClientRect(g_pApp->GetHwnd(), &rect);
		m_width = rect.right;
		m_height = rect.bottom;

		// Fill out the swap chain description.
		ZeroMemory(&m_d3dpp, sizeof(DXGI_SWAP_CHAIN_DESC));
		m_d3dpp.BufferCount = 1;
		m_d3dpp.BufferDesc.Width = m_width;
		m_d3dpp.BufferDesc.Height = m_height;
		m_d3dpp.BufferDesc.RefreshRate.Numerator = 60;
		m_d3dpp.BufferDesc.RefreshRate.Denominator = 1;
		m_d3dpp.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		m_d3dpp.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		m_d3dpp.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_d3dpp.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_d3dpp.Windowed = m_isWindowed;
		m_d3dpp.OutputWindow = g_pApp->GetHwnd();
		m_d3dpp.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		m_d3dpp.SampleDesc = m_sampleDesc;

		// Create the swap chain by obtaining the DXGIFactory that was used to create the device.
		IDXGIDevice* dxgiDevice = nullptr;
		if (FAILED(m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)))) {
			throw CRenderingError();
			return false;
		}

		IDXGIAdapter* dxgiAdapter = nullptr;
		if (FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter)))) {
			throw CRenderingError();
			return false;
		}

		// Get the factory finally.
		IDXGIFactory* dxgiFactory = nullptr;
		if (FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory)))) {
			throw CRenderingError();
			return false;
		}

		// Create the swap chain.
		if (FAILED(dxgiFactory->CreateSwapChain(m_pD3DDevice, &m_d3dpp, &m_pSwapChain))) {
			throw CRenderingError();
			return false;
		}

		SafeRelease(dxgiFactory);
		SafeRelease(dxgiAdapter);
		SafeRelease(dxgiDevice);

		// Create the render target view to the back buffer.
		ID3D10Texture2D* pBackBuffer;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&pBackBuffer));
		m_pD3DDevice->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView);

		SafeRelease(pBackBuffer);

		// Create the depth/stencil buffer and view.
		D3D10_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D10_TEXTURE2D_DESC));
		depthStencilDesc.SampleDesc = m_sampleDesc;
		depthStencilDesc.Width = m_width;
		depthStencilDesc.Height = m_height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Usage = D3D10_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;

		ID3D10Texture2D* pDepthStencilBuffer;
		hResult = m_pD3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencilBuffer);
		if (FAILED(hResult)) {
			throw CRenderingError();
			return false;
		}

		hResult = m_pD3DDevice->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
		if (FAILED(hResult)) {
			throw CRenderingError();
			return false;
		}

		// Bind the views to the output merger stage.
		m_pD3DDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		// Set the view port.
		ZeroMemory(&m_viewPort, sizeof(D3D10_VIEWPORT));
		m_viewPort.TopLeftX = 0;
		m_viewPort.TopLeftY = 0;
		m_viewPort.Width = static_cast<float>(m_width);
		m_viewPort.Height = static_cast<float>(m_height);
		m_viewPort.MinDepth = 0.0f;
		m_viewPort.MaxDepth = 1.0f;
	}

	void CD3D10Renderer::VPreRender()
	{
		m_pD3DDevice->ClearRenderTargetView(m_pRenderTargetView, m_backgroundColour);
		m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
		m_pD3DDevice->OMSetDepthStencilState(0, 0);

		m_pD3DDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		m_pD3DDevice->RSSetViewports(1, &m_viewPort);

	}

	void CD3D10Renderer::VPostRender()
	{
		m_pSwapChain->Present(0, 0);
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

	DXGI_SAMPLE_DESC CD3D10Renderer::CheckMultiSampleLevels()
	{
		unsigned int msaaCount = 4;
		unsigned int msaaQuality = -1;

		for (int samples = 4; samples <= D3D10_MAX_MULTISAMPLE_SAMPLE_COUNT; samples *= 2) {
			UINT currentMSAAQuality = -1;
			HRESULT hResult = m_pD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, samples, &currentMSAAQuality);

			if (FAILED(hResult)) {
				throw CRenderingError();
				return;
			}

			if (currentMSAAQuality == 0) {
				break;
			}
			else {
				msaaCount = samples;
				msaaQuality = currentMSAAQuality;
			}
		}

		ZeroMemory(&m_sampleDesc, sizeof(DXGI_SAMPLE_DESC));
		m_sampleDesc.Count = msaaCount;
		m_sampleDesc.Quality = msaaQuality;

		// DirectX 10 should always support at least 4 levels of multi-sampling.
		assert(msaaQuality != -1);

		return m_sampleDesc;
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

