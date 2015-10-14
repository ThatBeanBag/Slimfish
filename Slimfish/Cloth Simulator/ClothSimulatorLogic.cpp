// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ClothSimulatorLogic.cpp
// Description	: CClothSimulatorLogic implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ClothSimulatorStd.h"

// Library Includes

// This Include
#include "ClothSimulatorLogic.h"

// Local Includes
#include <Graphics/SlimRenderer.h>

struct TVertex {
	CVector3 position;
	CVector3 normal;
};

CClothSimulatorLogic::CClothSimulatorLogic()
	:m_Camera(nullptr)
{

}

CClothSimulatorLogic::~CClothSimulatorLogic()
{

}

bool CClothSimulatorLogic::Initialise()
{
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);

	CreateCloth(40, 40, 1, 1, 50.0f);

	// Create the vertex and vertex buffer.
	// Create vertex buffer.
	std::vector<TVertex> vertices;
	for (auto& pointMass : m_PointMasses) {
		for (const auto& link : pointMass->GetLinks()) {
			vertices.push_back({ pointMass->GetPosition(), CVector3::s_FORWARD });
			vertices.push_back({ link.GetPointMassB()->GetPosition(), CVector3::s_FORWARD });
		}
	}

	m_pClothVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(vertices, EGpuBufferUsage::STATIC);

	if (!m_pClothVertexBuffer) {
		return false;
	}

	// Create shaders.
	m_ClothRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothVS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_ClothRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothPS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
//	m_ClothRenderPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothGS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));
	m_ClothRenderPass.SetCullingMode(ECullingMode::NONE);
	m_ClothRenderPass.SetFillMode(EFillMode::SOLID);

	m_pVSParams = m_ClothRenderPass.GetVertexShader()->VCreateShaderParams("constantBuffer");

	// Setup camera.
	m_Camera.SetPosition(CVector3(0.0f, 0.0f, 10.0f));
	m_Camera.SetProjectionMode(EProjectionMode::PERSPECTIVE);
	m_Camera.SetNearClipDistance(0.1f);
	m_Camera.SetFarClipDistance(100.0f);
	m_Camera.SetFieldOfView(DegreesToRadians(60.0f));
	m_Camera.SetOrthographicSize(20.0f);

	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue(0.8f, 0.8f, 0.8f));


	return true;
}

void CClothSimulatorLogic::Update(float deltaTime)
{
	CPoint screenSize = g_pApp->GetRenderer()->GetWindowSize();
	m_Camera.SetAspectRatio(static_cast<float>(screenSize.GetX()) / static_cast<float>(screenSize.GetY()));

	// Update physics.
	float fixedDeltaTime = 16.0f;
	int constraintAccuracy = 5;
	
	int numTimeSteps = static_cast<int>(deltaTime + m_LeftOverDeltaTime / fixedDeltaTime);

	numTimeSteps = std::min(numTimeSteps, 5);

	m_LeftOverDeltaTime = deltaTime - (numTimeSteps * fixedDeltaTime);

	for (int i = 0; i <= numTimeSteps; ++i) {
		// Solve constraints.
		for (int j = 0; j <= constraintAccuracy; ++j) {
			for (auto& pointMass : m_PointMasses) {
				pointMass->SolveConstraints();
			}
		}

		// Update masses.
		for (auto& pointMass : m_PointMasses) {
			pointMass->Update(fixedDeltaTime);
		}
	}

}

void CClothSimulatorLogic::Render()
{
	/*// Create vertex buffer.
	std::vector<TVertex> vertices;
	for (auto& pointMass : m_PointMasses) {
		for (const auto& link : pointMass->GetLinks()) {
			vertices.push_back({ pointMass->GetPosition(), CVector3::s_FORWARD });
			vertices.push_back({ link.GetPointMassB()->GetPosition(), CVector3::s_FORWARD });
		}
	}

	auto pBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(vertices, EGpuBufferUsage::STATIC);*/

	m_Camera.UpdateViewTransform();

	m_pVSParams->SetConstant("gWVP", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());
	m_ClothRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_pVSParams);

	g_pApp->GetRenderer()->SetRenderPass(&m_ClothRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::LINELIST, m_pClothVertexBuffer);
}

void CClothSimulatorLogic::HandleInput(const CInput& input, float deltaTime)
{
	/*if (input.GetKeyRelease(EKeyCode::F1)) {
	g_pApp->GetRenderer()->ToggleWindowed(); // If only :(
	}*/

	CPoint mousePosition = input.GetMousePosition();

	// Handle rotation of camera.
	if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		CPoint deltaPosition = mousePosition - m_lastMousePosition;
		m_CameraYaw -= deltaPosition.GetX() * 0.01f;
		m_CameraPitch -= deltaPosition.GetY() * 0.01f;

		m_Camera.SetRotation(CQuaternion(m_CameraYaw, m_CameraPitch, 0.0f));
	}

	m_lastMousePosition = mousePosition;

	// Handle translation of camera.
	static float speed = 5.0f;
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

void CClothSimulatorLogic::CreateCloth(int width, int height, float restingDistance, float stiffness, float breakingForce)
{
	float midWidth = (width * restingDistance) / 2.0f;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			CVector3 position(restingDistance * x + midWidth, y * restingDistance, 0.0f);

			auto pPointMass = std::make_unique<CPointMass>(position);

			if (x != 0) {
				pPointMass->Attach(m_PointMasses.back().get(), restingDistance, stiffness, breakingForce);
			}

			if (y != 0) {
				pPointMass->Attach(m_PointMasses[(y - 1) * (width + 1) + x].get(), restingDistance, stiffness, breakingForce);
			}

			if (y == 0) {
				pPointMass->Pin(position);
			}

			m_PointMasses.push_back(std::move(pPointMass));
		}
	}
}
