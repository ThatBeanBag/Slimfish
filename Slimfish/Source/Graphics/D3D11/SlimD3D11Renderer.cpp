// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11Renderer.cpp
// Description	: CSlimD3D11Renderer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes
#include <D3D11SDKLayers.h>

// This Include
#include "SlimD3D11Renderer.h"

// Local Includes
#include "SlimD3D11VertexGpuBuffer.h"
#include "SlimD3D11IndexGpuBuffer.h"
#include "SlimD3D11ShaderProgram.h"
#include "SlimD3D11Texture.h"
#include "SlimD3D11RenderTexture.h"

namespace Slim {

CD3D11Renderer::CD3D11Renderer(int width, int height, bool isWindowed)
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
	ZeroMemory(&m_SamplerDescs[0], m_SamplerDescs.size() * sizeof(D3D11_SAMPLER_DESC));
}

CD3D11Renderer::~CD3D11Renderer()
{
	// We don't own the bound shaders, just invalidate our reference to them.
	m_pBoundGeometryShader = nullptr;
	m_pBoundPixelShader = nullptr;
	m_pBoundVertexShader = nullptr;

	SLIM_SAFE_RELEASE(m_pRenderTargetView);
	SLIM_SAFE_RELEASE(m_pDepthStencilView);
	SLIM_SAFE_RELEASE(m_pSwapChain);
	SLIM_SAFE_RELEASE(m_pDepthStencilBuffer);
	SLIM_SAFE_RELEASE(m_pBlendState);
	SLIM_SAFE_RELEASE(m_pRasterizerState);
	SLIM_SAFE_RELEASE(m_pDepthStencilState);

	for (size_t i = 0; i < m_SamplerStates.size(); ++i) {
		SLIM_SAFE_RELEASE(m_SamplerStates[i]);
	}

	SLIM_SAFE_RELEASE(m_pImmediateContext);

#ifdef _DEBUG
	// Report live objects.
	ID3D11Debug* pDebug;
	m_pD3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDebug));
	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	SLIM_SAFE_RELEASE(pDebug);
#endif // _DEBUG

	SLIM_SAFE_RELEASE(m_pD3DDevice);
}

bool CD3D11Renderer::VInitialize()
{
	UINT deviceFlags = 0;

#ifdef _DEBUG
	deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	// Create the device.
	HRESULT hResult = D3D11CreateDevice(
		0,							// Default adapter.
		D3D_DRIVER_TYPE_HARDWARE,	// Driver type.
		0,							// Software device.
		deviceFlags,				// Device flags.
		nullptr,					// Feature levels.
		0,							// Feature levels.
		D3D11_SDK_VERSION,			// SDK Version.
		&m_pD3DDevice,				// Device.
		nullptr,					// Feature levels.
		&m_pImmediateContext);	// Immediate Context.

	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create DirectX 11 device with error:" << GetErrorMessage(hResult);
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
	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;

	auto release = [&]() { 
		SLIM_SAFE_RELEASE(dxgiDevice);
		SLIM_SAFE_RELEASE(dxgiAdapter);
		SLIM_SAFE_RELEASE(dxgiFactory);
	};

	if (FAILED(m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)))) {
		release();
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to query device interface with error: " << GetErrorMessage(hResult);
		return false;
	}

	if (FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter)))) {
		release();
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to get the DXGI adapter with error: " << GetErrorMessage(hResult);
		return false;
	}

	// Get the factory finally.
	if (FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory)))) {
		release();
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to get the DXGI factory with error: " << GetErrorMessage(hResult);
		return false;
	}

	// Create the swap chain.
	if (FAILED(dxgiFactory->CreateSwapChain(m_pD3DDevice, &m_d3dpp, &m_pSwapChain))) {
		release();
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create swap chain with error: " << GetErrorMessage(hResult);
		return false;
	}

	release();

	// Create the render target view to the back buffer.
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	m_pD3DDevice->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView);

	SLIM_SAFE_RELEASE(pBackBuffer);

	// Create the depth/stencil buffer and view.
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.SampleDesc = m_SampleDesc;
	depthStencilDesc.Width = m_Width;
	depthStencilDesc.Height = m_Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hResult = m_pD3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil buffer with error: " << GetErrorMessage(hResult);
		return false;
	}

	hResult = m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil view with error: "  << GetErrorMessage(hResult);
		return false;
	}

	// Bind the views to the output merger stage.
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Set the view port.
	ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = static_cast<float>(m_Width);
	m_ViewPort.Height = static_cast<float>(m_Height);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	// Create the rasterizer state.
	ZeroMemory(&m_RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	m_RasterizerDesc.CullMode = D3D11_CULL_FRONT;	// Clockwise culling for right handed matrices.
	m_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	m_RasterizerDesc.DepthClipEnable = true;
		
	m_pD3DDevice->CreateRasterizerState(&m_RasterizerDesc, &m_pRasterizerState);
	m_pImmediateContext->RSSetState(m_pRasterizerState);

	return true;
}

