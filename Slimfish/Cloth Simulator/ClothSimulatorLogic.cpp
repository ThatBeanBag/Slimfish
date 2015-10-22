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
	m_LightCamera(nullptr)
{

}

CClothSimulatorLogic::~CClothSimulatorLogic()
{

}

bool CClothSimulatorLogic::Initialise()
{
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("COLOR", CInputElement::FORMAT_FLOAT4);

	int n = 40;
	int m = 40;
	m_ClothWidth = n;
	m_ClothHeight = m;
	CreateCloth(n, m, 0.3f, 0.9f, 50.0f);

	std::vector<TVertex> groundVertices = {
			{ CVector3(-100.0f, 0.0f, -100.0f), CVector3::s_UP, CColourValue::s_GREEN },
			{ CVector3(-100.0f, 0.0f, 100.0f), CVector3::s_UP,	CColourValue::s_GREEN },
			{ CVector3(100.0f, 0.0f, -100.0f), CVector3::s_UP,	CColourValue::s_GREEN },
			{ CVector3(100.0f, 0.0f, 100.0f), CVector3::s_UP,	CColourValue::s_GREEN }
	};

	m_pGroundVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(groundVertices);

	// Create vertex buffer.
	/*std::vector<TVertex> vertices;
	for (auto& pointMass : m_PointMasses) {
		for (const auto& link : pointMass->GetLinks()) {
			if (link.GetIsVisible()) {
				vertices.push_back({ pointMass->GetPosition(), CVector3::s_FORWARD, CColourValue::s_RED });
				vertices.push_back({ link.GetPointMassB()->GetPosition(), CVector3::s_FORWARD, CColourValue::s_RED });
			}
		}
	}*/

	// Generate vertices.
	std::vector<TVertex> vertices(n * m);
	for (unsigned int z = 0; z < m; ++z) {
		for (unsigned int x = 0; x < n; ++x) {
			TVertex vert;
			vert.position = m_PointMasses[(z * n) + x]->GetPosition();
			vert.normal = CVector3::s_FORWARD;
			//vert.u = (static_cast<float>(x) / static_cast<float>(iWidth - 1));
			//vert.v = (static_cast<float>(z) / static_cast<float>(iHeight - 1));

			vertices[x + (z * n)] = vert;
		}
	}

	m_pClothVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(vertices, EGpuBufferUsage::WRITE_ONLY, false);
	int numIndices = ((n * 2) * (m - 1) + (m - 2));

	// Generate indices
	std::vector<int> indices(numIndices);
	int index = 0;

	for (unsigned int z = 0; z < m - 1; ++z) {
		// Even rows move left to right, odd rows move right to left.
		if (z % 2 == 0) {
			// Is this an even row?
			for (int x = 0; x < static_cast<int>(n); ++x) {
				indices[index++] = x + z * n;
				indices[index++] = x + z * n + n;	// Next row.
			}

			// Insert degenerate vertex, if this isn't the last row.
			if (z != m - 2) {
				indices[index++] = n - 1 + (z * n);
			}
		}
		else {
			// This is an odd row.
			for (int x = static_cast<int>(n)-1; x >= 0; --x) {
				indices[index++] = x + z * n;
				indices[index++] = x + z * n + n;	// Next row.
			}

			// Insert degenerate vertex, if this isn't the last row.
			if (z != m - 2) {
				indices[index++] = z * n;
			}
		}
	}

	m_pClothIndexBuffer = g_pApp->GetRenderer()->CreateIndexBuffer(indices);

	if (!m_pClothVertexBuffer) {
		return false;
	}

	// Create shaders.
	m_ClothRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothVS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_ClothRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothPS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	//m_ClothRenderPass.SetCullingMode(ECullingMode::NONE);
	//m_ClothRenderPass.SetFillMode(EFillMode::WIREFRAME);

	m_pVSParams = m_ClothRenderPass.GetVertexShader()->VCreateShaderParams("constantBuffer");

	// Setup lighting.
	m_Light.SetType(LIGHT_DIRECTIONAL);
	m_Light.SetDiffuse(CColourValue(0.7f, 0.7f, 0.7f));
	m_Light.SetSpecular(CColourValue(1.0f, 1.0f, 1.0f, 100.0f));
	m_Light.SetDirection(CVector3::Normalise(CVector3(0.0f, 0.0f, -1.0f)));

	// Create the light camera.
	m_LightCamera.SetProjectionMode(EProjectionMode::PERSPECTIVE);
	m_LightCamera.SetPosition(CVector3(0, 50, 50));
	m_LightCamera.SetRotation(CQuaternion(Math::DegreesToRadians(0.0f), Math::DegreesToRadians(-45.0f), Math::DegreesToRadians(0.0f)));
	m_LightCamera.SetNearClipDistance(0.1f);
	m_LightCamera.SetFarClipDistance(500.0f);
	m_LightCamera.SetFieldOfView(90.0f);
	m_LightCamera.SetAspectRatio(static_cast<float>(s_SHADOW_MAP_WIDTH) / static_cast<float>(s_SHADOW_MAP_HEIGHT));
	m_LightCamera.SetOrthographicSize(1.0f);
	m_LightCamera.UpdateViewTransform();

	auto m_pPSParams = m_ClothRenderPass.GetPixelShader()->VCreateShaderParams("constantBuffer");
	m_pPSParams->SetConstant("gLight.type", m_Light.GetType());
	m_pPSParams->SetConstant("gLight.diffuse", m_Light.GetDiffuse());
	m_pPSParams->SetConstant("gLight.specular", m_Light.GetSpecular());
	m_pPSParams->SetConstant("gLight.direction", m_Light.GetDirection());
	m_pPSParams->SetConstant("gLight.range", m_Light.GetRange());
	m_pPSParams->SetConstant("gLight.attenuation", m_Light.GetAttenuation());
	m_pPSParams->SetConstant("gAmbientLight", CColourValue(0.3f, 0.3f, 0.3f));

	m_ClothRenderPass.GetPixelShader()->VUpdateShaderParams("constantBuffer", m_pPSParams);

	// Setup camera.
	m_Camera.SetPosition(CVector3(0.0f, 6.0f, 20.0f));
	m_Camera.SetProjectionMode(EProjectionMode::PERSPECTIVE);
	m_Camera.SetNearClipDistance(0.1f);
	m_Camera.SetFarClipDistance(1000.0f);
	m_Camera.SetFieldOfView(Math::DegreesToRadians(60.0f));
	m_Camera.SetOrthographicSize(20.0f);

	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue(0.8f, 0.8f, 0.8f));

	// Setup shadow map.
	m_DepthRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("DepthOnly_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_DepthRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("DepthOnly_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	auto pRenderDepthShaderParams = m_DepthRenderPass.GetVertexShader()->VCreateShaderParams("constantBuffer");
	pRenderDepthShaderParams->SetConstant("gWorldViewProjMatrix", m_LightCamera.GetViewProjMatrix());
	m_DepthRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", pRenderDepthShaderParams);

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

	m_AccumulatedTime += deltaTime;

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
		pointMass->Update(deltaTime);
	}

	for (auto& pointMass : m_PointMasses) {
		//pointMass->ApplyForce(CVector3(2.0f, 0.0f, 2.0f));
	}

	if (m_pGrabbedMass) {
		CRay ray = m_Camera.ScreenPointToRay(m_lastMousePosition);
		CVector3 intersectionPoint = ray.GetIntersectionPointOnRay(m_pGrabbedMass->GetPosition());

		m_pGrabbedMass->ApplyForce((intersectionPoint - m_pGrabbedMass->GetPosition()) * 500.0f);
	}

	static const float minClothDistance = 0.1f;
	static const float minClothDistanceSqr = minClothDistance * minClothDistance;

	CVector3 sphereOrigin(0.0f, 4.0f, -2.0f);
	float sphereRadius = 3.0f;

	for (int i = 0; i < m_PointMasses.size(); ++i) {
		/*for (int j = i + 1; j < m_PointMasses.size(); ++j) {
			auto toPointMass = m_PointMasses[i]->GetPosition() - m_PointMasses[j]->GetPosition();
			if (toPointMass.GetLengthSquared() < minClothDistanceSqr) {
				toPointMass = CVector3::Normalise(toPointMass);
				m_PointMasses[i]->SetPosition(m_PointMasses[j]->GetPosition() + (toPointMass * minClothDistance));
			}
		}*/

		auto toSphere = m_PointMasses[i]->GetPosition() - sphereOrigin;
		if (toSphere.GetLengthSquared() < sphereRadius * sphereRadius) {
			toSphere = CVector3::Normalise(toSphere);
			m_PointMasses[i]->SetPosition(sphereOrigin + toSphere * sphereRadius);
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
	m_pVSParams->SetConstant("gLightViewProjMatrix", m_LightCamera.GetViewProjMatrix());
	m_ClothRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_pVSParams);

	//RenderToShadowMap();

	g_pApp->GetRenderer()->SetRenderPass(&m_ClothRenderPass);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pClothVertexBuffer, m_pClothIndexBuffer);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pGroundVertexBuffer);
}

