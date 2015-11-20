// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ChunkManager.cpp
// Description	: CChunkManager implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "MarchingCubesStd.h"

// Library Includes

// This Include
#include "ChunkManager.h"

// Local Includes
#include <Math\SlimAxisAlignedBoundingBox.h>
#include <Graphics\SlimRenderer.h>
#include "Tables.h"

int SmartDivision(int x, int y)
{
	if (x < 0) {
		return -static_cast<int>((-x + y - 1) / static_cast<float>(y));
	}
	return static_cast<int>(x / static_cast<float>(y));
}

int SmartModulus(int x, int y)
{
	if (x < 0) {
		int n = static_cast<int>(-x / static_cast<float>(y));
		x += y * (n + 3);
	}
	return x % y;
}

int RoundUp(float x)
{
	return static_cast<int>(std::ceilf(x));
}

std::string ToString(const CVector3& position)
{
	std::ostringstream ss;

	ss << position.GetX() << " " << position.GetY() << " " << position.GetZ();
	return ss.str();
}

const std::array<int, CChunkManager::s_NUM_LOD> CChunkManager::s_CHUNK_SIZES = { 5, 10, 20 };
const std::array<float, CChunkManager::s_NUM_LOD> CChunkManager::s_LOD_DISTANCE_BIASES = { 1.0f, 0.7f, 0.5f };

CChunkManager::CChunkManager()
	:m_FreeBufferIDs(s_NUM_BUFFERS),
	m_VoxelDim(33),
	m_VoxelMargins(4),
	m_WireFrame(false),
	m_RenderPoints(false),
	m_AmboEnabled(true),
	m_EnableShadows(true)
{
	for (size_t i = 0; i < m_FreeBufferIDs.size(); ++i) {
		m_FreeBufferIDs[i] = i;
	}
}

CChunkManager::~CChunkManager()
{

}

