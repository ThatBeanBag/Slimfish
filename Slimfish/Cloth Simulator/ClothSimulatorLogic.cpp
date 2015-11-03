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
#include <Graphics/SlimGpuBuffer.h>
#include "Meshes.h"

const size_t CClothSimulatorLogic::s_SHADOW_MAP_WIDTH = 2048;
const size_t CClothSimulatorLogic::s_SHADOW_MAP_HEIGHT = 2048;

const size_t CClothSimulatorLogic::s_MAX_CLOTH_SIZE = 60;
const size_t CClothSimulatorLogic::s_MIN_CLOTH_SIZE = 5;
const size_t CClothSimulatorLogic::s_MAX_HOOKS = 10;
const size_t CClothSimulatorLogic::s_MIN_HOOKS = 1;
const int CClothSimulatorLogic::s_SLIDER_MIN_X = 105;
const int CClothSimulatorLogic::s_SLIDER_MAX_X = 252;

int IntLerp(int from, int to, float t)
{
	return static_cast<int>((t * static_cast<float>(to)) + (static_cast<float>(from) * (1 - t)));
}

float GetLerpAmount(int min, int max, int val) {
	return (val - min) / static_cast<float>(max - min);
}

struct TVertex {
	CVector3 position;
	CVector3 normal;
	float u;
	float v;
	CColourValue colour;
};

CClothSimulatorLogic::CClothSimulatorLogic()
	:m_Camera(nullptr),
	m_LightCamera(nullptr),
	m_Light(nullptr),
	m_fTimeScale(1.0f),
	m_SliderRectNumHooks(s_SLIDER_MIN_X, 10, 10, 25),
	m_SliderRectClothWidth(s_SLIDER_MIN_X, 40, 10, 25),
	m_SliderRectClothHeight(s_SLIDER_MIN_X, 70, 10, 25),
	m_pDraggingRect(nullptr),
	m_Sphere(3.1f),
	m_GroundPlane(CPlane(CVector3(0.0f, 0.1f, 0.0f), CVector3(0, 1, 0))),
	m_Pyrimad(6.7f, 6.7f, 6.7f),
	m_Capsule(2.1f, 6.1f)
{
	m_Sphere.SetTransform(CMatrix4x4::BuildTranslation(0.0f, 7.0f, -2.0f));
	m_Pyrimad.SetTransform(CMatrix4x4::BuildTranslation(0.0f, 7.0f, -2.0f));
	m_Capsule.SetTransform(CMatrix4x4::BuildTranslation(0.0f, 7.0f, -2.0f));
	
	m_Cloth.SetNumPointMassesX(30);
	m_Cloth.SetNumPointMassesY(30);
	m_Cloth.SetNumHooks(4);
	m_Cloth.SetHungFromHeight(34.0f);
	m_Cloth.SetLinkBreakingDistance(1000.0f);
	m_Cloth.SetRestingDistance(1.0f);
	m_Cloth.SetStiffness(0.9f);
	m_Cloth.SetPointMassDamping(0.1f);
	m_Cloth.SetPointMassMass(0.3f);

	auto width = GetLerpAmount(s_MIN_CLOTH_SIZE, s_MAX_CLOTH_SIZE, m_Cloth.GetNumPointMassesX());
	auto height = GetLerpAmount(s_MIN_CLOTH_SIZE, s_MAX_CLOTH_SIZE, m_Cloth.GetNumPointMassesX());

	m_SliderRectClothWidth.SetX(IntLerp(s_SLIDER_MIN_X, s_SLIDER_MAX_X, width));
	m_SliderRectClothHeight.SetX(IntLerp(s_SLIDER_MIN_X, s_SLIDER_MAX_X, height));
}

CClothSimulatorLogic::~CClothSimulatorLogic()
{

}

