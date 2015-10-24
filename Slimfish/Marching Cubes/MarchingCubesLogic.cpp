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

struct TVertex {
	CVector3 position;
	CVector3 normal;
};

struct TDensityVertex {
	CVector3 position;
	CVector2 texCoords;
};

CMarchingCubesLogic::CMarchingCubesLogic()
	:m_Camera(nullptr)
{

}

CMarchingCubesLogic::~CMarchingCubesLogic()
{

}

bool CMarchingCubesLogic::Initialise()
{
	m_VoxelVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VoxelVertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);

	m_TerrainVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_TerrainVertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);

	// Create render passes.
	m_BuildDensitiesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_BuildDensitiesPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_BuildDensitiesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("Density_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	m_pDensityRenderTarget = g_pApp->GetRenderer()->VCreateRenderTexture(
		"Density",
		m_VoxelDimPlusMargins,
		m_VoxelDimPlusMargins,
		m_VoxelDimPlusMargins,
		ETextureType::TYPE_3D);

	m_GenerateVerticesPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("MarchingCubes_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_GenerateVerticesPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("MarchingCubes_GS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	// Setup camera.
	m_Camera.SetPosition(CVector3(0.0f, 0.0f, 10.0f));
	m_Camera.SetPerspective(Math::DegreesToRadians(60.0f), 1.0f, 0.1f, 100.0f);

	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue(0.8f, 0.8f, 0.8f));

	return true;
}

void CMarchingCubesLogic::Update(float deltaTime)
{
	CPoint screenSize = g_pApp->GetRenderer()->GetWindowSize();
	m_Camera.SetAspectRatio(static_cast<float>(screenSize.GetX()) / static_cast<float>(screenSize.GetY()));
}

void CMarchingCubesLogic::Render()
{
	m_Camera.UpdateViewTransform();
}

void CMarchingCubesLogic::BuildDensities()
{
	auto pRenderer = g_pApp->GetRenderer();
	pRenderer->VSetRenderTarget(m_pDensityRenderTarget.get());
	pRenderer->VPreRender();

	pRenderer->SetRenderPass(&m_BuildDensitiesPass);
}

void CMarchingCubesLogic::HandleInput(const CInput& input, float deltaTime)
{
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