bool CChunkManager::Initialise()
{
	// Build vertex declarations.
	m_QuadVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_QuadVertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);

	m_DummyCornersVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT2);
	m_DummyCornersVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT2);

	m_NonEmptyCellListVertexDeclaration.AddElement("TEX", CInputElement::FORMAT_UINT);

	m_VertexListVertexDeclaration.AddElement("TEX", CInputElement::FORMAT_UINT);

	m_ChunkVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT4);
	m_ChunkVertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);

	// Create geometry.
	BuildQuad();
	BuildDummyCorners();
	BuildStreamOutputBuffers();

	CreateRenderTextures();

	// Create render passes.
	CreateBuildDensitiesPass();
	SLIM_INFO() << "Finished BuildDensitiesPass.";
	CreateListNonEmptyCellsPass();
	SLIM_INFO() << "Finished ListNonEmptyCellsPass.";
	CreateListVerticesPass();
	SLIM_INFO() << "Finished ListVerticesPass.";
	CreateSplatVertexIDsPass();
	SLIM_INFO() << "Finished SplatVertexIDsPass.";
	CreateGenerateVerticesPass();
	SLIM_INFO() << "Finished GenerateVerticesPass.";
	CreateGenerateIndicesPass();
	SLIM_INFO() << "Finished GenerateIndicesPass.";
	CreateDrawDepthPass();
	SLIM_INFO() << "Finished DrawDepthPass.";
	CreateDrawChunkPass();
	SLIM_INFO() << "Finished DrawChunkPass.";

	// Setup shader parameters.
	// Set the tables on the generate indices geometry shader.
	auto pTableBuffer = m_GenerateIndicesPass.GetGeometryShader()->GetShaderParams("CBTables");
	pTableBuffer->SetConstant("gEdgeStart", Tables::g_EDGE_START[0], Tables::GetSize(Tables::g_EDGE_START));
	pTableBuffer->SetConstant("gEdgeAxis", Tables::g_EDGE_AXIS[0], Tables::GetSize(Tables::g_EDGE_AXIS));
	pTableBuffer->SetConstant("gTriTable", Tables::g_TRI_TABLE[0], Tables::GetSize(Tables::g_TRI_TABLE));
	m_GenerateIndicesPass.GetGeometryShader()->UpdateShaderParams("CBTables", pTableBuffer);

	// Set the tables on the generate vertices vertex shader.
	pTableBuffer = m_GenerateVerticesPass.GetVertexShader()->GetShaderParams("CBTables");
	pTableBuffer->SetConstant("gEdgeStart", Tables::g_EDGE_START[0], Tables::GetSize(Tables::g_EDGE_START));
	pTableBuffer->SetConstant("gEdgeEnd", Tables::g_EDGE_END[0], Tables::GetSize(Tables::g_EDGE_END));
	m_GenerateVerticesPass.GetVertexShader()->UpdateShaderParams("CBTables", pTableBuffer);

	pTableBuffer = m_GenerateVerticesPass.GetVertexShader()->GetShaderParams("CBAmbientOcclusion");
	pTableBuffer->SetConstant("gAmboDistance", Tables::g_AMBO_DISTANCE[0], Tables::GetSize(Tables::g_AMBO_DISTANCE));
	pTableBuffer->SetConstant("gOcclusion", Tables::g_AMBO_OCCLUSION[0], Tables::GetSize(Tables::g_AMBO_OCCLUSION));
	pTableBuffer->SetConstant("gRayDirections", Tables::g_AMBO_RAY_DIRECTIONS[0], Tables::GetSize(Tables::g_AMBO_RAY_DIRECTIONS));
	m_GenerateVerticesPass.GetVertexShader()->UpdateShaderParams("CBAmbientOcclusion", pTableBuffer);

	m_pLodParams = m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBLod");
	m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBLod");
	m_BuildDensitiesPass.GetPixelShader()->GetShaderParams("CBLod");
	m_ListNonEmptyCellsPass.GetVertexShader()->GetShaderParams("CBLod");
	m_SplatVertexIDsPass.GetVertexShader()->GetShaderParams("CBLod");
	m_GenerateVerticesPass.GetVertexShader()->GetShaderParams("CBLod");
	m_GenerateIndicesPass.GetGeometryShader()->GetShaderParams("CBLod");

	m_pChunkParams = m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBChunk");
	m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBChunk");
	m_GenerateVerticesPass.GetVertexShader()->GetShaderParams("CBChunk");

	// Set the lod parameters of the chunk.
	m_pLodParams->SetConstant("gVoxelDim", static_cast<float>(m_VoxelDim));
	m_pLodParams->SetConstant("gVoxelDimMinusOne", static_cast<float>(m_VoxelDim - 1));
	m_pLodParams->SetConstant("gWVoxelSize", static_cast<float>(s_CHUNK_SIZES[0]) / static_cast<float>(m_VoxelDim - 1));
	m_pLodParams->SetConstant("gWChunkSize", static_cast<float>(s_CHUNK_SIZES[0]));
	m_pLodParams->SetConstant("gInvVoxelDim", 1.0f / static_cast<float>(m_VoxelDim));
	m_pLodParams->SetConstant("gInvVoxelDimMinusOne", 1 / static_cast<float>(m_VoxelDim - 1));
	m_pLodParams->SetConstant("gMargin", static_cast<float>(m_VoxelMargins));
	m_pLodParams->SetConstant("gVoxelDimPlusMargins", static_cast<float>(m_VoxelDim + m_VoxelMargins));
	m_pLodParams->SetConstant("gVoxelDimPlusMarginsMinusOne", static_cast<float>(m_VoxelDim + m_VoxelMargins - 1));
	m_pLodParams->SetConstant("gInvVoxelDimPlusMargins", 1.0f / static_cast<float>(m_VoxelDim + m_VoxelMargins));
	m_pLodParams->SetConstant("gInvVoxelDimPlusMarginsMinusOne", 1.0f / static_cast<float>(m_VoxelDim + m_VoxelMargins - 1));

	// Update all the CBLod buffers for shaders that use it.
	m_BuildDensitiesPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_BuildDensitiesPass.GetPixelShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_ListNonEmptyCellsPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_SplatVertexIDsPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_GenerateVerticesPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_GenerateIndicesPass.GetGeometryShader()->UpdateShaderParams("CBLod", m_pLodParams);

	m_pWVPParams = m_DrawChunkPass.GetVertexShader()->GetShaderParams("CBPerFrame");
	m_pLightingParams = m_DrawChunkPass.GetPixelShader()->GetShaderParams("CBLighting");

	if (m_AmboEnabled) {
		m_pLightingParams->SetConstant("gAmbientOcclusionInfluence", 1.0f);
	}
	else {
		m_pLightingParams->SetConstant("gAmbientOcclusionInfluence", 0.0f);
	}

	return true;
}

