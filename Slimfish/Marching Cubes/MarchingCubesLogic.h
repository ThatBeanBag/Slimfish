// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MarchingCubesLogic.h
// Description	: CMarchingCubesLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MARCHINGCUBESLOGIC_H__
#define __MARCHINGCUBESLOGIC_H__

// Library Includes

// Local Includes
#include <GameBase/SlimGameLogic.h>
#include <Graphics/SlimRenderPass.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimIndexGpuBuffer.h>
#include <Graphics/SlimCamera.h>
#include <Graphics/SlimShaderParams.h>
#include <Graphics/SlimRenderTexture.h>
#include <Graphics/SlimLight.h>

class CMarchingCubesLogic : public CGameLogic {
	// Member Functions
public:
	CMarchingCubesLogic();
	~CMarchingCubesLogic();

	virtual bool Initialise() override;
	void BuildQuad();
	void BuildVolume();

	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	void BuildChunk(const CVector3& chunkPosition, int lod);
	void BuildDensities();

	virtual void HandleInput(const CInput& input, float deltaTime) override;
protected:
private:
	// Member Variables
public:
protected:
private:
	// Declarations
	CVertexDeclaration m_QuadVertexDeclaration;
	CVertexDeclaration m_VoxelVertexDeclaration;

	// Buffers
	std::shared_ptr<AVertexGpuBuffer> m_pQuadVertices;
	std::shared_ptr<AIndexGpuBuffer> m_pQuadIndices;
	std::shared_ptr<AVertexGpuBuffer> m_pVolumeVertices;
	std::shared_ptr<AIndexGpuBuffer> m_pVolumeIndices;
	std::shared_ptr<AVertexGpuBuffer> m_pSOVertices;
	std::shared_ptr<AIndexGpuBuffer> m_pSOIndices;
	
	// Shader params.
	std::shared_ptr<CShaderParams> m_pChunkParams;
	std::shared_ptr<CShaderParams> m_pLodParams;
	std::shared_ptr<CShaderParams> m_pWVPParams;
	std::shared_ptr<CShaderParams> m_pLightingParams;

	std::unique_ptr<ARenderTexture> m_pDensityRenderTarget;
	CRenderPass m_BuildDensitiesPass;
	CRenderPass m_GenerateVerticesPass;

	// Voxel dimensions.
	int m_VoxelDim;
	int m_VoxelDimPlusMargins;
	int m_VoxelMargins;
	int m_ChunkSize;

	CCamera m_Camera;
	float m_CameraYaw;
	float m_CameraPitch;
	CPoint m_LastMousePosition;

	CLight m_Light;
};

#endif // __MARCHINGCUBESLOGIC_H__