void CD3D11Renderer::VPreRender()
{
	if (m_pBoundRenderTarget) {
		ID3D11DepthStencilView* pDepthStencilView = m_pBoundRenderTarget->GetDepthStencilView();
		if (!pDepthStencilView) {
			pDepthStencilView = m_pDepthStencilView;
		}

		m_pImmediateContext->ClearRenderTargetView(m_pBoundRenderTarget->GetRenderTargetView(), m_pBackgroundColour);
		m_pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_pImmediateContext->OMSetDepthStencilState(0, 0);
	}
	else {
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, m_pBackgroundColour);
		m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_pImmediateContext->OMSetDepthStencilState(0, 0);
	}

	//m_pD3DDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
}

void CD3D11Renderer::VPostRender()
{
	m_pSwapChain->Present(0, 0);
}

shared_ptr<AVertexGpuBuffer> CD3D11Renderer::VCreateVertexBuffer(size_t numVertices, size_t stride, const void* pSource, EGpuBufferUsage usage, bool isInSystemMemory)
{
	return shared_ptr<AVertexGpuBuffer>(new CD3D11VertexGpuBuffer(m_pD3DDevice, m_pImmediateContext, numVertices, stride, pSource, usage, isInSystemMemory));
}

shared_ptr<AIndexGpuBuffer> CD3D11Renderer::VCreateIndexBuffer(size_t numIndices, AIndexGpuBuffer::EIndexType indexType, const void* pSource, EGpuBufferUsage usage, bool isInSystemMemory)
{
	return shared_ptr<AIndexGpuBuffer>(new CD3D11IndexGpuBuffer(m_pD3DDevice, m_pImmediateContext, numIndices, indexType, pSource, usage, isInSystemMemory));
}

shared_ptr<AShaderProgram> CD3D11Renderer::VCreateShaderProgram(const std::string& name, EShaderProgramType type, const std::string& entry, const std::string& shaderModel)
{
	shared_ptr<AShaderProgram> pShaderProgram(new CD3D11ShaderProgram(m_pD3DDevice, m_pImmediateContext, name, type));
	pShaderProgram->SetEntryPoint(entry);
	pShaderProgram->SetShaderModel(shaderModel);

	if (pShaderProgram->VLoad()) {
		return pShaderProgram;
	}
	else {
		return nullptr;
	}
}

shared_ptr<ATexture> CD3D11Renderer::VLoadTexture(const std::string& name, ETextureType type, ETextureUsage usage)
{
	shared_ptr<CD3D11Texture> pTexture(new CD3D11Texture(m_pD3DDevice, m_pImmediateContext, name, type, usage));

	if (pTexture) {
		pTexture->VLoad();

		return pTexture;
	}
	else {
		return nullptr;
	}
}

