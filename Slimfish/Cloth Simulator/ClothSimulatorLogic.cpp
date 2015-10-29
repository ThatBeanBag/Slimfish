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

const size_t CClothSimulatorLogic::s_SHADOW_MAP_WIDTH = 2048;
const size_t CClothSimulatorLogic::s_SHADOW_MAP_HEIGHT = 2048;

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
	m_fTimeScale(1.0f)
{

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

	m_ClothWidth = 50;
	m_ClothHeight = 50;
	CreateCloth(m_ClothWidth, m_ClothHeight, 0.3f, 0.9f, 50.0f);

	std::vector<TVertex> groundVertices = {
			{ CVector3(-100.0f, 0.0f, -100.0f), CVector3::s_UP, 0.0f, 0.0f, CColourValue::s_GREEN },
			{ CVector3(-100.0f, 0.0f, 100.0f), CVector3::s_UP, 0.0f, 1.0f, CColourValue::s_GREEN },
			{ CVector3(100.0f, 0.0f, -100.0f), CVector3::s_UP, 1.0f, 0.0f, CColourValue::s_GREEN },
			{ CVector3(100.0f, 0.0f, 100.0f), CVector3::s_UP, 1.0f, 1.0f, CColourValue::s_GREEN }
	};

	m_pGroundVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(groundVertices);

	// Create vertex buffer.
	/*std::vector<TVertex> vertices;
	for (auto& pointMass : m_PointMasses) {
		for (const auto& link : pointMass->GetLinks()) {
			if (link.GetIsVisible()) {
				vertices.push_back({ pointMass->GetPosition(), CVector3::s_FORWARD, 0, 0, CColourValue::s_RED });
				vertices.push_back({ link.GetPointMassB()->GetPosition(), CVector3::s_FORWARD, 0, 0, CColourValue::s_RED });
			}
		}
	}

	m_pClothVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(vertices, EGpuBufferUsage::WRITE_ONLY);*/

	// Generate vertices.
	int numVertices = m_ClothWidth * m_ClothHeight;
	m_pClothVertexBuffer = g_pApp->GetRenderer()->VCreateVertexBuffer(numVertices, sizeof(TVertex), nullptr, EGpuBufferUsage::WRITE_ONLY);

	// Generate indices
	int numIndices = ((m_ClothWidth * 2) * (m_ClothHeight - 1) + (m_ClothHeight - 2)) + m_pClothVertexBuffer->GetNumVertices();
	m_pClothIndexBuffer = g_pApp->GetRenderer()->VCreateIndexBuffer(numIndices, AIndexGpuBuffer::INDEX_TYPE_32, nullptr, EGpuBufferUsage::WRITE_ONLY);

	UpdateClothVertices();

	CreateSphereVertices(10, 10);

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
	m_ClothRenderPass.AddTextureLayer()->SetTexture(m_pShadowMap->GetTexture());

	return true;
}

