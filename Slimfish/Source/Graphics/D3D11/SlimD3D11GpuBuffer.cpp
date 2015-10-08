// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11GpuBuffer.cpp
// Description	: CSlimD3D11GpuBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D11GpuBuffer.h"

// Local Includes

namespace Slim {

	CD3D11GpuBuffer::CD3D11GpuBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext,
									 EBufferType bufferType, size_t bufferSize, const void* pSource,
									 EGpuBufferUsage usage, bool isInSystemMemory)
		:AGpuBuffer(bufferSize, usage, isInSystemMemory)
	{
		ZeroMemory(&m_desc, sizeof(D3D11_BUFFER_DESC));

		m_desc.ByteWidth = bufferSize;
		m_desc.MiscFlags = 0;

		if (usage == EGpuBufferUsage::WRITE_ONLY) {
			m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		if (bufferType == BUFFER_TYPE_INDEX) {
			m_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		}
		else if (bufferType == BUFFER_TYPE_VERTEX) {
			m_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		}

		if (isInSystemMemory) {
			m_desc.Usage = D3D11_USAGE_STAGING;
			m_desc.BindFlags = 0;	// D3D11_USAGE_STAGING can't be bound to the graphics pipeline.
			m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		}
		else {
			m_desc.Usage = D3D11Conversions::GetUsage(usage);
		}

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pSource;

		HRESULT hResult = pDevice->CreateBuffer(&m_desc, &initData, m_pBuffer.GetAddressOf());
		if(FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create gpu buffer with error:" << GetErrorMessage(hResult);
			return;
		}
	}

	CD3D11GpuBuffer::~CD3D11GpuBuffer()
	{

	}

	void* CD3D11GpuBuffer::VLock(size_t offset, size_t size, EGpuBufferLockType lockType)
	{
		D3D11_MAP map = D3D11Conversions::GetLockType(lockType);

		D3D11_MAPPED_SUBRESOURCE mappedData;

		HRESULT hResult = m_pD3DImmediateContext->Map(m_pBuffer.Get(), 0, map, 0, &mappedData);
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to lock buffer with error " << GetErrorMessage(hResult);
			return nullptr;
		}

		return mappedData.pData;
	}

	void CD3D11GpuBuffer::VUnlock()
	{
		m_pD3DImmediateContext->Unmap(m_pBuffer.Get(), 0);
	}

	ID3D11Buffer* CD3D11GpuBuffer::GetD3DBuffer()
	{
		return m_pBuffer.Get();
	}

}
