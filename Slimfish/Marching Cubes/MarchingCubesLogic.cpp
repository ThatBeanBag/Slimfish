// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MarchingCubesLogic.cpp
// Description	: CMarchingCubesLogic implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "MarchingCubesStd.h"

// Library Includes

// This Include
#include "MarchingCubesLogic.h"

// Local Includes
#include <Graphics/SlimRenderer.h>
#include <Graphics/SlimGpuBuffer.h>
#include <Math/SlimAxisAlignedBoundingBox.h>
#include "Tables.h"

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

int SmartDivision(int x, int y) 
{
	if (x < 0) {
		return static_cast<int>(-((-x + y - 1)) / static_cast<float>(y));
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
	if (x > 0) {
		return static_cast<int>(std::ceilf(x));
	}
	else if (x < 0) {
		return -static_cast<int>(std::floorf(-x));
	}
	return 0;
}

void AddPointsSwizzled(std::vector<CVector2>& points, int x0, int x1, int y0, int y1, int spanX, int spanY)
{
	int xm = static_cast<int>((x0 + x1) / 2.0f);
	int ym = static_cast<int>((y0 + y1) / 2.0f);
	if (x0 == x1) {
		if (y0 == y1) {
			float x = x0 / static_cast<float>(spanX - 1);
			float y = y0 / static_cast<float>(spanY - 1);
			points.push_back(CVector2(x, y));
		}
		else { // 1 x spanY.
			AddPointsSwizzled(points, x0, x1, y0, ym, spanX, spanY);
			AddPointsSwizzled(points, x0, x1, ym + 1, y1, spanX, spanY);
		}
	}
	else if (y0 == y1) {	// spanX x 1.
		AddPointsSwizzled(points, x0, xm, y0, y1, spanX, spanY);
		AddPointsSwizzled(points, xm + 1, x1, y0, y1, spanX, spanY);
	}
	else {	// 2 x 2 or greater.
		AddPointsSwizzled(points, x0, xm, y0, ym, spanX, spanY);
		AddPointsSwizzled(points, xm + 1, x1, y0, ym, spanX, spanY);
		AddPointsSwizzled(points, x0, xm, ym + 1, y1, spanX, spanY);
		AddPointsSwizzled(points, xm + 1, x1, ym + 1, y1, spanX, spanY);
	}
}

CMarchingCubesLogic::CMarchingCubesLogic()
	:m_Camera(nullptr),
	m_Light(nullptr),
	m_VoxelDim(33),
	m_VoxelMargins(4),
	m_FreeBufferIDs(s_NUM_BUFFERS),
	m_bDrawOnlyOneChunk(true)
{
	for (size_t i = 0; i < m_FreeBufferIDs.size(); ++i) {
		m_FreeBufferIDs[i] = i;
	}

	m_VoxelDimPlusMargins = m_VoxelDim + m_VoxelMargins;
	m_ChunkSizes[0] = 4;
	m_ChunkSizes[1] = 10;
	m_ChunkSizes[2] = 32;

	for (int lod = 0; lod < s_NUM_LOD; ++lod) {
		for (int i = 0; i < s_NUM_CHUNKS_PER_DIM; ++i) {
			for (int j = 0; j < s_NUM_CHUNKS_PER_DIM; ++j) {
				for (int k = 0; k < s_NUM_CHUNKS_PER_DIM; ++k) {
					auto& chunk = m_Chunks[lod][i][j][k];
					chunk.lod = lod;
					chunk.bufferID = -1;
					ClearChunk(chunk);
				}
			}
		}
	}

	m_Light.SetType(LIGHT_DIRECTIONAL);
	m_Light.SetDiffuse(CColourValue(0.7f, 0.7f, 0.7f));
	m_Light.SetSpecular(CColourValue(1.0f, 1.0f, 1.0f));
	m_Light.SetRotation(CQuaternion(Math::DegreesToRadians(45), Math::DegreesToRadians(-45), 0));
}

CMarchingCubesLogic::~CMarchingCubesLogic()
{

}

bool CMarchingCubesLogic::Initialise()
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

	m_MarchingCubesVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);

	// Create geometry.
	BuildQuad();
	BuildDummyCorners();
	BuildStreamOutputBuffers();
	BuildDummyPoints();

	CreateRenderTextures();

	// Create render passes.
	CreateBuildDensitiesPass();
	CreateListNonEmptyCellsPass();
	CreateListVerticesPass();
	CreateSplatVertexIDsPass();
	CreateGenerateVerticesPass();
	CreateGenerateIndicesPass();
	CreateDrawChunkPass();
	CreateMarchingCubesPass(); // TODO: Remove this.

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

	pTableBuffer = m_MarchingCubesPass.GetGeometryShader()->GetShaderParams("CBTriTable");
	pTableBuffer->SetConstant("gTriTable", Tables::g_TRI_TABLE[0], Tables::GetSize(Tables::g_TRI_TABLE));
	m_MarchingCubesPass.GetGeometryShader()->UpdateShaderParams("CBTriTable", pTableBuffer);

	m_pLodParams = m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBLod");
	m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBLod");
	m_BuildDensitiesPass.GetPixelShader()->GetShaderParams("CBLod");
	m_ListNonEmptyCellsPass.GetVertexShader()->GetShaderParams("CBLod");
	m_SplatVertexIDsPass.GetVertexShader()->GetShaderParams("CBLod");
	m_GenerateVerticesPass.GetVertexShader()->GetShaderParams("CBLod");
	m_GenerateIndicesPass.GetGeometryShader()->GetShaderParams("CBLod");
	m_MarchingCubesPass.GetGeometryShader()->GetShaderParams("CBLod");

	m_pChunkParams = m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBChunk");
	m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBChunk");
	m_GenerateVerticesPass.GetVertexShader()->GetShaderParams("CBChunk");
	m_MarchingCubesPass.GetGeometryShader()->GetShaderParams("CBChunk");

	// Set the lod parameters of the chunk.
	m_pLodParams->SetConstant("gVoxelDim", static_cast<float>(m_VoxelDim));
	m_pLodParams->SetConstant("gVoxelDimMinusOne", static_cast<float>(m_VoxelDim - 1));
	m_pLodParams->SetConstant("gWVoxelSize", static_cast<float>(m_ChunkSizes[0]) / static_cast<float>(m_VoxelDim - 1));
	m_pLodParams->SetConstant("gWChunkSize", static_cast<float>(m_ChunkSizes[0]));
	m_pLodParams->SetConstant("gInvVoxelDim", 1.0f / static_cast<float>(m_VoxelDim));
	m_pLodParams->SetConstant("gInvVoxelDimMinusOne", 1 / static_cast<float>(m_VoxelDim - 1));
	m_pLodParams->SetConstant("gMargin", static_cast<float>(m_VoxelMargins));
	m_pLodParams->SetConstant("gVoxelDimPlusMargins", static_cast<float>(m_VoxelDimPlusMargins));
	m_pLodParams->SetConstant("gVoxelDimPlusMarginsMinusOne", static_cast<float>(m_VoxelDimPlusMargins - 1));
	m_pLodParams->SetConstant("gInvVoxelDimPlusMargins", 1.0f / static_cast<float>(m_VoxelDimPlusMargins));
	m_pLodParams->SetConstant("gInvVoxelDimPlusMarginsMinusOne", 1.0f / static_cast<float>(m_VoxelDimPlusMargins - 1));

	// Update all the CBLod buffers for shaders that use it.
	m_BuildDensitiesPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_BuildDensitiesPass.GetPixelShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_ListNonEmptyCellsPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_SplatVertexIDsPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_GenerateVerticesPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_GenerateIndicesPass.GetGeometryShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_MarchingCubesPass.GetGeometryShader()->UpdateShaderParams("CBLod", m_pLodParams);

	m_pWVPParams = m_DrawChunkPass.GetVertexShader()->GetShaderParams("CBPerFrame");
	m_pLightingParams = m_DrawChunkPass.GetPixelShader()->GetShaderParams("CBLighting");

	g_pApp->GetRenderer()->SetBackgroundColour(CColourValue(0.8f, 0.8f, 0.8f));

	m_pLightingParams->SetConstant("gLight.type", m_Light.GetType());
	m_pLightingParams->SetConstant("gLight.diffuse", m_Light.GetDiffuse());
	m_pLightingParams->SetConstant("gLight.specular", m_Light.GetSpecular());
	m_pLightingParams->SetConstant("gLight.direction", m_Light.GetRotation().GetDirection());
	m_pLightingParams->SetConstant("gLight.range", m_Light.GetRange());
	m_pLightingParams->SetConstant("gLight.attenuation", m_Light.GetAttenuation());
	m_pLightingParams->SetConstant("gFogStart", 20.0f);
	m_pLightingParams->SetConstant("gFogRange", 5.0f);
	m_pLightingParams->SetConstant("gFogColour", g_pApp->GetRenderer()->GetBackgroundColour());
	m_pLightingParams->SetConstant("gAmbientLight", CColourValue(0.3f, 0.3f, 0.3f));
	m_DrawChunkPass.GetPixelShader()->UpdateShaderParams("CBLighting", m_pLightingParams);

	// Setup camera.
	m_Camera.SetPosition(CVector3(0.0f, 1.0f, 1.0f));
	m_Camera.SetPerspective(Math::DegreesToRadians(60.0f), 1.0f, 0.1f, 100.0f);

	return true;
}

