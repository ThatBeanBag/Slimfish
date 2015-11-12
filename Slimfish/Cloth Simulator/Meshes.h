// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Meshes.h
// Description	: CMeshes declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MESHES_H__
#define __MESHES_H__

// Library Includes

// Local Includes
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimIndexGpuBuffer.h>

namespace Meshes {

	struct TVertex {
		CVector3 position;
		CVector3 normal;
		float u;
		float v;
		float burntLevel;
		CColourValue colour;
	};

	void CreateSphere(int rings, int segments, std::shared_ptr<AVertexGpuBuffer>& pVertexBuffer, std::shared_ptr<AIndexGpuBuffer>& pIndexBuffer);
	void CreateCapsule(float radius, float height, size_t rings, size_t segments, size_t verticalSegments, std::shared_ptr<AVertexGpuBuffer>& pVertexBuffer, std::shared_ptr<AIndexGpuBuffer>& pIndexBuffer);
	void CreateQuad(float width, float height, std::shared_ptr<AVertexGpuBuffer>& pVertexBuffer);
	void CreatePyramid(std::shared_ptr<AVertexGpuBuffer>& pVertexBuffer);
}

#endif	// __MESHES_H__