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
#include "TriTable.h"

struct TVertex {
	CVector3 position;
};

struct TQuadVertex {
	CVector3 position;
	CVector2 texCoords;
};

CMarchingCubesLogic::CMarchingCubesLogic()
	:m_Camera(nullptr),
	m_Light(nullptr),
	m_VoxelDim(33),
	m_VoxelMargins(4),
	m_VoxelDimPlusMargins(m_VoxelDim + m_VoxelMargins),
	m_ChunkSize(4)
{
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
	m_QuadVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_QuadVertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);

	m_VoxelVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	
	// Create geometry.
	BuildQuad();
	BuildVolume();

	// Create render passes.
	m_BuildDensitiesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_BuildDensitiesPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_BuildDensitiesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	auto pNoiseTexture0 = g_pApp->GetRenderer()->VCreateTexture("Textures/noise_0.vol");
	pNoiseTexture0->SetPixelFormat(ETexturePixelFormat::FLOAT_32_R);
	pNoiseTexture0->SetWidth(16);
	pNoiseTexture0->SetDepth(16);
	pNoiseTexture0->SetHeight(16);
	pNoiseTexture0->SetType(ETextureType::TYPE_3D);
	pNoiseTexture0->VLoadRaw();

	auto pNoiseTexture1 = g_pApp->GetRenderer()->VCreateTexture("Textures/noise_1.vol");
	pNoiseTexture1->SetPixelFormat(ETexturePixelFormat::FLOAT_32_R);
	pNoiseTexture1->SetWidth(16);
	pNoiseTexture1->SetDepth(16);
	pNoiseTexture1->SetHeight(16);
	pNoiseTexture1->SetType(ETextureType::TYPE_3D);
	pNoiseTexture1->VLoadRaw();

	auto pNoiseTexture2 = g_pApp->GetRenderer()->VCreateTexture("Textures/noise_2.vol");
	pNoiseTexture2->SetPixelFormat(ETexturePixelFormat::FLOAT_32_R);
	pNoiseTexture2->SetWidth(16);
	pNoiseTexture2->SetDepth(16);
	pNoiseTexture2->SetHeight(16);
	pNoiseTexture2->SetType(ETextureType::TYPE_3D);
	pNoiseTexture2->VLoadRaw();

	auto pNoiseTexture3 = g_pApp->GetRenderer()->VCreateTexture("Textures/noise_3.vol");
	pNoiseTexture3->SetPixelFormat(ETexturePixelFormat::FLOAT_32_R);
	pNoiseTexture3->SetWidth(16);
	pNoiseTexture3->SetDepth(16);
	pNoiseTexture3->SetHeight(16);
	pNoiseTexture3->SetType(ETextureType::TYPE_3D);
	pNoiseTexture3->VLoadRaw();

	m_BuildDensitiesPass.AddTextureLayer(pNoiseTexture0/*"Textures/noise_vol_0.dds"*/)->SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);
	m_BuildDensitiesPass.AddTextureLayer(pNoiseTexture1/*"Textures/noise_vol_1.dds"*/)->SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);
	m_BuildDensitiesPass.AddTextureLayer(pNoiseTexture2/*"Textures/noise_vol_2.dds"*/)->SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);
	m_BuildDensitiesPass.AddTextureLayer(pNoiseTexture3/*"Textures/noise_vol_3.dds"*/)->SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);

	// Create the render texture for building and storing the densities.
	auto pDensityTexture = g_pApp->GetRenderer()->VCreateTexture("DensityTexture");
	pDensityTexture->SetWidth(m_VoxelDim);
	pDensityTexture->SetHeight(m_VoxelDim);
	pDensityTexture->SetDepth(m_VoxelDim);
	pDensityTexture->SetType(ETextureType::TYPE_3D);
	pDensityTexture->SetUsage(ETextureUsage::RENDER_TARGET);
	pDensityTexture->SetPixelFormat(ETexturePixelFormat::FLOAT_32_R);
	pDensityTexture->VLoad();
	m_pDensityRenderTarget = g_pApp->GetRenderer()->VCreateRenderTexture(pDensityTexture);

	m_GenerateVerticesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("MarchingCubes_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_GenerateVerticesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("MarchingCubes_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));
	m_GenerateVerticesPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("MarchingCubes_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));

	auto pDensityTextureLayer = m_GenerateVerticesPass.AddTextureLayer();
	pDensityTextureLayer->SetTexture(m_pDensityRenderTarget->GetTexture());
	pDensityTextureLayer->SetTextureFilter(ETextureFilterType::POINT);
	pDensityTextureLayer->SetTextureAddressModes(ETextureAddressMode::CLAMP);

	// Set the tri-table on the geometry shader.
	auto pTriTableBuffer = m_GenerateVerticesPass.GetGeometryShader()->GetShaderParams("CBTriTable");
	pTriTableBuffer->SetConstant("gTriTable", TriTable::g_VALUES[0], TriTable::g_SIZE);
	m_GenerateVerticesPass.GetGeometryShader()->UpdateShaderParams("CBTriTable", pTriTableBuffer);

	m_pLodParams = m_GenerateVerticesPass.GetGeometryShader()->GetShaderParams("CBLod");
	m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBLod");
	m_BuildDensitiesPass.GetPixelShader()->GetShaderParams("CBLod");
	m_GenerateVerticesPass.GetPixelShader()->GetShaderParams("CBLod");

	m_pLodParams->SetConstant("gVoxelDim", static_cast<float>(m_VoxelDim));
	m_pLodParams->SetConstant("gVoxelDimMinusOne", static_cast<float>(m_VoxelDim) - 1.0f);
	m_pLodParams->SetConstant("gWVoxelSize", static_cast<float>(m_ChunkSize) / (static_cast<float>(m_VoxelDim) - 1.0f));
	m_pLodParams->SetConstant("gWChunkSize", static_cast<float>(m_ChunkSize));
	m_pLodParams->SetConstant("gInvVoxelDim", 1.0f / static_cast<float>(m_VoxelDim));
	m_pLodParams->SetConstant("gInvVoxelDimMinusOne", 1.0f / (static_cast<float>(m_VoxelDim) - 1.0f));
	m_pLodParams->SetConstant("gMargin", static_cast<float>(m_VoxelMargins));
	m_pLodParams->SetConstant("gVoxelDimPlusMargins", static_cast<float>(m_VoxelDimPlusMargins));
	m_pLodParams->SetConstant("gVoxelDimPlusMarginsMinusOne", static_cast<float>(m_VoxelDimPlusMargins) - 1.0f);
	m_pLodParams->SetConstant("gInvVoxelDimPlusMargins", 1.0f / static_cast<float>(m_VoxelDimPlusMargins));
	m_pLodParams->SetConstant("gInvVoxelDimPlusMarginsMinusOne", 1.0f / (static_cast<float>(m_VoxelDimPlusMargins) - 1.0f));
	m_BuildDensitiesPass.GetVertexShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_BuildDensitiesPass.GetPixelShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_GenerateVerticesPass.GetGeometryShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_GenerateVerticesPass.GetPixelShader()->UpdateShaderParams("CBLod", m_pLodParams);
	m_GenerateVerticesPass.SetCullingMode(ECullingMode::NONE);

	m_pChunkParams = m_BuildDensitiesPass.GetVertexShader()->GetShaderParams("CBChunk");
	m_pWVPParams = m_GenerateVerticesPass.GetGeometryShader()->GetShaderParams("CBPerFrame");
	m_pLightingParams = m_GenerateVerticesPass.GetPixelShader()->GetShaderParams("CBLighting");

	m_pLightingParams->SetConstant("gLight.type", m_Light.GetType());
	m_pLightingParams->SetConstant("gLight.diffuse", m_Light.GetDiffuse());
	m_pLightingParams->SetConstant("gLight.specular", m_Light.GetSpecular());
	m_pLightingParams->SetConstant("gLight.direction", m_Light.GetRotation().GetDirection());
	m_pLightingParams->SetConstant("gLight.range", m_Light.GetRange());
	m_pLightingParams->SetConstant("gLight.attenuation", m_Light.GetAttenuation());
	m_pLightingParams->SetConstant("gAmbientLight", CColourValue(0.3f, 0.3f, 0.3f));
	m_GenerateVerticesPass.GetPixelShader()->UpdateShaderParams("CBLighting", m_pLightingParams);

	// Setup camera.
	m_Camera.SetPosition(CVector3(2.0f, 2.0f, 2.0f));
	m_Camera.SetPerspective(Math::DegreesToRadians(60.0f), 1.0f, 0.1f, 100.0f);

	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue(0.8f, 0.8f, 0.8f));

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