void CMarchingCubesLogic::BuildQuad()
{
	std::vector<TQuadVertex> vertices = {
		TQuadVertex{ CVector3(-1.0f, 1.0f, 0.5f),	CVector2(0, 1) },
		TQuadVertex{ CVector3(-1.0f, -1.0f, 0.5f),	CVector2(0, 0) },
		TQuadVertex{ CVector3(1.0f, 1.0f, 0.5f),	CVector2(1, 1) },
		TQuadVertex{ CVector3(1.0f, -1.0f, 0.5f),	CVector2(1, 0) }
	};

	std::vector<int> indices = { 0, 2, 1, 3 };

	m_pQuadVertices = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
	m_pQuadIndices = g_pApp->GetRenderer()->CreateIndexBuffer(indices);
}

void CMarchingCubesLogic::BuildDummyCorners()
{
	std::vector<CVector2> writeUVs;
	std::vector<CVector2> readUVs;

	AddPointsSwizzled(writeUVs,
		0, 
		m_VoxelDim - 1, 
		0, 
		m_VoxelDim - 1, 
		m_VoxelDim, 
		m_VoxelDim);
	AddPointsSwizzled(readUVs, 
		m_VoxelMargins, 
		m_VoxelDimPlusMargins - 1, 
		m_VoxelMargins, 
		m_VoxelDimPlusMargins - 1, 
		m_VoxelDim + (m_VoxelMargins * 2), 
		m_VoxelDim + (m_VoxelMargins * 2));

	std::vector<TDummyCornerVertex> dummyCorners(writeUVs.size());
	for (size_t i = 0; i < dummyCorners.size(); ++i) {
		dummyCorners[i].writeUV = writeUVs[i];
		dummyCorners[i].readUV = readUVs[i];
	}

	m_pDummyCornersVertices = g_pApp->GetRenderer()->CreateVertexBuffer(dummyCorners);
}