void CChunkManager::Update(const CCamera& camera)
{
	// Only need to update visible chunks if the camera has moved or rotated.
	if (camera.GetRotation() != m_CameraRotation || camera.GetPosition() != m_CameraPosition) {
		m_CameraRotation = camera.GetRotation();
		m_CameraPosition = camera.GetPosition();
		auto cameraDirection = m_CameraRotation.GetDirection();

		// Invalidate the visibility of all chunks as we prepare to update the visibilities.
		m_VisibleChunkList.clear();

		for (int lod = 0; lod < 1; ++lod) {
			int chunkSize = s_CHUNK_SIZES[lod];

			// Get the centre of the array of chunks for this lod that we are interested in.
			// This is so that the centre of the chunks is in front of the camera and not behind
			// where they wouldn't be seen.
			auto chunkCentre = m_CameraPosition + (cameraDirection * static_cast<float>(chunkSize * s_NUM_CHUNKS_PER_DIM) * 0.5f);

			CVector3 chunkCentreInt(
				static_cast<float>(SmartDivision(RoundUp(chunkCentre.GetX()), chunkSize)),
				static_cast<float>(SmartDivision(RoundUp(chunkCentre.GetY()), chunkSize)),
				static_cast<float>(SmartDivision(RoundUp(chunkCentre.GetZ()), chunkSize))
			);

			int ib = SmartDivision(static_cast<int>(chunkCentreInt.GetX() - s_NUM_CHUNKS_PER_DIM / 2.0f), s_NUM_CHUNKS_PER_DIM) * s_NUM_CHUNKS_PER_DIM;
			int jb = SmartDivision(static_cast<int>(chunkCentreInt.GetY() - s_NUM_CHUNKS_PER_DIM / 2.0f), s_NUM_CHUNKS_PER_DIM) * s_NUM_CHUNKS_PER_DIM;
			int kb = SmartDivision(static_cast<int>(chunkCentreInt.GetZ() - s_NUM_CHUNKS_PER_DIM / 2.0f), s_NUM_CHUNKS_PER_DIM) * s_NUM_CHUNKS_PER_DIM;

			// Loop through all the chunks in the 3D array for this lod.
 			for (int i = 0; i < s_NUM_CHUNKS_PER_DIM; ++i) {
				for (int j = 0; j < s_NUM_CHUNKS_PER_DIM; ++j) {
					for (int k = 0; k < s_NUM_CHUNKS_PER_DIM; ++k) {
						CVector3 chunkPosition = GetChunkPosition(i, j, k, chunkCentreInt, chunkSize, ib, jb, kb);

						CVector3 chunkPositionMin = chunkPosition - chunkSize * 0.5f;
						CVector3 chunkPositionCentre = chunkPosition + chunkSize * 0.5f;
						CVector3 chunkPositionMax = chunkPosition + chunkSize * 1.5f;

						// Check to see if the chunk is in the view frustum.
						//bool isInView = true;
						bool isInView = camera.IsInView(CAxisAlignedBoundingBox(chunkPositionMin, chunkPositionMax));

						auto& chunkInfo = m_Chunks[lod][i][j][k];
						if (!isInView || chunkInfo.position != chunkPosition) {
							ClearChunk(chunkInfo);
						}

						if (!isInView) {
							chunkInfo.distanceFromCameraSqr = std::numeric_limits<float>::max();
						}
						else {
							CVector3 toChunk = chunkPositionCentre - m_CameraPosition;

							chunkInfo.position = chunkPosition;
							chunkInfo.distanceFromCameraSqr = toChunk.GetLengthSquared() * s_LOD_DISTANCE_BIASES[lod];
							chunkInfo.isVisible = true;
							chunkInfo.lod = lod;

							m_VisibleChunkList.push_back(&chunkInfo);
						}
					}
				}
			}
		}

		// Sort the chunks in terms of distance from camera.
		auto lessDistanceFromCamera = [](const TChunkInfo* plhs, const TChunkInfo* prhs) {
			return plhs->distanceFromCameraSqr < prhs->distanceFromCameraSqr;
		};
		std::sort(m_VisibleChunkList.begin(), m_VisibleChunkList.end(), lessDistanceFromCamera);
	}

	BuildChunks();
}

void CChunkManager::BuildChunks()
{
	// Build chunks.
	int numChunks = m_VisibleChunkList.size();
	int numWorkThisFrame = 0;
	int lowestPriorityChunk = numChunks - 1;

	for (int i = 0; i < numChunks && numWorkThisFrame < s_MAX_WORK_PER_FRAME - s_WORK_FOR_EMPTY_CHUNK; ++i) {
		auto pChunkInfo = m_VisibleChunkList[i];

		if (!pChunkInfo || pChunkInfo->bufferID >= 0 || pChunkInfo->hasPolys == false) {
			// This chunk already has an id or has no polys, so no need to build it.
			continue;
		}

		int bufferID = -1;

		if (!m_FreeBufferIDs.empty()) {
			bufferID = m_FreeBufferIDs.back();
			m_FreeBufferIDs.pop_back();
		}
		else {
			// Out of buffers. 
			// Try find a far away chunk and steal for the higher priority chunk.
			while (lowestPriorityChunk > i) {
				auto pLowestPriorityChunkInfo = m_VisibleChunkList[lowestPriorityChunk];
				lowestPriorityChunk--;

				if (pLowestPriorityChunkInfo->bufferID >= 0) {
					bufferID = pLowestPriorityChunkInfo->bufferID;
					pLowestPriorityChunkInfo->bufferID = -1;
					break;
				}
			}

			if (bufferID == -1) {
				// Failed to get a buffer to generate chunk to.
				// Clip off the rest of chunks as they aren't built for drawing.
				m_VisibleChunkList.resize(i);
				// Stop building chunks.
				break;
			}
		}

		pChunkInfo->bufferID = bufferID;
		if (!BuildChunk(*pChunkInfo)) {
			ClearChunk(*pChunkInfo);
			pChunkInfo->hasPolys = false;
			numWorkThisFrame += s_WORK_FOR_EMPTY_CHUNK;
		}
		else {
			pChunkInfo->hasPolys = true;
			numWorkThisFrame += s_WORK_FOR_NONEMPTY_CHUNK;
		}
	}

	/*auto isNull = [](const TChunkInfo* pChunkInfo) {
		return pChunkInfo == nullptr;
	};

	auto iter = std::remove_if(m_VisibleChunkList.begin(), m_VisibleChunkList.end(), isNull);
	if (iter != m_VisibleChunkList.end()) {
		m_VisibleChunkList.erase(iter);
	}*/
}

