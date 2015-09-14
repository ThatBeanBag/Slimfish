// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimIndexGpuBuffer.h
// Description	: CSlimIndexGpuBuffer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMINDEXGPUBUFFER_H__
#define __SLIMINDEXGPUBUFFER_H__

// Library Includes

// Local Includes
#include "SlimGpuBuffer.h"

namespace Slim {

/** Specialization of AGpuBuffer for index buffers.
*/
class AIndexGpuBuffer : public AGpuBuffer {
public:
	enum EIndexType {
		// 16-bit, for use with short typed indices.
		INDEX_TYPE_16 = 2,
		// 32-bit, for use wih int typed indices.
		INDEX_TYPE_32 = 4
	};

	// Member Functions
public:
	/** Construct a index buffer.
		@author Hayden Asplet
		@param
			numIndices Number of indices to store in the buffer.
		@param
			type Index type either INDEX_TYPE_16 or INDEX_TYPE_32.
		@param
			usage Describes how the buffer is intended to be used. See EGpuBufferUsage.
		@param
			isInSystemMemory True if the buffer should be stored in system memory and not
			video memory.
	*/
	AIndexGpuBuffer(size_t numIndices,
					EIndexType type,
					EGpuBufferUsage usage,
					bool isInSystemMemory);
	/** Destructor.
		@author Hayden Asplet
	*/
	virtual ~AIndexGpuBuffer();

	/** Get the number of indices stored in the buffer. @author Hayden Asplet */
	const size_t GetNumIndices() const;

	/** Get the index type. @author Hayden Asplet */
	const EIndexType GetIndexType() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	size_t m_NumIndices; // Size in bytes of a single index.
	EIndexType m_Type;
};
}

#endif	// __SLIMINDEXGPUBUFFER_H__