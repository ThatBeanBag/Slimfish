// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ChunkManager.h
// Description	: CChunkManager declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __CHUNKMANAGER_H__
#define __CHUNKMANAGER_H__

// Library Includes
#include <unordered_map>
#include <array>
#include <string>
#include <sstream>

// Local Includes
#include <GameBase/SlimGameLogic.h>
#include <Graphics/SlimRenderPass.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimIndexGpuBuffer.h>
#include <Graphics/SlimShaderParams.h>
#include <Graphics/SlimRenderTexture.h>
#include <Graphics/SlimCamera.h>
#include <Graphics/SlimLight.h>

// Vertex structures.
struct TVertex {
	CVector3 position;
};

struct TQuadVertex {
	CVector3 position;
	CVector2 texCoords;
};

struct TChunkVertex {
	CVector3 position;
	float ambientOcclusion;
	CVector3 normal;
};

struct TDummyCornerVertex {
	CVector2 writeUV;
	CVector2 readUV;
};

void AddPointsSwizzled(std::vector<CVector2>& points, int x0, int x1, int y0, int y1, int spanX, int spanY);

int SmartDivision(int x, int y);
int SmartModulus(int x, int y);
int RoundUp(float x);

std::string ToString(const CVector3& position);

/** Class for managing voxel chunks.
@remarks
	
*/
class CChunkManager {
	struct TChunkInfo {
		CVector3 position;
		int lod;
		int bufferID = -1;
		float distanceFromCameraSqr;
		bool hasPolys;
		bool isVisible;
	};

	struct TChunkMesh {
		std::shared_ptr<AVertexGpuBuffer> pVertexBuffer;
		std::shared_ptr<AIndexGpuBuffer> pIndexBuffer;
		size_t numIndices;
	};

	using TChunkMap = std::unordered_map<std::string, std::unique_ptr<TChunkInfo>>;

	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CChunkManager();
	
	/** Destructor
		@author Hayden Asplet
	*/
	~CChunkManager();

	/** Initialise the chunk manager.
	 	@author Hayden Asplet
	 	@return True if the manager initailised successfully.
	*/
	bool Initialise();

	/** Update the chunk manager.
	 	@author Hayden Asplet
	 	@param camera Camera used for rendering.
	*/
	void Update(const CCamera& camera);

	/** Toggle ambient occlusion enabled.
	 	@author Hayden Asplet
	*/
	void ToggleAmbientOcclusionEnabled();

	/** Toggle wire frame mode for drawing the chunks.
		@author Hayden Asplet
	*/
	void ToggleWireFrame();

	/** Toggle rendering th chunks as just the vertex points.
		@author Hayden Asplet
	*/
	void ToggleRenderPoints();

	/** Toggle shadows enabled.
	 	@author Hayden Asplet
	*/
	void ToggleShadows();

	/** Draw all the currently loaded chunks.
	 	@author Hayden Asplet
	 	@param camera Camera to render all the chunks in view of.
	 	@param lightCamera The light camera for shadow mapping.
	 	@param light The light information for lighting the chunks.
	*/
	void DrawChunks(const CCamera& camera, const CCamera& lightCamera, const CLight& light);

	std::shared_ptr<ATexture> GetShadowMap();
protected:
private:
	/** Internal method for drawing the chunks to the shadow map.
		@author Hayden Asplet
		@param lightCamera The light's camera to project the chunks by.
	*/
	void DrawShadowMap(const CCamera& lightCamera);

	/** Internal method for building the chunks that need loading.
	 	@author Hayden Asplet
	*/
	void BuildChunks();

	/** Clear a chunk.
	 	@author Hayden Asplet
	*/
	void ClearChunk(TChunkInfo& chunkInfo);

	/** Build a single chunk.
	 	@author Hayden Asplet
	 	@param chunkInfo chunkInfo thats used to build the chunk. Also gets populated with loaded information.
	 	@return True if the chunk has polygons.
	*/
	bool BuildChunk(TChunkInfo& chunkInfo);

	// Initialization stuff for rendering and building chunks.
	// Methods for building static meshes.
	void BuildQuad();
	void BuildDummyCorners();
	void BuildStreamOutputBuffers();