void CMarchingCubesLogic::BuildStreamOutputBuffers()
{
	m_pNonEmptyCellListVertices = g_pApp->GetRenderer()->VCreateVertexBuffer(
		64000, sizeof(int), nullptr, EGpuBufferUsage::STATIC, true);

	m_pVertexListVertices = g_pApp->GetRenderer()->VCreateVertexBuffer(
		64000, sizeof(int), nullptr, EGpuBufferUsage::STATIC, true);

	for (size_t i = 0; i < s_NUM_BUFFERS; ++i) {
		m_ChunkVertexBuffers[i] = g_pApp->GetRenderer()->VCreateVertexBuffer(
			m_VoxelDim * 1000, sizeof(TChunkVertex), nullptr, EGpuBufferUsage::STATIC, true);

		m_ChunkIndexBuffers[i] = g_pApp->GetRenderer()->VCreateIndexBuffer(
			m_VoxelDim * 100 * 6, AIndexGpuBuffer::INDEX_TYPE_32, nullptr, EGpuBufferUsage::STATIC, true);
	}
}

void CMarchingCubesLogic::BuildDummyPoints()
{
	std::vector<CVector3> vertices;
	for (auto i = 0; i < m_VoxelDim; ++i) {
		for (auto j = 0; j < m_VoxelDim; ++j) {
			for (auto k = 0; k < m_VoxelDim; ++k) {
				vertices.emplace_back(
					(i / static_cast<float>(m_VoxelDim - 1)),
					(j / static_cast<float>(m_VoxelDim - 1)),
					(k / static_cast<float>(m_VoxelDim - 1)));
			}
		}
	}

	m_pDummyPoints = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
}

void CMarchingCubesLogic::CreateRenderTextures()
{
	auto pDensityTexture = g_pApp->GetRenderer()->VCreateTexture("DensityTexture");
	pDensityTexture->SetWidth(m_VoxelDim);
	pDensityTexture->SetHeight(m_VoxelDim);
	pDensityTexture->SetDepth(m_VoxelDim);
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
}

