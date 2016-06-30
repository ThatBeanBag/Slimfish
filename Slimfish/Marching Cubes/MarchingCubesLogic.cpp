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

struct TSphereVertex {
	CVector3 position;
	CVector2 texCoord;
};

CMarchingCubesLogic::CMarchingCubesLogic()
	:m_Camera(nullptr),
	m_Light(nullptr),
	m_LightCamera(nullptr),
	m_DisplayShadowMap(false),
	m_DisplayControls(true)
{
	m_LightYaw = Math::DegreesToRadians(-45);
	m_LightPitch = Math::DegreesToRadians(-45);

	m_Light.SetType(LIGHT_DIRECTIONAL);
	m_Light.SetDiffuse(ToColourValue(CColour(255, 184, 19)));
	m_Light.SetDiffuse(ToColourValue(CColour(222, 222, 222)));
	m_Light.SetSpecular(ToColourValue(CColour(255, 255, 255)));
	m_Light.SetRotation(CQuaternion(m_LightYaw, m_LightPitch, 0));

	m_Camera.SetPosition(CVector3(0.0f, 1.0f, 1.0f));
	m_Camera.SetPerspective(Math::DegreesToRadians(60.0f), 1.0f, 0.1f, 200.0f);

	m_LightCamera.SetOrthographic(100.0f, 1.0f, 0.0f, m_Camera.GetFarClipDistance());

	m_SkyDomeScale = { 
		m_Camera.GetFarClipDistance() * 0.9f,
		m_Camera.GetFarClipDistance() * 0.9f,
		m_Camera.GetFarClipDistance() * 0.9f,
	};
}

CMarchingCubesLogic::~CMarchingCubesLogic()
{

}

bool CMarchingCubesLogic::Initialise()
{
	// Create vertex declarations
	m_SphereVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_SphereVertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);

	// Setup camera.
	g_pApp->GetRenderer()->SetBackgroundColour(ToColourValue(CColour(135, 206, 250)));

	if (!m_2DRenderer.Initialise()) {
		return false;
	}

	m_pControlsImage = g_pApp->GetRenderer()->VLoadTexture("Textures/Controls.png");

	m_DrawSkyDomePass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("SkyBox_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_DrawSkyDomePass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("SkyBox_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_DrawSkyDomePass.AddTextureLayer("Textures/skybox.dds");
	m_DrawSkyDomePass.SetDepthCheckEnabled(false);
	m_DrawSkyDomePass.SetCullingMode(ECullingMode::NONE);

	m_pSkyBoxParams = m_DrawSkyDomePass.GetVertexShader()->GetShaderParams("CBPerFrame");

	CreateSkyDome(10, 10);

	return m_ChunkManager.Initialise();
}

void CMarchingCubesLogic::Update(float deltaTime)
{
	CPoint<> screenSize = g_pApp->GetRenderer()->GetWindowSize();
	m_Camera.SetAspectRatio(static_cast<float>(screenSize.GetX()) / static_cast<float>(screenSize.GetY()));
}

void CMarchingCubesLogic::Render()
{
	// Update camera.
	m_Camera.UpdateViewTransform();

	// Update sky box parameters.
	m_pSkyBoxParams->SetConstant("gWorldViewProjectionMatrix", m_Camera.GetViewProjMatrix() *
		CMatrix4x4::BuildTransform(m_Camera.GetPosition(), m_SkyDomeScale, CQuaternion::s_IDENTITY));
	m_DrawSkyDomePass.GetVertexShader()->UpdateShaderParams("CBPerFrame", m_pSkyBoxParams);

	m_LightCamera.SetPosition(m_Camera.GetPosition() - m_Light.GetRotation().GetDirection() * (m_LightCamera.GetFarClipDistance() / 2.0f));
	m_LightCamera.SetRotation(m_Light.GetRotation());
	m_LightCamera.UpdateViewTransform();

	// Draw the sky dome.
	g_pApp->GetRenderer()->SetRenderPass(&m_DrawSkyDomePass);
	g_pApp->GetRenderer()->VRender(m_SphereVertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pSphereVertices, m_pSphereIndices);

	// Load and draw chunks.
	m_ChunkManager.Update(m_Camera);
	m_ChunkManager.DrawChunks(m_Camera, m_LightCamera, m_Light);

	// Draw UI elements.
	if (m_DisplayShadowMap) {
		m_2DRenderer.Render(CRect<>(0, 170, 300, 300), m_ChunkManager.GetShadowMap());
	}

	if (m_DisplayControls) {
		m_2DRenderer.Render(CRect<>(0, 0, m_pControlsImage->GetWidth(), m_pControlsImage->GetHeight()), m_pControlsImage);
	}
}

void CMarchingCubesLogic::HandleInput(const CInput& input, float deltaTime)
{
	const auto& mousePosition = input.GetMousePosition();

	// Handle rotation of camera.
	if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		auto deltaPosition = mousePosition - m_LastMousePosition;
		m_CameraYaw -= deltaPosition.GetX() * 0.005f;
		m_CameraPitch -= deltaPosition.GetY() * 0.005f;

		m_Camera.SetRotation(CQuaternion(m_CameraYaw, m_CameraPitch, 0.0f));
	}

	if (input.IsMouseButtonDown(EMouseButton::RIGHT)) {
		auto deltaPosition = mousePosition - m_LastMousePosition;
		m_LightYaw -= deltaPosition.GetX() * 0.005f;
		m_LightPitch -= deltaPosition.GetY() * 0.005f;

		m_Light.SetRotation(CQuaternion(m_LightYaw, m_LightPitch, 0.0f));
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

void CMarchingCubesLogic::CreateSkyDome(size_t rings, size_t segments)
{
	std::vector<TSphereVertex> vertices;
	std::vector<int> indices;

	float fDeltaRingAngle = Math::s_PI / rings;
	float fDeltaSegAngle = 2 * Math::s_PI / segments;

	for (size_t ring = 0; ring <= rings; ++ring) {
		float r0 = std::sinf(ring * fDeltaRingAngle);
		float y0 = std::cosf(ring * fDeltaRingAngle);

		for (size_t seg = 0; seg <= segments; ++seg) {
			float x0 = r0 * std::sinf(seg * fDeltaSegAngle);
			float z0 = r0 * std::cosf(seg * fDeltaSegAngle);

			TSphereVertex vertex;

			vertex.position = CVector3(x0, y0, z0);
			vertex.texCoord = {
				static_cast<float>(seg) / static_cast<float>(segments),
				static_cast<float>(ring) / static_cast<float>(rings),
			};
	
			vertices.push_back(vertex);
		}
	}

	for (size_t ring = 0; ring < rings; ++ring) {
		for (size_t seg = 0; seg < segments; ++seg) {
			int first = (ring * (segments + 1) + seg);
			int second = first + segments + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	m_pSphereVertices = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
	m_pSphereIndices = g_pApp->GetRenderer()->CreateIndexBuffer(indices);
}
