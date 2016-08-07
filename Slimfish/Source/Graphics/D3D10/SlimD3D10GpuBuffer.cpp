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
#include "SlimD3D10VertexGpuBuffer.h"
#include "SlimD3D10IndexGpuBuffer.h"

namespace Slim {

	CD3D10GpuBuffer::CD3D10GpuBuffer(ID3D10Device* pDevice,
									 EGpuBufferType bufferType, size_t bufferSize, const void* pSource,
									 EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory)
		:AGpuBuffer(bufferSize, usage, bufferType, isOutput, isInSystemMemory),
		m_pD3DDevice(pDevice)
	{
		ZeroMemory(&m_desc, sizeof(D3D10_BUFFER_DESC));

		m_desc.ByteWidth = bufferSize;
		m_desc.MiscFlags = 0;

		if (usage == EGpuBufferUsage::WRITE_ONLY) {
			m_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		}

		if (bufferType == EGpuBufferType::INDEX) {
			m_desc.BindFlags = D3D10_BIND_INDEX_BUFFER;
		}
		else if (bufferType == EGpuBufferType::VERTEX) {
			m_desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		}

		if (isOutput) {
			m_desc.BindFlags |= D3D10_BIND_STREAM_OUTPUT;
		}

		if (isInSystemMemory) {
			m_desc.Usage = D3D10_USAGE_STAGING;
			m_desc.BindFlags = 0;	// D3D10_USAGE_STAGING can't be bound to the graphics pipeline.
			m_desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ | D3D10_CPU_ACCESS_WRITE;
		}
		else {
			m_desc.Usage = D3D10Conversions::GetUsage(usage);
		}

		HRESULT hResult = S_OK;

		if (pSource) {
			D3D10_SUBRESOURCE_DATA initData;
			initData.pSysMem = pSource;

			hResult = pDevice->CreateBuffer(&m_desc, &initData, m_pBuffer.GetAddressOf());
		} 
		else {
			hResult = pDevice->CreateBuffer(&m_desc, nullptr, m_pBuffer.GetAddressOf());
		}

		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create vertex or index buffer with error: " << GetErrorMessage(hResult);
		}
	}

	CD3D10GpuBuffer::~CD3D10GpuBuffer()
	{

	}

	void CD3D10GpuBuffer::VCopy(const std::shared_ptr<AGpuBuffer>& pBuffer, size_t size, size_t sourceOffset, size_t destinationOffset)
	{
		// Cast the buffer so that we can get the actual directX buffer.
		// Can't cast directly to CD3D10GpuBuffer as vertex and index buffers don't derive from CD3D10GpuBuffer.
		ID3D10Buffer* pD3DBuffer = nullptr;
		switch (pBuffer->GetType()){
			case EGpuBufferType::VERTEX: {
				pD3DBuffer = static_pointer_cast<CD3D10VertexGpuBuffer>(pBuffer)->GetD3DBuffer();
				break;
			}
			case EGpuBufferType::INDEX: {
				pD3DBuffer = static_pointer_cast<CD3D10IndexGpuBuffer>(pBuffer)->GetD3DBuffer();
				break;
			}
			case EGpuBufferType::UNKNOWN: {
				pD3DBuffer = static_pointer_cast<CD3D10GpuBuffer>(pBuffer)->GetD3DBuffer();
				break;
			}
			default: {
				break;
			}
		}

		assert(pBuffer);

		// Check to see if we are copying the entire buffer.
		if (size == this->GetSize() &&
			size == pBuffer->GetSize() &&
			sourceOffset == 0 &&
			destinationOffset == 0) {

			m_pD3DDevice->CopyResource(m_pBuffer.Get(), pD3DBuffer);
		}
		else {
			// Copy only a sub region.
			D3D10_BOX sourceRegion;
			sourceRegion.left = sourceOffset;
			sourceRegion.right = sourceOffset + size;
			sourceRegion.top = 0;
			sourceRegion.bottom = 1;
			sourceRegion.front = 0;
			sourceRegion.back = 1;

			m_pD3DDevice->CopySubresourceRegion(
				m_pBuffer.Get(),	// Destination buffer.
				0,					// DstSubtesource.
				destinationOffset,	// DstX
				0,					// DstY
				0,					// DstZ
				pD3DBuffer,	// SrcResource
				0,					// SrcSubresource
				&sourceRegion);		// SrcRegion.
		}
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
		return m_pBuffer.Get();
	}

}
