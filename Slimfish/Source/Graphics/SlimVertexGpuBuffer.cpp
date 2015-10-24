// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimVertexGpuBuffer.cpp
// Description	: CSlimVertexGpuBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimVertexGpuBuffer.h"

// Local Includes
#include "SlimVertexDeclaration.h"


namespace Slim {

	AVertexGpuBuffer::AVertexGpuBuffer(size_t numVertices, size_t stride, EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory)
		:AGpuBuffer(numVertices * stride, usage, isOutput, isInSystemMemory),
		m_NumVertices(numVertices),
		m_Stride(stride)
	{

	}

	AVertexGpuBuffer::~AVertexGpuBuffer()
	{

	}

	const size_t AVertexGpuBuffer::GetStride() const
{
		return m_Stride;
	}

	const size_t AVertexGpuBuffer::GetNumVertices() const
{
		return m_NumVertices;
	}

	void AVertexGpuBuffer::SetVertexDeclaration(const CVertexDeclaration& vertexDeclaration)
	{
		m_VertexDeclaration = vertexDeclaration;
	}

	const CVertexDeclaration& AVertexGpuBuffer::GetVertexDeclaration() const
	{
		return m_VertexDeclaration;
	}

}