void CMarchingCubesLogic::CreateBuildDensitiesPass()
{
	m_BuildDensitiesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_BuildDensitiesPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_BuildDensitiesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	std::vector<CColourValue> randomData(16 * 16 * 16);

	auto randomize = [](std::vector<CColourValue>& randomData) {
		for (auto& val : randomData) {
			val = {
				Math::Random(),
				Math::Random(),
				Math::Random(),
				Math::Random() 
			};
		}
	};

	randomize(randomData);

	auto pNoiseTexture0 = g_pApp->GetRenderer()->VCreateTexture("NoiseVolume0");
	pNoiseTexture0->SetPixelFormat(ETexturePixelFormat::FLOAT_32_RGBA);
	pNoiseTexture0->SetWidth(16);
	pNoiseTexture0->SetDepth(16);
	pNoiseTexture0->SetHeight(16);
	pNoiseTexture0->SetTextureType(ETextureType::TYPE_3D);
	pNoiseTexture0->LoadRaw(randomData);

	randomize(randomData);

	auto pNoiseTexture1 = g_pApp->GetRenderer()->VCreateTexture("NoiseVolume1");
	pNoiseTexture1->SetPixelFormat(ETexturePixelFormat::FLOAT_32_RGBA);
	pNoiseTexture1->SetWidth(16);
	pNoiseTexture1->SetDepth(16);
	pNoiseTexture1->SetHeight(16);
	pNoiseTexture1->SetTextureType(ETextureType::TYPE_3D);
	pNoiseTexture1->LoadRaw(randomData);

	randomize(randomData);

	auto pNoiseTexture2 = g_pApp->GetRenderer()->VCreateTexture("NoiseVolume2");
	pNoiseTexture2->SetPixelFormat(ETexturePixelFormat::FLOAT_32_RGBA);
	pNoiseTexture2->SetWidth(16);
	pNoiseTexture2->SetDepth(16);
	pNoiseTexture2->SetHeight(16);
	pNoiseTexture2->SetTextureType(ETextureType::TYPE_3D);
	pNoiseTexture2->LoadRaw(randomData);

	randomize(randomData);

	auto pNoiseTexture3 = g_pApp->GetRenderer()->VCreateTexture("NoiseVolume3");
	pNoiseTexture3->SetPixelFormat(ETexturePixelFormat::FLOAT_32_RGBA);
	pNoiseTexture3->SetWidth(16);
	pNoiseTexture3->SetDepth(16);
	pNoiseTexture3->SetHeight(16);
	pNoiseTexture3->SetTextureType(ETextureType::TYPE_3D);
	pNoiseTexture3->LoadRaw(randomData);

	m_BuildDensitiesPass.AddTextureLayer(pNoiseTexture0)->SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);
	m_BuildDensitiesPass.AddTextureLayer(pNoiseTexture1)->SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);
	m_BuildDensitiesPass.AddTextureLayer(pNoiseTexture2)->SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);
	m_BuildDensitiesPass.AddTextureLayer(pNoiseTexture3)->SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);

	m_BuildDensitiesPass.AddRenderTarget(m_pDensityRenderTarget.get());
}

void CMarchingCubesLogic::CreateListNonEmptyCellsPass()
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

void CMarchingCubesLogic::CreateListVerticesPass()
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

void CMarchingCubesLogic::CreateSplatVertexIDsPass()
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

void CMarchingCubesLogic::CreateGenerateVerticesPass()
{
	// Set shaders.
	m_GenerateVerticesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("GenerateVertices_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_GenerateVerticesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("GenerateVertices_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	// Add texture layers.
	auto pDensityTextureLayer = m_GenerateVerticesPass.AddTextureLayer();
	pDensityTextureLayer->SetTexture(m_pDensityRenderTarget->GetTexture());
	pDensityTextureLayer->SetTextureFilter(ETextureFilterType::POINT);
	pDensityTextureLayer->SetTextureAddressModes(ETextureAddressMode::CLAMP);

	// Set stream output targets.
	// This will be done later as each chunk has vertices that are generated.

	// Set states.
	m_GenerateVerticesPass.SetCullingMode(ECullingMode::NONE);
	// Disable depth checks and writes for stream output only.
	m_GenerateVerticesPass.SetDepthWriteEnabled(false);
	m_GenerateVerticesPass.SetDepthCheckEnabled(false);
	m_GenerateVerticesPass.SetColourWritesEnabled(false);
}

void CMarchingCubesLogic::CreateGenerateIndicesPass()
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

void CMarchingCubesLogic::CreateDrawChunkPass()
{
	// Set shaders.
	m_DrawChunkPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("DrawChunk_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_DrawChunkPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("DrawChunk_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));

	// Add texture layers.

	// Set stream output targets.
	// This the final pass it only uses output from previous passes.

	// Set states.
	// No states to set, defaults are fine.
	m_DrawChunkPass.SetCullingMode(ECullingMode::NONE);
}