bool CClothSimulatorLogic::Initialise()
{
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);
	m_VertexDeclaration.AddElement("COLOR", CInputElement::FORMAT_FLOAT4);

	// Create the meshes.
	Meshes::CreateQuad(200, 200, m_pGroundVertexBuffer);
	Meshes::CreatePyramid(m_pPyramidVertexBuffer);
	Meshes::CreateSphere(10, 10, m_pSphereVertexBuffer, m_pSphereIndexBuffer);
	Meshes::CreateCapsule(0.66667f, 2.0f, 10, 10, 30, m_pCapsuleVertexBuffer, m_pCapsuleIndexBuffer);

	// Create the cloth.
	CreateCloth();
	
	// Create 2D Images for user interface sliders.
	m_pSliderBarsImage = g_pApp->GetRenderer()->VLoadTexture("Textures/SliderBars.png");
	m_pSliderImage = g_pApp->GetRenderer()->VLoadTexture("Textures/Slider.png");
	// Initialise the 2D drawer. 
	if (!m_2DDrawer.Initialise()) {
		return false;
	}

	// Create shaders.
	m_ClothRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothVS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_ClothRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothPS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_ClothRenderPass.SetCullingMode(ECullingMode::NONE);

	m_pVSParams = m_ClothRenderPass.GetVertexShader()->GetShaderParams("constantBuffer");

	// Setup lighting.
	m_Light.SetType(LIGHT_DIRECTIONAL);
	m_Light.SetDiffuse(CColourValue(0.7f, 0.7f, 0.7f));
	m_Light.SetSpecular(CColourValue(1.0f, 1.0f, 1.0f, 100.0f));
	m_Light.SetRotation(CQuaternion(Math::DegreesToRadians(45), Math::DegreesToRadians(-45), 0));

	// Create the light camera.
	m_LightCamera.SetProjectionMode(EProjectionMode::PERSPECTIVE);
	m_LightCamera.SetPosition(CVector3(0, 50, 50));
	m_LightCamera.SetRotation(m_Light.GetRotation());
	m_LightCamera.SetNearClipDistance(0.1f);
	m_LightCamera.SetFarClipDistance(100.0f);
	m_LightCamera.SetFieldOfView(90.0f);
	m_LightCamera.SetAspectRatio(static_cast<float>(s_SHADOW_MAP_WIDTH) / static_cast<float>(s_SHADOW_MAP_HEIGHT));
	m_LightCamera.SetOrthographicSize(1.0f);
	m_LightCamera.UpdateViewTransform();

	m_pPSParams = m_ClothRenderPass.GetPixelShader()->GetShaderParams("constantBuffer");
	m_pPSParams->SetConstant("gLight.type", m_Light.GetType());
	m_pPSParams->SetConstant("gLight.diffuse", m_Light.GetDiffuse());
	m_pPSParams->SetConstant("gLight.specular", m_Light.GetSpecular());
	m_pPSParams->SetConstant("gLight.direction", m_Light.GetRotation().GetDirection());
	m_pPSParams->SetConstant("gLight.range", m_Light.GetRange());
	m_pPSParams->SetConstant("gLight.attenuation", m_Light.GetAttenuation());
	m_pPSParams->SetConstant("gAmbientLight", CColourValue(0.3f, 0.3f, 0.3f));

	m_ClothRenderPass.GetPixelShader()->UpdateShaderParams("constantBuffer", m_pPSParams);
	m_ClothRenderPass.AddTextureLayer("Textures/Cloth.jpg");

	// Setup camera.
	m_Camera.SetPosition(CVector3(0.0f, 6.0f, 20.0f));
	m_Camera.SetProjectionMode(EProjectionMode::PERSPECTIVE);
	m_Camera.SetNearClipDistance(0.1f);
	m_Camera.SetFarClipDistance(300.0f);
	m_Camera.SetFieldOfView(Math::DegreesToRadians(60.0f));
	m_Camera.SetOrthographicSize(20.0f);

	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue(0.8f, 0.8f, 0.8f));

	// Setup shadow map.
	m_DepthRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("DepthOnly_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_DepthRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("DepthOnly_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	auto pRenderDepthShaderParams = m_DepthRenderPass.GetVertexShader()->GetShaderParams("constantBuffer");
	pRenderDepthShaderParams->SetConstant("gWorldViewProjMatrix", m_LightCamera.GetViewProjMatrix());
	pRenderDepthShaderParams->SetConstant("gZFar", m_LightCamera.GetFarClipDistance());
	m_DepthRenderPass.GetVertexShader()->UpdateShaderParams("constantBuffer", pRenderDepthShaderParams);

	// Create the shadow map.
	m_pShadowMap = g_pApp->GetRenderer()->VCreateRenderTexture("ShadowMap", s_SHADOW_MAP_WIDTH, s_SHADOW_MAP_HEIGHT);
	//m_ClothRenderPass.AddTextureLayer()->SetTexture(m_pShadowMap->GetTexture());
	m_DepthRenderPass.AddRenderTarget(m_pShadowMap.get());

	m_GroundRenderPass = m_ClothRenderPass;
	m_SphereRenderPass = m_ClothRenderPass;
	m_GroundRenderPass.GetTextureLayer(0)->SetTexture(g_pApp->GetRenderer()->VLoadTexture("Textures/Ground.jpg"));
	m_SphereRenderPass.GetTextureLayer(0)->SetTexture(g_pApp->GetRenderer()->VLoadTexture("Textures/Planet.png"));

	return true;
}

