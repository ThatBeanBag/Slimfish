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
	m_Light(nullptr),
	m_DisplayShadowMap(false),
	m_DisplayControls(true)
{
	m_Light.SetType(LIGHT_DIRECTIONAL);
	m_Light.SetDiffuse(ToColourValue(CColour(255, 184, 19)));
	m_Light.SetDiffuse(ToColourValue(CColour(222, 222, 222)));
	m_Light.SetSpecular(ToColourValue(CColour(255, 255, 255)));
	m_Light.SetRotation(CQuaternion(Math::DegreesToRadians(-45), Math::DegreesToRadians(-45), 0));
}

CMarchingCubesLogic::~CMarchingCubesLogic()
{

}

bool CMarchingCubesLogic::Initialise()
{
	// Setup camera.
	m_Camera.SetPosition(CVector3(0.0f, 1.0f, 1.0f));
	m_Camera.SetPerspective(Math::DegreesToRadians(60.0f), 1.0f, 0.1f, 200.0f);
	g_pApp->GetRenderer()->SetBackgroundColour(ToColourValue(CColour(135, 206, 250)));

	if (!m_2DRenderer.Initialise()) {
		return false;
	}

	m_pControlsImage = g_pApp->GetRenderer()->VLoadTexture("Textures/Controls.png");

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

	CCamera lightCamera(nullptr);
	lightCamera.SetOrthographic(100.0f, 1.0f, 0.0f, m_Camera.GetFarClipDistance());
	lightCamera.SetPosition(m_Camera.GetPosition() - m_Light.GetRotation().GetDirection() * (lightCamera.GetFarClipDistance() / 2.0f));
	lightCamera.SetRotation(m_Light.GetRotation());
	lightCamera.UpdateViewTransform();
	//CCamera lightCamera = m_Camera;
	//lightCamera.SetAspectRatio(1.0f);
	//lightCamera.SetPosition(m_Camera.GetPosition() - m_Light.GetRotation().GetDirection() * 100);
	//lightCamera.SetRotation(m_Light.GetRotation());
	//lightCamera.UpdateViewTransform();

	m_ChunkManager.Update(m_Camera);
	m_ChunkManager.DrawChunks(m_Camera, lightCamera, m_Light);

	if (m_DisplayShadowMap) {
		m_2DRenderer.Render(CRect(0, 155, 300, 300), m_ChunkManager.GetShadowMap());
	}

	if (m_DisplayControls) {
		m_2DRenderer.Render(CRect(0, 0, m_pControlsImage->GetWidth(), m_pControlsImage->GetHeight()), m_pControlsImage);
	}
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
		m_ChunkManager.ToggleWireFrame();
	}
	if (input.GetKeyPress(EKeyCode::NUM_2)) {
		m_ChunkManager.ToggleAmbientOcclusionEnabled();
	}
	if (input.GetKeyPress(EKeyCode::NUM_3)) {
		m_ChunkManager.ToggleShadows();
	}
	if (input.GetKeyPress(EKeyCode::NUM_4)) {
		m_DisplayShadowMap = !m_DisplayShadowMap;
	}
	if (input.GetKeyPress(EKeyCode::NUM_5)) {
		m_ChunkManager.ToggleRenderPoints();
	}
	if (input.GetKeyPress(EKeyCode::H)) {
		m_DisplayControls = !m_DisplayControls;
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
