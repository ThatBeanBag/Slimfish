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
#include <numeric>

// This Include
#include "ClothSimulatorLogic.h"

// Local Includes
#include <Graphics/SlimRenderer.h>
#include <Graphics/SlimGpuBuffer.h>
#include "Meshes.h"

const size_t CClothSimulatorLogic::s_SHADOW_MAP_WIDTH = 2048;
const size_t CClothSimulatorLogic::s_SHADOW_MAP_HEIGHT = 2048;

const size_t CClothSimulatorLogic::s_MAX_CLOTH_SIZE = 35;
const size_t CClothSimulatorLogic::s_MIN_CLOTH_SIZE = 5;
const size_t CClothSimulatorLogic::s_MAX_HOOKS = 10;
const size_t CClothSimulatorLogic::s_MIN_HOOKS = 1;
const size_t CClothSimulatorLogic::s_MIN_SLIDER_X = 105;
const size_t CClothSimulatorLogic::s_MAX_SLIDER_X = 252;
const float CClothSimulatorLogic::s_MIN_FAN_FORCE = 0.0f;
const float CClothSimulatorLogic::s_MAX_FAN_FORCE = 50.0f;

size_t Lerp(size_t from, size_t to, float t)
{
	return static_cast<size_t>((t * static_cast<float>(to)) + (static_cast<float>(from) * (1 - t)));
}

float Lerp(float from, float to, float t)
{
	return (t * to) + (from * (1 - t));
}

float GetLerpAmount(size_t min, size_t max, size_t val) {
	return (val - min) / static_cast<float>(max - min);
}

float GetLerpAmount(float min, float max, float val) {
	return (val - min) / (max - min);
}

struct TVertex {
	CVector3 position;
	CVector3 normal;
	float u;
	float v;
	float burntLevel;
	CColourValue colour;
};

CClothSimulatorLogic::CClothSimulatorLogic()
	:m_Camera(nullptr),
	m_LightCamera(nullptr),
	m_Light(nullptr),
	m_fTimeScale(1.0f),
	m_SliderRectNumHooks(s_MIN_SLIDER_X, 10, 10, 25),
	m_SliderRectClothWidth(s_MIN_SLIDER_X, 40, 10, 25),
	m_SliderRectClothHeight(s_MIN_SLIDER_X, 70, 10, 25),
	m_SliderRectFanForce(s_MIN_SLIDER_X, 100, 10, 25),
	m_pDraggingRect(nullptr),
	m_Sphere(6.2f),
	m_GroundPlane(CPlane(CVector3(0.0f, 0.1f, 0.0f), CVector3(0, 1, 0))),
	m_Pyrimad(12.7f, 12.7f, 12.7f),
	m_Capsule(4.2f, 12.2f),
	m_FanForce(s_MIN_FAN_FORCE),
	m_FanPosition(CVector3::s_ZERO)
{
	m_CapsuleScale = { 6.0f, 6.0f, 6.0f };
	m_SphereScale = { 6.0f, 6.0f, 6.0f };
	m_PyramidScale = { 9.0f, 9.0f, 9.0f };
	m_Sphere.SetTransform(CMatrix4x4::BuildTranslation(0.0f, 10.0f, 5.0f));
	m_Pyrimad.SetTransform(CMatrix4x4::BuildTranslation(0.0f, 10.0f, 5.0f));
	m_Capsule.SetTransform(CMatrix4x4::BuildTranslation(0.0f, 10.0f, 5.0f));
	
	m_Cloth.SetNumPointMassesX(30);
	m_Cloth.SetNumPointMassesY(30);
	m_Cloth.SetNumHooks(4);
	m_Cloth.SetHungFromHeight(s_MAX_CLOTH_SIZE + 3.0f);
	m_Cloth.SetLinkBreakingDistance(4.0f);
	m_Cloth.SetRestingDistance(1.0f);
	m_Cloth.SetStiffness(0.9f);
	m_Cloth.SetPointMassDamping(0.1f);
	m_Cloth.SetPointMassMass(0.3f);

	auto tWidth = GetLerpAmount(s_MIN_CLOTH_SIZE, s_MAX_CLOTH_SIZE, m_Cloth.GetNumPointMassesX());
	auto tHeight = GetLerpAmount(s_MIN_CLOTH_SIZE, s_MAX_CLOTH_SIZE, m_Cloth.GetNumPointMassesX());
	auto tNumHooks = GetLerpAmount(s_MIN_HOOKS, s_MAX_HOOKS, m_Cloth.GetNumHooks());
	auto tFanForce = GetLerpAmount(s_MIN_FAN_FORCE, s_MAX_FAN_FORCE, m_FanForce);

	m_SliderRectClothWidth.SetX(Lerp(s_MIN_SLIDER_X, s_MAX_SLIDER_X, tWidth));
	m_SliderRectClothHeight.SetX(Lerp(s_MIN_SLIDER_X, s_MAX_SLIDER_X, tHeight));
	m_SliderRectNumHooks.SetX(Lerp(s_MIN_SLIDER_X, s_MAX_SLIDER_X, tNumHooks));
	m_SliderRectFanForce.SetX(Lerp(s_MIN_SLIDER_X, s_MAX_SLIDER_X, tFanForce));
}