void CChunkManager::ToggleAmbientOcclusionEnabled()
{
	m_AmboEnabled = !m_AmboEnabled;
	if (m_AmboEnabled) {
		m_pLightingParams->SetConstant("gAmbientOcclusionInfluence", 1.0f);
	}
	else {
		m_pLightingParams->SetConstant("gAmbientOcclusionInfluence", 0.0f);
	}
}

void CChunkManager::ToggleWireFrame()
{
	m_WireFrame = !m_WireFrame;
	if (m_WireFrame) {
		m_DrawChunkPass.SetFillMode(EFillMode::WIREFRAME);
	}
	else {
		m_DrawChunkPass.SetFillMode(EFillMode::SOLID);
	}
}

void CChunkManager::ToggleRenderPoints()
{
	m_RenderPoints = !m_RenderPoints;
}

void CChunkManager::ToggleShadows()
{
	m_EnableShadows = !m_EnableShadows;
}

void CChunkManager::DrawShadowMap(const CCamera& lightCamera)
{
	// Update parameters.
	m_pDrawDepthParams->SetConstant("gWorldViewProjectionMatrix", lightCamera.GetViewProjMatrix());
	m_pDrawDepthParams->SetConstant("gZFar", lightCamera.GetFarClipDistance());
	m_DrawDepthRenderPass.GetVertexShader()->UpdateShaderParams("CBPerFrame", m_pDrawDepthParams);

	auto pRenderer = g_pApp->GetRenderer();

	pRenderer->SetRenderPass(&m_DrawDepthRenderPass);

	for (auto& chunk : m_VisibleChunkList) {
		if (chunk) {
			auto bufferID = chunk->bufferID;
			if (bufferID >= 0) {
				// Draw the mesh.
				auto& pVertexBuffer = m_ChunkBuffers[bufferID].pVertexBuffer;
				auto& pIndexBuffer = m_ChunkBuffers[bufferID].pIndexBuffer;
				auto numIndices = m_ChunkBuffers[bufferID].numIndices;

				pRenderer->VRender(m_ChunkVertexDeclaration, EPrimitiveType::TRIANGLELIST, pVertexBuffer, pIndexBuffer, numIndices);
			}
		}
	}
}

void CChunkManager::DrawChunks(const CCamera& camera, const CCamera& lightCamera, const CLight& light)
{
	auto pRenderer = g_pApp->GetRenderer();

	if (m_EnableShadows) {
		DrawShadowMap(lightCamera);
	}
	else {
		// Clear the shadow map with in a really inefficient way.
		pRenderer->SetRenderPass(&m_DrawDepthRenderPass);
	}

	// Update parameters.
	m_pLightingParams->SetConstant("gLight.type", light.GetType());
	m_pLightingParams->SetConstant("gLight.diffuse", light.GetDiffuse());
	m_pLightingParams->SetConstant("gLight.specular", light.GetSpecular());
	m_pLightingParams->SetConstant("gLight.direction", light.GetRotation().GetDirection());
	m_pLightingParams->SetConstant("gLight.range", light.GetRange());
	m_pLightingParams->SetConstant("gLight.attenuation", light.GetAttenuation());
	m_pLightingParams->SetConstant("gFogStart", 60.0f);
	m_pLightingParams->SetConstant("gFogRange", 5.0f);
	m_pLightingParams->SetConstant("gFogColour", CColourValue(0.36862f, 0.36862f, 0.43137f));
	m_pLightingParams->SetConstant("gAmbientLight", CColourValue(0.5f, 0.5f, 0.5f));
	m_pLightingParams->SetConstant("gEyePosition", camera.GetPosition());
	m_DrawChunkPass.GetPixelShader()->UpdateShaderParams("CBLighting", m_pLightingParams);

	m_pWVPParams->SetConstant("gViewProjectionMatrix", camera.GetViewProjMatrix());
	m_pWVPParams->SetConstant("gLightViewProjectionMatrix", lightCamera.GetViewProjMatrix());
	m_DrawChunkPass.GetVertexShader()->UpdateShaderParams("CBPerFrame", m_pWVPParams);


	pRenderer->SetRenderPass(&m_DrawChunkPass);

	for (auto& chunk : m_VisibleChunkList) {
		if (!chunk) {
			continue;
		}

		auto bufferID = chunk->bufferID;

		if (bufferID >= 0) {
			// Not doing this at the moment as we are not using the different lods.
			//// Set the lod parameters of the chunk.
			//m_pLodParams->SetConstant("gWVoxelSize", static_cast<float>(s_CHUNK_SIZES[chunk->lod]) / static_cast<float>(m_VoxelDim - 1));
			//m_pLodParams->SetConstant("gWChunkSize", static_cast<float>(s_CHUNK_SIZES[chunk->lod]));
			//
			//// Update all the CBLod buffers for shaders that use it.
			//m_BuildDensitiesPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
			//m_BuildDensitiesPass.GetPixelShader()->UpdateShaderParams("CBLod", m_pLodParams);
			//m_ListNonEmptyCellsPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
			//m_SplatVertexIDsPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
			//m_GenerateVerticesPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
			//m_GenerateIndicesPass.GetGeometryShader()->UpdateShaderParams("CBLod", m_pLodParams);

			// Draw the mesh.
			auto& pVertexBuffer = m_ChunkBuffers[bufferID].pVertexBuffer;
			auto& pIndexBuffer = m_ChunkBuffers[bufferID].pIndexBuffer;
			auto numIndices = m_ChunkBuffers[bufferID].numIndices;

			if (!m_RenderPoints) {
				pRenderer->VRender(m_ChunkVertexDeclaration, EPrimitiveType::TRIANGLELIST, pVertexBuffer, pIndexBuffer, numIndices);
			}
			else {
				pRenderer->VRender(m_ChunkVertexDeclaration, EPrimitiveType::POINTLIST, pVertexBuffer, pIndexBuffer, numIndices);
			}
		}
	}
}

