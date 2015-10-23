// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ProceduralTerrainGenerationLogic.cpp
// Description	: CProceduralTerrainGenerationLogic implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ProceduralTerrainGenerationStd.h"

// Library Includes

// This Include
#include "ProceduralTerrainGenerationLogic.h"

// Local Includes
#include <Graphics/SlimRenderer.h>

struct TVertex {
	CVector3 position;
	CVector3 normal;
	CVector3 tangent;
	float u;
	float v;
};

CProceduralTerrainGenerationLogic::CProceduralTerrainGenerationLogic()
	:m_Camera(nullptr),
	m_Light(nullptr),
	m_Terrain(64, 0, 255)
{
	m_TerrainWorldTransform = CMatrix4x4::s_IDENTITY;
}

CProceduralTerrainGenerationLogic::~CProceduralTerrainGenerationLogic()
{

}

bool CProceduralTerrainGenerationLogic::Initialise()
{
	// Create vertex declarations.
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("TANGENT", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);

	// Create the vertex and index buffer.
	CreateTerrainIndices();
	m_pTerrainVertices = g_pApp->GetRenderer()->VCreateVertexBuffer(
		m_Terrain.GetSize() * m_Terrain.GetSize(),
		sizeof(TVertex),
		nullptr, 
		EGpuBufferUsage::WRITE_ONLY, 
		false);
	CreateTerrainVertices({ 200.0f, 100.0f, 200.0f });

	// Create shaders.
	m_TerrainRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("TerrainVS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_TerrainRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("TerrainPS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_pTerrainVSParams = m_TerrainRenderPass.GetVertexShader()->VCreateShaderParams("constantBuffer");
	m_pTerrainPSParams = m_TerrainRenderPass.GetPixelShader()->VCreateShaderParams("constantBuffer");
	m_pTerrainVSParams->SetConstant("gWorldMatrix", m_TerrainWorldTransform);
	m_pTerrainVSParams->SetConstant("gTexMatrix", CMatrix4x4::BuildScale(10.0f, 10.0f, 1.0f));

	// Init texture layers.
	m_TerrainRenderPass.AddTextureLayer("TerrainTextures/grass_diffuse.png")->SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	m_TerrainRenderPass.AddTextureLayer("TerrainTextures/cliff_diffuse.png")->SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	m_TerrainRenderPass.AddTextureLayer("TerrainTextures/sand_diffuse.png")->SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	m_TerrainRenderPass.AddTextureLayer("TerrainTextures/grass_normal.png")->SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	m_TerrainRenderPass.AddTextureLayer("TerrainTextures/cliff_normal.png")->SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	m_TerrainRenderPass.AddTextureLayer("TerrainTextures/sand_normal.png")->SetTextureFilter(ETextureFilterType::ANISOTROPIC);

	//m_TerrainRenderPass.SetFillMode(EFillMode::WIREFRAME);

	// Setup lighting.
	m_Light.SetType(LIGHT_DIRECTIONAL);
	m_Light.SetDiffuse(CColourValue(0.7f, 0.7f, 0.7f));
	m_Light.SetSpecular(CColourValue(1.0f, 1.0f, 1.0f, 1.0f));
	m_Light.SetRotation(CQuaternion(45, -45, 0));

	m_pTerrainPSParams->SetConstant("gLight.type", m_Light.GetType());
	m_pTerrainPSParams->SetConstant("gLight.diffuse", m_Light.GetDiffuse());
	m_pTerrainPSParams->SetConstant("gLight.specular", m_Light.GetSpecular());
	m_pTerrainPSParams->SetConstant("gLight.direction", m_Light.GetRotation().GetDirection());
	m_pTerrainPSParams->SetConstant("gLight.range", m_Light.GetRange());
	m_pTerrainPSParams->SetConstant("gLight.attenuation", m_Light.GetAttenuation());
	m_pTerrainPSParams->SetConstant("gFogStart", 200.0f);
	m_pTerrainPSParams->SetConstant("gFogRange", 40.0f);
	m_pTerrainPSParams->SetConstant("gFogColour", CColourValue(0.3686f, 0.3686f, 0.43137f, 1.0f));
	m_pTerrainPSParams->SetConstant("gAmbientLight", CColourValue(0.3f, 0.3f, 0.3f));
	m_TerrainRenderPass.GetPixelShader()->VUpdateShaderParams("constantBuffer", m_pTerrainPSParams);

	// Setup camera.
	m_Camera.SetPosition(CVector3(150.0f, 150.0f, 150.0f));
	m_Camera.SetProjectionMode(EProjectionMode::PERSPECTIVE);
	m_Camera.SetNearClipDistance(0.1f);
	m_Camera.SetFarClipDistance(1000.0f);
	m_Camera.SetFieldOfView(Math::DegreesToRadians(60.0f));
	m_Camera.SetOrthographicSize(20.0f);

	m_CameraYaw = Math::DegreesToRadians(45.0f);
	m_CameraPitch = Math::DegreesToRadians(-45.0f);
	m_Camera.SetRotation(CQuaternion(m_CameraYaw, m_CameraPitch, 0.0f));

	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue(0.8f, 0.8f, 0.8f));

	return true;
}

