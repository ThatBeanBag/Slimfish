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
#include <functional>
#include <D3D11SDKLayers.h>

// This Include
#include "SlimD3D10Renderer.h"

// Local Includes
#include "SlimD3D10VertexGpuBuffer.h"
#include "SlimD3D10IndexGpuBuffer.h"
#include "SlimD3D10ShaderProgram.h"
#include "SlimD3D10Texture.h"
#include "SlimD3D10RenderTexture.h"
#include "SlimD3D10GpuBuffer.h"

namespace Slim {

CD3D10Renderer::CD3D10Renderer(int width, int height, bool isWindowed)
	:ARenderer(width, height, isWindowed),
	m_pD3DDevice(nullptr),
	m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr),
	m_pDepthStencilState(nullptr),
	m_pRasterizerState(nullptr),
	m_SamplerStates(g_MAX_TEXTURE_LAYERS, nullptr),
	m_Textures(g_MAX_TEXTURE_LAYERS, nullptr),
	m_SamplerDescs(g_MAX_TEXTURE_LAYERS)
{
	ZeroMemory(&m_SamplerDescs[0], m_SamplerDescs.size() * sizeof(D3D10_SAMPLER_DESC));
}

CD3D10Renderer::~CD3D10Renderer()
{
	// We don't own the bound shaders, just invalidate our reference to them.
	m_pBoundGeometryShader = nullptr;
	m_pBoundPixelShader = nullptr;
	m_pBoundVertexShader = nullptr;

#ifdef _DEBUG
	// Report live objects.
	ComPtr<ID3D11Debug> pDebug;
	m_pD3DDevice.As(&pDebug);
	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif // _DEBUG

	//SLIM_SAFE_RELEASE(m_pD3DDevice);
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
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create DirectX 10 device with error:" << GetErrorMessage(hResult);
		return false;
	}

	// Check MSAA count and quality.
	DetermineMultiSampleLevels();

	// Get the width and height of the client rectangle.
	RECT rect;
	GetClientRect(g_pApp->GetHWND(), &rect);
	m_Width = rect.right;
	m_Height = rect.bottom;

	// Fill out the swap chain description.
	ZeroMemory(&m_d3dpp, sizeof(DXGI_SWAP_CHAIN_DESC));
	m_d3dpp.BufferCount = 1;
	m_d3dpp.BufferDesc.Width = m_Width;
	m_d3dpp.BufferDesc.Height = m_Height;
	m_d3dpp.BufferDesc.RefreshRate.Numerator = 60;
	m_d3dpp.BufferDesc.RefreshRate.Denominator = 1;
	m_d3dpp.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_d3dpp.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	m_d3dpp.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_d3dpp.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_d3dpp.Windowed = m_IsWindowed;
	m_d3dpp.OutputWindow = g_pApp->GetHWND();
	m_d3dpp.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	m_d3dpp.SampleDesc = m_SampleDesc;

	// Create the swap chain by obtaining the DXGIFactory that was used to create the device.
	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	ComPtr<IDXGIFactory> dxgiFactory = nullptr;

	hResult = m_pD3DDevice.As(&dxgiDevice);
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to query device interface with error: " << GetErrorMessage(hResult);
		return false;
	}

	hResult = dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to get the DXGI adapter with error: " << GetErrorMessage(hResult);
		return false;
	}

	// Get the factory finally.
	hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to get the DXGI factory with error: " << GetErrorMessage(hResult);
		return false;
	}

	// Create the swap chain.
	hResult = dxgiFactory->CreateSwapChain(m_pD3DDevice.Get(), &m_d3dpp, &m_pSwapChain);
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create swap chain with error: " << GetErrorMessage(hResult);
		return false;
	}

	// Create the render target view to the back buffer.
	ComPtr<ID3D10Texture2D> pBackBuffer;
	hResult = m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf()));
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to get back buffer with error: " << GetErrorMessage(hResult);
	}

	hResult = m_pD3DDevice->CreateRenderTargetView(pBackBuffer.Get(), 0, m_pRenderTargetView.GetAddressOf());
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create render target view with error: " << GetErrorMessage(hResult);
	}

	// Create the depth/stencil buffer and view.
	D3D10_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D10_TEXTURE2D_DESC));
	depthStencilDesc.SampleDesc = m_SampleDesc;
	depthStencilDesc.Width = m_Width;
	depthStencilDesc.Height = m_Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;

	hResult = m_pD3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf());
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil buffer with error: " << GetErrorMessage(hResult);
		return false;
	}

	hResult = m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf());
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil view with error: "  << GetErrorMessage(hResult);
		return false;
	}

	// Bind the views to the output merger stage.
	m_pD3DDevice->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	// Set the view port.
	ZeroMemory(&m_ViewPort, sizeof(D3D10_VIEWPORT));
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = m_Width;
	m_ViewPort.Height = m_Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	// Create the rasterizer state.
	ZeroMemory(&m_RasterizerDesc, sizeof(D3D10_RASTERIZER_DESC));

	m_RasterizerDesc.CullMode = D3D10_CULL_FRONT;	// Clockwise culling for right handed matrices.
	m_RasterizerDesc.FillMode = D3D10_FILL_SOLID;
	m_RasterizerDesc.DepthClipEnable = true;
		
	m_pD3DDevice->CreateRasterizerState(&m_RasterizerDesc, &m_pRasterizerState);
	m_pD3DDevice->RSSetState(m_pRasterizerState.Get());

	return true;
}

