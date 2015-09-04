// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10Texture.cpp
// Description	: CSlimD3D10Texture implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D10Texture.h"

// Local Includes

namespace Slim {

	CD3D10Texture::CD3D10Texture(ID3D10Device* pD3DDevice, const std::string& name, EType textureType, EUsage usage)
		:ATexture(name, textureType, usage),
		m_pD3DDevice(pD3DDevice)
	{

	}

	CD3D10Texture::~CD3D10Texture()
	{
		VUnload();
	}

	void CD3D10Texture::VLoad()
	{
		if (GetUsage() & USAGE_RENDER_TARGET) {
			switch (GetTextureType()) {
				case TEXTURE_TYPE_1D: {
					CreateRenderTarget1D();
					break;
				}
				case TEXTURE_TYPE_CUBIC:	// Fall through.
				case TEXTURE_TYPE_2D: {
					CreateRenderTarget2D();
					break;
				}
				case TEXTURE_TYPE_3D: {
					CreateRenderTarget3D();
					break;
				}
				default: {
					break;
				}
			}

			return;
		}

		D3DX10_IMAGE_INFO imageInfo;
		HRESULT hResult = D3DX10GetImageInfoFromFileA(GetName().c_str(), NULL, &imageInfo, NULL);
		if (FAILED(hResult)) {
			SLIM_THROW(EXCEPTION_RENDERING) << "Failed to create texture from file " << GetName() << CEndExcept();
		}

		D3DX10_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Usage = D3D10Conversions::GetUsage(GetUsage());
		loadInfo.CpuAccessFlags = D3D10Conversions::GetCPUAccessFlags(GetUsage());
		loadInfo.Width = D3DX10_DEFAULT;
		loadInfo.Height = D3DX10_DEFAULT;
		loadInfo.Depth = D3DX10_DEFAULT;
		loadInfo.FirstMipLevel = D3DX10_DEFAULT;
		loadInfo.MipLevels = D3DX10_DEFAULT;
		loadInfo.BindFlags = D3DX10_DEFAULT;
		loadInfo.MiscFlags = D3DX10_DEFAULT;
		loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		loadInfo.Filter = D3DX10_DEFAULT;
		loadInfo.MipFilter = D3DX10_DEFAULT;
		loadInfo.pSrcInfo = &imageInfo;

		if (loadInfo.Usage == D3D10_USAGE_DYNAMIC) {
			loadInfo.MipLevels = 1;
		}
		else if (loadInfo.Usage == D3D10_USAGE_STAGING) {
			loadInfo.BindFlags = 0;
		}

		hResult = D3DX10CreateTextureFromFileA(m_pD3DDevice, GetName().c_str(), &loadInfo, NULL, &m_pTexture, NULL);

		if (FAILED(hResult)) {
			SLIM_THROW(EXCEPTION_RENDERING) << "Failed to create texture from file " << GetName() << CEndExcept();
		}

		D3D10_RESOURCE_DIMENSION resourceType;
		m_pTexture->GetType(&resourceType);

		switch (resourceType) {
			case D3D10_RESOURCE_DIMENSION_BUFFER: {
				break;
			}
			case D3D10_RESOURCE_DIMENSION_TEXTURE1D: {
				m_pTexture1D = static_cast<ID3D10Texture1D*>(m_pTexture);
				CreateShaderResourceView1D();
				break;
			}
			case D3D10_RESOURCE_DIMENSION_TEXTURE2D: {
				m_pTexture2D = static_cast<ID3D10Texture2D*>(m_pTexture);
				CreateShaderResourceView2D();
				break;
			}
			case D3D10_RESOURCE_DIMENSION_TEXTURE3D: {
				m_pTexture3D = static_cast<ID3D10Texture3D*>(m_pTexture);
				CreateShaderResourceView3D();
				break;
			}
			default: {
				break;
			}
		}
	}