void CProceduralTerrainGenerationLogic::Update(float deltaTime)
{
	CPoint screenSize = g_pApp->GetRenderer()->GetWindowSize();
	m_Camera.SetAspectRatio(static_cast<float>(screenSize.GetX()) / static_cast<float>(screenSize.GetY()));
}

void CProceduralTerrainGenerationLogic::Render()
{
	m_Camera.UpdateViewTransform();

	m_pTerrainVSParams->SetConstant("gProjectionMatrix", m_Camera.GetProjectionMatrix());
	m_pTerrainVSParams->SetConstant("gViewMatrix", m_Camera.GetViewMatrix());
	m_pTerrainVSParams->SetConstant("gWorldMatrix", m_TerrainWorldTransform);
	m_TerrainRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_pTerrainVSParams);

	g_pApp->GetRenderer()->SetRenderPass(&m_TerrainRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pTerrainVertices, m_pTerrainIndices);
}

void CProceduralTerrainGenerationLogic::HandleInput(const CInput& input, float deltaTime)

{
	if (input.GetKeyPress(EKeyCode::NUM_1)) {
		// Step the diamond square algorithm.
		for (int i = 0; i < 3; ++i) {
			m_Terrain.Step();
		}

		// Recreate the vertices.
		CreateTerrainVertices({ 200.0f, 100.0f, 200.0f });
	}

	if (input.GetKeyPress(EKeyCode::NUM_4)) {
		if (m_TerrainRenderPass.GetFillMode() == EFillMode::SOLID) {
			m_TerrainRenderPass.SetFillMode(EFillMode::WIREFRAME);
		}
		else {
			m_TerrainRenderPass.SetFillMode(EFillMode::SOLID);
		}
	}

	if (input.GetKeyPress(EKeyCode::NUM_3)) {
		if (m_Terrain.IsDone()) {
			m_Terrain.Reset();
		}
		m_Terrain.Generate();

		// Recreate the vertices.
		CreateTerrainVertices({ 200.0f, 100.0f, 200.0f });
	}

	if (input.GetKeyPress(EKeyCode::R)) {
		m_Terrain.Reset();

		// Recreate the vertices.
		CreateTerrainVertices({ 200.0f, 100.0f, 200.0f });
	}

	if (input.GetKeyPress(EKeyCode::NUM_2)) {
		CreateTerrainVertices({ 200.0f, 100.0f, 200.0f }, true);
	}

	CPoint mousePosition = input.GetMousePosition();

	// Handle rotation of camera.
	if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		CPoint deltaPosition = mousePosition - m_lastMousePosition;
		m_CameraYaw -= deltaPosition.GetX() * 0.01f;
		m_CameraPitch -= deltaPosition.GetY() * 0.01f;

		m_Camera.SetRotation(CQuaternion(m_CameraYaw, m_CameraPitch, 0.0f));
	}

	m_lastMousePosition = mousePosition;

	/*if (input.GetKeyRelease(EKeyCode::F1)) {
		g_pApp->GetRenderer()->ToggleWindowed(); // If only :(
	}*/

	// Handle translation of camera.
	static float speed = 50.0f;
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