void CD3D10Renderer::VPreRender()
{
	if (!m_BoundRenderTargets.empty()) {
		for (auto& renderTarget : m_BoundRenderTargets) {
			ID3D10DepthStencilView* pDepthStencilView = renderTarget->GetDepthStencilView();
			if (!pDepthStencilView) {
				pDepthStencilView = m_pDepthStencilView.Get();
			}

			m_pD3DDevice->ClearRenderTargetView(renderTarget->GetRenderTargetView(), m_BackgroundColour);
			m_pD3DDevice->ClearDepthStencilView(pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
		}
	}
	else {
		m_pD3DDevice->ClearRenderTargetView(m_pRenderTargetView.Get(), m_BackgroundColour);
		m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
	}

	m_pD3DDevice->OMSetDepthStencilState(0, 0);
	m_pD3DDevice->RSSetViewports(1, &m_ViewPort);
}

void CD3D10Renderer::VPostRender()
{
	m_pSwapChain->Present(0, 0);
}

shared_ptr<AVertexGpuBuffer> CD3D10Renderer::VCreateVertexBuffer(size_t numVertices, size_t stride, const void* pSource, EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory)
{
	return std::make_shared<CD3D10VertexGpuBuffer>(
		m_pD3DDevice.Get(), 
		numVertices, 
		stride, 
		pSource, 
		usage, 
		isOutput,
		isInSystemMemory);
}

shared_ptr<AIndexGpuBuffer> CD3D10Renderer::VCreateIndexBuffer(size_t numIndices, AIndexGpuBuffer::EIndexType indexType, const void* pSource, EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory)
{
	return std::make_shared<CD3D10IndexGpuBuffer>(m_pD3DDevice.Get(), 
		numIndices, 
		indexType, 
		pSource, 
		usage,
		isOutput, 
		isInSystemMemory);
}

shared_ptr<AShaderProgram> CD3D10Renderer::VCreateShaderProgram(const std::string& name, EShaderProgramType type, const std::string& entry, const std::string& shaderModel)
{
	shared_ptr<AShaderProgram> pShaderProgram(new CD3D10ShaderProgram(m_pD3DDevice.Get(), name, type));
	pShaderProgram->SetEntryPoint(entry);
	pShaderProgram->SetShaderModel(shaderModel);

	if (pShaderProgram->VLoad()) {
		return pShaderProgram;
	}
	else {
		return nullptr;
	}
}

shared_ptr<ATexture> CD3D10Renderer::VLoadTexture(const std::string& name, ETextureType type, ETextureUsage usage)
{
	auto pTexture = std::make_shared<CD3D10Texture>(m_pD3DDevice.Get(), name, type, usage);

	if (pTexture) {
		pTexture->VLoad();

		return pTexture;
	}
	else {
		return nullptr;
	}
}

shared_ptr<ATexture> CD3D10Renderer::VCreateTexture(const std::string& name)
{
	return std::make_shared<CD3D10Texture>(m_pD3DDevice.Get(), name);
}

std::unique_ptr<ARenderTexture> CD3D10Renderer::VCreateRenderTexture(const std::string& name, size_t width, size_t height, size_t depth, ETextureType textureType, size_t msaaCount, size_t msaaQuality)
{
	// Create the texture as a render target.
	auto pTexture = std::make_shared<CD3D10Texture>(m_pD3DDevice.Get(), name, textureType, ETextureUsage::RENDER_TARGET);

	pTexture->SetWidth(width);
	pTexture->SetHeight(height);
	pTexture->SetDepth(depth);
	pTexture->SetMultiSampleCount(msaaCount);
	pTexture->SetMultiSampleQuality(msaaQuality);

	// Create the render target.
	pTexture->VLoad();

	return VCreateRenderTexture(pTexture);
}

std::unique_ptr<ARenderTexture> CD3D10Renderer::VCreateRenderTexture(std::shared_ptr<ATexture> pTexture)
{
	return std::make_unique<CD3D10RenderTexture>(m_pD3DDevice.Get(), pTexture);
}

void CD3D10Renderer::VSetWorldTransform(const CMatrix4x4& worldTransform)
{
		
}

void CD3D10Renderer::VSetViewTransform(const CMatrix4x4& viewTransform)
{
		
}

void CD3D10Renderer::VSetProjectionTransform(const CMatrix4x4& projectionTransform)
{
		
}

void CD3D10Renderer::VSetAmbientColour(const CColour& colour)
{
		
}

void CD3D10Renderer::VSetBackgroundColour(const CColourValue& colour)
{
	m_BackgroundColour = D3D10Conversions::GetColour(colour);
}

void CD3D10Renderer::VSetFog(EFogType fogType, const CColourValue& colour /*= TColourValue::s_BLACK*/, float start /*= 0.0f*/, float end /*= 1.0f*/, float exponentialDensity /*= 1.0f*/)
{
		
}

void CD3D10Renderer::VSetVertexDeclaration(const CVertexDeclaration& vertexDeclaration)
{
	assert(m_pBoundVertexShader);

	ID3D10InputLayout* pLayout = m_pBoundVertexShader->GetD3DInputLayout(&vertexDeclaration);
	assert(pLayout);

	m_pD3DDevice->IASetInputLayout(pLayout);
}

void CD3D10Renderer::VSetShaderProgram(shared_ptr<AShaderProgram> pShader)
{
	assert(pShader);

	switch (pShader->GetType()) {
		case EShaderProgramType::VERTEX: {
			m_pBoundVertexShader = static_pointer_cast<CD3D10ShaderProgram>(pShader);

			ID3D10VertexShader* pVertexShader = m_pBoundVertexShader->GetD3DVertexShader();
			assert(pVertexShader);

			// Set the shader.
			m_pD3DDevice->VSSetShader(pVertexShader);

			// Set constant buffers.
			const CD3D10ShaderProgram::TConstantBufferList& buffers = m_pBoundVertexShader->GetD3DConstantBuffers();
			if (!buffers.empty()) {
				m_pD3DDevice->VSSetConstantBuffers(0, buffers.size(), buffers[0].GetAddressOf());
			}
			else {
				m_pD3DDevice->VSSetConstantBuffers(0, 0, NULL);
			}

			break;
		}
		case EShaderProgramType::PIXEL: {
			m_pBoundPixelShader = static_pointer_cast<CD3D10ShaderProgram>(pShader);

			ID3D10PixelShader* pPixelShader = m_pBoundPixelShader->GetD3DPixelShader();
			assert(pPixelShader);

			// Set the shader.
			m_pD3DDevice->PSSetShader(pPixelShader);

			// Set the constant buffers.
			const CD3D10ShaderProgram::TConstantBufferList& buffers = m_pBoundPixelShader->GetD3DConstantBuffers();

			if (!buffers.empty()) {
				m_pD3DDevice->PSSetConstantBuffers(0, buffers.size(), buffers[0].GetAddressOf());
			}
			else {
				m_pD3DDevice->PSSetConstantBuffers(0, 0, NULL);
			}

			break;
		}
		case EShaderProgramType::GEOMETRY: {
			m_pBoundGeometryShader = static_pointer_cast<CD3D10ShaderProgram>(pShader);

			ID3D10GeometryShader* pGeometryShader = m_pBoundGeometryShader->GetD3DGeometryShader();
			assert(pGeometryShader);

			// Set the shader.
			m_pD3DDevice->GSSetShader(pGeometryShader);

			// Set the constant buffers.
			const CD3D10ShaderProgram::TConstantBufferList& buffers = m_pBoundGeometryShader->GetD3DConstantBuffers();
			if (!buffers.empty()) {
				m_pD3DDevice->GSSetConstantBuffers(0, buffers.size(), buffers[0].GetAddressOf());
			}
			else {
				m_pD3DDevice->GSSetConstantBuffers(0, 0, NULL);
			}

			break;
		}
		default: {
			break;
		}
	}
}

void CD3D10Renderer::VDisableShaderProgram(EShaderProgramType programType)
{
	switch (programType) {
		case EShaderProgramType::VERTEX: {
			m_pBoundVertexShader = nullptr;
			break;
		}
		case EShaderProgramType::PIXEL: {
			m_pBoundPixelShader = nullptr;
			break;
		}
		case EShaderProgramType::GEOMETRY: {
			m_pBoundGeometryShader = nullptr;
			break;
		}
		default: {
			break;
		}
	}
}

void CD3D10Renderer::VSetRenderTargets(std::vector<ARenderTexture*> renderTargets)
{
	if (!renderTargets.empty()) {
		// Clear existing bound render targets and make room for new targets.
		m_BoundRenderTargets.clear();
		m_BoundRenderTargets.resize(renderTargets.size());

		// Cast the render targets to their actual type and store in the bound targets.
		auto castToDerived = [](ARenderTexture* pRenderTarget) { return static_cast<CD3D10RenderTexture*>(pRenderTarget); };
		std::transform(renderTargets.begin(), renderTargets.end(), 
			m_BoundRenderTargets.begin(), castToDerived);

		std::vector<ID3D10RenderTargetView*> renderTargetViews(renderTargets.size(), nullptr);	// List of views.

		// Get the render target view of each of the render targets and store them in renderTargetViews.
		std::transform(m_BoundRenderTargets.begin(), m_BoundRenderTargets.end(),
			renderTargetViews.begin(), std::mem_fun(&CD3D10RenderTexture::GetRenderTargetView));

		ID3D10DepthStencilView* pDepthStencilView = m_BoundRenderTargets[0]->GetDepthStencilView();
		if (!pDepthStencilView) {
			// Use the default depth stencil view if the render target does not have one.
			pDepthStencilView = m_pDepthStencilView.Get();
		}
		
		// Clear the state, clearing everything back to the state at creation time.
		m_pD3DDevice->ClearState();

		// Set the render target.
		m_pD3DDevice->OMSetRenderTargets(1, &renderTargetViews[0], pDepthStencilView);
	}
	else {
		if (!m_BoundRenderTargets.empty()) {
			m_BoundRenderTargets.clear();

			// Clear the state, clearing everything back to the state at creation time.
			m_pD3DDevice->ClearState();

			// Set the render target.
			m_pD3DDevice->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
		}
	}
}

void CD3D10Renderer::VSetStreamOutTargets(const std::vector<std::shared_ptr<AGpuBuffer> >& buffers)
{
	if (buffers.empty()) {
		m_pD3DDevice->SOSetTargets(0, nullptr, nullptr);
	}
	else {
		std::vector<ID3D10Buffer*> d3dBuffers;
		std::vector<UINT> offsets;
		d3dBuffers.reserve(buffers.size());
		offsets.reserve(buffers.size());

		UINT offset = 0;

		for (auto& pBuffer : buffers) {
			auto pD3DBuffer = std::static_pointer_cast<CD3D10GpuBuffer>(pBuffer);
			d3dBuffers.push_back(pD3DBuffer->GetD3DBuffer());
			offsets.push_back(offset);
			offset += pD3DBuffer->GetSize();
		}

		m_pD3DDevice->SOSetTargets(buffers.size(), &d3dBuffers[0], &offsets[0]);
	}
}

void CD3D10Renderer::VSetStreamOutTarget(const std::shared_ptr<AGpuBuffer>& pBuffer)
{
	auto pD3DBuffer = std::static_pointer_cast<CD3D10GpuBuffer>(pBuffer)->GetD3DBuffer();
	m_pD3DDevice->SOSetTargets(1, &pD3DBuffer, nullptr);
}

void CD3D10Renderer::VSetVertexBuffer(const shared_ptr<AVertexGpuBuffer>& pVertexBuffer)
{
	shared_ptr<CD3D10VertexGpuBuffer> pD3DVertexBuffer = static_pointer_cast<CD3D10VertexGpuBuffer>(pVertexBuffer);
	assert(pD3DVertexBuffer);

	ID3D10Buffer* pD3DBuffer = pD3DVertexBuffer->GetD3DBuffer();
	size_t stride = pD3DVertexBuffer->GetStride();
	size_t offset = 0;

	m_pD3DDevice->IASetVertexBuffers(0, 1, &pD3DBuffer, &stride, &offset);
}

void CD3D10Renderer::VSetIndexBuffer(const shared_ptr<AIndexGpuBuffer>& pIndexBuffer)
{
	shared_ptr<CD3D10IndexGpuBuffer> pD3DIndexBuffer = static_pointer_cast<CD3D10IndexGpuBuffer>(pIndexBuffer);
	assert(pD3DIndexBuffer);

	ID3D10Buffer* pD3DBuffer = pD3DIndexBuffer->GetD3DBuffer();

	m_pD3DDevice->IASetIndexBuffer(pD3DBuffer, D3D10Conversions::GetFormat(pD3DIndexBuffer->GetIndexType()), 0);
}


void CD3D10Renderer::VRender(const CVertexDeclaration& vertexDeclaration, EPrimitiveType primitiveType, shared_ptr<AVertexGpuBuffer> pVertexBuffer, shared_ptr<AIndexGpuBuffer> pIndexBuffer /* = nullptr */)
{
	// Recreate pipeline states.
	HRESULT hResult = m_pD3DDevice->CreateBlendState(&m_BlendDesc, m_pBlendState.ReleaseAndGetAddressOf());
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create blend state with error: " << GetErrorMessage(hResult);
	}

	hResult = m_pD3DDevice->CreateRasterizerState(&m_RasterizerDesc, m_pRasterizerState.ReleaseAndGetAddressOf());
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create rasterizer state with error: " << GetErrorMessage(hResult);
	}

	hResult = m_pD3DDevice->CreateDepthStencilState(&m_DepthStencilDesc, m_pDepthStencilState.ReleaseAndGetAddressOf());
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth-stencil state with error: " << GetErrorMessage(hResult);
	}

	// Set pipeline states.
	m_pD3DDevice->RSSetState(m_pRasterizerState.Get());
	m_pD3DDevice->OMSetDepthStencilState(m_pDepthStencilState.Get(), m_StencilReferenceValue);
	m_pD3DDevice->OMSetBlendState(m_pBlendState.Get(), 0, 0xffffffff);
	
	std::vector<ID3D10SamplerState*> samplerStates;
	samplerStates.reserve(m_Textures.size());

	// Get number of valid texture stages.
	int numLayers = 0;
	for (size_t i = 0; i < m_Textures.size(); ++i) {
		if (!m_Textures[i]) {
			// Can't have holes in our texture layers.
			break;
		}

		CreateSamplerState(i);

		samplerStates.push_back(m_SamplerStates[i].Get());

		numLayers++;
	}

	if (numLayers != 0) {
		if (m_pBoundGeometryShader) {
			m_pD3DDevice->GSSetSamplers(0, samplerStates.size(), &samplerStates[0]);
			m_pD3DDevice->GSSetShaderResources(0, numLayers, &m_Textures[0]);
		}

		m_pD3DDevice->PSSetSamplers(0, samplerStates.size(), &samplerStates[0]);
		m_pD3DDevice->PSSetShaderResources(0, numLayers, &m_Textures[0]);
	}

	bool isUsingIndices = pIndexBuffer != nullptr;

	VSetVertexDeclaration(vertexDeclaration);
	m_pD3DDevice->IASetPrimitiveTopology(D3D10Conversions::GetPrimitiveType(primitiveType));

	// Set buffers.
	VSetVertexBuffer(pVertexBuffer);

	if (isUsingIndices) {
		VSetIndexBuffer(pIndexBuffer);

		m_pD3DDevice->DrawIndexed(pIndexBuffer->GetNumIndices(), 0, 0);
	}
	else {
		m_pD3DDevice->Draw(pVertexBuffer->GetNumVertices(), 0);
	}
}