	void CD3D10Texture::VUnload()
	{
		SLIM_SAFE_RELEASE(m_pTexture);
		SLIM_SAFE_RELEASE(m_pTexture1D);
		SLIM_SAFE_RELEASE(m_pTexture2D);
		SLIM_SAFE_RELEASE(m_pTexture3D);  
		SLIM_SAFE_RELEASE(m_pShaderResourceView);
	}

	const CImage CD3D10Texture::VGetImage() const
	{
		// Images must be 2D.
		assert(GetTextureType() == ATexture::TEXTURE_TYPE_2D);
		assert(m_pTexture2D);

		size_t width = GetSourceWidth();
		size_t height = GetSourceHeight();

		CImage image(width, height);

		D3D10_MAPPED_TEXTURE2D mappedTexture;
		HRESULT hResult = m_pTexture2D->Map(0, D3D10_MAP_READ, 0, &mappedTexture);	
		if (FAILED(hResult)) {
		// Failed mapping.
			SLIM_THROW(EXCEPTION_RENDERING) << "Failed to read DirectX 10 2D texture resource " << GetName() << CEndExcept();
		}

		// Read the texture and store into the map.
		float* pTexels = reinterpret_cast<float*>(mappedTexture.pData);

		for (size_t y = 0; y < height; ++y) {
			size_t yStartOffset = y * mappedTexture.RowPitch / 4;

			for (size_t x = 0; x < width; ++x) {
				size_t xStartOffset = x * 4;

				float r = pTexels[yStartOffset + xStartOffset + 0];	// Red.
				float g = pTexels[yStartOffset + xStartOffset + 1];	// Green.
				float b = pTexels[yStartOffset + xStartOffset + 2];	// Blue.
				float a = pTexels[yStartOffset + xStartOffset + 3];	// Alpha.

				// Write the texel to the image.
				image[y][x] = ToColour(CColourValue(r, g, b, a));
			}
		}

		return image;
	}

	ID3D10ShaderResourceView* CD3D10Texture::GetD3DTexture()
	{
		return m_pShaderResourceView;
	}

