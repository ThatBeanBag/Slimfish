// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11GpuBuffer.h
// Description	: CSlimD3D11GpuBuffer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D11GPUBUFFER_H__
#define __SLIMD3D11GPUBUFFER_H__

// Library Includes

// Local Includes
#include "SlimD3D11Forward.h"
#include "../SlimGpuBuffer.h"

namespace Slim {

	class CD3D11GpuBuffer : public AGpuBuffer {
	public:
		/** List of D3D11 buffer types.
		*/
		enum EBufferType {
			BUFFER_TYPE_VERTEX,
			BUFFER_TYPE_INDEX
		};

		// Member Functions
	public:
		/** Construct a D3D11 buffer on the gpu (can also be stored in system memory see isInSystemMemory).
		 	@author Hayden Asplet
		 	@param 
				pD3DDevice Pointer to the D3D11 device.
		 	@param 
				bufferType Type of the buffer e.g. vertex or index.
		 	@param 
				bufferSize Size of the buffer in bytes.
		 	@param 
				usage Describes how the buffer is intended to be used.
			@param
				isOutput True if the buffer is to be used as output from a geometry shader. This specifies
				that the buffer can be bound to the stream output stage of the pipeline and cannot be used
				for drawing.
		 	@param 
				isInSystemMemory True if the buffer should be stored in system memory and not in video 
				memory.
		*/
		CD3D11GpuBuffer(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pImmediateContext,
						EBufferType bufferType, size_t bufferSize, const void* pSource,
						EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory);
		/** Destructor
		 	@author Hayden Asplet
		*/
		~CD3D11GpuBuffer();

		/** Get the directX 10 specific buffer.
			@remarks
				This is to be used by the directX 10 renderer only.
		 	@author Hayden Asplet
		*/
		ID3D11Buffer* GetD3DBuffer();
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
		ID3D11Device* m_pD3DDevice;	// Pointer to the device, owned by the renderer.
		ID3D11DeviceContext* m_pD3DImmediateContext;	// Pointer to the immediate context, owned by the renderer.

		ComPtr<ID3D11Buffer> m_pBuffer;	// The buffer itself.
		D3D11_BUFFER_DESC m_desc;	// Description of the buffer.
	};

}

#endif	// __SLIMD3D11GPUBUFFER_H__