void CMarchingCubesLogic::CreateMarchingCubesPass()
{
	// Set shaders.
	m_MarchingCubesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("MarchingCubes_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_MarchingCubesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("MarchingCubes_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	// Add texture layers.
	auto pDensityTextureLayer = m_MarchingCubesPass.AddTextureLayer(m_pDensityRenderTarget->GetTexture());
	pDensityTextureLayer->SetTextureFilter(ETextureFilterType::POINT);
	pDensityTextureLayer->SetTextureAddressModes(ETextureAddressMode::CLAMP);
	//pDensityTextureLayer->SetTextureAddressModes(ETextureAddressMode::BORDER);
	//pDensityTextureLayer->SetTextureBorderColour(CColourValue(1.0f, 0.0f, 0.0f));
	// No texture layers to add.

	// Set stream output targets.
	// This gets set later.

	// Set states.
	m_MarchingCubesPass.SetDepthWriteEnabled(false);
	m_MarchingCubesPass.SetDepthCheckEnabled(false);
	m_MarchingCubesPass.SetColourWritesEnabled(false);
}

void CMarchingCubesLogic::Update(float deltaTime)
{
	CPoint screenSize = g_pApp->GetRenderer()->GetWindowSize();
	m_Camera.SetAspectRatio(static_cast<float>(screenSize.GetX()) / static_cast<float>(screenSize.GetY()));
}

void CMarchingCubesLogic::Render()
{
	// Update camera.
	m_Camera.UpdateViewTransform();

	// Update the view project matrix in the draw shaders as the camera may have changed.
	m_pWVPParams->SetConstant("gViewProjectionMatrix", m_Camera.GetViewProjMatrix());
	m_DrawChunkPass.GetVertexShader()->UpdateShaderParams("CBPerFrame", m_pWVPParams);

	m_pLightingParams->SetConstant("gEyePosition", m_Camera.GetPosition());
	m_DrawChunkPass.GetPixelShader()->UpdateShaderParams("CBLighting", m_pLightingParams);

	if (m_bDrawOnlyOneChunk) {
		/*auto numIndices = BuildChunk(m_TestChunkPosition, 0, 0);
		g_pApp->GetRenderer()->SetRenderPass(&m_DrawChunkPass);
		if (m_bRenderPoints) {
			g_pApp->GetRenderer()->VRender(m_ChunkVertexDeclaration, EPrimitiveType::TRIANGLELIST, m_ChunkVertexBuffers[0], m_ChunkIndexBuffers[0], numIndices);
		}
		else {
			g_pApp->GetRenderer()->VRender(m_ChunkVertexDeclaration, EPrimitiveType::POINTLIST, m_ChunkVertexBuffers[0], m_ChunkIndexBuffers[0], numIndices);
		}*/

		auto numIndices = BuildTestChunk(m_TestChunkPosition, 0, 0);
		g_pApp->GetRenderer()->SetRenderPass(&m_DrawChunkPass);
		if (m_bRenderPoints) {
			g_pApp->GetRenderer()->VRender(m_ChunkVertexDeclaration, EPrimitiveType::TRIANGLELIST, m_ChunkVertexBuffers[0], nullptr, numIndices);
		}
		else {
			g_pApp->GetRenderer()->VRender(m_ChunkVertexDeclaration, EPrimitiveType::POINTLIST, m_ChunkVertexBuffers[0], nullptr, numIndices);
		}

		numIndices = BuildTestChunk(m_TestChunkPosition + CVector3::s_FORWARD * m_ChunkSizes[0], 0, 0);
		g_pApp->GetRenderer()->SetRenderPass(&m_DrawChunkPass);
		if (m_bRenderPoints) {
			g_pApp->GetRenderer()->VRender(m_ChunkVertexDeclaration, EPrimitiveType::TRIANGLELIST, m_ChunkVertexBuffers[0], nullptr, numIndices);
		}
		else {
			g_pApp->GetRenderer()->VRender(m_ChunkVertexDeclaration, EPrimitiveType::POINTLIST, m_ChunkVertexBuffers[0], nullptr, numIndices);
		}

		return;
	}

	const auto& cameraPosition = m_Camera.GetPosition();
	auto cameraDirection = m_Camera.GetRotation().ToRotationMatrix().GetDirection();

	m_SortedChunks.clear();

	for (int lod = 0; lod < 1; ++lod) {
		int chunkSize = m_ChunkSizes[lod];

		// Get the centre of the array of chunks for this lod that we are interested in.
		// This is so that the centre of the chunks is in front of the camera and not behind
		// where they wouldn't be seen.
		auto chunkCentre = cameraPosition /*+ (cameraDirection * static_cast<float>(chunkSize * s_NUM_CHUNKS_PER_DIM) * 0.5f)*/;
		/*CVector3 chunkCentreInt(
			std::roundf(chunkCentre.GetX()),
			std::roundf(chunkCentre.GetY()),
			std::roundf(chunkCentre.GetZ())
		);*/

		CVector3 chunkCentreInt(
			static_cast<float>(SmartDivision(RoundUp(chunkCentre.GetX()), chunkSize)),
			static_cast<float>(SmartDivision(RoundUp(chunkCentre.GetY()), chunkSize)),
			static_cast<float>(SmartDivision(RoundUp(chunkCentre.GetZ()), chunkSize))
		);

		int ib = SmartDivision(static_cast<int>(-s_NUM_CHUNKS_PER_DIM / 2.0f + chunkCentreInt.GetX()), s_NUM_CHUNKS_PER_DIM) * s_NUM_CHUNKS_PER_DIM;
		int jb = SmartDivision(static_cast<int>(-s_NUM_CHUNKS_PER_DIM / 2.0f + chunkCentreInt.GetY()), s_NUM_CHUNKS_PER_DIM) * s_NUM_CHUNKS_PER_DIM;
		int kb = SmartDivision(static_cast<int>(-s_NUM_CHUNKS_PER_DIM / 2.0f + chunkCentreInt.GetZ()), s_NUM_CHUNKS_PER_DIM) * s_NUM_CHUNKS_PER_DIM;

		auto chunksCentreLocal = std::roundf(s_NUM_CHUNKS_PER_DIM / 2.0f * chunkSize);

		// Loop through all the chunks in the 3D array for this lod.
		for (int i = 0; i < s_NUM_CHUNKS_PER_DIM; ++i) {
			for (int j = 0; j < s_NUM_CHUNKS_PER_DIM; ++j) {
				for (int k = 0; k < s_NUM_CHUNKS_PER_DIM; ++k) {
					CVector3 chunkPosition(
						static_cast<float>(i * chunkSize), 
						static_cast<float>(j * chunkSize),
						static_cast<float>(k * chunkSize));
					chunkPosition += chunkCentreInt - chunksCentreLocal;

					chunkPosition = GetChunkPosition(i, j, k, chunkCentreInt, chunkSize, ib, jb, kb);

					CVector3 chunkPositionMin = chunkPosition - chunkSize * 0.5f;
					CVector3 chunkPositionCentre = chunkPosition + chunkSize * 0.5f;
					CVector3 chunkPositionMax = chunkPosition + chunkSize * 1.5f;

					// Check to see if the chunk is in the view frustum.
					bool isInView = m_Camera.IsInView(CAxisAlignedBoundingBox(chunkPositionMin, chunkPositionMax));

					auto& chunkInfo = m_Chunks[lod][i][j][k];

					// Check to see if the chunk is out of view or has moved.
					if (!isInView || chunkInfo.position != chunkPosition) {
						ClearChunk(chunkInfo);
						chunkInfo.position = chunkPosition;
					}

					if (!isInView) {
						chunkInfo.distanceFromCameraSqr = std::numeric_limits<float>::max();
					}
					else {
						CVector3 toChunk = chunkPositionCentre - cameraPosition;
						chunkInfo.distanceFromCameraSqr = toChunk.GetLengthSquared();

						m_SortedChunks.push_back(&chunkInfo);
					}
				}
			}
		}
	}
	// Sort the chunks in terms of distance from camera.
	auto lessDistanceFromCamera = [](const TChunkInfo* plhs, const TChunkInfo* prhs) {
		return plhs->distanceFromCameraSqr < prhs->distanceFromCameraSqr;
	};
	std::sort(m_SortedChunks.begin(), m_SortedChunks.end(), lessDistanceFromCamera);

	static const int s_MAX_WORK_PER_FRAME = 21;
	static const int s_WORK_FOR_EMPTY_CHUNK = 1;
	static const int s_WORK_FOR_NONEMPTY_CHUNK = 4;

	// Build chunks.
	int numChunks = m_SortedChunks.size();
	int numWorkThisFrame = 0;
	int lowestPriorityChunk = numChunks - 1;

	for (int i = 0; i < numChunks && numWorkThisFrame < s_MAX_WORK_PER_FRAME - s_WORK_FOR_EMPTY_CHUNK; ++i) {
		auto pChunkInfo = m_SortedChunks[i];

		if (pChunkInfo->bufferID >= 0 || pChunkInfo->hasPolys == false) {
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
				auto pLowestPriorityChunkInfo = m_SortedChunks[lowestPriorityChunk];
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
				m_SortedChunks.resize(i);
				// Stop building chunks.
				break;
			}
		}

		pChunkInfo->bufferID = bufferID;
		pChunkInfo->numIndices = BuildTestChunk(pChunkInfo->position, pChunkInfo->lod, bufferID);
		if (pChunkInfo->numIndices == 0) {
			ClearChunk(*pChunkInfo);
			pChunkInfo->hasPolys = false;
			numWorkThisFrame += s_WORK_FOR_EMPTY_CHUNK;
		}
		else {
			pChunkInfo->hasPolys = true;
			numWorkThisFrame += s_WORK_FOR_NONEMPTY_CHUNK;
		}
	}

	DrawTestChunks();
}

