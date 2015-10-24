// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11VertexGpuBuffer.cpp
// Description	: CD3D11VertexGpuBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D11VertexGpuBuffer.h"

// Local Includes
#include "SlimD3D11GpuBuffer.h"

namespace Slim {

	CD3D11VertexGpuBuffer::CD3D11VertexGpuBuffer(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pImmediateContext,
												 size_t numVertices, size_t stride, const void* pSource, 
												 EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory)
		:AVertexGpuBuffer(numVertices, stride, usage, isOutput, isInSystemMemory)
	{
		m_pImpl = std::make_unique<CD3D11GpuBuffer>(
			pD3DDevice, 
			pImmediateContext,
			CD3D11GpuBuffer::BUFFER_TYPE_VERTEX,
			numVertices * stride,
			pSource,
			usage, 
			isOutput, 
			isInSystemMemory);
	}

	CD3D11VertexGpuBuffer::~CD3D11VertexGpuBuffer()
	{

	}

	ID3D11Buffer* CD3D11VertexGpuBuffer::GetD3DBuffer()
	{
		return m_pImpl->GetD3DBuffer();
	}

	void* CD3D11VertexGpuBuffer::VLock(size_t offset, size_t size, EGpuBufferLockType lockType)
	{
		if (m_pImpl) {
			return m_pImpl->Lock(offset, size, lockType);
		}

		return nullptr;
	}

	void CD3D11VertexGpuBuffer::VUnlock()
	{
		if (m_pImpl) {
			m_pImpl->Unlock();
		}
	}

}