CClothSimulatorLogic::~CClothSimulatorLogic()
{

}

bool CClothSimulatorLogic::Initialise()
{
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);
	m_VertexDeclaration.AddElement("BURNT", CInputElement::FORMAT_FLOAT);
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
	m_pControlsImage = g_pApp->GetRenderer()->VLoadTexture("Textures/Controls.png");
	// Initialise the 2D drawer. 
	if (!m_2DDrawer.Initialise()) {
		return false;
	}

	// Create shaders.
	m_ClothRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothVS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_ClothRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothPS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_ClothRenderPass.SetCullingMode(ECullingMode::NONE);
	m_pVSParams = m_ClothRenderPass.GetVertexShader()->GetShaderParams("constantBuffer");

	m_SkyBoxRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("SkyBox_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_SkyBoxRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("SkyBox_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_SkyBoxRenderPass.AddTextureLayer("Textures/skybox.dds");
	m_SkyBoxRenderPass.SetCullingMode(ECullingMode::NONE);
	m_SkyBoxRenderPass.SetDepthCompareFunction(EComparisonFunction::LESS_EQUAL);
	m_pSkyBoxVSParams = m_SkyBoxRenderPass.GetVertexShader()->GetShaderParams("constantBuffer");

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
	m_Camera.SetPosition(CVector3(0.0f, 20.0f, 50.0f));
	m_Camera.SetProjectionMode(EProjectionMode::PERSPECTIVE);
	m_Camera.SetNearClipDistance(0.1f);
	m_Camera.SetFarClipDistance(1000.0f);
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
	if (m_fTimeScale > 0.0f) {
		m_Cloth.Update(deltaTime * m_fTimeScale);
	}

	if (m_pGrabbedMass) {
		CRay ray = m_Camera.ScreenPointToRay(m_lastMousePosition);
		CVector3 intersectionPoint = ray.GetIntersectionPoint(m_pGrabbedMass->GetPosition());

		m_pGrabbedMass->ApplyForce((intersectionPoint - m_pGrabbedMass->GetPosition()) * 1000.0f);
		//m_pGrabbedMass->Pin(intersectionPoint);
	}

	const auto& pointMasses = m_Cloth.GetPointMasses();

	for (auto& pointMass : pointMasses) {
		// Add fan force.
		auto fanToPoint = pointMass->GetPosition() - m_FanPosition;
		pointMass->ApplyForce(CVector3::Normalise(fanToPoint) * m_FanForce);

		// Resolve collisions.
		auto position = pointMass->GetPosition();
		CVector3 intersectionPoint;
		
		// Handle collision with the ground.
		if (m_GroundPlane.IsInside(position, intersectionPoint)) {
			pointMass->SetPosition(intersectionPoint);
		}

		// Handle collision with the object.
		switch (m_CurrentObject) {
			case ECollisionObject::NONE: {
				break;
			}
			case ECollisionObject::PYRAMID: {
				if (m_Pyrimad.IsInside(position, intersectionPoint)) {
					pointMass->SetPosition(intersectionPoint);
				}
				break;
			}
			case ECollisionObject::CAPSULE: {
				if (m_Capsule.IsInside(position, intersectionPoint)) {
					pointMass->SetPosition(intersectionPoint);
				}
				break;
			}
			case ECollisionObject::SPHERE: {
				if (m_Sphere.IsInside(position, intersectionPoint)) {
					pointMass->SetPosition(intersectionPoint);
				}
				break;
			}
		}
	}

	UpdateClothIndices();
	UpdateClothVertices();
}

