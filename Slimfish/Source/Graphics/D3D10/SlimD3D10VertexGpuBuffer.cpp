// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10VertexGpuBuffer.cpp
// Description	: CD3D10VertexGpuBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D10VertexGpuBuffer.h"

// Local Includes
#include "SlimD3D10GpuBuffer.h"

namespace Slim {

	CD3D10VertexGpuBuffer::CD3D10VertexGpuBuffer(ID3D10Device* pD3DDevice, 
												 size_t numVertices, size_t stride, const void* pSource, 
												 AGpuBuffer::EUsage usage, bool isInSystemMemory)
		:AVertexGpuBuffer(numVertices, stride, usage, isInSystemMemory)
	{
		m_pImpl = new CD3D10GpuBuffer(pD3DDevice, 
									  CD3D10GpuBuffer::BUFFER_TYPE_VERTEX, 
									  numVertices * stride, 
									  pSource, 
									  usage, isInSystemMemory);
	}

	CD3D10VertexGpuBuffer::~CD3D10VertexGpuBuffer()
	{
		SafeDelete(m_pImpl);
	}

	ID3D10Buffer* CD3D10VertexGpuBuffer::GetD3DBuffer()
	{
		return m_pImpl->GetD3DBuffer();
	}

	void* CD3D10VertexGpuBuffer::VLock(size_t offset, size_t size, ELockType lockType)
	{
		if (m_pImpl) {
			return m_pImpl->Lock(offset, size, lockType);
		}

		return nullptr;
	}

	void CD3D10VertexGpuBuffer::VUnlock()
	{
		if (m_pImpl) {
			m_pImpl->Unlock();
		}
	}

}