void CClothSimulatorLogic::Update(float deltaTime)
{
	CPoint screenSize = g_pApp->GetRenderer()->GetWindowSize();
	m_Camera.SetAspectRatio(static_cast<float>(screenSize.GetX()) / static_cast<float>(screenSize.GetY()));

	// Update physics.
	m_Cloth.Update(deltaTime * m_fTimeScale);

	if (m_pGrabbedMass) {
		CRay ray = m_Camera.ScreenPointToRay(m_lastMousePosition);
		CVector3 intersectionPoint = ray.GetIntersectionPointOnRay(m_pGrabbedMass->GetPosition());

		m_pGrabbedMass->ApplyForce((intersectionPoint - m_pGrabbedMass->GetPosition()) * 500.0f);
	}

	const auto& pointMasses = m_Cloth.GetPointMasses();

	for (unsigned int i = 0; i < pointMasses.size(); ++i) {
		// Collision with self.
		/*for (unsigned int j = i + 1; j < m_PointMasses.size(); ++j) {
			auto toPointMass = 
				(m_PointMasses[j]->GetPosition()) - 
				(m_PointMasses[i]->GetPosition());

			if (toPointMass.GetLengthSquared() < minClothDistanceSqr) {
				toPointMass = CVector3::Normalise(toPointMass);
				//m_PointMasses[i]->SetPosition(m_PointMasses[j]->GetPosition() - (toPointMass * (minClothDistance + 0.1f)));
				//m_PointMasses[j]->SetPosition(m_PointMasses[j]->GetPosition() + (toPointMass * (minClothDistance + 0.1f)));
				/ *m_PointMasses[i]->SetPosition(m_PointMasses[i]->GetLastPosition());
				m_PointMasses[j]->SetPosition(m_PointMasses[j]->GetLastPosition()); * /
				m_PointMasses[i]->ApplyForce(-toPointMass * (400.0f));
				m_PointMasses[j]->ApplyForce(toPointMass * (400.0f));
			}
		}*/

		// Resolve collisions.
		auto position = pointMasses[i]->GetPosition();
		CVector3 intersectionPoint;
		
		// Handle collision with the ground.
		if (m_GroundPlane.IsInside(position, intersectionPoint)) {
			pointMasses[i]->SetPosition(intersectionPoint);
		}

		// Handle collision with the object.
		switch (m_CurrentObject) {
			case ECollisionObject::NONE: {
				break;
			}
			case ECollisionObject::PYRAMID: {
				if (m_Pyrimad.IsInside(position, intersectionPoint)) {
					pointMasses[i]->SetPosition(intersectionPoint);
				}
				break;
			}
			case ECollisionObject::CAPSULE: {
				if (m_Capsule.IsInside(position, intersectionPoint)) {
					pointMasses[i]->SetPosition(intersectionPoint);
				}
				break;
			}
			case ECollisionObject::SPHERE: {
				if (m_Sphere.IsInside(position, intersectionPoint)) {
					pointMasses[i]->SetPosition(intersectionPoint);
				}
				break;
			}
		}
	}

	UpdateClothVertices();
}