void CClothSimulatorLogic::Render()
{
	m_Camera.UpdateViewTransform();

	// Update shader params.
	auto skyboxWorldMatrix = CMatrix4x4::BuildTransform(/*m_Camera.GetPosition()*/CVector3::s_ZERO, { 999.0f, 999.0f, 999.0f }, CQuaternion::s_IDENTITY);
	m_pSkyBoxVSParams->SetConstant("gWorldViewProjMatrix", m_Camera.GetViewProjMatrix() * skyboxWorldMatrix);
	m_SkyBoxRenderPass.GetVertexShader()->UpdateShaderParams("constantBuffer", m_pSkyBoxVSParams);

	m_pVSParams->SetConstant("gWorldViewProjMatrix", m_Camera.GetViewProjMatrix());
	m_pVSParams->SetConstant("gWorldMatrix", CMatrix4x4::s_IDENTITY);
	m_pVSParams->SetConstant("gLightViewProjMatrix", m_LightCamera.GetViewProjMatrix());
	m_ClothRenderPass.GetVertexShader()->UpdateShaderParams("constantBuffer", m_pVSParams);

	m_pPSParams->SetConstant("gEyePosition", m_Camera.GetPosition());
	m_ClothRenderPass.GetPixelShader()->UpdateShaderParams("constantBuffer", m_pPSParams);

	//RenderToShadowMap();

	g_pApp->GetRenderer()->SetRenderPass(&m_SkyBoxRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pSphereVertexBuffer, m_pSphereIndexBuffer);

	g_pApp->GetRenderer()->SetRenderPass(&m_ClothRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pClothVertexBuffer, m_pClothIndexBuffer);

	//g_pApp->GetRenderer()->SetRenderPass(&m_GroundRenderPass);
	//g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pGroundVertexBuffer);

	auto updateParams = [this](const CMatrix4x4& worldTransform) {
		m_pVSParams->SetConstant("gWorldViewProjMatrix", m_Camera.GetViewProjMatrix() * worldTransform);
		m_pVSParams->SetConstant("gWorldMatrix", worldTransform);
		m_pVSParams->SetConstant("gLightViewProjMatrix", m_LightCamera.GetViewProjMatrix());
		m_ClothRenderPass.GetVertexShader()->UpdateShaderParams("constantBuffer", m_pVSParams);
	};

	switch (m_CurrentObject) {
		case ECollisionObject::NONE: {
			break;
		}
		case ECollisionObject::PYRAMID: {
			updateParams(m_Pyrimad.GetTransform() * CMatrix4x4::BuildScale(m_PyramidScale));

			g_pApp->GetRenderer()->SetRenderPass(&m_SphereRenderPass);
			g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLELIST, m_pPyramidVertexBuffer);
			break;
		}
		case ECollisionObject::CAPSULE: {
			updateParams(m_Capsule.GetTransform() * CMatrix4x4::BuildScale(m_CapsuleScale));

			g_pApp->GetRenderer()->SetRenderPass(&m_SphereRenderPass);
			g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pCapsuleVertexBuffer, m_pCapsuleIndexBuffer);
			break;
		}
		case ECollisionObject::SPHERE: {
			updateParams(m_Sphere.GetTransform() * CMatrix4x4::BuildScale(m_SphereScale));

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
	m_2DDrawer.Render(
		CRect(10, 
		20 + m_pSliderBarsImage->GetHeight(), 
		m_pControlsImage->GetWidth(),
		m_pControlsImage->GetHeight()),
		m_pControlsImage);

	// Draw the sliders.
	m_2DDrawer.Render(m_SliderRectNumHooks, m_pSliderImage);
	m_2DDrawer.Render(m_SliderRectClothWidth, m_pSliderImage);
	m_2DDrawer.Render(m_SliderRectClothHeight, m_pSliderImage);
	m_2DDrawer.Render(m_SliderRectFanForce, m_pSliderImage);
}

