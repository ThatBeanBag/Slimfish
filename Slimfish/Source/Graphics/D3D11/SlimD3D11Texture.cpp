// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11Texture.cpp
// Description	: CSlimD3D11Texture implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D11Texture.h"

// Local Includes

namespace Slim {

	CD3D11Texture::CD3D11Texture(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pImmediateContext, 
		const std::string& name, ETextureType textureType, ETextureUsage usage) 
		:ATexture(name, textureType, usage),
		m_pD3DDevice(pD3DDevice),
		m_pD3DImmediateContext(pImmediateContext)
	{

	}

	CD3D11Texture::~CD3D11Texture()
	{
		ETextureUsage usage = GetUsage();

		VUnload();
	}

	void CD3D11Texture::VLoad()
	{
		if (GetUsage() == ETextureUsage::RENDER_TARGET) {
			switch (GetTextureType()) {
				case ETextureType::TYPE_1D: {
					CreateRenderTarget1D();
					break;
				}
				case ETextureType::TYPE_CUBIC:	// Fall through.
				case ETextureType::TYPE_2D: {
					CreateRenderTarget2D();
					break;
				}
				case ETextureType::TYPE_3D: {
					CreateRenderTarget3D();
					break;
				}
				default: {
					break;
				}
			}

			return;
		}

		D3DX11_IMAGE_INFO imageInfo;
		HRESULT hResult = D3DX11GetImageInfoFromFileA(GetName().c_str(), NULL, &imageInfo, NULL);
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create texture from file " << GetName() << " with error: " << GetErrorMessage(hResult);
		}

		SetPixelFormat(D3D11Conversions::GetPixelFormat(imageInfo.Format));

		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Usage = D3D11Conversions::GetUsage(GetUsage());
		loadInfo.CpuAccessFlags = D3D11Conversions::GetCPUAccessFlags(GetUsage());
		loadInfo.Width = D3DX11_DEFAULT;
		loadInfo.Height = D3DX11_DEFAULT;
		loadInfo.Depth = D3DX11_DEFAULT;
		loadInfo.FirstMipLevel = D3DX11_DEFAULT;
		loadInfo.MipLevels = D3DX11_DEFAULT;
		loadInfo.BindFlags = D3DX11_DEFAULT;
		loadInfo.MiscFlags = D3DX11_DEFAULT;
		loadInfo.Format = imageInfo.Format;
		loadInfo.MiscFlags = imageInfo.MiscFlags;
		loadInfo.Filter = D3DX11_DEFAULT;
		loadInfo.MipFilter = D3DX11_DEFAULT;
		loadInfo.pSrcInfo = &imageInfo;


		if (loadInfo.Usage == D3D11_USAGE_DYNAMIC) {
			loadInfo.MipLevels = 1;
		}
		else if (loadInfo.Usage == D3D11_USAGE_STAGING) {
			loadInfo.BindFlags = 0;
		}

		if (imageInfo.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) {
			SetTextureType(ETextureType::TYPE_CUBIC);
		}

