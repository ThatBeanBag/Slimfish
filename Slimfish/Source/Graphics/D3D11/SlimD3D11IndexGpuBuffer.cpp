// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11IndexGpuBuffer.cpp
// Description	: CD3D11IndexGpuBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D11IndexGpuBuffer.h"

// Local Includes
#include "SlimD3D11GpuBuffer.h"

namespace Slim {

	CD3D11IndexGpuBuffer::CD3D11IndexGpuBuffer(
		ID3D11Device* pD3DDevice,
		ID3D11DeviceContext* pImmediateContext,
		size_t numIndices, 
		EIndexType type, 
		const void* pSource,
		EGpuBufferUsage usage, 
		bool isInSystemMemory)
		:AIndexGpuBuffer(numIndices, type, usage, isInSystemMemory)
	{
		m_pImpl = std::make_unique<CD3D11GpuBuffer>(
			pD3DDevice,
			pImmediateContext,
			CD3D11GpuBuffer::BUFFER_TYPE_INDEX,
			GetSize(),
			pSource,
			usage, 
			isInSystemMemory);
	}

	CD3D11IndexGpuBuffer::~CD3D11IndexGpuBuffer()
	{

	}

	ID3D11Buffer* CD3D11IndexGpuBuffer::GetD3DBuffer()
	{
		return m_pImpl->GetD3DBuffer();
	}

	void* CD3D11IndexGpuBuffer::VLock(size_t offset, size_t size, EGpuBufferLockType lockType)
	{
		if (m_pImpl) {
			return m_pImpl->Lock(offset, size, lockType);
		}

		return nullptr;
	}

	void CD3D11IndexGpuBuffer::VUnlock()
	{
		if (m_pImpl) {
			m_pImpl->Unlock();
		}
	}

}

