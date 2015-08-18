// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10VertexGpuBuffer.h
// Description	: CSlimD3D10VertexGpuBuffer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10VERTEXGPUBUFFER_H__
#define __SLIMD3D10VERTEXGPUBUFFER_H__

// Library Includes

// Local Includes
#include "../SlimVertexGpuBuffer.h"
#include "SlimD3D10Forward.h"

namespace Slim {

	/** Specialization of a vertex gpu buffer for DirectX 10.
	@remarks
		This class just takes the name vertex buffer; the actual 
		implementation is in CD3D10GpuBuffer.
	*/
	class CD3D10VertexGpuBuffer : public AVertexGpuBuffer {
		// Member Functions
	public:
		/** Construct a D3D10 vertex buffer on the gpu (can also be stored in system 
			memory see parameter isInSystemMemory).
		 	@author Hayden Asplet
		 	@param 
				pD3DDevice Pointer to the D3D10 device.
			@param 
				numVertices Number of vertices to store in the buffer.
			@param 
				stride Size of a single vertex.
		 	@param 
				pSource Pointer to the initial source vertices to store in the buffer.
		 	@param 
				usage Describes how the buffer is intended to be used.
		 	@param 
				isInSystemMemory True if the buffer should be stored in system memory 
				and not in video memory.
		*/
		CD3D10VertexGpuBuffer(ID3D10Device* pD3DDevice, 
							  size_t numVertices, size_t stride, void* pSource,
							  AGpuBuffer::EUsage usage, bool isInSystemMemory);
		/** Destructor
		 	@author Hayden Asplet
		*/
		~CD3D10VertexGpuBuffer();

		/** Get the directX 10 specific buffer.
			@remarks
				This is to be used by the directX 10 renderer only.
			@author Hayden Asplet
		*/
		ID3D10Buffer* GetD3DBuffer();
	protected:
	private:
		/** Internal method for locking the vertex buffer for reading/writing to.
		 	@author Hayden Asplet
			@param 
				offset Offset in bytes to the start of region of the buffer to lock.
			@param 
				size Size in bytes of the region to lock.
		 	@param 
				lockType The type of lock e.g. LOCK_DISCARD, LOCK_READ_ONLY etc. see AGpuBuffer::ELockType.
		 	@return
				A pointer to the locked region of memory for reading/writing to.
		*/
		virtual void* VLock(size_t offset, size_t size, ELockType lockType) override;

		/** Internal method for unlocking the vertex buffer.
		 	@author Hayden Asplet
		*/
		virtual void VUnlock() override;

		// Member Variables
	public:
	protected:
	private:
		// This class just takes the name vertex buffer; the actual implementation is in CD3D10GpuBuffer.
		CD3D10GpuBuffer* m_pImpl;	// To avoid multiple inheritance and the dreaded diamond of death, use PIMPL (pointer to implementation).
	};
}

#endif	// __SLIMD3D10VERTEXGPUBUFFER_H__