void CD3D10Renderer::VSetColourWritesEnabled(const TColourWritesEnabled& colourWritesEnabled)
{
	UINT colourWritesMask = 0;
	if (colourWritesEnabled.red) {
		colourWritesMask |= D3D10_COLOR_WRITE_ENABLE_RED;
	}
	if (colourWritesEnabled.blue) {
		colourWritesMask |= D3D10_COLOR_WRITE_ENABLE_BLUE;
	}
	if (colourWritesEnabled.green) {
		colourWritesMask |= D3D10_COLOR_WRITE_ENABLE_GREEN;
	}
	if (colourWritesEnabled.alpha) {
		colourWritesMask |= D3D10_COLOR_WRITE_ENABLE_ALPHA;
	}

	m_BlendDesc.RenderTargetWriteMask[0] = colourWritesMask;
}

void CD3D10Renderer::VSetStencilBufferSettings(const TStencilBufferSettings& settings)
{
	m_StencilReferenceValue = settings.stencilReferenceValue;
	m_DepthStencilDesc.StencilEnable = settings.stencilCheckEnabled;
	m_DepthStencilDesc.StencilReadMask = settings.stencilWriteMask;
	m_DepthStencilDesc.StencilWriteMask = settings.stencilWriteMask;

	m_DepthStencilDesc.BackFace.StencilFunc = D3D10Conversions::GetComparisonFunction(settings.stencilCompareFunction);
	m_DepthStencilDesc.BackFace.StencilFailOp = D3D10Conversions::GetStencilOperation(settings.stencilFailOperation);
	m_DepthStencilDesc.BackFace.StencilDepthFailOp = D3D10Conversions::GetStencilOperation(settings.stencilDepthFailOperation);
	m_DepthStencilDesc.BackFace.StencilPassOp = D3D10Conversions::GetStencilOperation(settings.stencilPassOperation);

	m_DepthStencilDesc.FrontFace.StencilFunc = D3D10Conversions::GetComparisonFunction(settings.stencilCompareFunction);
	m_DepthStencilDesc.FrontFace.StencilFailOp = D3D10Conversions::GetStencilOperation(settings.stencilFailOperation);
	m_DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D10Conversions::GetStencilOperation(settings.stencilDepthFailOperation);
	m_DepthStencilDesc.FrontFace.StencilPassOp = D3D10Conversions::GetStencilOperation(settings.stencilPassOperation);
}