std::shared_ptr<ATexture> CChunkManager::GetShadowMap()
{
	return m_pShadowMapRenderTarget->GetTexture();
}

void CChunkManager::ClearChunk(TChunkInfo& chunkInfo)
{
	if (chunkInfo.bufferID >= 0) {
		m_FreeBufferIDs.push_back(chunkInfo.bufferID);
	}

	chunkInfo.bufferID = -1;
	chunkInfo.hasPolys = true;	// This chunk may or may not have polys.
}

bool CChunkManager::BuildChunk(TChunkInfo& chunkInfo)
{
	auto pRenderer = g_pApp->GetRenderer();

	// Update shader parameters
	m_pChunkParams->SetConstant("gWChunkPosition", chunkInfo.position);
	// Update CBChunk for the shaders that use it. 
	m_BuildDensitiesPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_BuildDensitiesPass.GetPixelShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_GenerateVerticesPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_ListNonEmptyCellsPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);

	// Set the stream output buffers.
	auto& pVertexBuffer = m_ChunkBuffers[chunkInfo.bufferID].pVertexBuffer;
	auto& pIndexBuffer = m_ChunkBuffers[chunkInfo.bufferID].pIndexBuffer;
	m_GenerateVerticesPass.ClearStreamOutputTargets();
	m_GenerateIndicesPass.ClearStreamOutputTargets();
	m_GenerateVerticesPass.AddStreamOutputTarget(pVertexBuffer);
	m_GenerateIndicesPass.AddStreamOutputTarget(pIndexBuffer);

	// Create the density volume.
	pRenderer->SetRenderPass(&m_BuildDensitiesPass);
	// Render multiple times, one for each slice of the volume texture.
	pRenderer->VRender(m_QuadVertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pQuadVertices, m_pQuadIndices, 0, m_VoxelDim + m_VoxelMargins);

	pRenderer->VBeginStreamOutQuery();
	// List non empty cells.
	pRenderer->SetRenderPass(&m_ListNonEmptyCellsPass);
	pRenderer->VRender(m_DummyCornersVertexDeclaration, EPrimitiveType::POINTLIST, m_pDummyCornersVertices, nullptr, 0, m_VoxelDim);
	auto numCells = pRenderer->VEndStreamOutQuery();

	// Early out if there's no polygons for this chunk to produce.
	if (numCells <= 0) {
		ClearChunk(chunkInfo);
		return false;
	}

	// List vertices.
	pRenderer->VBeginStreamOutQuery();
	pRenderer->SetRenderPass(&m_ListVerticesPass);
	pRenderer->VRender(m_NonEmptyCellListVertexDeclaration, EPrimitiveType::POINTLIST, m_pNonEmptyCellListVertices, nullptr, numCells);
	auto numVerts = pRenderer->VEndStreamOutQuery();

	// Splat vertex IDs.
	pRenderer->SetRenderPass(&m_SplatVertexIDsPass);
	pRenderer->VRender(m_VertexListVertexDeclaration, EPrimitiveType::POINTLIST, m_pVertexListVertices, nullptr, numVerts);

	// Generate vertices
	pRenderer->SetRenderPass(&m_GenerateVerticesPass);
	pRenderer->VRender(m_VertexListVertexDeclaration, EPrimitiveType::POINTLIST, m_pVertexListVertices, nullptr, numVerts);

	// Generate indices.
	pRenderer->VBeginStreamOutQuery();
	pRenderer->SetRenderPass(&m_GenerateIndicesPass);
	pRenderer->VRender(m_NonEmptyCellListVertexDeclaration, EPrimitiveType::POINTLIST, m_pNonEmptyCellListVertices, nullptr, numCells);
	m_ChunkBuffers[chunkInfo.bufferID].numIndices = pRenderer->VEndStreamOutQuery() * 3;

	return true;
}

