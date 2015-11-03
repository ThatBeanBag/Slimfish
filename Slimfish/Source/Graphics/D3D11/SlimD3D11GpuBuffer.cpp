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
#include "SlimD3D11VertexGpuBuffer.h"
#include "SlimD3D11IndexGpuBuffer.h"

namespace Slim {

	CD3D11GpuBuffer::CD3D11GpuBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext,
		EGpuBufferType bufferType, size_t bufferSize, const void* pSource,
		EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory)
		:AGpuBuffer(bufferSize, usage, bufferType, isOutput, isInSystemMemory),
		m_pD3DDevice(pDevice),
		m_pD3DImmediateContext(pImmediateContext)
	{
		ZeroMemory(&m_desc, sizeof(D3D11_BUFFER_DESC));

		m_desc.ByteWidth = bufferSize;
		m_desc.MiscFlags = 0;

		if (usage == EGpuBufferUsage::WRITE_ONLY) {
			m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		if (usage == EGpuBufferUsage::READ_ONLY) {
			m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		}

		if (bufferType == EGpuBufferType::INDEX) {
			m_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		}
		else if (bufferType == EGpuBufferType::VERTEX) {
			m_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		}

		if (isOutput) {
			m_desc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
		}

		if (isInSystemMemory) {
			m_desc.Usage = D3D11_USAGE_STAGING;
			m_desc.BindFlags = 0;	// D3D11_USAGE_STAGING can't be bound to the graphics pipeline.
			m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		}
		else {
			m_desc.Usage = D3D11Conversions::GetUsage(usage);
		}

		HRESULT hResult = S_OK;

		if (pSource) {
		// Is there initialisation data?
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = pSource;
			hResult = pDevice->CreateBuffer(&m_desc, &initData, m_pBuffer.GetAddressOf());
		}
		else {
			hResult = pDevice->CreateBuffer(&m_desc, nullptr, m_pBuffer.GetAddressOf());
		}

		if(FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create gpu buffer with error: " << GetErrorMessage(hResult);
			return;
		}
	}

	CD3D11GpuBuffer::~CD3D11GpuBuffer()
	{

	}

	void CD3D11GpuBuffer::VCopy(const std::shared_ptr<AGpuBuffer>& pBuffer, 
		size_t size, size_t sourceOffset, size_t destinationOffset)
	{
		// Cast the buffer so that we can get the actual directX buffer.
		// Can't cast directly to CD3D11GpuBuffer as vertex and index buffers don't derive from CD3D11GpuBuffer.
		ID3D11Buffer* pD3DBuffer = nullptr;
		switch (pBuffer->GetType()){
			case EGpuBufferType::VERTEX: {
				pD3DBuffer = static_pointer_cast<CD3D11VertexGpuBuffer>(pBuffer)->GetD3DBuffer();
				break;
			}
			case EGpuBufferType::INDEX: {
				pD3DBuffer = static_pointer_cast<CD3D11IndexGpuBuffer>(pBuffer)->GetD3DBuffer();
				break;
			}
			case EGpuBufferType::UNKNOWN: {
				pD3DBuffer = static_pointer_cast<CD3D11GpuBuffer>(pBuffer)->GetD3DBuffer();
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

			m_pD3DImmediateContext->CopyResource(m_pBuffer.Get(), pD3DBuffer);
		}
		else {
			// Copy only a sub region.
			D3D11_BOX sourceRegion;
			sourceRegion.left = sourceOffset;
			sourceRegion.right = sourceOffset + size;
			sourceRegion.top = 0;
			sourceRegion.bottom = 1;
			sourceRegion.front = 0;
			sourceRegion.back = 1;

			m_pD3DImmediateContext->CopySubresourceRegion(
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