std::unique_ptr<ARenderTexture> CD3D11Renderer::VCreateRenderTexture(const std::string& name, size_t width, size_t height, size_t msaaCount, size_t msaaQuality, ETextureType textureType /* = ETextureType::TYPE_2D */)
{
	// Create the texture as a render target.
	std::shared_ptr<CD3D11Texture> pTexture(new CD3D11Texture(m_pD3DDevice, m_pImmediateContext, name, textureType, ETextureUsage::RENDER_TARGET));

	pTexture->SetWidth(width);
	pTexture->SetHeight(height);
	pTexture->SetMultiSampleCount(msaaCount);
	pTexture->SetMultiSampleQuality(msaaQuality);

	// Create the render target.
	pTexture->VLoad();

	return std::unique_ptr<ARenderTexture>(new CD3D11RenderTexture(m_pD3DDevice, pTexture));
}

void CD3D11Renderer::VSetWorldTransform(const CMatrix4x4& worldTransform)
{
		
}

void CD3D11Renderer::VSetViewTransform(const CMatrix4x4& viewTransform)
{
		
}

void CD3D11Renderer::VSetProjectionTransform(const CMatrix4x4& projectionTransform)
{
		
}

void CD3D11Renderer::VSetAmbientColour(const CColour& colour)
{
		
}

void CD3D11Renderer::VSetBackgroundColour(const CColourValue& colour)
{
	D3D11Conversions::GetColour(colour, m_pBackgroundColour);
}

void CD3D11Renderer::VSetFog(EFogType fogType, const CColourValue& colour /*= TColourValue::s_BLACK*/, float start /*= 0.0f*/, float end /*= 1.0f*/, float exponentialDensity /*= 1.0f*/)
{
		
}

void CD3D11Renderer::VSetVertexDeclaration(const CVertexDeclaration& vertexDeclaration)
{
	assert(m_pBoundVertexShader);

	ID3D11InputLayout* pLayout = m_pBoundVertexShader->GetD3DInputLayout(&vertexDeclaration);
	assert(pLayout);

	m_pImmediateContext->IASetInputLayout(pLayout);
}

void CD3D11Renderer::VSetShaderProgram(shared_ptr<AShaderProgram> pShader)
{
	assert(pShader);

	switch (pShader->GetType()) {
		case EShaderProgramType::VERTEX: {
			m_pBoundVertexShader = static_pointer_cast<CD3D11ShaderProgram>(pShader);

			ID3D11VertexShader* pVertexShader = m_pBoundVertexShader->GetD3DVertexShader();
			assert(pVertexShader);

			// Set the shader.
			m_pImmediateContext->VSSetShader(pVertexShader, nullptr, 0);

			// Set constant buffers.
			const CD3D11ShaderProgram::TConstantBufferList& buffers = m_pBoundVertexShader->GetD3DConstantBuffers();
			if (!buffers.empty()) {
				m_pImmediateContext->VSSetConstantBuffers(0, buffers.size(), &buffers[0]);
			}
			else {
				m_pImmediateContext->VSSetConstantBuffers(0, 0, NULL);
			}

			break;
		}
		case EShaderProgramType::PIXEL: {
			m_pBoundPixelShader = static_pointer_cast<CD3D11ShaderProgram>(pShader);

			ID3D11PixelShader* pPixelShader = m_pBoundPixelShader->GetD3DPixelShader();
			assert(pPixelShader);

			// Set the shader.
			m_pImmediateContext->PSSetShader(pPixelShader, nullptr, 0);

			// Set the constant buffers.
			const CD3D11ShaderProgram::TConstantBufferList& buffers = m_pBoundPixelShader->GetD3DConstantBuffers();

			if (!buffers.empty()) {
				m_pImmediateContext->PSSetConstantBuffers(0, buffers.size(), &buffers[0]);
			}
			else {
				m_pImmediateContext->PSSetConstantBuffers(0, 0, NULL);
			}

			break;
		}
		case EShaderProgramType::GEOMETRY: {
			m_pBoundGeometryShader = static_pointer_cast<CD3D11ShaderProgram>(pShader);

			ID3D11GeometryShader* pGeometryShader = m_pBoundGeometryShader->GetD3DGeometryShader();
			assert(pGeometryShader);

			// Set the shader.
			m_pImmediateContext->GSSetShader(pGeometryShader, nullptr, 0);

			// Set the constant buffers.
			const CD3D11ShaderProgram::TConstantBufferList& buffers = m_pBoundGeometryShader->GetD3DConstantBuffers();
			if (!buffers.empty()) {
				m_pImmediateContext->GSSetConstantBuffers(0, buffers.size(), &buffers[0]);
			}
			else {
				m_pImmediateContext->GSSetConstantBuffers(0, 0, NULL);
			}

			break;
		}
		default: {
			break;
		}
	}
}

