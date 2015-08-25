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
	m_ProjectionMatrix(CMatrix4x4::s_IDENTITY),
	m_pShaderParamsPerFrame(new CShaderParams()),
	m_pShaderParamsPerObject(new CShaderParams()),
	m_pTextures(120, nullptr)
{
	m_ViewMatrix = CMatrix4x4::BuildLookAt(CVector3(0, 0, 500.0f), CVector3(), CVector3::s_UP);
	m_ProjectionMatrix = CMatrix4x4::BuildProjection(DegreesToRadians(90), 1.0f, 0.1f, 1000.0f);
}

CTestProjLogic::~CTestProjLogic()
{

}

bool CTestProjLogic::Initialise()
{
	TVertex pCubeVerts[] = {
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

	g_pApp->GetRenderer()->VSetBackgroundColour(TColourValue::s_GREEN);

	m_pVertexBuffer = g_pApp->GetRenderer()->VCreateVertexBuffer(
		24, 
		sizeof(TVertex), 
		pCubeVerts, 
		AGpuBuffer::USAGE_STATIC,
		false);

	m_pIndexBuffer = g_pApp->GetRenderer()->VCreateIndexBuffer(
		36,
		AIndexGpuBuffer::INDEX_TYPE_16,
		pIndices,
		AGpuBuffer::USAGE_STATIC,
		false);

	// Create the input layout.
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);
	m_VertexDeclaration.SetPrimitiveType(CVertexDeclaration::PRIMITIVE_TYPE_TRIANGLELIST);

	// Load the shader programs.
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

	m_pShaderParamsPerObject->AddConstant("gWorldMatrix", CShaderConstant::TYPE_MATRIX4X4);
	m_pShaderParamsPerObject->AddConstant("gTexMatrix", CShaderConstant::TYPE_MATRIX4X4);

	m_pShaderParamsPerFrame->AddConstant("gViewMatrix", CShaderConstant::TYPE_MATRIX4X4);
	m_pShaderParamsPerFrame->AddConstant("gProjectionMatrix", CShaderConstant::TYPE_MATRIX4X4);

	m_pShaderParamsPerFrame->SetConstant("gViewMatrix", m_ViewMatrix);
	m_pShaderParamsPerFrame->SetConstant("gProjectionMatrix", m_ProjectionMatrix);

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
	/*D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 0.25f * g_PI, 1.3f, 0.1f, 1000.0f);

	D3DXMATRIX viewMatrix;
	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 eye(0, 0, -10.0f);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);

	D3DXMATRIX worldMatrix;
	D3DXMatrixTranslation(&worldMatrix, 2.0f, 0.0f, 0.0f);

	D3DXMATRIX wvp = worldMatrix * viewMatrix * projectionMatrix;
	D3DXMatrixTranspose(&wvp, &wvp);

	m_pShaderParamsPerObject->SetConstant("gWorldMatrix", (float*)&wvp, 16);
	m_pVertexShader->VUpdateProgramParams("cbPerObject", m_pShaderParamsPerObject);
	m_pVertexShader->VUpdateProgramParams("cbPerFrame", m_pShaderParamsPerFrame);

	g_pApp->GetRenderer()->VSetShaderProgram(m_pVertexShader);
	g_pApp->GetRenderer()->VSetShaderProgram(m_pPixelShader);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, m_pVertexBuffer, m_pIndexBuffer);*/

	CTextureLayer textureLayer;
	textureLayer.SetTextureFilter(TFILTERB_TRILINEAR);
	textureLayer.SetTextureAddressModes(TADDRESS_MIRROR);
	textureLayer.SetTextureBorderColour(TColourValue::s_BLUE);
	textureLayer.SetTexture(m_pTextures[m_Frame]);
	g_pApp->GetRenderer()->SetTextureLayer(0, textureLayer);

	size_t width, height;
	g_pApp->GetRenderer()->GetWindowSize(width, height);

	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	CMatrix4x4 projectionMatrix = CMatrix4x4::BuildProjection(0.25f * g_PI, aspectRatio, 0.1f, 1000.0f);
	CMatrix4x4 viewMatrix = CMatrix4x4::BuildLookAt(CVector3(0, 2.0f, -10.0f), CVector3(), CVector3::s_UP);
	CMatrix4x4 worldMatrix = CMatrix4x4::BuildTranslation(2.0f, 0.0f, 0.0f);
	CMatrix4x4 texMatrix = CMatrix4x4::BuildTranslation(m_TexAnim, 0.0f, 0.0f) * CMatrix4x4::BuildRotationZ(m_TexAnim) * CMatrix4x4::BuildScale(2.0f, 2.0f, 1.0f);

	//CMatrix4x4 wvp = projectionMatrix * viewMatrix * worldMatrix;
	m_pShaderParamsPerFrame->SetConstant("gProjectionMatrix", projectionMatrix);
	m_pShaderParamsPerFrame->SetConstant("gViewMatrix", viewMatrix);
	m_pShaderParamsPerObject->SetConstant("gWorldMatrix", worldMatrix);
	m_pShaderParamsPerObject->SetConstant("gTexMatrix", texMatrix);

	m_pVertexShader->VUpdateProgramParams("cbPerObject", m_pShaderParamsPerObject);
	m_pVertexShader->VUpdateProgramParams("cbPerFrame", m_pShaderParamsPerFrame);

	g_pApp->GetRenderer()->VSetShaderProgram(m_pVertexShader);
	g_pApp->GetRenderer()->VSetShaderProgram(m_pPixelShader);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, m_pVertexBuffer, m_pIndexBuffer);

	worldMatrix = CMatrix4x4::BuildTranslation(-2.0f, 0.0f, 0.0f);
	texMatrix = CMatrix4x4::BuildTranslation(m_TexAnim, 0.0f, 0.0f);
	//wvp = projectionMatrix * viewMatrix * worldMatrix;

	textureLayer.SetTextureAddressModes(TADDRESS_MIRROR);
	textureLayer.SetTextureFilter(TFILTER_ANISOTROPIC, TFILTER_ANISOTROPIC, TFILTER_ANISOTROPIC);
	g_pApp->GetRenderer()->SetTextureLayer(0, textureLayer);

	m_pShaderParamsPerObject->SetConstant("gTexMatrix", texMatrix);
	m_pShaderParamsPerObject->SetConstant("gWorldMatrix", worldMatrix);
	m_pVertexShader->VUpdateProgramParams("cbPerObject", m_pShaderParamsPerObject);
	g_pApp->GetRenderer()->VRender(m_VertexDeclaration, m_pVertexBuffer, m_pIndexBuffer);

}
