// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10IndexGpuBuffer.cpp
// Description	: CD3D10IndexGpuBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D10IndexGpuBuffer.h"

// Local Includes
#include "SlimD3D10GpuBuffer.h"

namespace Slim {

	CD3D10IndexGpuBuffer::CD3D10IndexGpuBuffer(
		ID3D10Device* pD3DDevice, 
		size_t numIndices, 
		EIndexType type, 
		const void* pSource,
		EGpuBufferUsage usage, 
		bool isInSystemMemory)
		:AIndexGpuBuffer(numIndices, type, usage, isInSystemMemory)
	{
		m_pImpl = new CD3D10GpuBuffer(
			pD3DDevice,
			CD3D10GpuBuffer::BUFFER_TYPE_INDEX,
			GetSize(),
			pSource,
			usage, 
			isInSystemMemory);
	}

	CD3D10IndexGpuBuffer::~CD3D10IndexGpuBuffer()
	{
		SafeDelete(m_pImpl);
	}

	ID3D10Buffer* CD3D10IndexGpuBuffer::GetD3DBuffer()
	{
		return m_pImpl->GetD3DBuffer();
	}

	void* CD3D10IndexGpuBuffer::VLock(size_t offset, size_t size, EGpuBufferLockType lockType)
	{
		if (m_pImpl) {
			return m_pImpl->Lock(offset, size, lockType);
		}

		return nullptr;
	}

	void CD3D10IndexGpuBuffer::VUnlock()
	{
		if (m_pImpl) {
			m_pImpl->Unlock();
		}
	}

}