void CMarchingCubesLogic::DrawChunks()
{
	auto pRenderer = g_pApp->GetRenderer();

	for (auto& chunk : m_SortedChunks) {
		auto bufferID = chunk->bufferID;
		if (bufferID >= 0) {
			auto& pVertexBuffer = m_ChunkVertexBuffers[bufferID];
			auto& pIndexBuffer = m_ChunkIndexBuffers[bufferID];

			pRenderer->SetRenderPass(&m_DrawChunkPass);
			if (m_bRenderPoints) {
				pRenderer->VRender(m_ChunkVertexDeclaration, EPrimitiveType::TRIANGLELIST, pVertexBuffer, pIndexBuffer, chunk->numIndices);
			}
			else {
				pRenderer->VRender(m_ChunkVertexDeclaration, EPrimitiveType::POINTLIST, pVertexBuffer, pIndexBuffer, chunk->numIndices);
			}
		}
	}
}

void CMarchingCubesLogic::DrawTestChunks()
{
	auto pRenderer = g_pApp->GetRenderer();

	for (auto& chunk : m_SortedChunks) {
		auto bufferID = chunk->bufferID;
		if (bufferID >= 0) {
			auto& pVertexBuffer = m_ChunkVertexBuffers[bufferID];

			pRenderer->SetRenderPass(&m_DrawChunkPass);
			if (m_bRenderPoints) {
				pRenderer->VRender(m_ChunkVertexDeclaration, EPrimitiveType::TRIANGLELIST, pVertexBuffer, nullptr, chunk->numIndices);
			}
			else {
				pRenderer->VRender(m_ChunkVertexDeclaration, EPrimitiveType::POINTLIST, pVertexBuffer, nullptr, chunk->numIndices);
			}
		}
	}
}

