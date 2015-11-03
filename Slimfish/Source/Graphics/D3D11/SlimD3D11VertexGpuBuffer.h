// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11VertexGpuBuffer.h
// Description	: CSlimD3D11VertexGpuBuffer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D11VERTEXGPUBUFFER_H__
#define __SLIMD3D11VERTEXGPUBUFFER_H__

// Library Includes

// Local Includes
#include "../SlimVertexGpuBuffer.h"
#include "SlimD3D11Forward.h"

namespace Slim {

	/** Specialization of a vertex gpu buffer for DirectX 10.
	@remarks
		This class just takes the name vertex buffer; the actual 
		implementation is in CD3D11GpuBuffer.
	*/
	class CD3D11VertexGpuBuffer : public AVertexGpuBuffer {
		// Member Functions
	public:
		/** Construct a D3D11 vertex buffer on the gpu (can also be stored in system 
			memory see parameter isInSystemMemory).
		 	@author Hayden Asplet
		 	@param 
				pD3DDevice Pointer to the D3D11 device.
			@param
				pImmediateContext Pointer to the immediate context of the device.
			@param
				numVertices Number of vertices to store in the buffer.
			@param 
				stride Size of a single vertex.
		 	@param 
				pSource Pointer to the initial source vertices to store in the buffer.
		 	@param 
				usage Describes how the buffer is intended to be used.
			@param
				isOutput True if the buffer is to be used as output from a geometry shader. This specifies
				that the buffer can be bound to the stream output stage of the pipeline and cannot be used
				for drawing.
		 	@param 
				isInSystemMemory True if the buffer should be stored in system memory 
				and not in video memory.
		*/
		CD3D11VertexGpuBuffer(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pImmediateContext,
							  size_t numVertices, size_t stride, const void* pSource,
							  EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory);
		/** Destructor
		 	@author Hayden Asplet
		*/
		~CD3D11VertexGpuBuffer();

		virtual void VCopy(const std::shared_ptr<AGpuBuffer>& pBuffer, size_t size,
			size_t sourceOffset, size_t destinationOffset) override;

		/** Get the directX 10 specific buffer.
			@remarks
				This is to be used by the directX 10 renderer only.
			@author Hayden Asplet
		*/
		ID3D11Buffer* GetD3DBuffer();
	protected:
	private:
		/** Internal method for locking the vertex buffer for reading/writing to.
		 	@author Hayden Asplet
			@param 
				offset Offset in bytes to the start of region of the buffer to lock.
			@param 
				size Size in bytes of the region to lock.
		 	@param 
				lockType The type of lock e.g. LOCK_DISCARD, LOCK_READ_ONLY etc. see EGpuBufferLockType.
		 	@return
				A pointer to the locked region of memory for reading/writing to.
		*/
		virtual void* VLock(size_t offset, size_t size, EGpuBufferLockType lockType) override;

		/** Internal method for unlocking the vertex buffer.
		 	@author Hayden Asplet
		*/
		virtual void VUnlock() override;

		// Member Variables
	public:
	protected:
	private:
		// This class just takes the name vertex buffer; the actual implementation is in CD3D11GpuBuffer.
		std::unique_ptr<CD3D11GpuBuffer> m_pImpl;	// To avoid multiple inheritance and the dreaded diamond of death, use PIMPL (pointer to implementation).
	};
}

#endif	// __SLIMD3D11VERTEXGPUBUFFER_H__