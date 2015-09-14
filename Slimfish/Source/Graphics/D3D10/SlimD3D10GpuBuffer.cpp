// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10GpuBuffer.cpp
// Description	: CSlimD3D10GpuBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D10GpuBuffer.h"

// Local Includes

namespace Slim {

	CD3D10GpuBuffer::CD3D10GpuBuffer(ID3D10Device* pDevice,
									 EBufferType bufferType, size_t bufferSize, const void* pSource,
									 EGpuBufferUsage usage, bool isInSystemMemory)
		:AGpuBuffer(bufferSize, usage, isInSystemMemory)
	{
		ZeroMemory(&m_desc, sizeof(D3D10_BUFFER_DESC));

		m_desc.ByteWidth = bufferSize;
		m_desc.MiscFlags = 0;

		if (usage == EGpuBufferUsage::WRITE_ONLY) {
			m_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		}

		if (bufferType == BUFFER_TYPE_INDEX) {
			m_desc.BindFlags = D3D10_BIND_INDEX_BUFFER;
		}
		else if (bufferType == BUFFER_TYPE_VERTEX) {
			m_desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		}

		if (isInSystemMemory) {
			m_desc.Usage = D3D10_USAGE_STAGING;
			m_desc.BindFlags = 0;	// D3D10_USAGE_STAGING can't be bound to the graphics pipeline.
			m_desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ | D3D10_CPU_ACCESS_WRITE;
		}
		else {
			m_desc.Usage = D3D10Conversions::GetUsage(usage);
		}

		D3D10_SUBRESOURCE_DATA initData;
		initData.pSysMem = pSource;

		if (FAILED(pDevice->CreateBuffer(&m_desc, &initData, &m_pBuffer))) {
			//throw CRenderingError();
			// TODO: throw error.
			return;
		}
	}

	CD3D10GpuBuffer::~CD3D10GpuBuffer()
	{
		SafeRelease(m_pBuffer);
	}

	void* CD3D10GpuBuffer::VLock(size_t offset, size_t size, EGpuBufferLockType lockType)
	{
		D3D10_MAP map = D3D10Conversions::GetLockType(lockType);

		void* pSource;

		HRESULT hResult = m_pBuffer->Map(map, 0, &pSource);
		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to lock buffer with error " << GetErrorMessage(hResult);
			return nullptr;
		}

		return pSource;
	}

	void CD3D10GpuBuffer::VUnlock()
	{
		m_pBuffer->Unmap();
	}

	ID3D10Buffer* CD3D10GpuBuffer::GetD3DBuffer()
	{
		return m_pBuffer;
	}

}
