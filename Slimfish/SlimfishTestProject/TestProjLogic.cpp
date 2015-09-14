// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TestProjLogic.cpp
// Description	: CTestProjLogic implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimTestProjectStd.h"

// Library Includes
#include <d3dx10.h>	// TODO: Remove.
#include <Math\SlimVector.h>
#include <Graphics\SlimRenderer.h>
#include <Graphics\SlimLight.h>

// This Include
#include "TestProjLogic.h"

// Local Includes

struct TVertex {
	CVector3 m_Position;
	CVector3 m_Normal;
	float m_U;
	float m_V;
};

CTestProjLogic::CTestProjLogic()
	:m_WorldMatrix(CMatrix4x4::s_IDENTITY),
	m_ViewMatrix(CMatrix4x4::s_IDENTITY),
	m_ProjectionMatrix(CMatrix4x4::s_IDENTITY)
{
	m_ViewMatrix = CMatrix4x4::BuildLookAt(CVector3(0, 0, 500.0f), CVector3(), CVector3::s_UP);
	m_ProjectionMatrix = CMatrix4x4::BuildProjection(DegreesToRadians(90), 1.0f, 0.1f, 1000.0f);
}

CTestProjLogic::~CTestProjLogic()
{

}

bool CTestProjLogic::Initialise()
{
	/*TVertex pCubeVerts[] = {
		{ CVector3(-1.0f, -1.0f, -1.0f	), CVector3(0.0f, 0.0f, -1.0f ), 0.0f, 1.0f },
		{ CVector3(-1.0f, 1.0f, -1.0f	), CVector3(0.0f, 0.0f, -1.0f ), 0.0f, 0.0f },
		{ CVector3(1.0f, 1.0f, -1.0f	), CVector3(0.0f, 0.0f, -1.0f ), 1.0f, 0.0f },
		{ CVector3(1.0f, -1.0f, -1.0f	), CVector3(0.0f, 0.0f, -1.0f ), 1.0f, 1.0f },
		{ CVector3(-1.0f, -1.0f, 1.0f	), CVector3(0.0f, 0.0f, 1.0f  ), 1.0f, 1.0f },
		{ CVector3(1.0f, -1.0f, 1.0f	), CVector3(0.0f, 0.0f, 1.0f  ), 0.0f, 1.0f },
		{ CVector3(1.0f, 1.0f, 1.0f		), CVector3(0.0f, 0.0f, 1.0f  ), 0.0f, 0.0f },
		{ CVector3(-1.0f, 1.0f, 1.0f	), CVector3(0.0f, 0.0f, 1.0f  ), 1.0f, 0.0f },
		{ CVector3(-1.0f, 1.0f, -1.0f	), CVector3(0.0f, 1.0f, 0.0f  ), 0.0f, 1.0f },
		{ CVector3(-1.0f, 1.0f, 1.0f	), CVector3(0.0f, 1.0f, 0.0f  ), 0.0f, 0.0f },
		{ CVector3(1.0f, 1.0f, 1.0f	 	), CVector3(0.0f, 1.0f, 0.0f  ), 1.0f, 0.0f },
		{ CVector3(1.0f, 1.0f, -1.0f	), CVector3(0.0f, 1.0f, 0.0f  ), 1.0f, 1.0f },
		{ CVector3(-1.0f, -1.0f, -1.0f	), CVector3(0.0f, -1.0f, 0.0f ), 1.0f, 1.0f },
		{ CVector3(1.0f, -1.0f, -1.0f	), CVector3(0.0f, -1.0f, 0.0f ), 0.0f, 1.0f },
		{ CVector3(1.0f, -1.0f, 1.0f	), CVector3(0.0f, -1.0f, 0.0f ), 0.0f, 0.0f },
		{ CVector3(-1.0f, -1.0f, 1.0f	), CVector3(0.0f, -1.0f, 0.0f ), 1.0f, 0.0f },
		{ CVector3(-1.0f, -1.0f, 1.0f	), CVector3(-1.0f, 0.0f, 0.0f ), 0.0f, 1.0f },
		{ CVector3(-1.0f, 1.0f, 1.0f	), CVector3(-1.0f, 0.0f, 0.0f ), 0.0f, 0.0f },
		{ CVector3(-1.0f, 1.0f, -1.0f	), CVector3(-1.0f, 0.0f, 0.0f ), 1.0f, 0.0f },
		{ CVector3(-1.0f, -1.0f, -1.0f	), CVector3(-1.0f, 0.0f, 0.0f ), 1.0f, 1.0f },
		{ CVector3(1.0f, -1.0f, -1.0f	), CVector3(1.0f, 0.0f, 0.0f  ), 0.0f, 1.0f },
		{ CVector3(1.0f, 1.0f, -1.0f	), CVector3(1.0f, 0.0f, 0.0f  ), 0.0f, 0.0f },
		{ CVector3(1.0f, 1.0f, 1.0f		), CVector3(1.0f, 0.0f, 0.0f  ), 1.0f, 0.0f },
		{ CVector3(1.0f, -1.0f, 1.0f	), CVector3(1.0f, 0.0f, 0.0f  ), 1.0f, 1.0f },
	};

	short pIndices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 5, 6,
		4, 6, 7,

		// right face
		8, 9, 10,
		8, 10, 11,

		// top face
		12, 13, 14,
		12, 14, 15,

		// bottom face
		16, 17, 18,
		16, 18, 19,

		// left face
		20, 21, 22,
		20, 22, 23,
	};

	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue(0.52f, 0.8f , 0.92f));

	m_pVertexBuffer = g_pApp->GetRenderer()->VCreateVertexBuffer(
		24, 
		sizeof(TVertex), 
		pCubeVerts, 
		EGpuBufferUsage::STATIC,
		false);

	m_pIndexBuffer = g_pApp->GetRenderer()->VCreateIndexBuffer(
		36,
		AIndexGpuBuffer::INDEX_TYPE_16,
		pIndices,
		EGpuBufferUsage::STATIC,
		false);*/

	// Create the input layout.
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);
	m_VertexDeclaration.AddElement("TANGENT", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.SetPrimitiveType(CVertexDeclaration::PRIMITIVE_TYPE_TRIANGLELIST);

	m_pTexture = g_pApp->GetRenderer()->VLoadTexture("WoodCrate02.dds");

	for (size_t i = 0; i < m_pTextures.size(); ++i) {
		std::string filePath = "FireAnim/Fire";
		if (i + 1 < 10) {
			filePath += "0";
		}

		if (i + 1 < 100) {
			filePath += "0";
		}

		filePath += std::to_string(i + 1) + ".bmp";

		m_pTextures[i] = g_pApp->GetRenderer()->VLoadTexture(filePath);
	}

	m_specularLayer.SetTexture(g_pApp->GetRenderer()->VLoadTexture("defaultspec.dds"));
	m_TerrainTextureLayer.SetTexture(g_pApp->GetRenderer()->VLoadTexture("dirttexture.jpg"));
	m_TerrainTextureLayer.SetTextureAddressModes(ETextureAddressMode::WRAP);

	// Load the shader programs.
	m_pVertexShader = g_pApp->GetRenderer()->VCreateShaderProgram("VertexShader.hlsl", AShaderProgram::SHADER_TYPE_VERTEX);
	m_pPixelShader = g_pApp->GetRenderer()->VCreateShaderProgram("PixelShader.hlsl", AShaderProgram::SHADER_TYPE_PIXEL);

	m_pVertexShader->SetEntryPoint("main");
	m_pVertexShader->SetShaderModel("vs_4_0");
	if (!m_pVertexShader->VLoad()) {
		return false;
	}

	m_pPixelShader->SetEntryPoint("main");
	m_pPixelShader->SetShaderModel("ps_4_0");
	if (!m_pPixelShader->VLoad()) {
		return false;
	}

	m_pVertexParamsPerObject = m_pVertexShader->CreateShaderParams("cbPerObject");
	m_pVertexParamsPerFrame = m_pVertexShader->CreateShaderParams("cbPerFrame");
	m_pPixelParamsPerFrame = m_pPixelShader->CreateShaderParams("cbPerFrame");

	CVector3 eyePosition(0.0f, 2.0f, -10.0f);
	CVector3 lightDirection(0.0f, -1.0f, 1.0f);
	Normalise(lightDirection);

	CLight light;
	light.SetType(LIGHT_DIRECTIONAL);
	light.SetDiffuse(CColourValue::s_WHITE);
	light.SetSpecular(CColourValue::s_WHITE);
	light.SetDirection(Normalise(CVector3(0.0f, -1.0f, 1.0f)));

	m_pPixelParamsPerFrame->SetConstant("gLight.m_Type", light.GetType());
	m_pPixelParamsPerFrame->SetConstant("gLight.m_Diffuse", light.GetDiffuse());
	m_pPixelParamsPerFrame->SetConstant("gLight.m_Specular", light.GetSpecular());
	m_pPixelParamsPerFrame->SetConstant("gLight.m_Direction", light.GetDirection());
	m_pPixelParamsPerFrame->SetConstant("gEyePosition", eyePosition);
	m_pPixelParamsPerFrame->SetConstant("gAmbientLight", CColourValue(0.2f, 0.2f, 0.2f));

	m_pPixelShader->VUpdateProgramParams("cbPerFrame", m_pPixelParamsPerFrame);

	LoadTerrain();

	return true;
}