void CClothSimulatorLogic::Render()
{
	m_Camera.UpdateViewTransform();

	// Update shader params.
	m_pVSParams->SetConstant("gWorldViewProjMatrix", m_Camera.GetViewProjMatrix());
	m_pVSParams->SetConstant("gWorldMatrix", CMatrix4x4::s_IDENTITY);
	m_pVSParams->SetConstant("gLightViewProjMatrix", m_LightCamera.GetViewProjMatrix());
	m_ClothRenderPass.GetVertexShader()->UpdateShaderParams("constantBuffer", m_pVSParams);

	m_pPSParams->SetConstant("gEyePosition", m_Camera.GetPosition());
	m_ClothRenderPass.GetPixelShader()->UpdateShaderParams("constantBuffer", m_pPSParams);

	//RenderToShadowMap();

	g_pApp->GetRenderer()->SetRenderPass(&m_ClothRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pClothVertexBuffer, m_pClothIndexBuffer);

	g_pApp->GetRenderer()->SetRenderPass(&m_GroundRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pGroundVertexBuffer);

	CMatrix4x4 spherePosition = CMatrix4x4::BuildTranslation(0.0f, 7.0f, -2.0f) * CMatrix4x4::BuildScale(3.0f, 3.0f, 3.0f);
	m_pVSParams->SetConstant("gWorldViewProjMatrix", m_Camera.GetViewProjMatrix() * spherePosition);
	m_pVSParams->SetConstant("gWorldMatrix", spherePosition);
	m_pVSParams->SetConstant("gLightViewProjMatrix", m_LightCamera.GetViewProjMatrix());
	m_ClothRenderPass.GetVertexShader()->UpdateShaderParams("constantBuffer", m_pVSParams);

	switch (m_CurrentObject) {
		case ECollisionObject::NONE: {
			break;
		}
		case ECollisionObject::PYRAMID: {
			g_pApp->GetRenderer()->SetRenderPass(&m_SphereRenderPass);
			g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLELIST, m_pPyramidVertexBuffer);
			break;
		}
		case ECollisionObject::CAPSULE: {
			g_pApp->GetRenderer()->SetRenderPass(&m_SphereRenderPass);
			g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pCapsuleVertexBuffer, m_pCapsuleIndexBuffer);
			break;
		}
		case ECollisionObject::SPHERE: {
			g_pApp->GetRenderer()->SetRenderPass(&m_SphereRenderPass);
			g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pSphereVertexBuffer, m_pSphereIndexBuffer);
			break;
		}
	}

	RenderUserInterface();
}

void CClothSimulatorLogic::RenderUserInterface()
{
	// Draw the slider bars.
	m_2DDrawer.Render(CRect(10, 10, m_pSliderBarsImage->GetWidth(), m_pSliderBarsImage->GetHeight()), m_pSliderBarsImage);

	// Draw the sliders.
	m_2DDrawer.Render(m_SliderRectNumHooks, m_pSliderImage);
	m_2DDrawer.Render(m_SliderRectClothWidth, m_pSliderImage);
	m_2DDrawer.Render(m_SliderRectClothHeight, m_pSliderImage);
}

