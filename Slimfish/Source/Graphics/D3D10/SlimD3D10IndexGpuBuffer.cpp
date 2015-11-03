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
		bool isOutput,
		bool isInSystemMemory)
		:AIndexGpuBuffer(numIndices, type, usage, isOutput, isInSystemMemory)
	{
		m_pImpl = std::make_unique<CD3D10GpuBuffer>(
			pD3DDevice,
			EGpuBufferType::INDEX,
			GetSize(),
			pSource,
			usage, 
			isOutput,
			isInSystemMemory);
	}

	CD3D10IndexGpuBuffer::~CD3D10IndexGpuBuffer()
	{

	}

	void CD3D10IndexGpuBuffer::VCopy(const std::shared_ptr<AGpuBuffer>& pBuffer, size_t size, size_t sourceOffset, size_t destinationOffset)
	{
		m_pImpl->VCopy(pBuffer, size, sourceOffset, destinationOffset);
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