void CClothSimulatorLogic::HandleInput(const CInput& input, float deltaTime)
{
	/*if (input.GetKeyRelease(EKeyCode::F1)) {
	g_pApp->GetRenderer()->ToggleWindowed(); // If only :(
	}*/

	CPoint mousePosition = input.GetMousePosition();

	if (input.GetKeyRelease(EKeyCode::R)) {
		m_PointMasses.clear();
		int n = 40;
		int m = 40;
		m_ClothWidth = n;
		m_ClothHeight = m;
		CreateCloth(n, m, 0.5f, 0.9f, 4.0f);
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

	if (input.GetMouseButtonPress(EMouseButton::LEFT)) {
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
	}
	/*if (input.IsMouseButtonDown(EMouseButton::LEFT)) {
		CPoint deltaPosition = mousePosition - m_lastMousePosition;

		CRay ray = m_Camera.ScreenPointToRay(mousePosition);
		float distance = 100000000.0f;
		CPointMass* pMass = nullptr;

		for (auto& pointMass : m_PointMasses) {
			auto toRay = ray.GetIntersectionPointOnRay(pointMass->GetPosition()) - pointMass->GetPosition();
			float currentDistance = toRay.GetLength();

			if (currentDistance < 20.0f) {
				toRay /= currentDistance;
				float force = std::min((1.0f / currentDistance) * 100.0f, 500.0f);

				pointMass->ApplyForce(toRay * force);
			}
		}
	}*/

	if (input.IsMouseButtonDown(EMouseButton::RIGHT)) {
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

				pointMass->ApplyForce(toRay * force);
			}
		}
	}

	if (input.GetMouseButtonRelease(EMouseButton::LEFT)) {
		if (m_pGrabbedMass) {
			m_pGrabbedMass->DetachPin();
			m_pGrabbedMass = nullptr;
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

	for (unsigned int z = 0; z < m_ClothWidth; ++z) {
		for (unsigned int x = 0; x < m_ClothHeight; ++x) {
			TVertex vert;
			vert.position = m_PointMasses[(z * m_ClothWidth) + x]->GetPosition();

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

				CVector3 neighbourPosition = m_PointMasses[neighbourX + neighbourZ * m_ClothWidth]->GetPosition();
				CVector3 currentNormal(0, 0, 0);
				CVector3 currentLine = vert.position - neighbourPosition;

				currentNormal = CVector3::CrossProduct(lastLine, currentLine);
				averagedNormal += currentNormal;
				lastLine = currentLine;

				increment(i);
			}

			vert.normal = CVector3::Normalise(averagedNormal);
			vert.colour = CColourValue::s_RED;

			pVertices[x + (z * m_ClothWidth)] = vert;
		}
	}
}