void CChunkManager::BuildQuad()
{
	std::vector<TQuadVertex> vertices = {
		TQuadVertex{ CVector3(-1.0f, 1.0f, 0.5f), CVector2(0, 1) },
		TQuadVertex{ CVector3(-1.0f, -1.0f, 0.5f), CVector2(0, 0) },
		TQuadVertex{ CVector3(1.0f, 1.0f, 0.5f), CVector2(1, 1) },
		TQuadVertex{ CVector3(1.0f, -1.0f, 0.5f), CVector2(1, 0) }
	};

	std::vector<int> indices = { 0, 2, 1, 3 };

	m_pQuadVertices = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
	m_pQuadIndices = g_pApp->GetRenderer()->CreateIndexBuffer(indices);
}

void CChunkManager::BuildDummyCorners()
{
	std::vector<CVector2> writeUVs;
	std::vector<CVector2> readUVs;

	std::vector<TDummyCornerVertex> dummyCorners(m_VoxelDim * m_VoxelDim);
	for (auto i = 0U; i < m_VoxelDim; ++i) {
		for (auto j = 0U; j < m_VoxelDim; ++j) {
			dummyCorners[j + i * m_VoxelDim].writeUV = {
				i / static_cast<float>(m_VoxelDim - 1),
				j / static_cast<float>(m_VoxelDim - 1)
			};

			dummyCorners[j + i * m_VoxelDim].readUV = {
				(m_VoxelMargins + i) / static_cast<float>(m_VoxelDim + m_VoxelMargins - 1),
				(m_VoxelMargins + j) / static_cast<float>(m_VoxelDim + m_VoxelMargins - 1)
			};
		}
	}

	/*AddPointsSwizzled(writeUVs,
		0,
		m_VoxelDim - 1,
		0,
		m_VoxelDim - 1,
		m_VoxelDim,
		m_VoxelDim);
	AddPointsSwizzled(readUVs,
		m_VoxelMargins,
		m_VoxelDim + m_VoxelMargins - 1,
		m_VoxelMargins,
		m_VoxelDim + m_VoxelMargins - 1,
		m_VoxelDim + m_VoxelMargins,
		m_VoxelDim + m_VoxelMargins);

	for (size_t i = 0; i < dummyCorners.size(); ++i) {
		dummyCorners[i].writeUV = writeUVs[i];
		dummyCorners[i].readUV = readUVs[i];
	}*/

	m_pDummyCornersVertices = g_pApp->GetRenderer()->CreateVertexBuffer(dummyCorners);
}

void CChunkManager::BuildStreamOutputBuffers()
{
	m_pNonEmptyCellListVertices = g_pApp->GetRenderer()->VCreateVertexBuffer(
		64000, sizeof(int), nullptr, EGpuBufferUsage::STATIC, true);

	m_pVertexListVertices = g_pApp->GetRenderer()->VCreateVertexBuffer(
		64000, sizeof(int), nullptr, EGpuBufferUsage::STATIC, true);

	for (auto& buffer : m_ChunkBuffers) {
		buffer.pVertexBuffer = g_pApp->GetRenderer()->VCreateVertexBuffer(
			m_VoxelDim * 300, sizeof(TChunkVertex), nullptr, EGpuBufferUsage::STATIC, true);

		buffer.pIndexBuffer = g_pApp->GetRenderer()->VCreateIndexBuffer(
			m_VoxelDim * 300 * 6, AIndexGpuBuffer::INDEX_TYPE_32, nullptr, EGpuBufferUsage::STATIC, true);
	}
}

void CChunkManager::CreateRenderTextures()
{
	auto pDensityTexture = g_pApp->GetRenderer()->VCreateTexture("DensityTexture");
	pDensityTexture->SetWidth(m_VoxelDim + m_VoxelMargins);
	pDensityTexture->SetHeight(m_VoxelDim + m_VoxelMargins);
	pDensityTexture->SetDepth(m_VoxelDim + m_VoxelMargins);
	pDensityTexture->SetTextureType(ETextureType::TYPE_3D);
	pDensityTexture->SetUsage(ETextureUsage::RENDER_TARGET);
	pDensityTexture->SetPixelFormat(ETexturePixelFormat::FLOAT_32_R);
	pDensityTexture->VLoad();
	m_pDensityRenderTarget = g_pApp->GetRenderer()->VCreateRenderTexture(pDensityTexture);

	auto pVertexIDsTexture = g_pApp->GetRenderer()->VCreateTexture("VertexIDsTextures");
	pVertexIDsTexture->SetWidth(m_VoxelDim * 3);
	pVertexIDsTexture->SetHeight(m_VoxelDim);
	pVertexIDsTexture->SetDepth(m_VoxelDim);
	pVertexIDsTexture->SetTextureType(ETextureType::TYPE_3D);
	pVertexIDsTexture->SetUsage(ETextureUsage::RENDER_TARGET);
	pVertexIDsTexture->SetPixelFormat(ETexturePixelFormat::UINT_32_R);
	pVertexIDsTexture->VLoad();
	m_pVertexIDRenderTarget = g_pApp->GetRenderer()->VCreateRenderTexture(pVertexIDsTexture);

	// Create the shadow map render target.
	m_pShadowMapRenderTarget = g_pApp->GetRenderer()->VCreateRenderTexture(
		"ShadowMap", s_SHADOW_MAP_WIDTH, s_SHADOW_MAP_HEIGHT);

	m_pShadowMapRenderTarget->SetBackgroundColour(CColourValue::s_WHITE);
}

