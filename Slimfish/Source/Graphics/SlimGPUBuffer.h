// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimGpuBuffer.h
// Description	: AGPUBuffer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMGPUBUFFER_H__
#define __SLIMGPUBUFFER_H__

// Library Includes

// Local Includes

namespace Slim {

/** List of usage flags that describe how a buffer is used.
	@note
		This enumeration is weakly typed, as in it can be implicitly
		converted to and from an int this gives it the functionality to be
		used as a flag, but the downside is that it lacks type safety.
	*/
enum class EGpuBufferUsage : int {
	// Indicates that the buffer will rarely be modified.
	STATIC = 1,
	// Buffer requires dynamic use, allows for locking with options LOCK_DISCARD and 
	// LOCK_NO_OVERWRITE. Dynamic buffers are typically not placed in video memory
	// instead they are placed in AGP memory.
	DYNAMIC = 2,
	// Indicates the buffer will only be written. Using LOCK_READ_ONLY with a buffer
	// created with this usage is not allowed.
	WRITE_ONLY = 4,
	// Indicates that the buffer will only be read from. Use LOCK_READ_ONLY with a
	// buffer created with this usage.
	READ_ONLY = 8,
	// Indicates that the application will be refilling the entire contents frequently.
	DISCARDABLE = 16
};

enum class EGpuBufferType {
	UNKNOWN,
	VERTEX,
	INDEX
};

/** List of locking options that describe a type of lock to perform when locking a buffer.
*/
enum class EGpuBufferLockType {
	// Normal lock mode, allows both reading and writing.
	NORMAL,
	// Discard all memory in the buffer, this option is only valid when the buffer is created 
	// using USAGE_DYNAMIC.
	DISCARD,
	// Lock the buffer for reading only as in the application will not write to the buffer. 
	// Not allowed for buffers created with USAGE_WRITE_ONLY and is compulsory for buffers 
	// created with USAGE_STATIC.
	READ_ONLY,
	// Same as NORMAL except indicates the application will not overwrite any region of 
	// the buffer that has been used this frame.
	NO_OVERWRITE
};

/** Abstract class representing a buffer primarily stored in video memory (can be
	stored in system memory as well).
	@remarks
		This class defines the interface for all buffers stored on the GPU, such as
		vertex buffers, index buffers etc. Buffers do not have to be stored on the
		GPU; buffers can be created in system memory instead.
	@note
		Concrete subclasses should override VLock, and VUnlock as well as creating their
		own constructor to create the buffer.
	@note 
		It is preferable NOT to lock buffers manually (using Lock and Unlock). Instead use
		CGPUBufferLock, which enacts an RAII style implementation for locking and unlocking
		buffers. See CGPUBufferLock for more details.
	*/
class AGpuBuffer {
	// Member Functions
public:
	/** Construct a buffer stored in video memory (can also be stored in system memory
		see isInSystemMemory).
		@author Hayden Asplet
		@param 
			size Size of the buffer in bytes.
		@param
			usage How the buffer is intended to be used e.g. statically, dynamically, write only
			etc. see EGpuBufferUsage.
		@param
			type Type of buffer e.g. VERTEX or INDEX buffer.
		@param
			isOutput True if the buffer is to be used as output from a geometry shader. This specifies
			that the buffer can be bound to the stream output stage of the pipeline and cannot be used
			for drawing.
		@param
			isInSystemMemory True if the buffer should be stored in system memory and not in video
			memory on the GPU. This is the least optimal for buffers that need to sent to the GPU
			at some point, but can be used for heavily dynamic buffers that will be changed frequently
			by the CPU.
	*/
	AGpuBuffer(size_t size, EGpuBufferUsage usage, EGpuBufferType type, bool isOutput, bool isInSystemMemory);

	/** Destructor
		@author Hayden Asplet
	*/
	virtual ~AGpuBuffer();

	/** Lock the buffer for potential reading/writing to.
		@author Hayden Asplet
		@param
			offset Offset from the start of the buffer to the locked region.
		@param
			size Size of the region to lock in bytes.
		@param
			lockType The type of lock to perform e.g. LOCK_NORMAL (for both reading and writing),
			LOCK_DISCARD (discard the current buffer's contents), LOCK_READ_ONLY and LOCK_NO_OVERWRITE.
			See EGpuBufferLockType for more detials.
		@return
			A pointer to the contents of the buffer for both or either reading and writing to depending
			on the lock type.
	*/
	void* Lock(size_t offset, size_t size, EGpuBufferLockType lockType);

	/** Unlock a the locked buffer. @author Hayden Asplet */
	void Unlock();