void CClothSimulatorLogic::HandleInput(const CInput& input, float deltaTime)
{
	if (input.GetKeyRelease(EKeyCode::F1)) {
		g_pApp->GetRenderer()->ToggleWindowed(); // If only :(
	}

	CPoint mousePosition = input.GetMousePosition();

	if (input.GetKeyRelease(EKeyCode::R)) {
		CreateCloth();
	}

	if (input.GetKeyRelease(EKeyCode::NUM_1)) {
		m_CurrentObject = ECollisionObject::NONE;
	}
	if (input.GetKeyRelease(EKeyCode::NUM_4)) {
		m_CurrentObject = ECollisionObject::CAPSULE;
	}
	else if (input.GetKeyRelease(EKeyCode::NUM_3)) {
		m_CurrentObject = ECollisionObject::SPHERE;
	}
	else if (input.GetKeyRelease(EKeyCode::NUM_2)) {
		m_CurrentObject = ECollisionObject::PYRAMID;
	}

	if (input.GetKeyRelease(EKeyCode::X)) {
		m_Cloth.UnPin();
	}

	if (input.GetKeyPress(EKeyCode::NUM_1)) {
		if (m_ClothRenderPass.GetFillMode() == EFillMode::SOLID) {
			m_ClothRenderPass.SetFillMode(EFillMode::WIREFRAME);
		}
		else {
			m_ClothRenderPass.SetFillMode(EFillMode::SOLID);
		}
	}

	if (input.GetMouseButtonPress(EMouseButton::LEFT)) {
		if (m_SliderRectClothHeight.IsInside(mousePosition)) {
			m_pDraggingRect = &m_SliderRectClothHeight;
		}
		else if (m_SliderRectClothWidth.IsInside(mousePosition)) {
			m_pDraggingRect = &m_SliderRectClothWidth;
		}
		else if (m_SliderRectNumHooks.IsInside(mousePosition)) {
			m_pDraggingRect = &m_SliderRectNumHooks;
		}
	}

	if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		CPoint deltaPosition = mousePosition - m_lastMousePosition;
		
		CRay ray = m_Camera.ScreenPointToRay(mousePosition);
		float distance = 10.0f;
		CPointMass* pMass = nullptr;

		for (auto& pointMass : m_Cloth.GetPointMasses()) {
			float currentDistance = ray.GetDistanceToPointSquared(pointMass->GetPosition());
			if (currentDistance < distance) {
				m_pGrabbedMass = pointMass.get();
				distance = currentDistance;
			}
		}

		if (m_pGrabbedMass) {
			if (m_pGrabbedMass->IsPinned()) {
				m_pGrabbedMass->DetachPin();
			}

			m_pGrabbedMass->ClearLinks();
			UpdateClothIndices();
		}

		// Update slider position.
		if (m_pDraggingRect) {
			m_pDraggingRect->SetX(mousePosition.GetX());
			if (m_pDraggingRect->GetX() < s_SLIDER_MIN_X) {
				m_pDraggingRect->SetX(s_SLIDER_MIN_X);
			}
			else if (m_pDraggingRect->GetX() > s_SLIDER_MAX_X) {
				m_pDraggingRect->SetX(s_SLIDER_MAX_X);
			}
		}
	}

	/*if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		CPoint deltaPosition = mousePosition - m_lastMousePosition;

		CRay ray = m_Camera.ScreenPointToRay(mousePosition);
		float distance = 100000000.0f;
		CPointMass* pMass = nullptr;

		for (auto& pointMass : m_PointMasses) {
			auto toRay = ray.GetIntersectionPointOnRay(pointMass->GetPosition()) - pointMass->GetPosition();
			float currentDistance = toRay.GetLength();

			if (currentDistance < 5.0f) {
				toRay /= currentDistance;
				float force = std::min((1.0f / currentDistance) * 10.0f, 20.0f);

				pointMass->ApplyForce(toRay * force);
			}
		}
	}*/

	if (input.GetMouseButtonRelease(EMouseButton::LEFT)) {
		if (m_pGrabbedMass) {
			m_pGrabbedMass->DetachPin();
			m_pGrabbedMass = nullptr;
		}

		if (m_pDraggingRect) {
			float t = (m_pDraggingRect->GetX() - s_SLIDER_MIN_X) / static_cast<float>(s_SLIDER_MAX_X - s_SLIDER_MIN_X);

			if (m_pDraggingRect == &m_SliderRectClothWidth) {
				m_Cloth.SetNumPointMassesX(IntLerp(s_MIN_CLOTH_SIZE, s_MAX_CLOTH_SIZE, t));

				CreateCloth();
			}
			else if (m_pDraggingRect == &m_SliderRectClothHeight) {
				m_Cloth.SetNumPointMassesY(IntLerp(s_MIN_CLOTH_SIZE, s_MAX_CLOTH_SIZE, t));

				CreateCloth();
			}
			else if (m_pDraggingRect == &m_SliderRectNumHooks) {
				auto numHooks = IntLerp(s_MIN_HOOKS, s_MAX_HOOKS, t);
				m_Cloth.SetNumHooks(numHooks);
				m_Cloth.PinCloth();
			}

			m_pDraggingRect = nullptr;
		}
	}

	if (input.GetKeyPress(EKeyCode::P)) {
		if (m_fTimeScale != 0.0f) {
			m_fTimeScale = 0.0f;
		}
		else {
			m_fTimeScale = 1.0f;
		}
	}

	if (input.IsMouseButtonDown(EMouseButton::RIGHT)) {
		// Push the point masses around.
		CPoint deltaPosition = mousePosition - m_lastMousePosition;

		CRay ray = m_Camera.ScreenPointToRay(mousePosition);
		float distance = 100000000.0f;
		CPointMass* pMass = nullptr;

		for (auto& pointMass : m_Cloth.GetPointMasses()) {
			auto toRay = pointMass->GetPosition() - ray.GetIntersectionPointOnRay(pointMass->GetPosition());
			float currentDistance = toRay.GetLength();

			if (currentDistance < 20.0f) {
				toRay /= currentDistance;
				float force = std::min((1.0f / currentDistance) * 100.0f, 500.0f);

				pointMass->ApplyForce((toRay + m_Camera.GetRotation().GetDirection()) * force);
			}
		}
	}


	// Handle rotation of camera.
	if (input.IsMouseButtonDown(EMouseButton::MIDDLE)) {
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

void CClothSimulatorLogic::UpdateClothVertices()
{
	CGpuBufferLock lock(m_pClothVertexBuffer, 0, m_pClothVertexBuffer->GetSize(), EGpuBufferLockType::DISCARD);
	TVertex* pVertices = reinterpret_cast<TVertex*>(lock.GetLockedContents());

	auto numPointMassesX = m_Cloth.GetNumPointMassesX();
	auto numPointMassesY = m_Cloth.GetNumPointMassesY();
	const auto& pointMasses = m_Cloth.GetPointMasses();

	for (unsigned int y = 0; y < numPointMassesY; ++y) {
		for (unsigned int x = 0; x < numPointMassesX; ++x) {
			TVertex vert;
			vert.position = pointMasses[(y * numPointMassesX) + x]->GetPosition();
			vert.u = x / static_cast<float>(numPointMassesX);
			vert.v = y / static_cast<float>(numPointMassesY);

			pVertices[x + (y * numPointMassesX)] = vert;
		}
	}

	for (unsigned int y = 0; y < numPointMassesY; ++y) {
		for (unsigned int x = 0; x < numPointMassesX; ++x) {
			TVertex& vert = pVertices[(y * numPointMassesX) + x];
			// Calculate normal.
			CVector3 averagedNormal(0, 0, 0);
			CVector3 lastLine(0, 0, 0);

			// Start at the top right neighbour.
			int neighbourX = x + 1;
			int neighbourZ = y - 1;

			auto increment = [&](int _i) {
				// Increment to next neighbour, so that we search 
				// neighbour vertices in counter-clockwise order.
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
					neighbourX >= static_cast<int>(numPointMassesX) ||
					neighbourZ >= static_cast<int>(numPointMassesY)) {
					// Is this neighbour outside the image?
					// Don't evaluate it.
					increment(i);

					continue;
				}

				CVector3 neighbourPosition = pVertices[neighbourX + neighbourZ * numPointMassesX].position;
				CVector3 currentNormal(0, 0, 0);
				CVector3 currentLine = vert.position - neighbourPosition;

				currentNormal = CVector3::CrossProduct(lastLine, currentLine);
				averagedNormal += currentNormal;
				lastLine = currentLine;

				increment(i);
			}

			vert.normal = CVector3::Normalise(averagedNormal);
		}
	}
}