void CD3D10Renderer::VSetDepthBufferSettings(bool enabled, bool writeEnabled, EComparisonFunction compareFunction)
{
	m_DepthStencilDesc.DepthEnable = enabled;
	if (writeEnabled) {
		m_DepthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	}
	else {
		m_DepthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;
	}

	m_DepthStencilDesc.DepthFunc = D3D10Conversions::GetComparisonFunction(compareFunction);
}

void CD3D10Renderer::VSetCullingMode(ECullingMode cullingMode)
{
	m_RasterizerDesc.CullMode = D3D10Conversions::GetCullingMode(cullingMode);
}

void CD3D10Renderer::VSetFillMode(EFillMode fillMode)
{
	m_RasterizerDesc.FillMode = D3D10Conversions::GetFillMode(fillMode);
}

void CD3D10Renderer::VSetBlendingMode(const TBlendingMode& blendingMode)
{
	if (blendingMode.sourceColourBlendFactor == EBlendFactor::ONE && blendingMode.destColourBlendFactor == EBlendFactor::ZERO &&
		blendingMode.sourceAlphaBlendFactor == EBlendFactor::ONE && blendingMode.destAlphaBlendFactor == EBlendFactor::ZERO) {
	// Using default blending mode, just disable.
		m_BlendDesc.BlendEnable[0] = false;
	}
	else {
		m_BlendDesc.BlendEnable[0] = true;
		m_BlendDesc.SrcBlend = D3D10Conversions::GetBlendFactor(blendingMode.sourceColourBlendFactor);
		m_BlendDesc.DestBlend = D3D10Conversions::GetBlendFactor(blendingMode.destColourBlendFactor);
		m_BlendDesc.BlendOp = D3D10Conversions::GetBlendOperation(blendingMode.colourBlendOperation);
		m_BlendDesc.SrcBlendAlpha = D3D10Conversions::GetBlendFactor(blendingMode.sourceAlphaBlendFactor);
		m_BlendDesc.DestBlendAlpha = D3D10Conversions::GetBlendFactor(blendingMode.destAlphaBlendFactor);
		m_BlendDesc.BlendOpAlpha = D3D10Conversions::GetBlendOperation(blendingMode.alphaBlendOperation);
		//m_BlendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;	// This gets set elsewhere in VSetColourWritesEnabled().
		m_BlendDesc.AlphaToCoverageEnable = false;
	}
}