	/** Copy the contents of a buffer into this one.
	 	@author Hayden Asplet
	 	@param 
			prhsBuffer Pointer to the right hand side source buffer to copy from.
		@param 
			sourceOffset Offset in bytes to the beginning of the region to copy from this buffer.
		@param 
			destinationOffset Offset in bytes to the starting location to put the copied 
			contents in the destination buffer.
		@param 
			size Size in bytes of the region in the buffer to copy.
	*/
	virtual void VCopy(const std::shared_ptr<AGpuBuffer>& prhsBuffer, size_t size,
		size_t sourceOffset = 0, size_t destinationOffset = 0) = 0;

	/** Get how the buffer is intended to be used. @author Hayden Asplet */
	const EGpuBufferUsage GetUsage() const;
	/** Get the size of the buffer in bytes. @author Hayden Asplet */
	const size_t GetSize() const;
	/** Set the type. @author Hayden Asplet */
	void SetType(EGpuBufferType type);
	/** Get the type. @author Hayden Asplet */
	const EGpuBufferType GetType() const;
	/** Get if the buffer is to be used as output from a geometry shader and not for rendering. @author Hayden Asplet */
	const bool IsOutput() const;
	/** Get if the buffer is stored in system memory or not. @author Hayden Asplet */
	const bool IsInSystemMemory() const;
protected:
private:
	/** Internal delegating method to lock the buffer for potential reading/writing to.
		@note Should be implemented by derived classes.
		@author Hayden Asplet
		@param
			offset Offset from the start of the buffer to the locked region.
		@param
			size Size of the region to lock in bytes.
		@param
			lockType The type of lock to perform e.g. NORMAL (for both reading and writing),
			DISCARD (discard the current buffer's contents), READ_ONLY and NO_OVERWRITE.
			See EGpuBufferLockType for more detials.
		@return
			A pointer to the contents of the buffer for both or either reading and writing to depending
			on the lock type.
		*/
	virtual void* VLock(size_t offset, size_t size, EGpuBufferLockType lockType) = 0;

	/** Internal delegating method to unlock the buffer. @author Hayden Asplet */
	virtual void VUnlock() = 0;

	// Member Variables
public:
protected:
private:
	EGpuBufferUsage m_Usage;// Usage 
	EGpuBufferType m_Type;	// Type of buffer e.g. VERTEX or INDEX buffer.
	size_t m_BufferSize;	// Size of the buffer in bytes.
	bool m_IsOutput;
	bool m_IsInSystemMemory;// True if the buffer is stored in system memory (CPU) and not on the GPU.

	size_t m_LockOffset;	// Offset from the start of the buffer to the locked region (if locked at all).
	size_t m_LockSize;		// Size of the region of the buffer currently locked (if locked at all).
	bool m_IsLocked;		// True if the buffer is currently locked.
};

/** Class implementing an RAII approach to locking GPU buffers.
@remarks
	Locking buffers manually can lead to situations where buffers do not get unlocked. This is a large 
	concern as locking a buffer typically stalls the GPU, where it waits patiently for the buffer to be
	unlocked so that it may resume it's own processing. If a buffer forever fails to be unlock the application
	will come to a huge halt. To circumvent these problems this class aims to allow for buffers to be 
	automatically unlocked when this object goes out of scope. Whilst capitalising on RAII it also allows for
	exception safe code. Therefore buffers should never be locked manually, instead creating a buffer lock
	is always advised.
*/
class CGpuBufferLock {
	// Member Functions
public:
	/** 
	 	@author Hayden Asplet
	 	@param 
			pBuffer The GPU buffer to lock.
		@param
			offset Offset from the start of the buffer to the locked region.
		@param
			size Size of the region to lock in bytes.
		@param
			lockType The type of lock to perform e.g. LOCK_NORMAL (for both reading and writing),
			LOCK_DISCARD (discard the current buffer's contents), LOCK_READ_ONLY and LOCK_NO_OVERWRITE.
			See EGpuBufferLockType for more detials.
	*/
	CGpuBufferLock(const shared_ptr<AGpuBuffer>& pBuffer, size_t offset, size_t size, EGpuBufferLockType lockType);

	/** Destructor
		@remarks
			Unlocks the buffer.
	 	@author Hayden Asplet
	*/
	~CGpuBufferLock();

	/** Get the contents of the buffer for potential reading/writing to.
	 	@author Hayden Asplet
	 	@return A pointer to the locked contents.
	*/
	void* GetLockedContents();
protected:
private:

	// Member Variables
public:
protected:
private:
	shared_ptr<AGpuBuffer> m_pBuffer;	// The locked buffer.
	void* m_pLockedData;	// The contents of the locked buffer.
};

}

#endif	// __SLIMGPUBUFFER_H__