void CClothSimulatorLogic::UpdateClothIndices()
{
	CGpuBufferLock indexLock(m_pClothIndexBuffer, 0, m_pClothIndexBuffer->GetSize(), EGpuBufferLockType::DISCARD);
	int* pIndices = reinterpret_cast<int*>(indexLock.GetLockedContents());

	auto index = 0;
	auto n = m_Cloth.GetNumPointMassesX();
	auto m = m_Cloth.GetNumPointMassesY();

	for (unsigned int z = 0; z < m - 1; ++z) {
		// Even rows move left to right, odd rows move right to left.
		if (z % 2 == 0) {
			bool alreadyGenerate = false;

			// Is this an even row?
			for (int x = 0; x < static_cast<int>(n); ++x) {
				pIndices[index++] = x + z * n;

				// Is the link broken? If so insert a degenerate triangle.
				auto pointMass = m_Cloth.GetPointMass(x, z);
				if (z < m - 1 && !m_Cloth.GetPointMass(x, z + 1)->HasLinkTo(pointMass)) {
					pIndices[index++] = x + z * n;	// Insert degenerate triangle.
				}
				else if (x > 0 && z < m - 1 && !m_Cloth.GetPointMass(x, z + 1)->HasLinkTo(m_Cloth.GetPointMass(x - 1, z + 1))) {
					pIndices[index++] = x + z * n;	// Insert degenerate triangle.
				}

				pIndices[index++] = x + z * n + n;	// Next row.

				// Is the link broken? If so insert a degenerate triangle.
				if (z < m - 1 && !m_Cloth.GetPointMass(x, z + 1)->HasLinkTo(pointMass)) {
					pIndices[index++] = x + (z + 1) * n;	// Insert degenerate triangle.
					alreadyGenerate = true;
				}
				else if (x < n - 1 && !m_Cloth.GetPointMass(x + 1, z)->HasLinkTo(pointMass)) {
					pIndices[index++] = x + (z + 1) * n;	// Insert degenerate triangle.
					alreadyGenerate = true;
				}
			}

			// Insert degenerate vertex, if this isn't the last row and we haven't already.
			if (z != m - 2 && !alreadyGenerate) {
				pIndices[index++] = n - 1 + (z * n);
			}
		}
		else {
			bool alreadyDegenerate = false;

			// This is an odd row.
			for (int x = static_cast<int>(n)-1; x >= 0; --x) {
				pIndices[index++] = x + z * n;

				// Is the link broken? If so, insert a degenerate triangle.
				auto pointMass = m_Cloth.GetPointMass(x, z);
				if (z < m - 1 && !m_Cloth.GetPointMass(x, z + 1)->HasLinkTo(pointMass)) {
					pIndices[index++] = x + z * n;	// Insert degenerate triangle.
				}
				else if (x < n - 1 && z < m - 1 && !m_Cloth.GetPointMass(x + 1, z + 1)->HasLinkTo(m_Cloth.GetPointMass(x, z + 1))) {
					pIndices[index++] = x + z * n;	// Insert degenerate triangle.
				}

				pIndices[index++] = x + z * n + n;	// Next row.

				// Is the link broken? If so, insert a degenerate triangle.
				if (z < m - 1 && !m_Cloth.GetPointMass(x, z + 1)->HasLinkTo(pointMass)) {
					pIndices[index++] = x + (z + 1) * n;	// Insert degenerate triangle.
					alreadyDegenerate = true;
				}
				else if (x > 0 && !pointMass->HasLinkTo(m_Cloth.GetPointMass(x - 1, z))) {
					pIndices[index++] = x + (z + 1) * n;	// Insert degenerate triangle.
					alreadyDegenerate = true;
				}
			}

			// Insert degenerate vertex, if this isn't the last row and we haven't already.
			if (z != m - 2 && !alreadyDegenerate) {
				pIndices[index++] = z * n;
			}
		}
	}

	assert(index < m_pClothIndexBuffer->GetNumIndices());

	for (size_t i = index; i < m_pClothIndexBuffer->GetNumIndices(); ++i) {
		pIndices[i] = pIndices[index - 1];
	}
}