DXGI_SAMPLE_DESC CD3D10Renderer::DetermineMultiSampleLevels()
{
	unsigned int msaaCount = 4;
	unsigned int msaaQuality = -1;

	for (int samples = 4; samples <= D3D10_MAX_MULTISAMPLE_SAMPLE_COUNT; samples *= 2) {
		UINT currentMSAAQuality = -1;
		HRESULT hResult = m_pD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, samples, &currentMSAAQuality);

		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to determine multi-sampling levels with error: " << GetErrorMessage(hResult);
		}

		if (currentMSAAQuality == 0) {
			break;
		}
		else {
			msaaCount = samples;
			msaaQuality = currentMSAAQuality - 1;
		}
	}

	ZeroMemory(&m_SampleDesc, sizeof(DXGI_SAMPLE_DESC));
	m_SampleDesc.Count = msaaCount;
	m_SampleDesc.Quality = msaaQuality;

	// DirectX 10 should always support at least 4 levels of multi-sampling.
	assert(msaaQuality != -1);

	return m_SampleDesc;
}

void CD3D10Renderer::VSetTextureLayerFiltering(size_t layer, ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter)
{
	m_SamplerDescs[layer].Filter = D3D10Conversions::GetFilter(minFilter, magFilter, mipFilter);
	if (m_SamplerDescs[layer].Filter == D3D10_FILTER_ANISOTROPIC) {
		m_SamplerDescs[layer].MaxAnisotropy = 16;
	}
}