void CD3D11Renderer::VDisableShaderProgram(EShaderProgramType programType)
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

void CD3D11Renderer::VSetRenderTarget(ARenderTexture* pRenderTarget)
{
	if (pRenderTarget) {
		m_pBoundRenderTarget = static_cast<CD3D11RenderTexture*>(pRenderTarget);

		ID3D11RenderTargetView* pRenderTargetView = m_pBoundRenderTarget->GetRenderTargetView();
		ID3D11DepthStencilView* pDepthStencilView = m_pBoundRenderTarget->GetDepthStencilView();
		if (!pDepthStencilView) {
			// Use the default depth stencil view if the render target does not have one.
			pDepthStencilView = m_pDepthStencilView;
		}
		
		// Clear the state, clearing everything back to the state at creation time.
		m_pImmediateContext->ClearState();

		// Set the render target.
		m_pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	}
	else {
		m_pBoundRenderTarget = nullptr;

		// Clear the state, clearing everything back to the state at creation time.
		m_pImmediateContext->ClearState();

		// Set the render target.
		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	}
}

void CD3D11Renderer::VSetVertexBuffer(shared_ptr<AVertexGpuBuffer> pVertexBuffer)
{
	shared_ptr<CD3D11VertexGpuBuffer> pD3DVertexBuffer = static_pointer_cast<CD3D11VertexGpuBuffer>(pVertexBuffer);
	assert(pD3DVertexBuffer);

	ID3D11Buffer* pD3DBuffer = pD3DVertexBuffer->GetD3DBuffer();
	size_t stride = pD3DVertexBuffer->GetStride();
	size_t offset = 0;

	m_pImmediateContext->IASetVertexBuffers(0, 1, &pD3DBuffer, &stride, &offset);
}

void CD3D11Renderer::VSetIndexBuffer(shared_ptr<AIndexGpuBuffer> pIndexBuffer)
{
	shared_ptr<CD3D11IndexGpuBuffer> pD3DIndexBuffer = static_pointer_cast<CD3D11IndexGpuBuffer>(pIndexBuffer);
	assert(pD3DIndexBuffer);

	ID3D11Buffer* pD3DBuffer = pD3DIndexBuffer->GetD3DBuffer();

	m_pImmediateContext->IASetIndexBuffer(pD3DBuffer, D3D11Conversions::GetFormat(pD3DIndexBuffer->GetIndexType()), 0);
}