	// Methods for creating the render passes to build and draw chunks.
	/** Create render textures, density volume, vertex ID volume and shadow map.
	 	@author Hayden Asplet
	*/
	void CreateRenderTextures();
	/** Create the render pass, which builds the density volume.
		@author Hayden Asplet
	*/
	void CreateBuildDensitiesPass();
	/**Create the render pass, which lists all the non empty cells.
		@author Hayden Asplet
	*/
	void CreateListNonEmptyCellsPass();	
	/** Create the render pass, which lists all vertices on the edges 3, 0 and 8.
		@author Hayden Asplet
	*/
	void CreateListVerticesPass();
	/** Create the render pass, which splats the vertex IDs onto the vertex ID volume.
		@author Hayden Asplet
	*/
	void CreateSplatVertexIDsPass();
	/** Create the render pass, which steams out the generated vertices of a chunk.
		@author Hayden Asplet
	*/
	void CreateGenerateVerticesPass();
	/** Create the render pass, which steams out the generated indices of a chunk.
		@author Hayden Asplet
	*/
	void CreateGenerateIndicesPass();
	/** Create the render pass, which draws a chunk's depth information from the perspective of a light.
		@author Hayden Asplet
	*/
	void CreateDrawDepthPass();	
	/** Create the render pass, which draws a chunk.
		@author Hayden Asplet
	*/
	void CreateDrawChunkPass();

	/** Internal convenience with for getting a work space chunk position based on i, j, k indices.
	 	@author Hayden Asplet
	 	@param centreChunkPosition Centre position for all the chunks being propagated.
	 	@param chunkSize The size of the chunk
	 	@return World space position of the bottom left corner of the chunk.
	*/
	CVector3 GetChunkPosition(int i, int j, int k, const CVector3& centreChunkPosition, int chunkSize, int ib, int jb, int kb);

	// Member Variables
public:
	static const int s_NUM_BUFFERS = 500;
	static const int s_NUM_LOD = 3;
	static const int s_NUM_CHUNKS_PER_DIM = 21;
	static const int s_MAX_WORK_PER_FRAME = 41;
	static const int s_WORK_FOR_EMPTY_CHUNK = 1;
	static const int s_WORK_FOR_NONEMPTY_CHUNK = 4;
	static const std::array<int, s_NUM_LOD> s_CHUNK_SIZES;
	static const std::array<float, s_NUM_LOD> s_LOD_DISTANCE_BIASES;

	static const size_t s_SHADOW_MAP_WIDTH = 4096;
	static const size_t s_SHADOW_MAP_HEIGHT = 4096;

	static const size_t s_NOISE_VOLUME_DIM = 16;
	static const size_t s_NUM_NOISE_VOLUMES = 4;

	using TChunks = std::array < std::array < std::array <
		std::array<TChunkInfo, s_NUM_CHUNKS_PER_DIM>,
		s_NUM_CHUNKS_PER_DIM > ,
		s_NUM_CHUNKS_PER_DIM > ,
		s_NUM_LOD > ;

protected:
private:
	size_t m_VoxelDim;
	size_t m_VoxelMargins;

	CVector3 m_CameraPosition;
	CQuaternion m_CameraRotation;

	// The pool of buffers that store the meshes 
	std::array<TChunkMesh, s_NUM_BUFFERS> m_ChunkBuffers;
	TChunks m_Chunks;
	std::vector<TChunkInfo*> m_VisibleChunkList;

	// Manages the list of buffers that are free.
	std::vector<size_t> m_FreeBufferIDs;

	//
	// Rendering and building of chunks passes.
	//

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

	// Shader params.
	std::shared_ptr<CShaderParams> m_pChunkParams;
	std::shared_ptr<CShaderParams> m_pLodParams;
	std::shared_ptr<CShaderParams> m_pWVPParams;
	std::shared_ptr<CShaderParams> m_pLightingParams;
	std::shared_ptr<CShaderParams> m_pDrawDepthParams;

	// Render targets.
	std::unique_ptr<ARenderTexture> m_pDensityRenderTarget;
	std::unique_ptr<ARenderTexture> m_pVertexIDRenderTarget;
	std::unique_ptr<ARenderTexture> m_pShadowMapRenderTarget;

	// Textures
	std::array<std::shared_ptr<ATexture>, s_NUM_NOISE_VOLUMES> m_NoiseVolumes;

	// Render passes.
	CRenderPass m_BuildDensitiesPass;
	CRenderPass m_ListNonEmptyCellsPass;
	CRenderPass m_ListVerticesPass;
	CRenderPass m_SplatVertexIDsPass;
	CRenderPass m_GenerateVerticesPass;
	CRenderPass m_GenerateIndicesPass;
	CRenderPass m_DrawDepthRenderPass;
	CRenderPass m_DrawChunkPass;

	// Flags for different ways to draw.
	bool m_RenderPoints;
	bool m_AmboEnabled;
	bool m_WireFrame;
	bool m_EnableShadows;
};

#endif	// __CHUNKMANAGER_H__