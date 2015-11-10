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
#include <array>

// Local Includes
#include <GameBase/SlimGameLogic.h>
#include <Graphics/SlimRenderPass.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimIndexGpuBuffer.h>
#include <Graphics/SlimCamera.h>
#include <Graphics/SlimShaderParams.h>
#include <Graphics/SlimRenderTexture.h>
#include <Graphics/SlimLight.h>

/**  Small meta program for making multi-dimensional std::arrays a little more readable.
@remarks
	Declare an array like so: TMultiDimArray<int, 50, 30, 50> ints;
	This creates a 50 x 30 x 50 array of integers.
*/

/*
template<typename T, size_t I, size_t... J>
struct TGetMultiDimArray {
	using TNested = typename TGetMultiDimArray<T, J...>::TType;
	using TType = std::array<TNested, I>; 
};

template<typename T, size_t I>
struct TGetMultiDimArray {
	using TType = std::array <T, I>;
};

template<typename T, size_t I, size_t... J>
using TMultiDimArray = typename TGetMultiDimArray<T, I, J...>::TType;*/

class CMarchingCubesLogic : public CGameLogic {
	struct TChunkInfo {
		CVector3 position;
		int bufferID;
		float distanceFromCameraSqr;
		bool hasPolys;
		int lod;
		size_t numIndices;
	};

	// Member Functions
public:
	CMarchingCubesLogic();
	~CMarchingCubesLogic();

	virtual bool Initialise() override;

	void BuildQuad();
	void BuildDummyCorners();
	void BuildStreamOutputBuffers();
	void BuildDummyPoints();

	void CreateRenderTextures();
	void CreateBuildDensitiesPass();
	void CreateListNonEmptyCellsPass();
	void CreateListVerticesPass();
	void CreateSplatVertexIDsPass();
	void CreateGenerateVerticesPass();
	void CreateGenerateIndicesPass();
	void CreateDrawChunkPass();
	void CreateMarchingCubesPass();

	virtual void Update(float deltaTime) override;

	virtual void Render() override;
	void DrawChunks();
	void DrawTestChunks();
	size_t BuildChunk(const CVector3& chunkPosition, size_t lod, size_t chunkBufferID);
	size_t BuildTestChunk(const CVector3& chunkPosition, size_t lod, size_t chunkBufferID);

	virtual void HandleInput(const CInput& input, float deltaTime) override;

	void ClearChunk(TChunkInfo& chunk);
	CVector3 GetChunkPosition(int i, int j, int k, const CVector3& centreChunkPosition, int chunkSize, int ib, int jb, int kb);
protected:
private:
	// Member Variables
public:
	static const int s_NUM_BUFFERS = 300;
	static const int s_NUM_LOD = 3;
	static const int s_NUM_CHUNKS_PER_DIM = 13;

	using TChunks = std::array<std::array<std::array<
		std::array<TChunkInfo, s_NUM_CHUNKS_PER_DIM>, 
		s_NUM_CHUNKS_PER_DIM>, 
		s_NUM_CHUNKS_PER_DIM>, 
		s_NUM_LOD>;

	/*using TChunks = TMultiDimArray<TChunkInfo,
		s_NUM_LOD,
		s_NUM_CHUNKS_PER_DIM,
		s_NUM_CHUNKS_PER_DIM,
		s_NUM_CHUNKS_PER_DIM>;*/
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

	std::array<std::shared_ptr<AVertexGpuBuffer>, s_NUM_BUFFERS> m_ChunkVertexBuffers;
	std::array<std::shared_ptr<AIndexGpuBuffer>, s_NUM_BUFFERS> m_ChunkIndexBuffers;
	
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

	// Temp test stuff.
	CVertexDeclaration m_MarchingCubesVertexDeclaration;
	CRenderPass m_MarchingCubesPass;
	std::shared_ptr<AVertexGpuBuffer> m_pDummyPoints;

	// Voxel dimensions.
	int m_VoxelDim;
	int m_VoxelDimPlusMargins;
	int m_VoxelMargins;
	std::array<int, s_NUM_LOD> m_ChunkSizes;

	TChunks m_Chunks;
	std::vector<int> m_FreeBufferIDs;

	std::vector<TChunkInfo*> m_SortedChunks;

	CCamera m_Camera;
	float m_CameraYaw;
	float m_CameraPitch;
	CPoint m_LastMousePosition;

	CLight m_Light;

	bool m_bRenderPoints;
	bool m_bDrawOnlyOneChunk;
	CVector3 m_TestChunkPosition;
};

#endif // __MARCHINGCUBESLOGIC_H__