void CClothSimulatorLogic::Update(float deltaTime)
{
	CPoint screenSize = g_pApp->GetRenderer()->GetWindowSize();
	m_Camera.SetAspectRatio(static_cast<float>(screenSize.GetX()) / static_cast<float>(screenSize.GetY()));

	// Update physics.
	static float fixedDeltaTime = 16.0f / 1000.0f;
	static int constraintAccuracy = 2;

	/*m_AccumulatedTime += deltaTime;
	if (m_AccumulatedTime <= fixedDeltaTime) {
		return;
	}

	m_AccumulatedTime = 0.0f;*/

	static const float minClothDistance = 0.05f;
	static const float minClothDistanceSqr = minClothDistance * minClothDistance;

	CVector3 sphereOrigin(0.0f, 4.0f, -2.0f);
	float sphereRadius = 3.1f;

	for (int j = 0; j < constraintAccuracy; ++j) {
		for (auto& pointMass : m_PointMasses) {
			pointMass->SolveConstraints();
		}
	}

	/*while (m_AccumulatedTime >= fixedDeltaTime) {
		for (auto& pointMass : m_PointMasses) {
			pointMass->Update(fixedDeltaTime);
		}

		m_AccumulatedTime -= fixedDeltaTime;
	}*/

	for (auto& pointMass : m_PointMasses) {
		pointMass->Update(deltaTime * m_fTimeScale);
	}

	/*for (auto& pointMass : m_PointMasses) {
		pointMass->ApplyForce(CVector3(2.0f, 0.0f, 2.0f));
	}*/

	if (m_pGrabbedMass) {
		CRay ray = m_Camera.ScreenPointToRay(m_lastMousePosition);
		CVector3 intersectionPoint = ray.GetIntersectionPointOnRay(m_pGrabbedMass->GetPosition());

		m_pGrabbedMass->ApplyForce((intersectionPoint - m_pGrabbedMass->GetPosition()) * 500.0f);
	}

	for (unsigned int i = 0; i < m_PointMasses.size(); ++i) {
		/*for (unsigned int j = i + 1; j < m_PointMasses.size(); ++j) {
			auto toPointMass = m_PointMasses[j]->GetPosition() - m_PointMasses[i]->GetPosition();
			if (toPointMass.GetLengthSquared() < minClothDistanceSqr) {
				toPointMass = CVector3::Normalise(toPointMass);
				m_PointMasses[i]->SetPosition(m_PointMasses[j]->GetPosition() - (toPointMass * (minClothDistance + 0.1f)));
				m_PointMasses[j]->SetPosition(m_PointMasses[i]->GetPosition() + (toPointMass * (minClothDistance + 0.1f)));
				/ *m_PointMasses[i]->SetPosition(m_PointMasses[i]->GetLastPosition());
				m_PointMasses[j]->SetPosition(m_PointMasses[j]->GetLastPosition()); * /
				//m_PointMasses[i]->ApplyForce(-toPointMass * (minClothDistance + 0.1f));
				//m_PointMasses[j]->ApplyForce(toPointMass * (minClothDistance + 0.1f));
			}
		}*/

		auto position = m_PointMasses[i]->GetPosition();

		auto toSphere = position - sphereOrigin;
		if (toSphere.GetLengthSquared() < sphereRadius * sphereRadius) {
			toSphere = CVector3::Normalise(toSphere);
			m_PointMasses[i]->SetPosition(sphereOrigin + toSphere * sphereRadius);
		}

		if (position.GetY() < 0.5f) {
			position.SetY(0.5f);
			m_PointMasses[i]->SetPosition(position);
			//auto force = m_PointMasses[i]->GetTotalForce();
			//force.SetY(0);

			//m_PointMasses[i]->ApplyForce(-force * 0.0f);
		}
	}

	UpdateClothVertices();
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

	// Update shader params.
	m_pVSParams->SetConstant("gWorldViewProjMatrix", m_Camera.GetViewProjMatrix());
	m_pVSParams->SetConstant("gWorldMatrix", CMatrix4x4::s_IDENTITY);
	m_pVSParams->SetConstant("gLightViewProjMatrix", m_LightCamera.GetViewProjMatrix());
	m_ClothRenderPass.GetVertexShader()->UpdateShaderParams("constantBuffer", m_pVSParams);

	m_pPSParams->SetConstant("gEyePosition", m_Camera.GetPosition());
	m_ClothRenderPass.GetPixelShader()->UpdateShaderParams("constantBuffer", m_pPSParams);

	RenderToShadowMap();

	g_pApp->GetRenderer()->SetRenderPass(&m_ClothRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pClothVertexBuffer, m_pClothIndexBuffer);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pGroundVertexBuffer);

	CMatrix4x4 spherePosition = CMatrix4x4::BuildTranslation(0.0f, 4.0f, -2.0f) * CMatrix4x4::BuildScale(3.0f, 3.0f, 3.0f);
	m_pVSParams->SetConstant("gWorldViewProjMatrix", m_Camera.GetViewProjMatrix() * spherePosition);
	m_pVSParams->SetConstant("gWorldMatrix", spherePosition);
	m_pVSParams->SetConstant("gLightViewProjMatrix", m_LightCamera.GetViewProjMatrix());
	m_ClothRenderPass.GetVertexShader()->UpdateShaderParams("constantBuffer", m_pVSParams);

	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pSphereVertices, m_pSphereIndices);
}

