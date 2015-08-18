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
#include "SlimVertexDeclaration.h"

namespace Slim {

	/** Class representing a specialization of AGpuBuffer for vertex buffers.
	@remarks
		
	*/
	class AVertexGpuBuffer : public AGpuBuffer {
		// Member Functions
	public:
		/** Construct a vertex gpu buffer.
		 	@author Hayden Asplet
			@param 
				numVertices Number of vertices.
			@param 
				stride Size in bytes of a single vertex.
		 	@param 
				usage Specifies how the buffer is intended to be used. See AGpuBuffer::EUsage.
		 	@param 
				isInSystemMemory True if the buffer should be stored in system memory and not
				video memory.
		*/
		AVertexGpuBuffer(size_t numVertices, 
						 size_t stride,
						 AGpuBuffer::EUsage usage, 
						 bool isInSystemMemory);
		/** Destructor
			@author Hayden Asplet
		*/
		virtual ~AVertexGpuBuffer();

		/** Get the vertex stride (size of a single vertex in bytes). @author Hayden Asplet */
		const size_t GetStride() const;
		/** Get the number of vertices in the buffer. @author Hayden Asplet */
		const size_t GetNumVertices() const;

		/** Set the vertex declaration, which describes how the vertices in the buffer are to be 
			interpreted by the rendering system.
		 	@author Hayden Asplet
		*/
		void SetVertexDeclaration(const CVertexDeclaration& vertexDeclaration);

		/** Get the vertex declaration, which describes how the vertices in the buffer are to be
			interpreted by the rendering system.
		 	@author Hayden Asplet
		*/
		const CVertexDeclaration& GetVertexDeclaration() const;
	protected:
	private:
		// Member Variables
	public:
	protected:
	private:
		CVertexDeclaration m_VertexDeclaration;

		size_t m_NumVertices;	// Number of bytes.
		size_t m_Stride;		// Size in bytes of a single vertex.
	};

}

#endif	// __SLIMVERTEXGPUBUFFER_H__