void CD3D10Renderer::VSetTexture(size_t layer, const shared_ptr<ATexture>& pTexture, bool disable /*= false*/)
{
	if (disable) {
		m_Textures[layer] = nullptr;
	}
	else {
		shared_ptr<CD3D10Texture> pD3DTexture = static_pointer_cast<CD3D10Texture>(pTexture);
		assert(pD3DTexture);

		m_Textures[layer] = pD3DTexture->GetD3DTexture();

	}
}

void CD3D10Renderer::VSetTextureLayerBlendState(size_t layer, const TTextureLayerBlendState& blendState)
{

}

void CD3D10Renderer::VSetTextureAddressModes(size_t layer, const TTextureUVWAddressModes& addressModes)
{
	m_SamplerDescs[layer].AddressU = D3D10Conversions::GetAddressMode(addressModes.m_u);
	m_SamplerDescs[layer].AddressV = D3D10Conversions::GetAddressMode(addressModes.m_v);
	m_SamplerDescs[layer].AddressW = D3D10Conversions::GetAddressMode(addressModes.m_w);
}

void CD3D10Renderer::VSetTextureBorderColour(size_t layer, const CColourValue& colour)
{
	m_SamplerDescs[layer].BorderColor[0] = colour.m_r;
	m_SamplerDescs[layer].BorderColor[1] = colour.m_g;
	m_SamplerDescs[layer].BorderColor[2] = colour.m_b;
	m_SamplerDescs[layer].BorderColor[3] = colour.m_a;
}