void CTestProjLogic::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;
	m_TexAnim += deltaTime;

	if (m_ElapsedTime > 4.0f / 120.0f) {
		m_ElapsedTime = 0;

		m_Frame++;
		if (m_Frame >= 120) {
			m_Frame = 0;
		}
	}
}

void CTestProjLogic::Render()
{
	CTextureLayer textureLayer;
	textureLayer.SetTextureFilter(ETextureFilterTypeBroad::TRILINEAR);
	textureLayer.SetTextureAddressModes(ETextureAddressMode::MIRROR);
	textureLayer.SetTextureBorderColour(CColourValue::s_BLUE);
	textureLayer.SetTexture(m_pTextures[m_Frame]);
	g_pApp->GetRenderer()->SetTextureLayer(0, textureLayer);
	g_pApp->GetRenderer()->SetTextureLayer(1, m_specularLayer);

	size_t width, height;
	g_pApp->GetRenderer()->GetWindowSize(width, height);

	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	CMatrix4x4 projectionMatrix = CMatrix4x4::BuildProjection(0.25f * g_PI, aspectRatio, 0.1f, 1000.0f);
	CMatrix4x4 viewMatrix = CMatrix4x4::BuildLookAt(CVector3(0, 2.0f, -10.0f), CVector3(), CVector3::s_UP);
	CMatrix4x4 worldMatrix = CMatrix4x4::BuildTranslation(2.0f, 0.0f, 0.0f);
	CMatrix4x4 texMatrix = CMatrix4x4::BuildTranslation(m_TexAnim, 0.0f, 0.0f) * CMatrix4x4::BuildRotationZ(m_TexAnim) * CMatrix4x4::BuildScale(2.0f, 2.0f, 1.0f);

	//CMatrix4x4 wvp = projectionMatrix * viewMatrix * worldMatrix;
	m_pVertexParamsPerFrame->SetConstant("gProjectionMatrix", projectionMatrix);
	m_pVertexParamsPerFrame->SetConstant("gViewMatrix", viewMatrix);
	m_pVertexParamsPerObject->SetConstant("gWorldMatrix", worldMatrix);
	m_pVertexParamsPerObject->SetConstant("gTexMatrix", texMatrix);

	m_pVertexShader->VUpdateProgramParams("cbPerObject", m_pVertexParamsPerObject);
	m_pVertexShader->VUpdateProgramParams("cbPerFrame", m_pVertexParamsPerFrame);

	g_pApp->GetRenderer()->VSetShaderProgram(m_pVertexShader);
	g_pApp->GetRenderer()->VSetShaderProgram(m_pPixelShader);
	m_VertexDeclaration.SetPrimitiveType(CVertexDeclaration::PRIMITIVE_TYPE_TRIANGLELIST);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, m_pVertexBuffer, m_pIndexBuffer);

	worldMatrix = CMatrix4x4::BuildTranslation(0.0f, -1.0f, 0.0f) * CMatrix4x4::BuildScale(15.0f, 2.0f, 15.0f);
	texMatrix = CMatrix4x4::BuildScale(1.0f, 1.0f, 1.0f);
	//wvp = projectionMatrix * viewMatrix * worldMatrix;

	textureLayer.SetTextureAddressModes(ETextureAddressMode::WRAP);
	textureLayer.SetTextureFilter(ETextureFilterType::ANISOTROPIC, ETextureFilterType::ANISOTROPIC, ETextureFilterType::ANISOTROPIC);
	g_pApp->GetRenderer()->SetTextureLayer(0, m_TerrainTextureLayer);

	m_pVertexParamsPerObject->SetConstant("gTexMatrix", texMatrix);
	m_pVertexParamsPerObject->SetConstant("gWorldMatrix", worldMatrix);
	m_pVertexShader->VUpdateProgramParams("cbPerObject", m_pVertexParamsPerObject);
	m_VertexDeclaration.SetPrimitiveType(CVertexDeclaration::PRIMITIVE_TYPE_TRIANGLESTRIP);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, m_pTerrainVertices, m_pTerrainIndices);

}

