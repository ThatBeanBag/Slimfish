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

CMarchingCubesLogic::CMarchingCubesLogic()
	:m_Camera(nullptr),
	m_Light(nullptr)
{
	/*for (size_t i = 0; i < m_FreeBufferIDs.size(); ++i) {
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
	}*/

	m_Light.SetType(LIGHT_DIRECTIONAL);
	m_Light.SetDiffuse(ToColourValue(CColour(255, 184, 19)));
	m_Light.SetDiffuse(ToColourValue(CColour(222, 222, 222)));
	m_Light.SetSpecular(ToColourValue(CColour(255, 184, 19)));
	m_Light.SetRotation(CQuaternion(Math::DegreesToRadians(-45), Math::DegreesToRadians(-45), 0));
}

CMarchingCubesLogic::~CMarchingCubesLogic()
{

}

bool CMarchingCubesLogic::Initialise()
{
	/*// Build vertex declarations.
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
	CreateDrawChunkPass();
	SLIM_INFO() << "Finished DrawChunkPass.";
	CreateMarchingCubesPass(); // TODO: Remove this.
	SLIM_INFO() << "Finished MarchingCubesPass.";

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


	m_pLightingParams->SetConstant("gLight.type", m_Light.GetType());
	m_pLightingParams->SetConstant("gLight.diffuse", m_Light.GetDiffuse());
	m_pLightingParams->SetConstant("gLight.specular", m_Light.GetSpecular());
	m_pLightingParams->SetConstant("gLight.direction", m_Light.GetRotation().GetDirection());
	m_pLightingParams->SetConstant("gLight.range", m_Light.GetRange());
	m_pLightingParams->SetConstant("gLight.attenuation", m_Light.GetAttenuation());
	m_pLightingParams->SetConstant("gFogStart", 50.0f);
	m_pLightingParams->SetConstant("gFogRange", 5.0f);
	m_pLightingParams->SetConstant("gFogColour", g_pApp->GetRenderer()->GetBackgroundColour());
	m_pLightingParams->SetConstant("gAmbientLight", CColourValue(0.5f, 0.5f, 0.5f));
	m_DrawChunkPass.GetPixelShader()->UpdateShaderParams("CBLighting", m_pLightingParams);*/

	// Setup camera.
	m_Camera.SetPosition(CVector3(0.0f, 1.0f, 1.0f));
	m_Camera.SetPerspective(Math::DegreesToRadians(60.0f), 1.0f, 0.1f, 100.0f);
	g_pApp->GetRenderer()->SetBackgroundColour(ToColourValue(CColour(135, 206, 250)));

	return m_ChunkManager.Initialise();
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

	m_ChunkManager.Update(m_Camera);
	m_ChunkManager.DrawChunks(m_Camera, m_Light);
}

void CMarchingCubesLogic::HandleInput(const CInput& input, float deltaTime)
{
	CPoint mousePosition = input.GetMousePosition();

	// Handle rotation of camera.
	if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		CPoint deltaPosition = mousePosition - m_LastMousePosition;
		m_CameraYaw -= deltaPosition.GetX() * 0.005f;
		m_CameraPitch -= deltaPosition.GetY() * 0.005f;

		m_Camera.SetRotation(CQuaternion(m_CameraYaw, m_CameraPitch, 0.0f));
	}

	if (input.GetKeyPress(EKeyCode::NUM_1)) {
		// Set fill mode.
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

	if (input.GetKeyRelease(EKeyCode::F1)) {
		g_pApp->GetRenderer()->ToggleWindowed(); // If only :(
	}

	// Handle translation of camera.
	float speed = 5.0f;
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