size_t CMarchingCubesLogic::BuildChunk(const CVector3& chunkPosition, size_t lod, size_t chunkBufferID)
{
	auto pRenderer = g_pApp->GetRenderer();

	// Update shader parameters
	m_pChunkParams->SetConstant("gWChunkPosition", chunkPosition);
	// Update CBChunk for the shaders that use it. 
	m_BuildDensitiesPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_BuildDensitiesPass.GetPixelShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_GenerateVerticesPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_ListNonEmptyCellsPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);

	// Set the stream output buffers.
	auto& pVertexBuffer = m_ChunkVertexBuffers[chunkBufferID];
	auto& pIndexBuffer = m_ChunkIndexBuffers[chunkBufferID];
	m_GenerateVerticesPass.ClearStreamOutputTargets();
	m_GenerateIndicesPass.ClearStreamOutputTargets();
	m_GenerateVerticesPass.AddStreamOutputTarget(pVertexBuffer);
	m_GenerateIndicesPass.AddStreamOutputTarget(pIndexBuffer);

	// Create the density volume.
	pRenderer->SetRenderPass(&m_BuildDensitiesPass);
	// Render multiple times, one for each slice of the volume texture.
	pRenderer->VRender(m_QuadVertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pQuadVertices, m_pQuadIndices, 0, m_VoxelDimPlusMargins);

	pRenderer->VBeginStreamOutQuery();
	// List non empty cells.
	pRenderer->SetRenderPass(&m_ListNonEmptyCellsPass);
	pRenderer->VRender(m_DummyCornersVertexDeclaration, EPrimitiveType::POINTLIST, m_pDummyCornersVertices, nullptr, 0, m_VoxelDim);
	auto numCells = pRenderer->VEndStreamOutQuery();

	// Use early out if there's no polygons for this chunk to produce.
	if (numCells <= 0) {
		return 0;
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
	auto numIndices = pRenderer->VEndStreamOutQuery() * 3;

	return numIndices;
}

size_t CMarchingCubesLogic::BuildTestChunk(const CVector3& chunkPosition, size_t lod, size_t chunkBufferID)
{
	auto pRenderer = g_pApp->GetRenderer();

	// Update shader parameters
	m_pChunkParams->SetConstant("gWChunkPosition", chunkPosition);
	// Update CBChunk for the shaders that use it. 
	m_BuildDensitiesPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_BuildDensitiesPass.GetPixelShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_GenerateVerticesPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_ListNonEmptyCellsPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_MarchingCubesPass.GetGeometryShader()->UpdateShaderParams("CBChunk", m_pChunkParams);

	// Set the stream output buffers.
	auto& pVertexBuffer = m_ChunkVertexBuffers[chunkBufferID];
	auto& pIndexBuffer = m_ChunkIndexBuffers[chunkBufferID];
	m_MarchingCubesPass.ClearStreamOutputTargets();
	m_MarchingCubesPass.AddStreamOutputTarget(pVertexBuffer);

	// Create the density volume.
	pRenderer->SetRenderPass(&m_BuildDensitiesPass);
	// Render multiple times, one for each slice of the volume texture.
	pRenderer->VRender(m_QuadVertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pQuadVertices, m_pQuadIndices, 0, m_VoxelDimPlusMargins);

	pRenderer->VBeginStreamOutQuery();
	// List non empty cells.
	pRenderer->SetRenderPass(&m_ListNonEmptyCellsPass);
	pRenderer->VRender(m_DummyCornersVertexDeclaration, EPrimitiveType::POINTLIST, m_pDummyCornersVertices, nullptr, 0, m_VoxelDim);
	auto numCells = pRenderer->VEndStreamOutQuery();

	// Use early out if there's no polygons for this chunk to produce.
	if (numCells <= 0) {
		return 0;
	}

	pRenderer->VBeginStreamOutQuery();
	pRenderer->SetRenderPass(&m_MarchingCubesPass);
	pRenderer->VRender(m_MarchingCubesVertexDeclaration, EPrimitiveType::POINTLIST, m_pDummyPoints);
	auto numVerts = pRenderer->VEndStreamOutQuery() * 3;

	return numVerts;
}

