// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimVertexGpuBuffer.h
// Description	: CSlimVertexGpuBuffer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMVERTEXGPUBUFFER_H__
#define __SLIMVERTEXGPUBUFFER_H__

// Library Includes

// Local Includes
#include "SlimGpuBuffer.h"

namespace Slim {

	/** Class representing a specialization of AGpuBuffer for vertex buffers.
	@remarks
		
	*/
	class CVertexGpuBuffer : public AGpuBuffer {
		// Member Functions
	public:
		CVertexGpuBuffer(size_t numVertices, size_t stride, AGpuBuffer::EUsage usage, bool isInSystemMemory);
		~CVertexGpuBuffer();

		size_t GetStride() const;
		size_t GetNumVertices() const;
	protected:
	private:
		// Member Variables
	public:
	protected:
	private:
		size_t m_numVertices;
		size_t m_stride;
	};

}

#endif	// __SLIMVERTEXGPUBUFFER_H__