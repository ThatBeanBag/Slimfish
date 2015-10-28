// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11RenderTexture.cpp
// Description	: CSlimD3D11RenderTexture implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D11RenderTexture.h"

// Local Includes
#include "SlimD3D11Texture.h"

namespace Slim {

	CD3D11RenderTexture::CD3D11RenderTexture(ID3D11Device* pDevice, shared_ptr<ATexture> pTexture) 
		:ARenderTexture(pTexture),
		m_pD3DDevice(pDevice),
		m_pDepthStencilView(nullptr),
		m_pRenderTargetView(nullptr)
	{
		assert(GetTexture());
		assert(GetTexture()->GetUsage() == ETextureUsage::RENDER_TARGET);

		shared_ptr<CD3D11Texture> pD3DTexture = static_pointer_cast<CD3D11Texture>(GetTexture());

		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
<<<<<<< HEAD
		desc.Format = D3D11Conversions::GetPixelFormat(pTexture->GetPixelFormat());	// TODO: this is a hack constructor should really take this as a parameter.
=======
		desc.Format = D3D11Conversions::GetPixelFormat(pTexture->GetPixelFormat());
>>>>>>> da6959e4adef2c222b6d399cb6646a9021dd8345

		HRESULT hResult = S_OK;
		ComPtr<ID3D11Resource> pDepthStencilBuffer = nullptr;
		D3D11_DEPTH_STENCIL_VIEW_DESC descDepthView;
		ZeroMemory(&descDepthView, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		switch (pD3DTexture->GetTextureType()) {
			case ETextureType::TYPE_1D: {
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
				descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = 0;

				// Create depth-stencil buffer
				ComPtr<ID3D11Texture1D> pDepthStencilBuffer1D = nullptr;
				D3D11_TEXTURE1D_DESC descDepth;

				descDepth.Width = pD3DTexture->GetWidth();
				descDepth.MipLevels = 1;
				descDepth.ArraySize = 1;
				descDepth.Format = DXGI_FORMAT_D32_FLOAT;
				descDepth.Usage = D3D11_USAGE_DEFAULT;
				descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				descDepth.CPUAccessFlags = 0;
				descDepth.MiscFlags = 0;
				descDepthView.Format = descDepth.Format;

				hResult = m_pD3DDevice->CreateTexture1D(&descDepth, nullptr, pDepthStencilBuffer1D.GetAddressOf());
				if (FAILED(hResult)) {
					SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth buffer for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
				}

				hResult = pDepthStencilBuffer1D.As(&pDepthStencilBuffer);
				if (FAILED(hResult)) {
					SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth buffer for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
				}

				break;
			}
			case ETextureType::TYPE_2D: {
				if (pD3DTexture->GetMultiSampleCount() > 1) {
					desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
					descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				}
				else {
					desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
					descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				}
				desc.Texture2D.MipSlice = 0;

				// Create depth-stencil buffer.
				ComPtr<ID3D11Texture2D> pDepthStencilBuffer2D = nullptr;
				D3D11_TEXTURE2D_DESC descDepth;

				descDepth.Width = pD3DTexture->GetWidth();
				descDepth.Height = pD3DTexture->GetHeight();
				descDepth.MipLevels = 1;
				descDepth.ArraySize = 1;
				descDepth.Format = DXGI_FORMAT_D32_FLOAT;
				descDepth.SampleDesc.Count = pTexture->GetMultiSampleCount();
				descDepth.SampleDesc.Quality = pTexture->GetMultiSampleQuality();
				descDepth.Usage = D3D11_USAGE_DEFAULT;
				descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				descDepth.CPUAccessFlags = 0;
				descDepth.MiscFlags = 0;
				descDepthView.Format = descDepth.Format;

				hResult = m_pD3DDevice->CreateTexture2D(&descDepth, nullptr, pDepthStencilBuffer2D.GetAddressOf());
				if (FAILED(hResult)) {
					SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth buffer for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
				}

				hResult = pDepthStencilBuffer2D.As(&pDepthStencilBuffer);
				if (FAILED(hResult)) {
					SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth buffer for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
				}
				break;
			}
			case ETextureType::TYPE_3D: {
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = 0;
				desc.Texture3D.FirstWSlice = 0;
				desc.Texture3D.WSize = -1;

				/*ComPtr<ID3D11Texture2D> pDepthStencilBuffer2D = nullptr;
				D3D11_TEXTURE2D_DESC descDepth;

				descDepth.Width = pD3DTexture->GetWidth();
				descDepth.Height = pD3DTexture->GetHeight();
				descDepth.ArraySize = pD3DTexture->GetDepth();
				descDepth.MipLevels = 1;
				descDepth.Format = DXGI_FORMAT_D32_FLOAT;
				descDepth.SampleDesc.Count = pTexture->GetMultiSampleCount();
				descDepth.SampleDesc.Quality = pTexture->GetMultiSampleQuality();
				descDepth.Usage = D3D11_USAGE_DEFAULT;
				descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				descDepth.CPUAccessFlags = 0;
				descDepth.MiscFlags = 0;

				descDepthView.Format = descDepth.Format;
				descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				descDepthView.Texture2DArray.ArraySize = -1;

				hResult = m_pD3DDevice->CreateTexture2D(&descDepth, nullptr, pDepthStencilBuffer2D.GetAddressOf());
				if (FAILED(hResult)) {
					SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth buffer for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
				}

				hResult = pDepthStencilBuffer2D.As(&pDepthStencilBuffer);
				if (FAILED(hResult)) {
					SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth buffer for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
				}*/
				break;
			}
			case ETextureType::TYPE_CUBIC: {
				if (pD3DTexture->GetMultiSampleCount() > 1) {
					desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
					descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
				}
				else {
					desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				}
				desc.Texture2DArray.MipSlice = 0;
				desc.Texture2DArray.ArraySize = 6;

				// Create depth-stencil buffer.
				ComPtr<ID3D11Texture2D> pDepthStencilBuffer2D = nullptr;
				D3D11_TEXTURE2D_DESC descDepth;

				descDepth.Width = pD3DTexture->GetWidth();
				descDepth.Height = pD3DTexture->GetHeight();
				descDepth.MipLevels = 1;
				descDepth.ArraySize = 6;
				descDepth.Format = DXGI_FORMAT_D32_FLOAT;
				descDepth.SampleDesc.Count = pTexture->GetMultiSampleCount();
				descDepth.SampleDesc.Quality = pTexture->GetMultiSampleQuality();
				descDepth.Usage = D3D11_USAGE_DEFAULT;
				descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				descDepth.CPUAccessFlags = 0;
				descDepth.MiscFlags = 0;

				descDepthView.Format = descDepth.Format;
				descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				descDepthView.Texture2DArray.ArraySize = -1;

				hResult = m_pD3DDevice->CreateTexture2D(&descDepth, nullptr, pDepthStencilBuffer2D.GetAddressOf());
				if (FAILED(hResult)) {
					SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth buffer for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
				}

				hResult = pDepthStencilBuffer2D.As(&pDepthStencilBuffer);
				if (FAILED(hResult)) {
					SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth buffer for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
				}
				break;
			}
			default: {
				break;
			}
		}

		hResult = m_pD3DDevice->CreateRenderTargetView(pD3DTexture->GetD3DResource(), &desc, m_pRenderTargetView.GetAddressOf());
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create render target view for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
			return;
		}

		if (pTexture->GetTextureType() != ETextureType::TYPE_3D) {
			hResult = m_pD3DDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), &descDepthView, m_pDepthStencilView.GetAddressOf());
			if (FAILED(hResult)) {
				SLIM_THROW(EExceptionType::RENDERING) << "Failed to create depth stencil view for " << pTexture->GetName() << " with error: " << GetErrorMessage(hResult);
			}
		}
	}

	CD3D11RenderTexture::~CD3D11RenderTexture()
	{

	}

	ID3D11DepthStencilView* CD3D11RenderTexture::GetDepthStencilView()
	{
		return m_pDepthStencilView.Get();
	}

	ID3D11RenderTargetView* CD3D11RenderTexture::GetRenderTargetView()
	{
		return m_pRenderTargetView.Get();
	}

}