void CMarchingCubesLogic::HandleInput(const CInput& input, float deltaTime)
{
	CPoint mousePosition = input.GetMousePosition();

	// Handle rotation of camera.
	if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		CPoint deltaPosition = mousePosition - m_LastMousePosition;
		m_CameraYaw -= deltaPosition.GetX() * 0.01f;
		m_CameraPitch -= deltaPosition.GetY() * 0.01f;

		m_Camera.SetRotation(CQuaternion(m_CameraYaw, m_CameraPitch, 0.0f));
	}

	if (input.GetKeyPress(EKeyCode::NUM_1)) {
		if (m_DrawChunkPass.GetFillMode() == EFillMode::SOLID) {
			m_DrawChunkPass.SetFillMode(EFillMode::WIREFRAME);
		}
		else {
			m_DrawChunkPass.SetFillMode(EFillMode::SOLID);
		}
	}

	if (input.GetKeyPress(EKeyCode::NUM_2)) {
		m_bRenderPoints = !m_bRenderPoints;
	}


	if (input.GetKeyPress(EKeyCode::NUM_3)) {
		m_bDrawOnlyOneChunk = !m_bDrawOnlyOneChunk;
	}

	if (input.GetKeyPress(EKeyCode::UP_ARROW)) {
		m_TestChunkPosition += CVector3::s_FORWARD;
	}
	if (input.GetKeyPress(EKeyCode::DOWN_ARROW)) {
		m_TestChunkPosition -= CVector3::s_FORWARD;
	}
	if (input.GetKeyPress(EKeyCode::RIGHT_ARROW)) {
		m_TestChunkPosition += CVector3::s_RIGHT;
	}
	if (input.GetKeyPress(EKeyCode::LEFT_ARROW)) {
		m_TestChunkPosition -= CVector3::s_RIGHT;
	}

	m_LastMousePosition = mousePosition;

	/*if (input.GetKeyRelease(EKeyCode::F1)) {
	g_pApp->GetRenderer()->ToggleWindowed(); // If only :(
	}*/

	// Handle translation of camera.
	static float speed = 2.0f;
	if (input.IsKeyDown(EKeyCode::LEFT_SHIFT)) {
		speed = 10.0f;
	}

	CVector3 cameraTranslation = CVector3::s_ZERO;
	const CQuaternion& cameraRotation = m_Camera.GetRotation();

	if (input.IsKeyDown(EKeyCode::W)) {
		cameraTranslation += cameraRotation.GetDirection();
	}

	if (input.IsKeyDown(EKeyCode::S)) {
		cameraTranslation -= cameraRotation.GetDirection();
	}

	if (input.IsKeyDown(EKeyCode::A)) {
		cameraTranslation -= cameraRotation.GetRight();
	}

	if (input.IsKeyDown(EKeyCode::D)) {
		cameraTranslation += cameraRotation.GetRight();
	}

	if (input.IsKeyDown(EKeyCode::SPACE)) {
		cameraTranslation += cameraRotation.GetUp();
	}

	if (input.IsKeyDown(EKeyCode::LEFT_CONTROL)) {
		cameraTranslation -= cameraRotation.GetUp();
	}

	if (cameraTranslation != CVector3::s_ZERO) {
		cameraTranslation = CVector3::Normalise(cameraTranslation);
		cameraTranslation *= deltaTime * speed;

		m_Camera.SetPosition(cameraTranslation + m_Camera.GetPosition());
	}
}

void CMarchingCubesLogic::ClearChunk(TChunkInfo& chunk)
{
	if (chunk.bufferID >= 0) {
		m_FreeBufferIDs.push_back(chunk.bufferID);
	}

	chunk.bufferID = -1;
	chunk.hasPolys = true;	// This chunk may or may not have polys.
}

CVector3 CMarchingCubesLogic::GetChunkPosition(int i, int j, int k, const CVector3& centreChunkPosition, int chunkSize, int ib, int jb, int kb)
{
	static const int s_HalfNumChunkPerDim = static_cast<int>(s_NUM_CHUNKS_PER_DIM / 2.0f);
	int i1 = i + ib;
	int j1 = j + jb;
	int k1 = k + kb;

	if (i < SmartModulus(static_cast<int>(centreChunkPosition.GetX()) - s_HalfNumChunkPerDim, s_NUM_CHUNKS_PER_DIM)) {
		i1 += s_NUM_CHUNKS_PER_DIM;
	}
	if (j < SmartModulus(static_cast<int>(centreChunkPosition.GetY()) - s_HalfNumChunkPerDim, s_NUM_CHUNKS_PER_DIM)) {
		j1 += s_NUM_CHUNKS_PER_DIM;
	}
	if (k < SmartModulus(static_cast<int>(centreChunkPosition.GetZ()) - s_HalfNumChunkPerDim, s_NUM_CHUNKS_PER_DIM)) {
		k1 += s_NUM_CHUNKS_PER_DIM;
	}

	return CVector3(
		static_cast<float>(i1 * chunkSize), 
		static_cast<float>(j1 * chunkSize), 
		static_cast<float>(k1 * chunkSize));
}