void CChunkManager::CreateBuildDensitiesPass()
{
	// Set shaders.
	m_BuildDensitiesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_BuildDensitiesPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_BuildDensitiesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	std::vector<CColourValue> randomData(s_NOISE_VOLUME_DIM * s_NOISE_VOLUME_DIM * s_NOISE_VOLUME_DIM);

	auto randomize = [](std::vector<CColourValue>& randomData) {
		for (auto& val : randomData) {
			val = { Math::Random(), Math::Random(), Math::Random(), Math::Random() };
		}
	};

	// Build the noise volumes.
	for (auto& pNoiseVolume : m_NoiseVolumes) {
		randomize(randomData);

		pNoiseVolume = g_pApp->GetRenderer()->VCreateTexture("NoiseVolume0");
		pNoiseVolume->SetPixelFormat(ETexturePixelFormat::FLOAT_32_RGBA);
		pNoiseVolume->SetTextureType(ETextureType::TYPE_3D);
		pNoiseVolume->SetDimensions(s_NOISE_VOLUME_DIM, s_NOISE_VOLUME_DIM, s_NOISE_VOLUME_DIM);
		pNoiseVolume->LoadRaw(randomData);

		m_BuildDensitiesPass.AddTextureLayer(pNoiseVolume)->SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);
	}

	m_BuildDensitiesPass.AddRenderTarget(m_pDensityRenderTarget.get());
}

