// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10RenderTexture.cpp
// Description	: CSlimD3D10RenderTexture implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D10RenderTexture.h"

// Local Includes
#include "SlimD3D10Texture.h"

namespace Slim {

	CD3D10RenderTexture::CD3D10RenderTexture(ID3D10Device* pDevice, shared_ptr<ATexture> pTexture) :ARenderTexture(pTexture),
		m_pD3DDevice(pDevice),
		m_pDepthStencilView(nullptr),
		m_pRenderTargetView(nullptr)
	{
		assert(GetTexture());
		assert(GetTexture()->GetUsage() == ETextureUsage::RENDER_TARGET);

		shared_ptr<CD3D10Texture> pD3DTexture = static_pointer_cast<CD3D10Texture>(GetTexture());

		D3D10_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// TODO: this is a hack constructor should really take this as a parameter.

		switch (pD3DTexture->GetTextureType()) {
			case ETextureType::TYPE_1D: {
				desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = 0;
				break;
			}
			case ETextureType::TYPE_2D: {
				if (pD3DTexture->GetMultiSampleCount() > 1) {
					desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DMS;
				}
				else {
					desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
				}

				desc.Texture2D.MipSlice = 0;
				break;
			}
			case ETextureType::TYPE_3D: {
				desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = 0;
				break;
			}
			case ETextureType::TYPE_CUBIC: {
				if (pD3DTexture->GetMultiSampleCount() > 1) {
					desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DMSARRAY;
				}
				else {
					desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DARRAY;
				}

				desc.Texture2DArray.MipSlice = 0;
				desc.Texture2DArray.ArraySize = 6;
				break;
			}
			default: {
				break;
			}
		}

		HRESULT hResult = m_pD3DDevice->CreateRenderTargetView(pD3DTexture->GetD3DResource(), &desc, m_pRenderTargetView.GetAddressOf());
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create render target view for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
			return;
		}

		ComPtr<ID3D10Texture2D> pDepthStencilBuffer = nullptr;
		D3D10_TEXTURE2D_DESC descDepth;

		descDepth.Width = pD3DTexture->GetWidth();
		descDepth.Height = pD3DTexture->GetHeight();
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = pTexture->GetMultiSampleCount();
		descDepth.SampleDesc.Quality = pTexture->GetMultiSampleQuality();
		descDepth.Usage = D3D10_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		hResult = m_pD3DDevice->CreateTexture2D(&descDepth, nullptr, pDepthStencilBuffer.GetAddressOf());
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth buffer for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
		}

		D3D10_DEPTH_STENCIL_VIEW_DESC descDepthView;
		ZeroMemory(&descDepthView, sizeof(D3D10_DEPTH_STENCIL_VIEW_DESC));
		descDepthView.Format = descDepth.Format;
		if (pD3DTexture->GetMultiSampleCount() > 1) {
			descDepthView.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2DMS;
		}
		else {
			descDepthView.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		}

		hResult = m_pD3DDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), &descDepthView, m_pDepthStencilView.GetAddressOf());
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil view for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
		}
	}

	CD3D10RenderTexture::~CD3D10RenderTexture()
	{

	}

	ID3D10DepthStencilView* CD3D10RenderTexture::GetDepthStencilView()
	{
		return m_pDepthStencilView.Get();
	}

	ID3D10RenderTargetView* CD3D10RenderTexture::GetRenderTargetView()
	{
		return m_pRenderTargetView.Get();
	}

}