		hResult = D3DX11CreateTextureFromFileA(m_pD3DDevice, GetName().c_str(), &loadInfo, NULL, m_pTexture.GetAddressOf(), NULL);

		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create texture from file " << GetName() << " with error: " << GetErrorMessage(hResult);
		}

		D3D11_RESOURCE_DIMENSION resourceType;
		m_pTexture->GetType(&resourceType);

		switch (resourceType) {
			case D3D11_RESOURCE_DIMENSION_BUFFER: {
				break;
			}
			case D3D11_RESOURCE_DIMENSION_TEXTURE1D: {
				m_pTexture1D = static_cast<ID3D11Texture1D*>(m_pTexture.Get());
				//m_pTexture1D->AddRef();

				// Get the texture description.
				D3D11_TEXTURE1D_DESC desc;
				m_pTexture1D->GetDesc(&desc);

				SetSourceWidth(desc.Width);
				SetWidth(desc.Width);

				if (GetUsage() == ETextureUsage::STATIC) {
					CreateShaderResourceView1D(desc);
				}

				break;
			}
			case D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
				m_pTexture2D = static_cast<ID3D11Texture2D*>(m_pTexture.Get());
				//m_pTexture2D->AddRef();

				// Get the texture description.
				D3D11_TEXTURE2D_DESC desc;
				m_pTexture2D->GetDesc(&desc);

				SetSourceWidth(desc.Width);
				SetSourceHeight(desc.Height);
				SetWidth(desc.Width);
				SetHeight(desc.Height);

				if (GetUsage() == ETextureUsage::STATIC) {
					CreateShaderResourceView2D(desc);
				}

				break;
			}
			case D3D11_RESOURCE_DIMENSION_TEXTURE3D: {
				m_pTexture3D = static_cast<ID3D11Texture3D*>(m_pTexture.Get());
				//m_pTexture3D->AddRef();

				// Get the texture description.
				D3D11_TEXTURE3D_DESC desc;
				m_pTexture3D->GetDesc(&desc);

				SetSourceWidth(desc.Width);
				SetSourceHeight(desc.Height);
				SetSourceDepth(desc.Depth);
				SetWidth(desc.Width);
				SetHeight(desc.Height);
				SetDepth(desc.Depth);

				if (GetUsage() == ETextureUsage::STATIC) {
					CreateShaderResourceView3D(desc);
				}

				break;
			}
			default: {
				break;
			}
		}
	}

	void CD3D11Texture::VLoadRaw(const void* pData, size_t stride)
	{
		switch (GetTextureType()) {
			case ETextureType::TYPE_1D: {
				CreateRenderTarget1D(pData, stride);
				break;
			}
			case ETextureType::TYPE_CUBIC:	// Fall through.
			case ETextureType::TYPE_2D: {
				CreateRenderTarget2D(pData, stride);
				break;
			}
			case ETextureType::TYPE_3D: {
				CreateRenderTarget3D(pData, stride);
				break;
			}
			default: {
				break;
			}
		}
	}

	void CD3D11Texture::VUnload()
	{

	}

	const CImage CD3D11Texture::VGetImage() const
	{
		// Images must be 2D.
		assert(GetTextureType() == ETextureType::TYPE_2D);
		assert(GetPixelFormat() == ETexturePixelFormat::UNORM_8_RGBA);
		assert(m_pTexture2D);

		size_t width = GetSourceWidth();
		size_t height = GetSourceHeight();

		CImage image(width, height);

		D3D11_MAPPED_SUBRESOURCE mappedTexture;
		HRESULT hResult = m_pD3DImmediateContext->Map(m_pTexture2D.Get(), 0, D3D11_MAP_READ, 0, &mappedTexture);	
		if (FAILED(hResult)) {
		// Failed mapping.
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to read DirectX 11 2D texture resource for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}

		// Read the texture and store into the image.
		CColour* pTexels = reinterpret_cast<CColour*>(mappedTexture.pData);

		for (size_t y = 0; y < height; ++y) {
			size_t yStartOffset = y * mappedTexture.RowPitch / 4;

			for (size_t x = 0; x < width; ++x) {
				CColour colour = pTexels[yStartOffset + x];
				image[y][x] = colour;
			}
		}

		m_pD3DImmediateContext->Unmap(m_pTexture2D.Get(), 0);

		return image;
	}

	ID3D11ShaderResourceView* CD3D11Texture::GetD3DTexture()
	{
		return m_pShaderResourceView.Get();
	}

	ID3D11Resource* CD3D11Texture::GetD3DResource()
	{
		return m_pTexture.Get();
	}

	void CD3D11Texture::CreateRenderTarget1D(const void* pData /*= nullptr*/, size_t stride /*= 0*/)
	{
		assert(GetWidth() > 0);

		D3D11_TEXTURE1D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE1D_DESC));

		desc.Width = GetWidth();
		desc.ArraySize = 1;
		desc.Format = D3D11Conversions::GetPixelFormat(GetPixelFormat());
		desc.Usage = D3D11Conversions::GetUsage(GetUsage());
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D11Conversions::GetCPUAccessFlags(GetUsage());
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.MipLevels = 1;

		SetSourceWidth(desc.Width);

		HRESULT hResult;

		if (pData) {
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
			initData.SysMemPitch = GetWidth() * stride;
			initData.SysMemSlicePitch = 0;
			initData.pSysMem = pData;

			hResult = m_pD3DDevice->CreateTexture1D(&desc, &initData, m_pTexture1D.GetAddressOf());
		}
		else {
			hResult = m_pD3DDevice->CreateTexture1D(&desc, NULL, m_pTexture1D.GetAddressOf());
		}

		if (FAILED(hResult)) {
			VUnload();
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create 1D render target for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}

		// Grab the base resource.
		//hResult = m_pTexture1D->QueryInterface(__uuidof(ID3D11Resource), reinterpret_cast<void**>(&m_pTexture));
		hResult = m_pTexture1D.As(&m_pTexture);
		if (FAILED(hResult)) {
			VUnload();
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to grab base resource for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}

		CreateShaderResourceView1D(desc);
	}

	void CD3D11Texture::CreateRenderTarget2D(const void* pData /*= nullptr*/, size_t stride /*= 0*/)
	{
		assert(GetWidth() > 0 && GetHeight() > 0);

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

		desc.Width = GetWidth();
		desc.Height = GetHeight();
		desc.ArraySize = 1;							// If we wanted an array of 2D texture, we could assign this to GetDepth().
		desc.Format = D3D11Conversions::GetPixelFormat(GetPixelFormat());
		desc.Usage = D3D11Conversions::GetUsage(GetUsage());
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D11Conversions::GetCPUAccessFlags(GetUsage());
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.MipLevels = 1;

		SetSourceWidth(desc.Width);
		SetSourceHeight(desc.Height);

		// Set multi-sample levels for the render target.
		desc.SampleDesc.Count = GetMultiSampleCount();
		desc.SampleDesc.Quality = GetMultiSampleQuality();

		if (GetTextureType() == ETextureType::TYPE_CUBIC) {
			desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
			desc.ArraySize = 6;
		}
		
		HRESULT hResult;

		if (pData) {
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
			initData.SysMemPitch = GetWidth() * stride;
			initData.SysMemSlicePitch = 0;
			initData.pSysMem = pData;

			hResult = m_pD3DDevice->CreateTexture2D(&desc, &initData, m_pTexture2D.GetAddressOf());
		}
		else {
			hResult = m_pD3DDevice->CreateTexture2D(&desc, NULL, m_pTexture2D.GetAddressOf());
		}

		if (FAILED(hResult)) {
			VUnload();
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create 2D render target for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}

		// Grab the base resource.
		hResult = m_pTexture2D.As(&m_pTexture);
		if (FAILED(hResult)) {
			VUnload();
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to grab base resource for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}

		CreateShaderResourceView2D(desc);
	}

	void CD3D11Texture::CreateRenderTarget3D(const void* pData /*= nullptr*/, size_t stride /*= 0*/)
	{
		assert(GetWidth() > 0 && GetHeight() > 0 && GetDepth() > 0);

		D3D11_TEXTURE3D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE3D_DESC));

		desc.Width = GetWidth();
		desc.Height = GetHeight();
		desc.Depth = GetDepth();
		desc.Format = D3D11Conversions::GetPixelFormat(GetPixelFormat());
		desc.Usage = D3D11Conversions::GetUsage(GetUsage());
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D11Conversions::GetCPUAccessFlags(GetUsage());
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.MipLevels = 1;

		SetSourceWidth(desc.Width);
		SetSourceHeight(desc.Height);
		SetSourceDepth(desc.Depth);

		HRESULT hResult;

		if (pData) {
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
			initData.SysMemPitch = GetWidth() * stride;
			initData.SysMemSlicePitch = GetWidth() * GetHeight() * stride;
			initData.pSysMem = pData;

			hResult = m_pD3DDevice->CreateTexture3D(&desc, &initData, m_pTexture3D.GetAddressOf());
		}
		else {
			hResult = m_pD3DDevice->CreateTexture3D(&desc, NULL, m_pTexture3D.GetAddressOf());
		}

		if (FAILED(hResult)) {
			VUnload();
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create 3D  render target for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}

		// Grab the base resource.
		hResult = m_pTexture3D.As(&m_pTexture);
		if (FAILED(hResult)) {
			VUnload();
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to grab base resource for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}

		CreateShaderResourceView3D(desc);
	}

	void CD3D11Texture::CreateShaderResourceView1D(D3D11_TEXTURE1D_DESC desc)
	{
		assert(m_pTexture1D); // Must have a 1D texture to create a shader resource view for.

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		srvDesc.Texture1D.MipLevels = desc.MipLevels;
		srvDesc.Texture1D.MostDetailedMip = 0;

		HRESULT hResult = m_pD3DDevice->CreateShaderResourceView(m_pTexture1D.Get(), &srvDesc, m_pShaderResourceView.GetAddressOf());
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create shader resource view for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}
	}

	void CD3D11Texture::CreateShaderResourceView2D(D3D11_TEXTURE2D_DESC desc)
	{
		assert(m_pTexture2D); // Must have a 2D texture to create a shader resource view for.

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if (GetTextureType() == ETextureType::TYPE_CUBIC) {
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		}

		// Handle multi-sampled render target.
		if (GetUsage() == ETextureUsage::RENDER_TARGET && (GetMultiSampleCount() > 1 || GetMultiSampleQuality() > 0)) {
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		}

		HRESULT hResult = m_pD3DDevice->CreateShaderResourceView(m_pTexture2D.Get(), &srvDesc, m_pShaderResourceView.GetAddressOf());
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create shader resource view for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}
	}

	void CD3D11Texture::CreateShaderResourceView3D(D3D11_TEXTURE3D_DESC desc)
	{
		assert(m_pTexture3D); // Must have a 3D texture to create a shader resource view for.

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		srvDesc.Texture3D.MipLevels = desc.MipLevels;
		srvDesc.Texture3D.MostDetailedMip = 0;

		HRESULT hResult = m_pD3DDevice->CreateShaderResourceView(m_pTexture3D.Get(), &srvDesc, m_pShaderResourceView.GetAddressOf());
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create shader resource view for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}
	}

}