void CClothSimulatorLogic::HandleInput(const CInput& input, float deltaTime)
{
	if (input.GetKeyRelease(EKeyCode::F1)) {
		g_pApp->GetRenderer()->ToggleWindowed(); // If only :(
	}

	const auto& mousePosition = input.GetMousePosition();

	// Reset the cloth with R.
	if (input.GetKeyRelease(EKeyCode::R)) {
		CreateCloth();
	}

	// Move the pins in and out with keys 2 & 3.
	if (input.IsKeyDown(EKeyCode::NUM_2) || input.IsKeyDown(EKeyCode::NUM_3)) {
		const auto& pinnedPoints = m_Cloth.GetPinnedPoints();

		auto speed = 0.0f;
		if (input.IsKeyDown(EKeyCode::NUM_2) && !input.IsKeyDown(EKeyCode::NUM_3)) {
			speed = 5.0f;
		}
		else if (input.IsKeyDown(EKeyCode::NUM_3) && !input.IsKeyDown(EKeyCode::NUM_2)) {
			speed = -5.0f;
		}

		if (!pinnedPoints.empty()) {
			auto addPinnedPoint = [](const CVector3& total, const CPointMass* pPoint) {
				return total + pPoint->GetPinPosition();
			};

			auto average = std::accumulate(pinnedPoints.begin(), pinnedPoints.end(), CVector3::s_ZERO, addPinnedPoint);
			average /= static_cast<float>(pinnedPoints.size());

			for (auto& pPinnedPoint : pinnedPoints) {
				auto toCentre = average - pPinnedPoint->GetPinPosition();
				toCentre = CVector3::Normalise(toCentre);
				pPinnedPoint->Pin(pPinnedPoint->GetPinPosition() + (toCentre * deltaTime * speed));
			}
		}
	}

	if (input.GetKeyRelease(EKeyCode::NUM_4)) {
		if (m_Cloth.GetLinkBreakingDistance() == std::numeric_limits<float>::max()) {
			m_Cloth.SetLinkBreakingDistance(4.0f);
		}
		else {
			m_Cloth.SetLinkBreakingDistance(std::numeric_limits<float>::max());
		}
	}
	
	// Cycle collision shapes with V.
	if (input.GetKeyPress(EKeyCode::V)) {
		int nextObject = static_cast<int>(m_CurrentObject) + 1;
		if (nextObject < static_cast<int>(ECollisionObject::MAX)) {
			m_CurrentObject = static_cast<ECollisionObject>(nextObject);
		}
		else {
			m_CurrentObject = ECollisionObject::NONE;
		}
	}

	if (input.GetKeyPress(EKeyCode::F)) {
		// Get the point on the ray. 
		auto ray = m_Camera.ScreenPointToRay(mousePosition);
		auto pointOnGround = CVector3::s_ZERO;
		auto hitsGroundPlane = ray.GetIntersectionPoint(m_GroundPlane.GetPlane(), pointOnGround);
		if (hitsGroundPlane) {
			m_FanPosition = pointOnGround;
		}
	}

	// Unpin the cloth with X.
	if (input.GetKeyRelease(EKeyCode::X)) {
		m_Cloth.UnPin();
	}

	// Enter wire frame mode with 1.
	if (input.GetKeyPress(EKeyCode::NUM_1)) {
		if (m_ClothRenderPass.GetFillMode() == EFillMode::SOLID) {
			m_ClothRenderPass.SetFillMode(EFillMode::WIREFRAME);
		}
		else {
			m_ClothRenderPass.SetFillMode(EFillMode::SOLID);
		}
	}

	// Handle Cutting.
	if (input.IsKeyDown(EKeyCode::C)) {
		CPointMass* pGrabbedMass = GetClosestPoint(mousePosition, 1.0f);

		if (pGrabbedMass) {
			if (pGrabbedMass->IsPinned()) {
				pGrabbedMass->DetachPin();
			}

			pGrabbedMass->ClearLinks();
			//UpdateClothIndices();
		}
	}

	// Handle burning.
	if (input.IsKeyDown(EKeyCode::B)) {
		CPoint deltaPosition = mousePosition - m_lastMousePosition;

		CRay ray = m_Camera.ScreenPointToRay(mousePosition);
		float distance = 10.0f;
		CPointMass* pMass = nullptr;
		auto x = 0U;
		auto y = 0U;

		for (size_t i = 0; i < m_Cloth.GetNumPointMassesX(); ++i) {
			for (size_t j = 0; j < m_Cloth.GetNumPointMassesY(); ++j) {
				auto pPointMass = m_Cloth.GetPointMass(i, j);
				float currentDistance = ray.GetDistanceToPointSquared(pPointMass->GetPosition());
				if (currentDistance < distance) {
					pMass = pPointMass;
					distance = currentDistance;
					x = i;
					y = j;
				}
			}
		}

		if (pMass) {
			if (pMass->IsPinned()) {
				pMass->DetachPin();
			}

			m_Cloth.BurnCloth(x, y);
		}
	}

	if (input.GetMouseButtonPress(EMouseButton::LEFT)) {
		m_pGrabbedMass = GetClosestPoint(mousePosition, 1.0f);
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
			auto toRay = pointMass->GetPosition() - ray.GetIntersectionPoint(pointMass->GetPosition());
			float currentDistance = toRay.GetLength();

			if (currentDistance < 20.0f) {
				toRay /= currentDistance;
				float force = std::min((1.0f / currentDistance) * 700.0f, 500.0f);

				pointMass->ApplyForce((toRay + m_Camera.GetRotation().GetDirection()) * force);
			}
		}
	}

	HandleCameraInput(input, deltaTime);
	HandleSliderInput(input, deltaTime);
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
			vert.burntLevel = pointMasses[(y * numPointMassesX) + x]->GetBurntLevel();

			pVertices[x + (y * numPointMassesX)] = vert;
		}
	}

	/*for (unsigned int y = 0; y < numPointMassesY; ++y) {
		for (unsigned int x = 0; x < numPointMassesX; ++x) {
			TVertex& vert = pVertices[(y * numPointMassesX) + x];
			// Calculate normal.
			bool flipOrder = false;

			auto neighbourX = x + 1;
			auto neighbourZ = y + 1;
			if (x == numPointMassesX - 1) {
				auto neighbourX = x - 1;
				flipOrder = !flipOrder;
			}

			if (y == numPointMassesY - 1) {
				auto neighbourY = y - 1;
				//flipOrder = !flipOrder;
			}

			auto neighbourPosition1 = pVertices[x + neighbourZ * numPointMassesX].position;
			auto neighbourPosition2 = pVertices[neighbourX + y * numPointMassesX].position;
			auto toNeighbour1 = vert.position - neighbourPosition1;
			auto toNeighbour2 = vert.position - neighbourPosition2;

			auto currentNormal = CVector3::CrossProduct(toNeighbour1, toNeighbour2);

			if (flipOrder) {
				currentNormal -= currentNormal;
			}

			vert.normal = CVector3::Normalise(currentNormal);
		}
	}*/

	for (unsigned int y = 0; y < numPointMassesY; ++y) {
		for (unsigned int x = 0; x < numPointMassesX; ++x) {
			TVertex& vert = pVertices[(y * numPointMassesX) + x];
			// Calculate normal.
			auto averagedNormal = CVector3::s_ZERO;
			auto lastLine = CVector3::s_ZERO;

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
					lastLine = CVector3::s_ZERO;

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

CPointMass* CClothSimulatorLogic::GetClosestPoint(CPoint mousePosition, float minDistance)
{
	auto ray = m_Camera.ScreenPointToRay(mousePosition);
	auto distance = minDistance * minDistance;
	CPointMass* pMass = nullptr;

	for (auto& pointMass : m_Cloth.GetPointMasses()) {
		float currentDistance = ray.GetDistanceToPointSquared(pointMass->GetPosition());
		if (currentDistance < distance) {
			pMass = pointMass.get();
			distance = currentDistance;
		}
	}

	return pMass;
}

void CClothSimulatorLogic::HandleSliderInput(const CInput& input, float deltaTime)
{
	const auto& mousePosition = input.GetMousePosition();

	// Handle slider ui elements.
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
		else if (m_SliderRectFanForce.IsInside(mousePosition)) {
			m_pDraggingRect = &m_SliderRectFanForce;
		}
	}

	if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		// Update slider position if we are moving it.
		if (m_pDraggingRect) {
			m_pDraggingRect->SetX(mousePosition.GetX());
			if (m_pDraggingRect->GetX() < s_MIN_SLIDER_X) {
				m_pDraggingRect->SetX(s_MIN_SLIDER_X);
			}
			else if (m_pDraggingRect->GetX() > s_MAX_SLIDER_X) {
				m_pDraggingRect->SetX(s_MAX_SLIDER_X);
			}

			float t = GetLerpAmount(s_MIN_SLIDER_X, s_MAX_SLIDER_X, m_pDraggingRect->GetX());
			// Update sliders that update immediately.
			if (m_pDraggingRect == &m_SliderRectNumHooks) {
				auto numHooks = Lerp(s_MIN_HOOKS, s_MAX_HOOKS, t);
				m_Cloth.SetNumHooks(numHooks);
				m_Cloth.PinCloth();
			}
			else if (m_pDraggingRect == &m_SliderRectFanForce) {
				m_FanForce = Lerp(s_MIN_FAN_FORCE, s_MAX_FAN_FORCE, t);
			}
		}
	}

	if (input.GetMouseButtonRelease(EMouseButton::LEFT)) {
		if (m_pGrabbedMass) {
			m_pGrabbedMass->DetachPin();
			m_pGrabbedMass = nullptr;
		}

		if (m_pDraggingRect) {
			float t = GetLerpAmount(s_MIN_SLIDER_X, s_MAX_SLIDER_X, m_pDraggingRect->GetX());

			if (m_pDraggingRect == &m_SliderRectClothWidth) {
				m_Cloth.SetNumPointMassesX(Lerp(s_MIN_CLOTH_SIZE, s_MAX_CLOTH_SIZE, t));

				CreateCloth();
			}
			else if (m_pDraggingRect == &m_SliderRectClothHeight) {
				m_Cloth.SetNumPointMassesY(Lerp(s_MIN_CLOTH_SIZE, s_MAX_CLOTH_SIZE, t));

				CreateCloth();
			}

			m_pDraggingRect = nullptr;
		}
	}
}

void CClothSimulatorLogic::HandleCameraInput(const CInput& input, float deltaTime)
{
	const auto& mousePosition = input.GetMousePosition();

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