void CD3D10Renderer::CreateSamplerState(size_t layer)
{
	m_SamplerStates[layer].Reset();

	HRESULT hResult = m_pD3DDevice->CreateSamplerState(&m_SamplerDescs[layer], &m_SamplerStates[layer]);
	if (FAILED(hResult)) {
		// TODO: display error.
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create sampler state for layer " << layer
			<< "with error: " << GetErrorMessage(hResult);
	}
}

std::vector<D3D10_INPUT_ELEMENT_DESC> CD3D10Renderer::GetD3DVertexDeclaration(const CVertexDeclaration& vertexDeclaration)
{
	size_t numElements = vertexDeclaration.GetNumElements();

	std::vector<D3D10_INPUT_ELEMENT_DESC> d3dInputElements(numElements);

	int accumulativeOffset = 0;

	for (size_t i = 0; i < d3dInputElements.size(); ++i) {
		// Fill the structure.
		const CInputElement& element = vertexDeclaration.GetElement(i);

		ZeroMemory(&d3dInputElements[i], sizeof(D3D10_INPUT_ELEMENT_DESC));

		d3dInputElements[i].SemanticName = element.GetSemanticName().c_str();
		d3dInputElements[i].AlignedByteOffset = accumulativeOffset;
		d3dInputElements[i].Format = D3D10Conversions::GetFormat(element.GetFormat());
		d3dInputElements[i].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;

		accumulativeOffset += element.GetSize();
	}

	return std::move(d3dInputElements);
}

