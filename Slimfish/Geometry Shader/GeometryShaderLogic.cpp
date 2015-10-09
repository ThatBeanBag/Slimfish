// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GeometryShaderLogic.cpp
// Description	: CGeometryShaderLogic implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GeometryShaderStd.h"

// Library Includes

// This Include
#include "GeometryShaderLogic.h"

// Local Includes
#include <Graphics/SlimRenderer.h>

struct TVertex {
	CVector3 position;
};

CGeometryShaderLogic::CGeometryShaderLogic()
	:m_Camera(nullptr)
{

}

CGeometryShaderLogic::~CGeometryShaderLogic()
{

}

bool CGeometryShaderLogic::Initialise()
{
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);

	// Create the vertex and vertex buffer.
	std::vector<TVertex> verts(1, { CVector3::s_ZERO });
	m_pStarVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(verts);

	if (!m_pStarVertexBuffer) {
		return false;
	}

	// Create shaders.
	m_StarRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("StarVS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_StarRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("StarPS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_StarRenderPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("StarGS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));

	m_pGSParams = m_StarRenderPass.GetGeometryShader()->VCreateShaderParams("constantBuffer");

	// Setup camera.
	m_Camera.SetPosition(CVector3(0.0f, 0.0f, 00.0f));
	m_Camera.SetProjectionMode(EProjectionMode::PERSPECTIVE);
	m_Camera.SetNearClipDistance(0.1f);
	m_Camera.SetFarClipDistance(1000.0f);
	m_Camera.SetFieldOfView(DegreesToRadians(90.0f));

	return true;
}

void CGeometryShaderLogic::Update(float deltaTime)
{
	
}

void CGeometryShaderLogic::Render()
{
	m_pGSParams->SetConstant("gWVP", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());
	m_StarRenderPass.GetGeometryShader()->VUpdateShaderParams("constantBuffer", m_pGSParams);

	g_pApp->GetRenderer()->SetRenderPass(&m_StarRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::POINTLIST, m_pStarVertexBuffer);
}

void CGeometryShaderLogic::HandleInput(const CInput& input, float deltaTime)
{
	CPoint mousePosition = input.GetMousePosition();

	// Handle rotation of camera.
	if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		CPoint deltaPosition = mousePosition - m_lastMousePosition;
		m_CameraYaw -= deltaPosition.GetX() * 0.01f;
		m_CameraPitch -= deltaPosition.GetY() * 0.01f;

		m_Camera.SetRotation(CQuaternion(m_CameraYaw, m_CameraPitch, 0.0f));
	}

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
