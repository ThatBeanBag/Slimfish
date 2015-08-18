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
#include "SlimD3D10VertexGpuBuffer.h"
#include "SlimD3D10IndexGpuBuffer.h"
#include "SlimD3D10ShaderProgram.h"
#include "SlimD3D10Texture.h"

namespace Slim {

	CD3D10Renderer::CD3D10Renderer(int width, int height, bool isWindowed)
		:ARenderer(width, height, isWindowed),
		m_pD3DDevice(nullptr),
		m_pSwapChain(nullptr),
		m_pRenderTargetView(nullptr),
		m_pDepthStencilState(nullptr),
		m_pRasterizerState(nullptr),
		m_SamplerStates(g_MAX_TEXTURE_LAYERS, nullptr),
		m_Textures(g_MAX_TEXTURE_LAYERS, nullptr)
	{
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
		depthStencilDesc.Width = m_Width;
		depthStencilDesc.Height = m_Height;
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
		m_viewPort.Width = m_Width;
		m_viewPort.Height = m_Height;
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

	shared_ptr<AVertexGpuBuffer> CD3D10Renderer::VCreateVertexBuffer(size_t numVertices, size_t stride, void* pSource, AGpuBuffer::EUsage usage, bool isInSystemMemory)
	{
		return shared_ptr<AVertexGpuBuffer>(new CD3D10VertexGpuBuffer(m_pD3DDevice, numVertices, stride, pSource, usage, isInSystemMemory));
	}

	shared_ptr<AIndexGpuBuffer> CD3D10Renderer::VCreateIndexBuffer(size_t numIndices, AIndexGpuBuffer::EIndexType indexType, void* pSource, AGpuBuffer::EUsage usage, bool isInSystemMemory)
	{
		return shared_ptr<AIndexGpuBuffer>(new CD3D10IndexGpuBuffer(m_pD3DDevice, numIndices, indexType, pSource, usage, isInSystemMemory));
	}

	shared_ptr<ATexture> CD3D10Renderer::VLoadTexture(const std::string& name)
	{
		shared_ptr<CD3D10Texture> pTexture(new CD3D10Texture(m_pD3DDevice, name, ATexture::TEXTURE_TYPE_2D, ATexture::USAGE_STATIC));

		if (pTexture) {
			pTexture->VLoad();

			return pTexture;
		}
		else {
			return nullptr;
		}
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

	void CD3D10Renderer::VSetVertexDeclaration(const CVertexDeclaration& vertexDeclaration)
	{
		assert(m_pBoundVertexShader);

		std::vector<D3D10_INPUT_ELEMENT_DESC> d3dInputElements = GetD3DVertexDeclaration(vertexDeclaration);

		ID3D10InputLayout* pLayout;

		const CD3D10ShaderProgram::TByteCode& byteCode = m_pBoundVertexShader->GetByteCode();

		HRESULT hResult = m_pD3DDevice->CreateInputLayout(
			&d3dInputElements[0],				// Input element descriptions.
			d3dInputElements.size(),			// Number of elements.
			&byteCode[0],						// Byte code.
			byteCode.size(),					// Length of byte code.
			&pLayout);							// Output input layout.

		if (FAILED(hResult)) {
			// TODO: throw exception.
		}

		// Get the primitive type.
		D3D10_PRIMITIVE_TOPOLOGY primitiveType = D3D10Conversions::GetPrimitiveType(vertexDeclaration.GetPrimitiveType());

		m_pD3DDevice->IASetInputLayout(pLayout);
		m_pD3DDevice->IASetPrimitiveTopology(primitiveType);
	}

	void CD3D10Renderer::VSetShaderProgram(shared_ptr<AShaderProgram> pShader)
	{
		assert(pShader);

		switch (pShader->GetType()) {
			case AShaderProgram::SHADER_TYPE_VERTEX: {
				m_pBoundVertexShader = static_pointer_cast<CD3D10ShaderProgram>(pShader);

				ID3D10VertexShader* pVertexShader = m_pBoundVertexShader->GetD3DVertexShader();
				assert(pVertexShader);

				// Set the shader.
				m_pD3DDevice->VSSetShader(pVertexShader);

				// Set constant buffers.
				const CD3D10ShaderProgram::TConstantBufferList& buffers = m_pBoundVertexShader->GetD3DConstantBuffers();
				m_pD3DDevice->VSSetConstantBuffers(0, buffers.size(), &buffers[0]);

				break;
			}
			case AShaderProgram::SHADER_TYPE_PIXEL: {
				m_pBoundPixelShader = static_pointer_cast<CD3D10ShaderProgram>(pShader);

				ID3D10PixelShader* pPixelShader = m_pBoundVertexShader->GetD3DPixelShader();
				assert(pPixelShader);

				// Set the shader.
				m_pD3DDevice->PSSetShader(pPixelShader);

				// Set the constant buffers.
				const CD3D10ShaderProgram::TConstantBufferList& buffers = m_pBoundPixelShader->GetD3DConstantBuffers();
				m_pD3DDevice->PSSetConstantBuffers(0, buffers.size(), &buffers[0]);

				break;
			}
			case AShaderProgram::SHADER_TYPE_GEOMETRY: {
				m_pBoundGeometryShader = static_pointer_cast<CD3D10ShaderProgram>(pShader);

				ID3D10GeometryShader* pGeometryShader = m_pBoundVertexShader->GetD3DGeometryShader();
				assert(pGeometryShader);

				// Set the shader.
				m_pD3DDevice->GSSetShader(pGeometryShader);

				// Set the constant buffers.
				const CD3D10ShaderProgram::TConstantBufferList& buffers = m_pBoundGeometryShader->GetD3DConstantBuffers();
				m_pD3DDevice->GSSetConstantBuffers(0, buffers.size(), &buffers[0]);

				break;
			}
			default: {
				break;
			}
		}
	}

	void CD3D10Renderer::VSetVertexBuffer(shared_ptr<AVertexGpuBuffer> pVertexBuffer)
	{
		shared_ptr<CD3D10VertexGpuBuffer> pD3DVertexBuffer = static_pointer_cast<CD3D10VertexGpuBuffer>(pVertexBuffer);
		assert(pD3DVertexBuffer);

		ID3D10Buffer* pD3DBuffer = pD3DVertexBuffer->GetD3DBuffer();
		size_t stride = pD3DVertexBuffer->GetStride();
		size_t offset = 0;

		m_pD3DDevice->IASetVertexBuffers(0, 1, &pD3DBuffer, &stride, &offset);
	}

	void CD3D10Renderer::VSetIndexBuffer(shared_ptr<AIndexGpuBuffer> pIndexBuffer)
	{
		shared_ptr<CD3D10IndexGpuBuffer> pD3DIndexBuffer = static_pointer_cast<CD3D10IndexGpuBuffer>(pIndexBuffer);
		assert(pD3DIndexBuffer);

		ID3D10Buffer* pD3DBuffer = pD3DIndexBuffer->GetD3DBuffer();

		m_pD3DDevice->IASetIndexBuffer(pD3DBuffer, D3D10Conversions::GetFormat(pD3DIndexBuffer->GetIndexType()), 0);
	}

	void CD3D10Renderer::VRender(const CVertexDeclaration& vertexDeclaration, shared_ptr<AVertexGpuBuffer> pVertexBuffer, shared_ptr<AIndexGpuBuffer> pIndexBuffer /* = nullptr */)
	{
		m_pD3DDevice->OMSetDepthStencilState(0, 0);
		m_pD3DDevice->OMSetBlendState(0, 0, 0xffffffff);

		std::vector<ID3D10ShaderResourceView*> shaderResources;
		for (size_t i = 0; i < m_Textures.size(); ++i) {
			if (m_Textures[i]) {
				shaderResources.push_back(m_Textures[i]);
			}
		}

		if (!shaderResources.empty()) {
			m_pD3DDevice->PSSetShaderResources(0, shaderResources.size(), &shaderResources[0]);
		}

		size_t primitiveCount = 0;
		bool isUsingIndices = pIndexBuffer != nullptr;

		size_t vertexCount = pVertexBuffer->GetNumVertices();
		if (isUsingIndices) {
			vertexCount = pIndexBuffer->GetNumIndices();
		}

		// Calculate primitive count.
		switch (vertexDeclaration.GetPrimitiveType()) {
			case CVertexDeclaration::PRIMITIVE_TYPE_POINTLIST: {
				primitiveCount = vertexCount;
				break;
			}
			case CVertexDeclaration::PRIMITIVE_TYPE_LINELIST: {
				primitiveCount = static_cast<size_t>(vertexCount / 2.0f);
				break;
			}
			case CVertexDeclaration::PRIMITIVE_TYPE_LINESTRIP: {
				if (vertexCount >= 1) {
					primitiveCount = static_cast<size_t>(vertexCount - 1);
				}

				break;
			}
			case CVertexDeclaration::PRIMITIVE_TYPE_TRIANGLELIST: {
				primitiveCount = static_cast<size_t>(vertexCount / 3.0f);
				break;
			}
			case CVertexDeclaration::PRIMITIVE_TYPE_TRIANGLESTRIP: {
				if (vertexCount >= 2) {
					primitiveCount = static_cast<size_t>(vertexCount - 2);
				}

				break;
			}
			default: {
				assert(false);	// Shouldn't hit this branch.
				break;
			}
		}

		if (primitiveCount == 0) {
			return;
		}

		VSetVertexDeclaration(vertexDeclaration);

		// Set buffers.
		VSetVertexBuffer(pVertexBuffer);
		if (isUsingIndices) {
			VSetIndexBuffer(pIndexBuffer);

			m_pD3DDevice->DrawIndexed(pIndexBuffer->GetNumIndices(), 0, 0);
		}
		else {
			m_pD3DDevice->Draw(vertexCount, 0);
		}
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
		if (disable) {
			m_Textures[layer] = nullptr;
		}
		else {
			shared_ptr<CD3D10Texture> pD3DTexture = static_pointer_cast<CD3D10Texture>(pTexture);
			assert(pD3DTexture);

			m_Textures[layer] = pD3DTexture->GetD3DTexture();
		}
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

}