void CD3D11Renderer::VRender(const CVertexDeclaration& vertexDeclaration, EPrimitiveType primitiveType, shared_ptr<AVertexGpuBuffer> pVertexBuffer, shared_ptr<AIndexGpuBuffer> pIndexBuffer /* = nullptr */)
{
	auto release = [this]() {
		SLIM_SAFE_RELEASE(m_pBlendState);
		SLIM_SAFE_RELEASE(m_pRasterizerState);
		SLIM_SAFE_RELEASE(m_pDepthStencilState);
	};

	// Release so we can create new states.
	release();

	HRESULT hResult = m_pD3DDevice->CreateBlendState(&m_BlendDesc, &m_pBlendState);
	if (FAILED(hResult)) {
		release();
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create blend state with error: " << GetErrorMessage(hResult);
	}

	hResult = m_pD3DDevice->CreateRasterizerState(&m_RasterizerDesc, &m_pRasterizerState);
	if (FAILED(hResult)) {
		release();
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create rasterizer state with error: " << GetErrorMessage(hResult);
	}

	hResult = m_pD3DDevice->CreateDepthStencilState(&m_DepthStencilDesc, &m_pDepthStencilState);
	if (FAILED(hResult)) {
		release();
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth-stencil state with error: " << GetErrorMessage(hResult);
	}

	m_pImmediateContext->RSSetState(m_pRasterizerState);
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, m_StencilReferenceValue);
	m_pImmediateContext->OMSetBlendState(m_pBlendState, 0, 0xffffffff);
		
	// Get number of valid texture stages.
	int numLayers = 0;
	for (size_t i = 0; i < m_Textures.size(); ++i) {
		if (!m_Textures[i]) {
			// Can't have holes in our texture layers.
			break;
		}

		CreateSamplerState(i);

		numLayers++;
	}

	if (numLayers != 0) {
		if (m_pBoundGeometryShader) {
			m_pImmediateContext->GSSetSamplers(0, numLayers, &m_SamplerStates[0]);
			m_pImmediateContext->GSSetShaderResources(0, numLayers, &m_Textures[0]);
		}

		m_pImmediateContext->PSSetSamplers(0, numLayers, &m_SamplerStates[0]);
		m_pImmediateContext->PSSetShaderResources(0, numLayers, &m_Textures[0]);
	}

	bool isUsingIndices = pIndexBuffer != nullptr;

	VSetVertexDeclaration(vertexDeclaration);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11Conversions::GetPrimitiveType(primitiveType));

	// Set buffers.
	VSetVertexBuffer(pVertexBuffer);

	if (isUsingIndices) {
		VSetIndexBuffer(pIndexBuffer);

		m_pImmediateContext->DrawIndexed(pIndexBuffer->GetNumIndices(), 0, 0);
	}
	else {
		m_pImmediateContext->Draw(pVertexBuffer->GetNumVertices(), 0);
	}
}

void CD3D11Renderer::VSetColourWritesEnabled(const TColourWritesEnabled& colourWritesEnabled)
{
	UINT colourWritesMask = 0;
	if (colourWritesEnabled.red) {
		colourWritesMask |= D3D11_COLOR_WRITE_ENABLE_RED;
	}
	if (colourWritesEnabled.blue) {
		colourWritesMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
	}
	if (colourWritesEnabled.green) {
		colourWritesMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
	}
	if (colourWritesEnabled.alpha) {
		colourWritesMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
	}

	m_BlendDesc.RenderTarget[0].RenderTargetWriteMask = colourWritesMask;
}

void CD3D11Renderer::VSetStencilBufferSettings(const TStencilBufferSettings& settings)
{
	m_StencilReferenceValue = settings.stencilReferenceValue;
	m_DepthStencilDesc.StencilEnable = settings.stencilCheckEnabled;
	m_DepthStencilDesc.StencilReadMask = settings.stencilWriteMask;
	m_DepthStencilDesc.StencilWriteMask = settings.stencilWriteMask;

	m_DepthStencilDesc.BackFace.StencilFunc = D3D11Conversions::GetComparisonFunction(settings.stencilCompareFunction);
	m_DepthStencilDesc.BackFace.StencilFailOp = D3D11Conversions::GetStencilOperation(settings.stencilFailOperation);
	m_DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11Conversions::GetStencilOperation(settings.stencilDepthFailOperation);
	m_DepthStencilDesc.BackFace.StencilPassOp = D3D11Conversions::GetStencilOperation(settings.stencilPassOperation);

	m_DepthStencilDesc.FrontFace.StencilFunc = D3D11Conversions::GetComparisonFunction(settings.stencilCompareFunction);
	m_DepthStencilDesc.FrontFace.StencilFailOp = D3D11Conversions::GetStencilOperation(settings.stencilFailOperation);
	m_DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11Conversions::GetStencilOperation(settings.stencilDepthFailOperation);
	m_DepthStencilDesc.FrontFace.StencilPassOp = D3D11Conversions::GetStencilOperation(settings.stencilPassOperation);
}