void CClothSimulatorLogic::HandleInput(const CInput& input, float deltaTime)
{
	if (input.GetKeyRelease(EKeyCode::F1)) {
		g_pApp->GetRenderer()->ToggleWindowed(); // If only :(
	}

	CPoint mousePosition = input.GetMousePosition();

	if (input.GetKeyRelease(EKeyCode::R)) {
		m_PointMasses.clear();
		CreateCloth(m_ClothWidth, m_ClothHeight, 0.3f, 0.9f, 50.0f);
		m_pGrabbedMass = nullptr;
	}

	if (input.GetKeyPress(EKeyCode::NUM_1)) {
		if (m_ClothRenderPass.GetFillMode() == EFillMode::SOLID) {
			m_ClothRenderPass.SetFillMode(EFillMode::WIREFRAME);
		}
		else {
			m_ClothRenderPass.SetFillMode(EFillMode::SOLID);
		}
	}

	if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		CPoint deltaPosition = mousePosition - m_lastMousePosition;
		
		CRay ray = m_Camera.ScreenPointToRay(mousePosition);
		float distance = 10.0f;
		CPointMass* pMass = nullptr;

		for (auto& pointMass : m_PointMasses) {
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

		for (auto& pointMass : m_PointMasses) {
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

	/*std::vector<TVertex> vertices;
	int iIndex = 0;
	for (auto& pointMass : m_PointMasses) {
		for (const auto& link : pointMass->GetLinks()) {
			if (link.GetIsVisible()) {
				pVertices[iIndex].position = pointMass->GetPosition();
				pVertices[iIndex].normal = CVector3::s_FORWARD;
				pVertices[iIndex].colour = CColourValue::s_RED;
				++iIndex;
				pVertices[iIndex].position = link.GetPointMassB()->GetPosition();
				pVertices[iIndex].normal = CVector3::s_FORWARD;
				pVertices[iIndex].colour = CColourValue::s_RED;
				++iIndex;
				/ * vertices.push_back({ pointMass->GetPosition(), CVector3::s_FORWARD });
				vertices.push_back({ link.GetPointMassB()->GetPosition(), CVector3::s_FORWARD }); * /
			}
		}
	}

	for (int i = iIndex; i < m_pClothVertexBuffer->GetSize() / m_pClothVertexBuffer->GetStride(); ++i) {
		pVertices[i].position = CVector3(1000, 1000, 1000);
	}*/

	for (unsigned int z = 0; z < m_ClothHeight; ++z) {
		for (unsigned int x = 0; x < m_ClothWidth; ++x) {
			TVertex vert;
			vert.position = m_PointMasses[(z * m_ClothWidth) + x]->GetPosition();
			vert.u = x / static_cast<float>(m_ClothWidth);
			vert.v = z / static_cast<float>(m_ClothHeight);

			pVertices[x + (z * m_ClothWidth)] = vert;
		}
	}

	for (unsigned int z = 0; z < m_ClothHeight; ++z) {
		for (unsigned int x = 0; x < m_ClothWidth; ++x) {
			TVertex& vert = pVertices[(z * m_ClothWidth) + x];
			// Calculate normal.
			CVector3 averagedNormal(0, 0, 0);
			CVector3 lastLine(0, 0, 0);

			// Start at the top right neighbour.
			int neighbourX = x + 1;
			int neighbourZ = z - 1;

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
					neighbourX >= static_cast<int>(m_ClothWidth) ||
					neighbourZ >= static_cast<int>(m_ClothHeight)) {
					// Is this neighbour outside the image?
					// Don't evaluate it.
					increment(i);

					continue;
				}

				CVector3 neighbourPosition = pVertices[neighbourX + neighbourZ * m_ClothWidth].position;
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

	CGpuBufferLock indexLock(m_pClothIndexBuffer, 0, m_pClothIndexBuffer->GetSize(), EGpuBufferLockType::DISCARD);
	int* pIndices = reinterpret_cast<int*>(indexLock.GetLockedContents());

	auto index = 0;
	auto n = m_ClothWidth;
	auto m = m_ClothHeight;

	auto GetPointMass = [this](unsigned int x, unsigned int z) {
		return m_PointMasses[x + z * m_ClothWidth].get();
	};

	auto insertDegeneratesEven = [&](unsigned int x, unsigned int z) {
		auto pointMass = GetPointMass(x, z);
		// ^
		// |
		if (z < m - 1 && !GetPointMass(x, z + 1)->HasLinkTo(pointMass)) {
			pIndices[index++] = x + z * n;	// Insert degenerate triangle.
			return true;
		}
		// <--
		else if (x > 0 && z < m - 1 && !GetPointMass(x , z + 1)->HasLinkTo(GetPointMass(x - 1, z + 1))) {
			pIndices[index++] = x + z * n;	// Insert degenerate triangle.
			return true;
		}

		return false;
	};

	auto insertDegeneratesEven2 = [&](unsigned int x, unsigned int z) {
		auto pointMass = GetPointMass(x, z);
		// ^
		// |
		if (z < m - 1 && !GetPointMass(x, z + 1)->HasLinkTo(pointMass)) {
			pIndices[index++] = x + (z + 1) * n;	// Insert degenerate triangle.
			return true;
		}
		// <--
		else if (x < n - 1 && !GetPointMass(x + 1, z)->HasLinkTo(pointMass)) {
			pIndices[index++] = x + (z + 1) * n;	// Insert degenerate triangle.
			return true;
		}

		return false;
	};

	auto insertDegeneratesOdd = [&](unsigned int x, unsigned int z) {
		auto pointMass = GetPointMass(x, z);
		// ^
		// |
		if (z < m - 1 && !GetPointMass(x, z + 1)->HasLinkTo(pointMass)) {
			pIndices[index++] = x + z * n;	// Insert degenerate triangle.
			return true;
		}
		// <--
		else if (x < n - 1 && z < m - 1 && !GetPointMass(x + 1, z + 1)->HasLinkTo(GetPointMass(x , z + 1))) {
			pIndices[index++] = x + z * n;	// Insert degenerate triangle.
			return true;
		}

		return false;
	};

	auto insertDegeneratesOdd2 = [&](unsigned int x, unsigned int z) {
		auto pointMass = GetPointMass(x, z);
		// ^
		// |
		if (z < m - 1 && !GetPointMass(x, z + 1)->HasLinkTo(pointMass)) {
			pIndices[index++] = x + (z + 1) * n;	// Insert degenerate triangle.
			return true;
		}
		// <--
		else if (x > 0 && !pointMass->HasLinkTo(GetPointMass(x - 1, z))) {
			pIndices[index++] = x + (z + 1) * n;	// Insert degenerate triangle.
			return true;
		}

		return false;
	};

	for (unsigned int z = 0; z < m - 1; ++z) {
		// Even rows move left to right, odd rows move right to left.
		if (z % 2 == 0) {
			bool alreadyGenerate = false;

			// Is this an even row?
			for (int x = 0; x < static_cast<int>(n); ++x) {
				pIndices[index++] = x + z * n;
				insertDegeneratesEven(x, z);
				pIndices[index++] = x + z * n + n;	// Next row.
				alreadyGenerate = insertDegeneratesEven2(x, z);
			}

			// Insert degenerate vertex, if this isn't the last row.
			if (z != m - 2 && !alreadyGenerate) {
				pIndices[index++] = n - 1 + (z * n);
			}
		}
		else {
			bool alreadyGenerate = false;

			// This is an odd row.
			for (int x = static_cast<int>(n)-1; x >= 0; --x) {
				pIndices[index++] = x + z * n;
				insertDegeneratesOdd(x, z);
				pIndices[index++] = x + z * n + n;	// Next row.
				alreadyGenerate = insertDegeneratesOdd2(x, z);
			}

			// Insert degenerate vertex, if this isn't the last row.
			if (z != m - 2 && !alreadyGenerate) {
				pIndices[index++] = z * n;
			}
		}
	}

	for (int i = index; i < m_pClothIndexBuffer->GetNumIndices(); ++i) {
		pIndices[i] = pIndices[index - 1];
	}
}

void CClothSimulatorLogic::CreateSphereVertices(int rings, int segments)
{
	std::vector<TVertex> vertices((rings + 1) * (segments + 1));
	std::vector<int> indices(6 * rings * (segments + 1));
	int index = 0;
	int vertexIndex = 0;

	float fDeltaRingAngle = Math::s_PI / rings;
	float fDeltaSegAngle = 2 * Math::s_PI / segments;

	for (int ring = 0; ring <= rings; ++ring) {
		float r0 = std::sinf(ring * fDeltaRingAngle);
		float y0 = std::cosf(ring * fDeltaRingAngle);

		for (int seg = 0; seg <= segments; ++seg) {
			float x0 = r0 * std::sinf(seg * fDeltaSegAngle);
			float z0 = r0 * std::cosf(seg * fDeltaSegAngle);

			auto& vertex = vertices[ring * segments + seg];

			vertex.position = CVector3(x0, y0, z0);
			vertex.u = static_cast<float>(seg) / static_cast<float>(segments);
			vertex.v = static_cast<float>(ring) / static_cast<float>(rings);
			vertex.normal = CVector3::Normalise(r0 * vertex.position);

			if (ring != rings) {
				indices[index++] = vertexIndex + segments + 1;
				indices[index++] = vertexIndex;
				indices[index++] = vertexIndex + segments;
				indices[index++] = vertexIndex + segments + 1;
				indices[index++] = vertexIndex + 1;
				indices[index++] = vertexIndex;
				vertexIndex++;
			}
		}
	}

	m_pSphereVertices = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
	m_pSphereIndices = g_pApp->GetRenderer()->CreateIndexBuffer(indices);
}

void CClothSimulatorLogic::RenderToShadowMap()
{
	// Set the render target to start writing to the shadow map.
	g_pApp->GetRenderer()->VSetRenderTarget(m_pShadowMap.get());
	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue::s_WHITE);
	g_pApp->GetRenderer()->VPreRender();

	g_pApp->GetRenderer()->SetRenderPass(&m_DepthRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pClothVertexBuffer, m_pClothIndexBuffer);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pGroundVertexBuffer);

	// Set the render target back.
	g_pApp->GetRenderer()->VSetRenderTarget(nullptr);
	g_pApp->GetRenderer()->VPreRender();
}


void CClothSimulatorLogic::CreateCloth(int width, int height, float restingDistance, float stiffness, float breakingForce)
{
	float midWidth = (width * restingDistance) / 2.0f;
	float midHeight = (height * restingDistance) / 2.0f;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			CVector3 position(restingDistance * x - midWidth, ((height - y) * restingDistance) + 5.0f, 0.0f);

			auto pPointMass = std::make_unique<CPointMass>(position, 1.0f, 0.01f);
			float diagonalRestingDistance = sqrt( 2 * (restingDistance * restingDistance));

			if (x != 0) {
				pPointMass->Attach(m_PointMasses.back().get(), restingDistance, stiffness, breakingForce);

				if (y != 0) {
					//pPointMass->Attach(m_PointMasses[(y - 1) * width + (x - 1)].get(), diagonalRestingDistance, stiffness, breakingForce, false);
				}
			}

			if (y != 0) {
				pPointMass->Attach(m_PointMasses[(y - 1) * (width) + x].get(), restingDistance, stiffness, breakingForce);
				if (x < width - 1) {
					//pPointMass->Attach(m_PointMasses[(y - 1) * width + (x + 1)].get(), diagonalRestingDistance, stiffness, breakingForce, false);
				}
			}

			if (y == 0 && (x % static_cast<int>((width / 4.0f)) == 0 || x == width - 1)) {
				pPointMass->Pin(position);
			}

			m_PointMasses.push_back(std::move(pPointMass));
		}
	}
}