void CClothSimulatorLogic::CreateSphereVertices(float rings, float segments)
{
	std::vector<TVertex> vertices((rings + 1) * (segments + 1));
	std::vector<short> indices(6 * rings * (segments + 1));
	int index = 0;
	int vertexIndex = 0;

	float fDeltaRingAngle = Math::s_PI / rings;
	float fDeltaSegAngle = 2 * Math::s_PI / segments;

	for (int ring = 0; ring <= rings; ++ring) {
		float r0 = 1.0f * std::sinf(ring * fDeltaRingAngle);
		float y0 = 1.0f * std::cosf(ring * fDeltaRingAngle);

		for (int seg = 0; seg <= segments; ++seg) {
			float x0 = r0 * std::sinf(seg * fDeltaSegAngle);
			float z0 = r0 * std::cosf(seg * fDeltaSegAngle);

			vertices[ring * segments + seg].position = CVector3(x0, y0, z0);
			vertices[ring * segments + seg].u = static_cast<float>(seg) / static_cast<float>(segments);
			vertices[ring * segments + seg].v = static_cast<float>(ring) / static_cast<float>(rings);

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

			if (x != 0) {
				pPointMass->Attach(m_PointMasses.back().get(), restingDistance, stiffness, breakingForce);

				if (y != 0) {
					//pPointMass->Attach(m_PointMasses[(y - 1) * width + (x - 1)].get(), restingDistance, stiffness, breakingForce, false);
				}
			}

			if (y != 0) {
				pPointMass->Attach(m_PointMasses[(y - 1) * (width) + x].get(), restingDistance, stiffness, breakingForce);
				if (x < width - 1) {
					//pPointMass->Attach(m_PointMasses[(y - 1) * width + (x + 1)].get(), restingDistance, stiffness, breakingForce, false);
				}
			}

			if (y == 0 && (x % static_cast<int>((width / 4.0f)) == 0 || x == width - 1)) {
				pPointMass->Pin(position);
			}

			m_PointMasses.push_back(std::move(pPointMass));
		}
	}
}