void CD3D11Renderer::VSetDepthBufferSettings(bool enabled, bool writeEnabled, EComparisonFunction compareFunction)
{
	m_DepthStencilDesc.DepthEnable = enabled;
	if (writeEnabled) {
		m_DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	}
	else {
		m_DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	}

	m_DepthStencilDesc.DepthFunc = D3D11Conversions::GetComparisonFunction(compareFunction);
}

void CD3D11Renderer::VSetCullingMode(ECullingMode cullingMode)
{
	m_RasterizerDesc.CullMode = D3D11Conversions::GetCullingMode(cullingMode);
}

void CD3D11Renderer::VSetFillMode(EFillMode fillMode)
{
	m_RasterizerDesc.FillMode = D3D11Conversions::GetFillMode(fillMode);
}

void CD3D11Renderer::VDrawText(const std::string text, const CPoint& position, const CColour& colour)
{

}

void CD3D11Renderer::VSetBlendingMode(const TBlendingMode& blendingMode)
{
	if (blendingMode.sourceColourBlendFactor == EBlendFactor::ONE && blendingMode.destColourBlendFactor == EBlendFactor::ZERO &&
		blendingMode.sourceAlphaBlendFactor == EBlendFactor::ONE && blendingMode.destAlphaBlendFactor == EBlendFactor::ZERO) {
	// Using default blending mode, just disable.
		m_BlendDesc.RenderTarget[0].BlendEnable = false;
	}
	else {
		m_BlendDesc.RenderTarget[0].BlendEnable = true;
		m_BlendDesc.RenderTarget[0].SrcBlend = D3D11Conversions::GetBlendFactor(blendingMode.sourceColourBlendFactor);
		m_BlendDesc.RenderTarget[0].DestBlend = D3D11Conversions::GetBlendFactor(blendingMode.destColourBlendFactor);
		m_BlendDesc.RenderTarget[0].BlendOp = D3D11Conversions::GetBlendOperation(blendingMode.colourBlendOperation);
		m_BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11Conversions::GetBlendFactor(blendingMode.sourceAlphaBlendFactor);
		m_BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11Conversions::GetBlendFactor(blendingMode.destAlphaBlendFactor);
		m_BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11Conversions::GetBlendOperation(blendingMode.alphaBlendOperation);
		//m_BlendDesc.RenderTargetWriteMask[0] = D3D11_COLOR_WRITE_ENABLE_ALL;	// This gets set elsewhere in VSetColourWritesEnabled().
		m_BlendDesc.AlphaToCoverageEnable = false;
	}
}

