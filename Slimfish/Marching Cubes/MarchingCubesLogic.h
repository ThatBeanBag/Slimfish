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
	void BuildDummyCorners();
	void CreateRenderTextures();
	void CreateBuildDensitiesPass();
	void CreateListNonEmptyCellsPass();
	void CreateListVerticesPass();
	void CreateSplatVertexIDsPass();
	void CreateGenerateVerticesPass();
	void CreateGenerateIndicesPass();
	void CreateDrawChunkPass();

	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	void BuildChunk(const CVector3& chunkPosition, size_t lod, size_t chunkBufferID);
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
	CVertexDeclaration m_DummyCornersVertexDeclaration;
	CVertexDeclaration m_NonEmptyCellListVertexDeclaration;
	CVertexDeclaration m_VertexListVertexDeclaration;
	CVertexDeclaration m_ChunkVertexDeclaration;

	// Buffers
	// For building densities.
	std::shared_ptr<AVertexGpuBuffer> m_pQuadVertices;
	std::shared_ptr<AIndexGpuBuffer> m_pQuadIndices;

	std::shared_ptr<AVertexGpuBuffer> m_pDummyCornersVertices;
	std::shared_ptr<AVertexGpuBuffer> m_pNonEmptyCellListVertices;	// Stream out from list non-empty cells
	std::shared_ptr<AVertexGpuBuffer> m_pVertexListVertices;	// Stream out from list vertices.

	std::vector<std::shared_ptr<AVertexGpuBuffer>> m_pChunkVertexBuffers;
	std::vector<std::shared_ptr<AIndexGpuBuffer>> m_pChunkIndexBuffers;
	
	// Shader params.
	std::shared_ptr<CShaderParams> m_pChunkParams;
	std::shared_ptr<CShaderParams> m_pLodParams;
	std::shared_ptr<CShaderParams> m_pWVPParams;
	std::shared_ptr<CShaderParams> m_pLightingParams;

	// Render targets.
	std::unique_ptr<ARenderTexture> m_pDensityRenderTarget;
	std::unique_ptr<ARenderTexture> m_pVertexIDRenderTarget;

	// Render passes.
	CRenderPass m_BuildDensitiesPass;
	CRenderPass m_ListNonEmptyCellsPass;
	CRenderPass m_ListVerticesPass;
	CRenderPass m_SplatVertexIDsPass;
	CRenderPass m_GenerateVerticesPass;
	CRenderPass m_GenerateIndicesPass;
	CRenderPass m_DrawChunkPass;

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