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

		D3DX10_IMAGE_LOAD_INFO loadInfo;
		ZeroMemory(&loadInfo, sizeof(D3DX10_IMAGE_LOAD_INFO));

		HRESULT hResult = D3DX10CreateTextureFromFileA(m_pD3DDevice, GetName().c_str(), NULL, NULL, &m_pTexture, NULL);

		if (FAILED(hResult)) {
			// TODO: throw error.
			return;
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

		switch (GetTextureType()) {
		case TEXTURE_TYPE_1D: {
			loadInfo.Usage = D3D10Conversions::GetUsage(GetUsage());
			loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
			loadInfo.CpuAccessFlags = D3D10Conversions::GetCPUAccessFlags(GetUsage());
			loadInfo.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;
			//loadInfo.


			break;
		}
		case TEXTURE_TYPE_2D: {
			break;
		}
		case TEXTURE_TYPE_3D: {
			break;
		}
		case TEXTURE_TYPE_CUBIC: {
			break;
		}
		default: {
			break;
		}
		}
	}

	void CD3D10Texture::VUnload()
	{
		SafeRelease(m_pTexture);
		SafeRelease(m_pTexture1D);
		SafeRelease(m_pTexture2D);
		SafeRelease(m_pTexture3D);
		SafeRelease(m_pShaderResourceView);
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

		CreateShaderResourceView1D();
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

		CreateShaderResourceView1D();
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
		srvDesc.Texture1D.MostDetailedMip = desc.MipLevels - 1;

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
		srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;

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
		srvDesc.Texture3D.MostDetailedMip = desc.MipLevels - 1;

		HRESULT hResult = m_pD3DDevice->CreateShaderResourceView(m_pTexture3D, &srvDesc, &m_pShaderResourceView);
		if (FAILED(hResult)) {
			// TODO: throw error.
		}
	}

}