void CTestProjLogic::HandleInput(const CInput& input, float deltaTime)
{
	
}

void CTestProjLogic::LoadTerrain()
{
	shared_ptr<ATexture> pHeightMap = g_pApp->GetRenderer()->VLoadTexture("terrain.png", ETextureUsage::READ_ONLY);

	if (pHeightMap) {
		CImage image = pHeightMap->VGetImage();


		int numPixels = image.GetSize();
		std::vector<TVertex> vertices(numPixels);

		for (unsigned int z = 0; z < image.GetHeight(); ++z) {
			for (unsigned int x = 0; x < image.GetWidth(); ++x) {
				CColour colour = image[z][x];
				TVertex vert;

				vert.m_Position.SetX(static_cast<float>(x) / static_cast<float>(image.GetWidth()) - 0.5f);
				vert.m_Position.SetY(static_cast<float>(colour.m_r) / 255.0f - 0.5f);
				vert.m_Position.SetZ(static_cast<float>(z) / static_cast<float>(image.GetHeight()) - 0.5f);
				vert.m_U = (static_cast<float>(x) / static_cast<float>(image.GetWidth() - 1));
				vert.m_V = (static_cast<float>(z) / static_cast<float>(image.GetWidth() - 1));

				vertices[x + z * image.GetWidth()] = vert;
			}
		}

		for (unsigned int z = 0; z < image.GetHeight(); ++z) {
			for (unsigned int x = 0; x < image.GetWidth(); ++x) {
				TVertex currentVert = vertices[x + z * image.GetWidth()];
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
						neighbourX >= static_cast<int>(image.GetWidth()) || 
						neighbourZ >= static_cast<int>(image.GetHeight())) {
						// Is this neighbour outside the image?
						// Don't evaluate it.
						increment(i);

						continue;
					}

					TVertex neighbourVert = vertices[neighbourX + neighbourZ * image.GetWidth()];
					CVector3 currentNormal(0, 0, 0);
					CVector3 currentLine = currentVert.m_Position - neighbourVert.m_Position;

					currentNormal = CrossProduct(lastLine, currentLine);
					averagedNormal += currentNormal;
					lastLine = currentLine;

					increment(i);
				}

				currentVert.m_Normal = Normalise(averagedNormal);
				vertices[x + z * image.GetWidth()] = currentVert;
			}
		}

		m_pTerrainVertices = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);

		int numIndices = ((image.GetWidth() * 2) * (image.GetHeight() - 1) + (image.GetHeight() - 2));

		// Generate indices
		std::vector<int> indices(numIndices);
		int index = 0;

		for (unsigned int z = 0; z < image.GetHeight() - 1; ++z) {
			// Even rows move left to right, odd rows move right to left.
			if (z % 2 == 0) {
				// Is this an even row?
				for (int x = 0; x < static_cast<int>(image.GetWidth()); ++x) {
					indices[index] = x + z * image.GetWidth();
					++index;
					indices[index] = x + z * image.GetWidth() + image.GetWidth();	// Next row.
					++index;
				}

				// Insert degenerate vertex, if this isn't the last row.
				if (z != image.GetHeight() - 2) {
					// Is this the last row?
					indices[index] = image.GetWidth() - 1 + (z * image.GetWidth());
					++index;
				}
			}
			else {
				// This is an odd row.
				for (int x = static_cast<int>(image.GetWidth()) - 1; x >= 0; --x) {
					indices[index] = x + z * image.GetWidth();
					++index;
					indices[index] = x + z * image.GetWidth() + image.GetWidth();	// Next row.
					++index;
				}

				// Insert degenerate vertex, if this isn't the last row.
				if (z != image.GetHeight() - 2) {
					// Is this the last row?
					indices[index] = z * image.GetWidth();
					++index;
				}
			}
		}

		m_pTerrainIndices = g_pApp->GetRenderer()->CreateIndexBuffer(indices);
	}
}