void CClothSimulatorLogic::RenderToShadowMap()
{
	// Set the render target to start writing to the shadow map.
	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue::s_WHITE);
	//g_pApp->GetRenderer()->VPreRender();

	g_pApp->GetRenderer()->SetRenderPass(&m_DepthRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pClothVertexBuffer, m_pClothIndexBuffer);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pGroundVertexBuffer);
}

void CClothSimulatorLogic::CreateCloth()
{
	// Create vertex buffer.
	int numVertices = m_Cloth.GetNumPointMassesX() * m_Cloth.GetNumPointMassesY();
	m_pClothVertexBuffer = g_pApp->GetRenderer()->VCreateVertexBuffer(numVertices, sizeof(TVertex), nullptr, EGpuBufferUsage::WRITE_ONLY);

	// Create index buffer.
	int numIndices = ((m_Cloth.GetNumPointMassesX() * 2) * (m_Cloth.GetNumPointMassesY() - 1) + (m_Cloth.GetNumPointMassesY() - 2));
	// Add a little extra indices in the buffer for degenerate triangles 
	// so that we can removed triangles that have broken links.
	numIndices *= 2;

	m_pClothIndexBuffer = g_pApp->GetRenderer()->VCreateIndexBuffer(numIndices, AIndexGpuBuffer::INDEX_TYPE_32, nullptr, EGpuBufferUsage::WRITE_ONLY);

	m_pGrabbedMass = nullptr;
	m_Cloth.Initalise();

	// Build the cloth vertices and indices initially.
	UpdateClothVertices();
	UpdateClothIndices();
}

