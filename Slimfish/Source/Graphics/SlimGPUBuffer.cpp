// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimGPUBuffer.cpp
// Description	: CSlimGPUBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimGPUBuffer.h"

// Local Includes

namespace Slim {

AGpuBuffer::AGpuBuffer(EUsage usage, bool isInSystemMemory)
	:m_usage(usage),
	m_bufferSize(0),
	m_isInSystemMemory(isInSystemMemory),
	m_lockOffset(0),
	m_lockSize(0),
	m_isLocked(false)
{

}

AGpuBuffer::~AGpuBuffer()
{

}

void* AGpuBuffer::Lock(size_t offset, size_t size, ELockType lockType)
{
	assert(offset + size <= m_bufferSize);	// Don't overrun the buffers.
	assert(!m_isLocked);	// Locking the buffer twice without unlocking is invalid.

	m_lockOffset = offset;
	m_lockSize = size;
	
	// Call the derived class's implementation to lock the buffer.
	return VLock(offset, size, lockType);
}

void AGpuBuffer::Unlock()
{
	assert(m_isLocked);	// Can't unlock if we haven't locked.

	// Call the derived class's implementation to unlock the buffer.
	VUnlock();
}

const AGpuBuffer::EUsage AGpuBuffer::GetUsage() const
{
	return m_usage;
}

const size_t AGpuBuffer::GetSize() const
{
	return m_bufferSize;
}

const bool AGpuBuffer::IsInSystemMemory() const
{
	return m_isInSystemMemory;
}

/************************************************************************/
/* CGPUBufferLock implementation
/************************************************************************/

CGPUBufferLock::CGPUBufferLock(const shared_ptr<AGpuBuffer>& pBuffer, size_t offset, size_t size, AGpuBuffer::ELockType lockType) 
	:m_pBuffer(pBuffer),
	m_pLockedData(nullptr)
{
	assert(m_pBuffer);

	// Lock the buffer.
	m_pLockedData = pBuffer->Lock(offset, size, lockType);
}

CGPUBufferLock::~CGPUBufferLock()
{
	assert(m_pBuffer);
	m_pBuffer->Unlock();
}

void* CGPUBufferLock::GetLockedContents()
{
	return m_pLockedData;
}

}