void CD3D10Renderer::VDrawText(const std::string text, const CPoint& position, const CColour& colour)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void CD3D10Renderer::VSetWindowed(bool windowed)
{
	m_pSwapChain->SetFullscreenState(!windowed, NULL);
}

void CD3D10Renderer::VOnResize()
{
	RECT rect;

	GetClientRect(g_pApp->GetHWND(), &rect);

	size_t width = rect.right - rect.left;
	size_t height = rect.bottom - rect.top;

	if (width == m_Width && height == m_Height) {
		return;
	}

	if (width == 0 || height == 0) {
		return;
	}

	m_Width = width;
	m_Height = height;
		
	unsigned int flags = 0;
	if (m_IsWindowed) {
		flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	}

	m_pRenderTargetView.Reset();
	m_pDepthStencilBuffer.Reset();
	m_pDepthStencilView.Reset();

	m_pSwapChain->ResizeBuffers(m_d3dpp.BufferCount, width, height, m_d3dpp.BufferDesc.Format, flags);

	// Create the render target view to the back buffer.
	ComPtr<ID3D10Texture2D> pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf()));
	m_pD3DDevice->CreateRenderTargetView(pBackBuffer.Get(), 0, &m_pRenderTargetView);

	// Create the depth/stencil buffer and view.
	D3D10_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D10_TEXTURE2D_DESC));
	depthStencilDesc.SampleDesc = m_SampleDesc;
	depthStencilDesc.Width = m_Width;
	depthStencilDesc.Height = m_Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;

	HRESULT hResult = m_pD3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf());
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil buffer " << GetErrorMessage(hResult);
		return;
	}

	hResult = m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf());
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil view " << GetErrorMessage(hResult);
		return;
	}

	// Bind the views to the output merger stage.
	m_pD3DDevice->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	// Set the view port.
	ZeroMemory(&m_ViewPort, sizeof(D3D10_VIEWPORT));
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = m_Width;
	m_ViewPort.Height = m_Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
}

}

