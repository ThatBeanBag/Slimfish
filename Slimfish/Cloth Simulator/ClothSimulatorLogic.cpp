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
#include "../Source/Graphics/SlimLight.h"

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

	int n= 70;
	int m = 70;
	m_ClothWidth = n;
	m_ClothHeight = m;
	CreateCloth(n, m, 1.0f, 0.9f, 50.0f);

	std::vector<TVertex> groundVertices = {
			{ CVector3(-100.0f, 0.0f, -100.0f), CVector3::s_UP },
			{ CVector3(-100.0f, 0.0f, 100.0f), CVector3::s_UP },
			{ CVector3(100.0f, 0.0f, -100.0f), CVector3::s_UP },
			{ CVector3(100.0f, 0.0f, 100.0f), CVector3::s_UP }
	};

	m_pGroundVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(groundVertices);

	// Create the vertex and vertex buffer.
	// Create vertex buffer.
	/*std::vector<TVertex> vertices;
	for (auto& pointMass : m_PointMasses) {
		for (const auto& link : pointMass->GetLinks()) {
			vertices.push_back({ pointMass->GetPosition(), CVector3::s_FORWARD });
			vertices.push_back({ link.GetPointMassB()->GetPosition(), CVector3::s_FORWARD });
		}
	}*/

	// Generate vertices.
	std::vector<TVertex> vertices(n * m);
	for (unsigned int z = 0; z < m; ++z) {
		for (unsigned int x = 0; x < n; ++x) {
			TVertex vert;
			vert.position = m_PointMasses[(z * n) + x]->GetPosition();
			//vert.position.SetX(static_cast<float>(x) / static_cast<float>(n)-0.5f);
			//vert.position.SetY(0.0f);
			//vert.position.SetZ(static_cast<float>(z) / static_cast<float>(m)-0.5f);
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
//	m_ClothRenderPass.SetGeometryShader(g_pApp->GetRenderer()->VCreateShaderProgram("ClothGS.hlsl", EShaderProgramType::GEOMETRY, "main", "gs_4_0"));
	m_ClothRenderPass.SetCullingMode(ECullingMode::NONE);
	m_ClothRenderPass.SetFillMode(EFillMode::SOLID);

	m_pVSParams = m_ClothRenderPass.GetVertexShader()->VCreateShaderParams("constantBuffer");

	CLight m_Light;
	m_Light.SetType(LIGHT_DIRECTIONAL);
	m_Light.SetDiffuse(CColourValue(0.7f, 0.7f, 0.7f));
	m_Light.SetSpecular(CColourValue(1.0f, 1.0f, 1.0f, 100.0f));
	m_Light.SetDirection(CVector3::Normalise(CVector3(-1.0f, -0.8f, 1.0f)));

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
	m_Camera.SetPosition(CVector3(0.0f, 0.0f, 10.0f));
	m_Camera.SetProjectionMode(EProjectionMode::PERSPECTIVE);
	m_Camera.SetNearClipDistance(0.1f);
	m_Camera.SetFarClipDistance(1000.0f);
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
	int constraintAccuracy = 2;

	for (auto& pointMass : m_PointMasses) {
		pointMass->ApplyForce(CVector3(2.0f, 0.0f, 2.0f));
	}

	//for (int i = 0; i <= numTimeSteps; ++i) {
		// Solve constraints.
		for (int j = 0; j < constraintAccuracy; ++j) {
			for (auto& pointMass : m_PointMasses) {
				pointMass->SolveConstraints();
			}
		}

		// Update masses.
		for (auto& pointMass : m_PointMasses) {
			pointMass->Update(deltaTime);
		}
	//}

	CGpuBufferLock lock(m_pClothVertexBuffer, 0, m_pClothVertexBuffer->GetSize(), EGpuBufferLockType::DISCARD);
	TVertex* pVertices = reinterpret_cast<TVertex*>(lock.GetLockedContents());

	for (unsigned int z = 0; z < m_ClothWidth; ++z) {
		for (unsigned int x = 0; x < m_ClothHeight; ++x) {
			TVertex vert;
			vert.position = m_PointMasses[(z * m_ClothWidth) + x]->GetPosition();
			vert.normal = CVector3::s_FORWARD;

			pVertices[x + (z * m_ClothWidth)] = vert;
		}
	}

	// Compute normals.
	for (unsigned int z = 0; z < m_ClothWidth; ++z) {
		for (unsigned int x = 0; x < m_ClothHeight; ++x) {
			TVertex& currentVert = pVertices[x + z * m_ClothWidth];
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
					neighbourX >= static_cast<int>(m_ClothWidth) ||
					neighbourZ >= static_cast<int>(m_ClothHeight)) {
					// Is this neighbour outside the image?
					// Don't evaluate it.
					increment(i);

					continue;
				}

				TVertex neighbourVert = pVertices[neighbourX + neighbourZ * m_ClothWidth];
				CVector3 currentNormal(0, 0, 0);
				CVector3 currentLine = currentVert.position - neighbourVert.position;

				currentNormal = CVector3::CrossProduct(lastLine, currentLine);
				averagedNormal += currentNormal;
				lastLine = currentLine;

				increment(i);
			}

			currentVert.normal = CVector3::Normalise(averagedNormal);
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
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pClothVertexBuffer, m_pClothIndexBuffer);
	//g_pApp->GetRenderer()->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pGroundVertexBuffer);
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
	float midHeight = (height * restingDistance) / 2.0f;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			CVector3 position(restingDistance * x, 0.0f, height - (y * restingDistance));

			auto pPointMass = std::make_unique<CPointMass>(position, 1.0f, 1.0f);

			if (x != 0) {
				pPointMass->Attach(m_PointMasses.back().get(), restingDistance, stiffness, breakingForce);
			}

			if (y != 0) {
				pPointMass->Attach(m_PointMasses[(y - 1) * (width) + x].get(), restingDistance, stiffness, breakingForce);
			}

			if (y == height - 1 && (x % static_cast<int>((width / 4.0f)) == 0 || x == width - 1)) {
				pPointMass->Pin(position);
			}

			m_PointMasses.push_back(std::move(pPointMass));
		}
	}
}