	void CD3D10Texture::CreateRenderTarget1D()
	{
		assert(GetWidth() > 0);

		D3D10_TEXTURE1D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D10_TEXTURE1D_DESC));

		desc.Width = GetWidth();
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO: this is a hack, should be equivalent to back buffer format.
		desc.Usage = D3D10Conversions::GetUsage(GetUsage());
		desc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D10Conversions::GetCPUAccessFlags(GetUsage());
		desc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

		HRESULT hResult = m_pD3DDevice->CreateTexture1D(&desc, NULL, &m_pTexture1D);
		if (FAILED(hResult)) {
			VUnload();
			// TODO: throw exception.
		}

		// Grab the base resource.
		hResult = m_pTexture1D->QueryInterface(__uuidof(ID3D10Resource), reinterpret_cast<void**>(&m_pTexture));
		if (FAILED(hResult)) {
			VUnload();
			// TODO: throw exception.
		}

		CreateShaderResourceView1D();
	}

	void CD3D10Texture::CreateRenderTarget2D()
	{
		assert(GetWidth() > 0 && GetHeight() > 0);

		D3D10_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D10_TEXTURE2D_DESC));

		desc.Width = GetWidth();
		desc.Height = GetHeight();
		desc.ArraySize = 1;							// If we wanted an array of 2D texture, we could assign this to GetDepth().
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// TODO: this is a hack, should be equivalent to back buffer format.
		desc.Usage = D3D10Conversions::GetUsage(GetUsage());
		desc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D10Conversions::GetCPUAccessFlags(GetUsage());
		desc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

		// Set multi-sample levels for the render target.
		desc.SampleDesc.Count = GetMultiSampleCount();
		desc.SampleDesc.Quality = GetMultiSampleQuality();

		if (GetTextureType() == TEXTURE_TYPE_CUBIC) {
			desc.MiscFlags |= D3D10_RESOURCE_MISC_TEXTURECUBE;
			desc.ArraySize = 6;
		}

		HRESULT hResult = m_pD3DDevice->CreateTexture2D(&desc, NULL, &m_pTexture2D);
		if (FAILED(hResult)) {
			VUnload();
			// TODO: throw exception.
		}

		// Grab the base resource.
		hResult = m_pTexture1D->QueryInterface(__uuidof(ID3D10Resource), reinterpret_cast<void**>(&m_pTexture));
		if (FAILED(hResult)) {
			VUnload();
			// TODO: throw exception.
		}

		CreateShaderResourceView2D();
	}

	void CD3D10Texture::CreateRenderTarget3D()
	{
		assert(GetWidth() > 0 && GetHeight() > 0 && GetDepth() > 0);

		D3D10_TEXTURE3D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D10_TEXTURE3D_DESC));

		desc.Width = GetWidth();
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO: this is a hack, should be equivalent to back buffer format.
		desc.Usage = D3D10Conversions::GetUsage(GetUsage());
		desc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D10Conversions::GetCPUAccessFlags(GetUsage());
		desc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

		HRESULT hResult = m_pD3DDevice->CreateTexture3D(&desc, NULL, &m_pTexture3D);
		if (FAILED(hResult)) {
			VUnload();
			// TODO: throw exception.
		}

		// Grab the base resource.
		hResult = m_pTexture1D->QueryInterface(__uuidof(ID3D10Resource), reinterpret_cast<void**>(&m_pTexture));
		if (FAILED(hResult)) {
			VUnload();
			// TODO: throw exception.
		}

		CreateShaderResourceView3D();
	}

	void CD3D10Texture::CreateShaderResourceView1D()
	{
		assert(m_pTexture1D); // Must have a 1D texture to create a shader resource view for.

		D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));

		// Get the texture description.
		D3D10_TEXTURE1D_DESC desc;
		m_pTexture1D->GetDesc(&desc);

		SetSourceWidth(desc.Width);

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
		srvDesc.Texture1D.MipLevels = desc.MipLevels;
		srvDesc.Texture1D.MostDetailedMip = 0;

		HRESULT hResult = m_pD3DDevice->CreateShaderResourceView(m_pTexture1D, &srvDesc, &m_pShaderResourceView);
		if (FAILED(hResult)) {
			// TODO: throw error.
		}
	}

	void CD3D10Texture::CreateShaderResourceView2D()
	{
		assert(m_pTexture2D); // Must have a 2D texture to create a shader resource view for.

		D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));

		// Get the texture description.
		D3D10_TEXTURE2D_DESC desc;
		m_pTexture2D->GetDesc(&desc);

		SetSourceWidth(desc.Width);
		SetSourceHeight(desc.Height);

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if (GetTextureType() == TEXTURE_TYPE_CUBIC) {
			srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
		}

		// Handle multi-sampled render target.
		if (GetUsage() & USAGE_RENDER_TARGET && GetMultiSampleCount() > 1 || GetMultiSampleQuality() > 0) {
			srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DMS;
		}

		HRESULT hResult = m_pD3DDevice->CreateShaderResourceView(m_pTexture2D, &srvDesc, &m_pShaderResourceView);
		if (FAILED(hResult)) {
			// TODO: throw error.
		}
	}

	void CD3D10Texture::CreateShaderResourceView3D()
	{
		assert(m_pTexture3D); // Must have a 3D texture to create a shader resource view for.

		D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));

		// Get the texture description.
		D3D10_TEXTURE3D_DESC desc;
		m_pTexture3D->GetDesc(&desc);

		SetSourceWidth(desc.Width);
		SetSourceHeight(desc.Height);
		SetSourceDepth(desc.Depth);

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE3D;
		srvDesc.Texture3D.MipLevels = desc.MipLevels;
		srvDesc.Texture3D.MostDetailedMip = 0;

		HRESULT hResult = m_pD3DDevice->CreateShaderResourceView(m_pTexture3D, &srvDesc, &m_pShaderResourceView);
		if (FAILED(hResult)) {
			// TODO: throw error.
		}
	}

}

