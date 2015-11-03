// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimIndexGpuBuffer.cpp
// Description	: CIndexGpuBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimIndexGpuBuffer.h"

// Local Includes

namespace Slim {

	AIndexGpuBuffer::AIndexGpuBuffer(size_t numIndices, EIndexType type, EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory)
		:AGpuBuffer(numIndices * type, usage, EGpuBufferType::INDEX, isOutput, isInSystemMemory),
		m_NumIndices(numIndices),
		m_Type(type)
	{

	}

	AIndexGpuBuffer::~AIndexGpuBuffer()
	{

	}

	const size_t AIndexGpuBuffer::GetNumIndices() const
	{
		return m_NumIndices;
	}

	const AIndexGpuBuffer::EIndexType AIndexGpuBuffer::GetIndexType() const
	{
		return m_Type;
	}

}