void CProceduralTerrainGenerationLogic::CreateTerrainVertices(const CVector3& scale, bool smooth)
{
	CGpuBufferLock lock(m_pTerrainVertices, 0, m_pTerrainVertices->GetSize(), EGpuBufferLockType::DISCARD);
	TVertex* pVertices = reinterpret_cast<TVertex*>(lock.GetLockedContents());

	auto size = m_Terrain.GetSize();
	const auto& heights = m_Terrain.GetHeights();
	auto numPixels = size * size;
	std::vector<TVertex> vertices(numPixels);

	for (unsigned int z = 0; z < size; ++z) {
		auto currentZ = z * size;

		for (unsigned int x = 0; x < size; ++x) {
			auto height = heights[z][x];
			auto& vert = pVertices[x + currentZ];


			if (m_Terrain.IsDone() || smooth) {
				auto numNeighbours = 1;
				for (auto neighbourZ = z - 1; neighbourZ <= z + 1; ++neighbourZ) {
					for (auto neighbourX = x - 1; neighbourX <= x + 1; ++neighbourX) {
						if (neighbourX < 0 || neighbourZ < 0 ||
							neighbourX >= static_cast<int>(size) ||
							neighbourZ >= static_cast<int>(size)) {
							continue;
						}

						height += heights[neighbourZ][neighbourX];
						numNeighbours++;
					}
				}
				height /= numNeighbours;
			}

			vert.position.SetX((static_cast<float>(x) / static_cast<float>(size) - 0.5f) * scale.GetX());
			vert.position.SetY((static_cast<float>(height) / (m_Terrain.GetYMax() - m_Terrain.GetYMin()) - 0.5f) * scale.GetY());
			vert.position.SetZ((static_cast<float>(z) / static_cast<float>(size) - 0.5f) * scale.GetZ());
			vert.u = (static_cast<float>(x) / static_cast<float>(size - 1));
			vert.v = (static_cast<float>(z) / static_cast<float>(size - 1));
		}
	}

	// Compute normals.
	for (unsigned int z = 0; z < size; ++z) {
		for (unsigned int x = 0; x < size; ++x) {
			TVertex& currentVert = pVertices[x + z * size];
			CVector3 averagedNormal(0, 0, 0);
			CVector3 lastLine(0, 0, 0);

			// Start at the top right neighbour.
			int neighbourX = x + 1;
			int neighbourZ = z - 1;

			auto increment = [&](int _i) {
				// Increment to next neighbour, so that we search 
				// neighbour vertices int counter-clockwise order.
				if (_i < 2) {
					--neighbourX;
				}
				else if (_i < 4) {
					++neighbourZ;
				}
				else if (_i < 6) {
					++neighbourX;
				}
				else {
					--neighbourZ;
				}
			};

			for (unsigned int i = 0; i < 8; ++i) {
				if (neighbourX < 0 || neighbourZ < 0 ||
					neighbourX >= static_cast<int>(size) ||
					neighbourZ >= static_cast<int>(size)) {
					// Is this neighbour outside the image?
					// Don't evaluate it.
				}
				else {
					TVertex neighbourVert = pVertices[neighbourX + neighbourZ * size];
					CVector3 currentNormal(0, 0, 0);
					CVector3 currentLine = currentVert.position - neighbourVert.position;

					currentNormal = CVector3::CrossProduct(lastLine, currentLine);
					averagedNormal += currentNormal;
					lastLine = currentLine;

				}

				increment(i);
			}

			currentVert.normal = CVector3::Normalise(averagedNormal);
		}
	}

	// Calculate tangents.
	for (auto i = 0U; i < m_TerrainIndices.size() - 2; i += 3) {
		auto& vert1 = pVertices[m_TerrainIndices[i]];
		auto& vert2 = pVertices[m_TerrainIndices[i + 1]];
		auto& vert3 = pVertices[m_TerrainIndices[i + 2]];

		// Delta position
		auto deltaPos1 = vert2.position - vert1.position;
		auto deltaPos2 = vert3.position - vert1.position;

		// UV delta
		auto deltaU1 = vert2.u - vert1.u;
		auto deltaU2 = vert3.u - vert1.u;
		auto deltaV1 = vert2.v - vert1.v;
		auto deltaV2 = vert3.v - vert1.v;

		auto r = 1.0f / (deltaU1 * deltaV2 - deltaV1 * deltaU2);
		auto tangent = (deltaPos1 * deltaV2 - deltaPos2 * deltaV1) * r;
		vert1.tangent = tangent;
		vert2.tangent = tangent;
		vert3.tangent = tangent;
	}
}

void CProceduralTerrainGenerationLogic::CreateTerrainIndices()
{
	auto size = m_Terrain.GetSize();
	auto numIndices = ((size * 2) * (size - 1) + (size - 2));

	// Generate indices
	m_TerrainIndices = std::vector<int>(numIndices);
	int index = 0;

	for (unsigned int z = 0; z < size - 1; ++z) {
		// Even rows move left to right, odd rows move right to left.
		if (z % 2 == 0) {
			// Is this an even row?
			for (int x = 0; x < static_cast<int>(size); ++x) {
				m_TerrainIndices[index++] = x + z * size;
				m_TerrainIndices[index++] = x + z * size + size;	// Next row.
			}

			// Insert degenerate vertex, if this isn't the last row.
			if (z != size - 2) {
				// Is this the last row?
				m_TerrainIndices[index++] = size - 1 + (z * size);
			}
		}
		else {
			// This is an odd row.
			for (int x = static_cast<int>(size) - 1; x >= 0; --x) {
				m_TerrainIndices[index++] = x + z * size;
				m_TerrainIndices[index++] = x + z * size + size;	// Next row.
			}

			// Insert degenerate vertex, if this isn't the last row.
			if (z != size - 2) {
				// Is this the last row?
				m_TerrainIndices[index++] = z * size;
			}
		}
	}

	m_pTerrainIndices = g_pApp->GetRenderer()->CreateIndexBuffer(m_TerrainIndices);
}