void CChunkManager::CreateListNonEmptyCellsPass()
{
	// Set shaders.
	m_ListNonEmptyCellsPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("ListNonEmptyCells_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_ListNonEmptyCellsPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("ListNonEmptyCells_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	// Add texture layers.
	auto pDensityTextureLayer = m_ListNonEmptyCellsPass.AddTextureLayer();
	pDensityTextureLayer->SetTexture(m_pDensityRenderTarget->GetTexture());
	pDensityTextureLayer->SetTextureFilter(ETextureFilterType::POINT);
	pDensityTextureLayer->SetTextureAddressModes(ETextureAddressMode::CLAMP);

	// Set stream output targets.
	m_ListNonEmptyCellsPass.AddStreamOutputTarget(m_pNonEmptyCellListVertices);

	// Set states.
	m_ListNonEmptyCellsPass.SetCullingMode(ECullingMode::NONE);
	// Disable depth checks and writes for stream output only.
	m_ListNonEmptyCellsPass.SetDepthWriteEnabled(false);
	m_ListNonEmptyCellsPass.SetDepthCheckEnabled(false);
	m_ListNonEmptyCellsPass.SetColourWritesEnabled(false);
}

void CChunkManager::CreateListVerticesPass()
{
	// Set shaders.
	m_ListVerticesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("ListVertices_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_ListVerticesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("ListVertices_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	// Add texture layers.
	// No texture layers to add.

	// Set stream output targets.
	m_ListVerticesPass.AddStreamOutputTarget(m_pVertexListVertices);

	// Set states.
	m_ListVerticesPass.SetCullingMode(ECullingMode::NONE);
	// Disable depth checks and writes for stream output only.
	m_ListVerticesPass.SetDepthWriteEnabled(false);
	m_ListVerticesPass.SetDepthCheckEnabled(false);
	m_ListVerticesPass.SetColourWritesEnabled(false);
}

void CChunkManager::CreateSplatVertexIDsPass()
{
	// Set shaders.
	m_SplatVertexIDsPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("SplatVertexIDs_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_SplatVertexIDsPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("SplatVertexIDs_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));
	m_SplatVertexIDsPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("SplatVertexIDs_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));

	// Set the render target.
	m_SplatVertexIDsPass.AddRenderTarget(m_pVertexIDRenderTarget.get());

	// Add texture layers.
	// No texture layers to add.

	// Set stream output targets.
	// This pass renders to a volume instead of streaming out.

	// Set states.
	m_SplatVertexIDsPass.SetCullingMode(ECullingMode::NONE);
	// Disable depth checks and writes for 3D Volume.
	m_SplatVertexIDsPass.SetDepthWriteEnabled(false);
	m_SplatVertexIDsPass.SetDepthCheckEnabled(false);
}

void CChunkManager::CreateGenerateVerticesPass()
{
	// Set shaders.
	m_GenerateVerticesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("GenerateVertices_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_GenerateVerticesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("GenerateVertices_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	// Add texture layers.
	auto pDensityTextureLayer = m_GenerateVerticesPass.AddTextureLayer();
	pDensityTextureLayer->SetTexture(m_pDensityRenderTarget->GetTexture());
	pDensityTextureLayer->SetTextureFilter(ETextureFilterType::POINT);
	pDensityTextureLayer->SetTextureAddressModes(ETextureAddressMode::CLAMP);

	for (auto pNoiseVolume : m_NoiseVolumes) {
		m_GenerateVerticesPass.AddTextureLayer(pNoiseVolume)->SetTextureFilter(ETextureFilterType::LINEAR);
	}

	// Set stream output targets.
	// This will be done later as each chunk has vertices that are generated.

	// Set states.
	m_GenerateVerticesPass.SetCullingMode(ECullingMode::NONE);
	// Disable depth checks and writes for stream output only.
	m_GenerateVerticesPass.SetDepthWriteEnabled(false);
	m_GenerateVerticesPass.SetDepthCheckEnabled(false);
	m_GenerateVerticesPass.SetColourWritesEnabled(false);
}

void CChunkManager::CreateGenerateIndicesPass()
{
	// Set shaders.
	m_GenerateIndicesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("GenerateIndices_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_GenerateIndicesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("GenerateIndices_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	// Add texture layers.
	auto pDensityTextureLayer = m_GenerateIndicesPass.AddTextureLayer();
	pDensityTextureLayer->SetTexture(m_pVertexIDRenderTarget->GetTexture());
	pDensityTextureLayer->SetTextureFilter(ETextureFilterType::POINT);
	pDensityTextureLayer->SetTextureAddressModes(ETextureAddressMode::CLAMP);

	// Set stream output targets.
	// This will be done later as each chunk has indices that are generated.

	// Set states.
	m_GenerateIndicesPass.SetCullingMode(ECullingMode::NONE);
	// Disable depth checks and writes for stream output only.
	m_GenerateIndicesPass.SetDepthWriteEnabled(false);
	m_GenerateIndicesPass.SetDepthCheckEnabled(false);
	m_GenerateIndicesPass.SetColourWritesEnabled(false);
}

void CChunkManager::CreateDrawDepthPass()
{
	// Set shaders.
	m_DrawDepthRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("DepthOnly_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_DrawDepthRenderPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("DepthOnly_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));

	// Add texture layers.
	m_pDrawDepthParams = m_DrawDepthRenderPass.GetVertexShader()->GetShaderParams("CBPerFrame");

	// Set stream output targets.

	// Set states.
	m_DrawDepthRenderPass.AddRenderTarget(m_pShadowMapRenderTarget.get());
}

void CChunkManager::CreateDrawChunkPass()
{
	// Set shaders.
	m_DrawChunkPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("DrawChunk_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_DrawChunkPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("DrawChunk_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));

	// Add texture layers.
	auto pShadowMapLayer = m_DrawChunkPass.AddTextureLayer(m_pShadowMapRenderTarget->GetTexture());
	pShadowMapLayer->SetTextureFilter(ETextureFilterType::POINT);
	pShadowMapLayer->SetTextureAddressModes(ETextureAddressMode::BORDER);
	pShadowMapLayer->SetTextureBorderColour(CColourValue::s_WHITE);

	m_DrawChunkPass.AddTextureLayer("Textures/GrassDiffuse.png");
	m_DrawChunkPass.AddTextureLayer("Textures/StoneDiffuse.png");
	m_DrawChunkPass.AddTextureLayer("Textures/StoneDiffuse.png");
	m_DrawChunkPass.AddTextureLayer("Textures/GrassNormal.png");
	m_DrawChunkPass.AddTextureLayer("Textures/StoneNormal.png");
	m_DrawChunkPass.AddTextureLayer("Textures/StoneNormal.png");

	// Set stream output targets.
	// This the final pass it only uses output from previous passes.

	// Set states.
	// No states to set, defaults are fine.
}

CVector3 CChunkManager::GetChunkPosition(int i, int j, int k, const CVector3& centreChunkPosition, int chunkSize, int ib, int jb, int kb)
{
	static const float s_HalfNumChunkPerDim = s_NUM_CHUNKS_PER_DIM / 2.0f;
	int i1 = i + ib;
	int j1 = j + jb;
	int k1 = k + kb;

	if (i < SmartModulus(static_cast<int>(centreChunkPosition.GetX() - s_HalfNumChunkPerDim), s_NUM_CHUNKS_PER_DIM)) {
		i1 += s_NUM_CHUNKS_PER_DIM;
	}
	if (j < SmartModulus(static_cast<int>(centreChunkPosition.GetY() - s_HalfNumChunkPerDim), s_NUM_CHUNKS_PER_DIM)) {
		j1 += s_NUM_CHUNKS_PER_DIM;												
	}																			
	if (k < SmartModulus(static_cast<int>(centreChunkPosition.GetZ() - s_HalfNumChunkPerDim), s_NUM_CHUNKS_PER_DIM)) {
		k1 += s_NUM_CHUNKS_PER_DIM;
	}

	assert(SmartModulus(i1, s_NUM_CHUNKS_PER_DIM) == i);
	assert(SmartModulus(j1, s_NUM_CHUNKS_PER_DIM) == j);
	assert(SmartModulus(k1, s_NUM_CHUNKS_PER_DIM) == k);

	return CVector3(
		static_cast<float>(i1 * chunkSize),
		static_cast<float>(j1 * chunkSize),
		static_cast<float>(k1 * chunkSize));
}
