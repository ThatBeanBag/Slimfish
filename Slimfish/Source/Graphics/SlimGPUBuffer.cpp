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

AGpuBuffer::AGpuBuffer(size_t size, EGpuBufferUsage usage, bool isInSystemMemory)
	:m_Usage(usage),
	m_BufferSize(size),
	m_IsInSystemMemory(isInSystemMemory),
	m_LockOffset(0),
	m_LockSize(0),
	m_IsLocked(false)
{

}

AGpuBuffer::~AGpuBuffer()
{

}

void* AGpuBuffer::Lock(size_t offset, size_t size, EGpuBufferLockType lockType)
{
	assert(offset + size <= m_BufferSize);	// Don't overrun the buffers.
	assert(!m_IsLocked);	// Locking the buffer twice without unlocking is invalid.

	m_LockOffset = offset;
	m_LockSize = size;
	m_IsLocked = true;
	
	// Call the derived class's implementation to lock the buffer.
	return VLock(offset, size, lockType);
}

void AGpuBuffer::Unlock()
{
	assert(m_IsLocked);	// Can't unlock if we haven't locked.

	// Call the derived class's implementation to unlock the buffer.
	VUnlock();

	m_IsLocked = false;
}

const EGpuBufferUsage AGpuBuffer::GetUsage() const
{
	return m_Usage;
}

const size_t AGpuBuffer::GetSize() const
{
	return m_BufferSize;
}

const bool AGpuBuffer::IsOutput() const
{
	return m_IsOutput;
}

const bool AGpuBuffer::IsInSystemMemory() const
{
	return m_IsInSystemMemory;
}

/************************************************************************/
/* CGPUBufferLock implementation
/************************************************************************/

CGpuBufferLock::CGpuBufferLock(const shared_ptr<AGpuBuffer>& pBuffer, size_t offset, size_t size, EGpuBufferLockType lockType) 
	:m_pBuffer(pBuffer),
	m_pLockedData(nullptr)
{
	assert(m_pBuffer);

	// Lock the buffer.
	m_pLockedData = pBuffer->Lock(offset, size, lockType);
}

CGpuBufferLock::~CGpuBufferLock()
{
	assert(m_pBuffer);
	m_pBuffer->Unlock();
}

void* CGpuBufferLock::GetLockedContents()
{
	return m_pLockedData;
}

}
