// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10GpuBuffer.h
// Description	: CSlimD3D10GpuBuffer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10GPUBUFFER_H__
#define __SLIMD3D10GPUBUFFER_H__

// Library Includes

// Local Includes
#include "SlimD3D10Forward.h"
#include "../SlimGpuBuffer.h"

namespace Slim {

	class CD3D10GpuBuffer : public AGpuBuffer {
	public:
		/** List of D3D10 buffer types.
		*/
		enum EBufferType {
			BUFFER_TYPE_VERTEX,
			BUFFER_TYPE_INDEX
		};

		// Member Functions
	public:
		/** Construct a D3D10 buffer on the gpu (can also be stored in system memory see isInSystemMemory).
		 	@author Hayden Asplet
		 	@param 
				pD3DDevice Pointer to the D3D10 device.
		 	@param 
				bufferType Type of the buffer e.g. vertex or index.
		 	@param 
				bufferSize Size of the buffer in bytes.
		 	@param 
				usage Describes how the buffer is intended to be used.
		 	@param 
				isInSystemMemory True if the buffer should be stored in system memory and not in video 
				memory.
		*/
		CD3D10GpuBuffer(ID3D10Device* pD3DDevice, 
						EBufferType bufferType, size_t bufferSize, const void* pSource,
						EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory);
		/** Destructor
		 	@author Hayden Asplet
		*/
		~CD3D10GpuBuffer();

		/** Get the directX 10 specific buffer.
			@remarks
				This is to be used by the directX 10 renderer only.
		 	@author Hayden Asplet
		*/
		ID3D10Buffer* GetD3DBuffer();
	protected:
	private:
		/** @copydoc AGpuBuffer::VLock */
		virtual void* VLock(size_t offset, size_t size, EGpuBufferLockType lockType) override;
		/** @copydoc AGpuBuffer::VUnlock */
		virtual void VUnlock() override;
		// Member Variables
	public:
	protected:
	private:
		ID3D10Device* m_pD3DDevice;	// Pointer to the device, owned by the renderer.
		ComPtr<ID3D10Buffer> m_pBuffer;	// The buffer itself.
		D3D10_BUFFER_DESC m_desc;	// Description of the buffer.
	};

}

#endif	// __SLIMD3D10GPUBUFFER_H__