DXGI_SAMPLE_DESC CD3D11Renderer::DetermineMultiSampleLevels()
{
	unsigned int msaaCount = 4;
	unsigned int msaaQuality = -1;

	for (int samples = 4; samples <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; samples *= 2) {
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

void CD3D11Renderer::VSetTextureLayerFiltering(size_t layer, ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter)
{
	m_SamplerDescs[layer].Filter = D3D11Conversions::GetFilter(minFilter, magFilter, mipFilter);
	if (m_SamplerDescs[layer].Filter == D3D11_FILTER_ANISOTROPIC) {
		m_SamplerDescs[layer].MaxAnisotropy = 16;
	}
}

void CD3D11Renderer::VSetTexture(size_t layer, const shared_ptr<ATexture>& pTexture, bool disable /*= false*/)
{
	if (disable) {
		m_Textures[layer] = nullptr;
	}
	else {
		shared_ptr<CD3D11Texture> pD3DTexture = static_pointer_cast<CD3D11Texture>(pTexture);
		assert(pD3DTexture);

		m_Textures[layer] = pD3DTexture->GetD3DTexture();
	}
}

void CD3D11Renderer::VSetTextureLayerBlendState(size_t layer, const TTextureLayerBlendState& blendState)
{

}

void CD3D11Renderer::VSetTextureAddressModes(size_t layer, const TTextureUVWAddressModes& addressModes)
{
	m_SamplerDescs[layer].AddressU = D3D11Conversions::GetAddressMode(addressModes.m_u);
	m_SamplerDescs[layer].AddressV = D3D11Conversions::GetAddressMode(addressModes.m_v);
	m_SamplerDescs[layer].AddressW = D3D11Conversions::GetAddressMode(addressModes.m_w);
}

void CD3D11Renderer::VSetTextureBorderColour(size_t layer, const CColourValue& colour)
{
	m_SamplerDescs[layer].BorderColor[0] = colour.m_r;
	m_SamplerDescs[layer].BorderColor[1] = colour.m_g;
	m_SamplerDescs[layer].BorderColor[2] = colour.m_b;
	m_SamplerDescs[layer].BorderColor[3] = colour.m_a;
}

void CD3D11Renderer::CreateSamplerState(size_t layer)
{
	SLIM_SAFE_RELEASE(m_SamplerStates[layer]);

	HRESULT hResult = m_pD3DDevice->CreateSamplerState(&m_SamplerDescs[layer], &m_SamplerStates[layer]);
	if (FAILED(hResult)) {
		// TODO: display error.
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create sampler state for layer " << layer
			<< "with error: " << GetErrorMessage(hResult);
	}
}

std::vector<D3D11_INPUT_ELEMENT_DESC> CD3D11Renderer::GetD3DVertexDeclaration(const CVertexDeclaration& vertexDeclaration)
{
	size_t numElements = vertexDeclaration.GetNumElements();

	std::vector<D3D11_INPUT_ELEMENT_DESC> d3dInputElements(numElements);

	int accumulativeOffset = 0;

	for (size_t i = 0; i < d3dInputElements.size(); ++i) {
		// Fill the structure.
		const CInputElement& element = vertexDeclaration.GetElement(i);

		ZeroMemory(&d3dInputElements[i], sizeof(D3D11_INPUT_ELEMENT_DESC));

		d3dInputElements[i].SemanticName = element.GetSemanticName().c_str();
		d3dInputElements[i].AlignedByteOffset = accumulativeOffset;
		d3dInputElements[i].Format = D3D11Conversions::GetFormat(element.GetFormat());
		d3dInputElements[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		accumulativeOffset += element.GetSize();
	}

	return std::move(d3dInputElements);
}

void CD3D11Renderer::VSetWindowed(bool windowed)
{
	m_pSwapChain->SetFullscreenState(!windowed, NULL);
}

void CD3D11Renderer::VOnResize()
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

	SLIM_SAFE_RELEASE(m_pRenderTargetView);
	SLIM_SAFE_RELEASE(m_pDepthStencilBuffer);
	SLIM_SAFE_RELEASE(m_pDepthStencilView);

	m_pSwapChain->ResizeBuffers(m_d3dpp.BufferCount, width, height, m_d3dpp.BufferDesc.Format, flags);

	// Create the render target view to the back buffer.
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	m_pD3DDevice->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView);

	SLIM_SAFE_RELEASE(pBackBuffer);

	// Create the depth/stencil buffer and view.
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.SampleDesc = m_SampleDesc;
	depthStencilDesc.Width = m_Width;
	depthStencilDesc.Height = m_Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	HRESULT hResult = m_pD3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil buffer " << GetErrorMessage(hResult);
		return;
	}

	hResult = m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
	if (FAILED(hResult)) {
		SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil view " << GetErrorMessage(hResult);
		return;
	}

	// Bind the views to the output merger stage.
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Set the view port.
	ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = static_cast<float>(m_Width);
	m_ViewPort.Height = static_cast<float>(m_Height);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
}

}