void CMarchingCubesLogic::BuildVolume()
{
	std::vector<CVector3> vertices;
	vertices.reserve(m_VoxelDim * m_VoxelDim * m_VoxelDim);

	for (int z = 0; z < m_VoxelDim; ++z) {
		float zVal = z / static_cast<float>(m_VoxelDim);

		for (int y = 0; y < m_VoxelDim; ++y) {
			float yVal = y / static_cast<float>(m_VoxelDim);

			for (int x = 0; x < m_VoxelDim; ++x) {
				float xVal = x / static_cast<float>(m_VoxelDim);
				vertices.emplace_back(xVal, yVal, zVal);
			}
		}
	}

	m_pVolumeVertices = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
	m_pVolumeIndices = nullptr;
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

	m_pWVPParams->SetConstant("gWorldViewProjectionMatrix", m_Camera.GetViewProjMatrix());
	m_GenerateVerticesPass.GetGeometryShader()->UpdateShaderParams("CBPerFrame", m_pWVPParams);

	// Create densities.
	BuildChunk(CVector3::s_ZERO, 0);
}

void CMarchingCubesLogic::BuildChunk(const CVector3& chunkPosition, int lod)
{
	// Update shader parameters
	m_pChunkParams->SetConstant("gWChunkPosition", chunkPosition);

	BuildDensities();

	m_GenerateVerticesPass.GetGeometryShader()->UpdateShaderParams("CBChunk", m_pChunkParams);
	m_GenerateVerticesPass.GetPixelShader()->UpdateShaderParams("CBChunk", m_pChunkParams);

	auto pRenderer = g_pApp->GetRenderer();
	pRenderer->SetRenderPass(&m_GenerateVerticesPass);
	pRenderer->VRender(m_VoxelVertexDeclaration, EPrimitiveType::POINTLIST, m_pVolumeVertices, m_pVolumeIndices);
}

void CMarchingCubesLogic::BuildDensities()
{
	auto pRenderer = g_pApp->GetRenderer();

	// Set the render target to the density texture.
	pRenderer->VSetRenderTarget(m_pDensityRenderTarget.get());
	pRenderer->VPreRender();

	// Update parameters.
	m_BuildDensitiesPass.GetVertexShader()->UpdateShaderParams("CBChunk", m_pChunkParams);

	// Draw the quad.
	pRenderer->SetRenderPass(&m_BuildDensitiesPass);
	pRenderer->VRender(m_QuadVertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pQuadVertices, m_pQuadIndices);

	// Set render target back.
	pRenderer->VSetRenderTarget(nullptr);
	pRenderer->VPreRender();
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

	m_LastMousePosition = mousePosition;

	/*if (input.GetKeyRelease(EKeyCode::F1)) {
	g_pApp->GetRenderer()->ToggleWindowed(); // If only :(
	}*/

	// Handle translation of camera.
	static float speed = 25.0f;
	if (input.IsKeyDown(EKeyCode::LEFT_SHIFT)) {
		speed = 50.0f;
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