/*
void CClothSimulatorLogic::CreatePointMasses(float hungFrom, float linkBreakingForce, float mass, float damping)
{
	m_PointMasses.clear();
	m_PointMasses.reserve(width * height);
	float midWidth = (width * restingDistance) / 2.0f;
	float midHeight = (height * restingDistance) / 2.0f;

	for (int y = 0; y < height; ++y) {
		float yPosition = hungFrom - (y * m_ClothRestingDistance);

		for (int x = 0; x < width; ++x) {
			CVector3 position(restingDistance * x - midWidth, ((height - y) * restingDistance) + 5.0f, 0.0f);

			auto pPointMass = std::make_unique<CPointMass>(position, 1.0f, 0.01f);
			float diagonalRestingDistance = sqrt(2 * (m_ClothRestingDistance * m_ClothRestingDistance));

			// Create links.
			if (x != 0) {	// Horizontal link.
				pPointMass->Attach(
					GetPointMass(x - 1, y),
					m_ClothRestingDistance,
					m_ClothStiffness,
					linkBreakingForce);

				// Create tightly bound link.
				if (y != 0) {
					pPointMass->Attach(
						GetPointMass(x - 1, y - 1),
						diagonalRestingDistance,
						m_ClothStiffness / 3.0f,
						linkBreakingForce,
						false);
				}
			}

			if (y != 0) {	// Vertical link.
				pPointMass->Attach(
					GetPointMass(x, y - 1),
					m_ClothRestingDistance,
					m_ClothStiffness,
					linkBreakingForce);

				// Create tightly bound link.
				if (x < width - 1) {
					pPointMass->Attach(
						GetPointMass(x + 1, y - 1),
						diagonalRestingDistance,
						m_ClothStiffness / 3.0f,
						linkBreakingForce,
						false);
				}
			}

			m_PointMasses.push_back(std::move(pPointMass));
		}
	}
}


void CClothSimulatorLogic::CreatePointMasses(float width, float height, float hungFrom, float stiffness, float breakingForce)
{
	m_PointMasses.clear();
	m_PointMasses.reserve(m_ClothWidth * m_ClothHeight);

	m_ClothRestingDistance = width / m_ClothWidth;
	m_ClothStiffness = stiffness;
	m_ClothHungFromHeight = hungFrom;

	float midWidth = width / 2.0f;
	float midHeight = height / 2.0f;

	for (int y = 0; y < m_ClothHeight; ++y) {
		float yPosition = hungFrom - (y * m_ClothRestingDistance);

		for (int x = 0; x < m_ClothWidth; ++x) {
			CVector3 position(m_ClothRestingDistance * x - midWidth, yPosition, 0.0f);

			auto pPointMass = std::make_unique<CPointMass>(position, 1.0f, 0.01f);
			float diagonalRestingDistance = sqrt(2 * (m_ClothRestingDistance * m_ClothRestingDistance));

			// Create links.
			if (x != 0) {	// Horizontal link.
				pPointMass->Attach(
					GetPointMass(x - 1, y),
					m_ClothRestingDistance,
					stiffness, 
					breakingForce);

				// Create tightly bound link.
				if (y != 0) {
					pPointMass->Attach(
						GetPointMass(x - 1, y - 1), 
						diagonalRestingDistance, 
						stiffness / 3.0f, 
						breakingForce, 
						false);
				}
			}

			if (y != 0) {	// Vertical link.
				pPointMass->Attach(
					GetPointMass(x, y - 1), 
					m_ClothRestingDistance,
					stiffness,
					breakingForce);

				// Create tightly bound link.
				if (x < width - 1) {
					pPointMass->Attach(
						GetPointMass(x + 1, y - 1),
						diagonalRestingDistance, 
						stiffness / 3.0f,
						breakingForce,
						false);
				}
			}

			m_PointMasses.push_back(std::move(pPointMass));
		}
	}

	PinCloth();
}

void CClothSimulatorLogic::PinCloth()
{
	for (int x = 0; x < m_ClothWidth && x < m_PointMasses.size(); ++x) {
		auto pointsPerHook = static_cast<int>(m_ClothWidth / static_cast<float>(m_ClothHooks));
		pointsPerHook = std::max(pointsPerHook, 1); // Avoids division by 0.

		if (x % pointsPerHook == 0 || x == m_ClothWidth - 1) {
			CVector3 position(m_ClothRestingDistance * x - 45.0f / 2.0f, m_ClothHungFromHeight, 0.0f);
			GetPointMass(x, 0)->Pin(position);
		}
	}
}

CPointMass* CClothSimulatorLogic::GetPointMass(size_t x, size_t y)
{
	return m_PointMasses[x + y * m_ClothWidth].get();
}
*/
