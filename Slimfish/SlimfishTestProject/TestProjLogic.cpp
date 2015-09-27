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
#include <Math\SlimVector.h>
#include <Graphics\SlimRenderer.h>
#include <Graphics\SlimLight.h>
#include <DirectXMath.h>

// This Include
#include "TestProjLogic.h"

// Local Includes

struct TVertex {
	CVector3 m_Position;
	CVector3 m_Normal;
	CVector3 m_Tangent;
	float m_U;
	float m_V;
};

struct TSkyBoxVertex {
	CVector3 m_Position;
	float m_U;
	float m_V;
};

CTestProjLogic::CTestProjLogic()
	:m_TerrainWorldTransform(CMatrix4x4::s_IDENTITY),
	m_WaterTransform(CMatrix4x4::s_IDENTITY),
	m_ViewMatrix(CMatrix4x4::s_IDENTITY),
	m_ProjectionMatrix(CMatrix4x4::s_IDENTITY)
{
	m_WaterTransform = CMatrix4x4::BuildScale(400.0f, 1.0f, 400.0f);
	m_TerrainWorldTransform = CMatrix4x4::BuildTranslation(0.0f, -10.0f, 0.0f) * CMatrix4x4::BuildRotationY(DegreesToRadians(0));
	m_ProjectionMatrix = CMatrix4x4::BuildProjection(DegreesToRadians(110), 1.0f, 0.1f, 1000.0f);
	m_SkyBoxWorldTransform = CMatrix4x4::BuildScale(999, 999, 999);

	m_ViewMatrix = CMatrix4x4::BuildYawPitchRoll(m_CameraYaw, m_CameraPitch, 0);
	CMatrix4x4 invView = m_ViewMatrix * CMatrix4x4::BuildTranslation(0.0f, 0.0f, 50.0f);
	m_EyePosition = invView.GetPosition();
	m_ViewMatrix = CMatrix4x4::BuildTranslation(0.0f, 0.0f, -50.0f) * m_ViewMatrix.GetTranspose();
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
	CLight light;
	light.SetType(LIGHT_DIRECTIONAL);
	light.SetDiffuse(CColourValue(0.7f, 0.7f, 0.7f));
	light.SetSpecular(CColourValue(1.0f, 0.8f, 0.6f, 100.0f));

	light.SetDirection(CVector3::Normalise(CVector3(-1.0f, -1.5f, 1.0f)));
	float atten[3] = { light.GetAttenuationConstant(), light.GetAttenuationLinear(), light.GetAttenuationQuadratic() };

	// Create the input layout.
	m_WaterVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_WaterVertexDeclaration.AddElement("NORMAL", CInputElement::FORMAT_FLOAT3);
	m_WaterVertexDeclaration.AddElement("TANGENT", CInputElement::FORMAT_FLOAT3);
	m_WaterVertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);

	CTextureLayer waterTexture;
	CTextureLayer waterSpecular;
	CTextureLayer waterNormalMap;
	waterTexture.SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	waterSpecular.SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	waterNormalMap.SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	waterTexture.SetTexture(g_pApp->GetRenderer()->VLoadTexture("WaterTextures/Wavy_Water - Transparent.png"));
	waterSpecular.SetTexture(g_pApp->GetRenderer()->VLoadTexture("WaterTextures/Specular.png"));
	waterNormalMap.SetTexture(g_pApp->GetRenderer()->VLoadTexture("WaterTextures/Wavy_Water - Height (Normal Map 2).png"));
	m_WaterRenderPass.AddTextureLayer(waterTexture);
	m_WaterRenderPass.AddTextureLayer(waterSpecular);
	m_WaterRenderPass.AddTextureLayer(waterNormalMap);

	TBlendingMode blendingMode;
	//blendingMode.sourceColourBlendFactor = EBlendFactor::SOURCE_ALPHA;
	//blendingMode.destColourBlendFactor = EBlendFactor::INVERSE_SOURCE_ALPHA;
	m_WaterRenderPass.SetBlendingMode(blendingMode);

	m_WaterRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("WaterVS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_WaterRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("WaterPS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));

	m_WaterVSParams = m_WaterRenderPass.GetVertexShader()->VCreateShaderParams("constantBuffer");
	m_WaterPSParams = m_WaterRenderPass.GetPixelShader()->VCreateShaderParams("constantBuffer");

	m_WaterVSParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_WaterVSParams->SetConstant("gProjectionMatrix", m_ProjectionMatrix);
	m_WaterPSParams->SetConstant("gLight.type", light.GetType());
	m_WaterPSParams->SetConstant("gLight.diffuse", light.GetDiffuse());
	m_WaterPSParams->SetConstant("gLight.specular", light.GetSpecular());
	m_WaterPSParams->SetConstant("gLight.direction", light.GetDirection());
	m_WaterPSParams->SetConstant("gLight.range", light.GetRange());
	m_WaterPSParams->SetConstant("gLight.attenuation", atten, 3);
	m_WaterPSParams->SetConstant("gEyePosition", m_ViewMatrix.GetPosition());
	m_WaterPSParams->SetConstant("gFogStart", 200.0f);
	m_WaterPSParams->SetConstant("gFogRange", 40.0f);
	m_WaterPSParams->SetConstant("gFogColour", CColourValue(0.3686f, 0.3686f, 0.43137f, 1.0f));
	m_WaterPSParams->SetConstant("gAmbientLight", CColourValue(0.3f, 0.3f, 0.3f));

	size_t windowWidth, windowHeight;
	g_pApp->GetRenderer()->GetWindowSize(windowWidth, windowHeight);
	m_pReflectionRenderTarget = g_pApp->GetRenderer()->VCreateRenderTexture("Reflection", windowWidth, windowHeight);
	m_pRefractionRenderTarget = g_pApp->GetRenderer()->VCreateRenderTexture("Refraction", windowWidth, windowHeight);
	CTextureLayer reflectionLayer;
	CTextureLayer refractionLayer;
	reflectionLayer.SetTextureAddressModes(ETextureAddressMode::MIRROR);
	refractionLayer.SetTextureAddressModes(ETextureAddressMode::MIRROR);
	reflectionLayer.SetTexture(m_pReflectionRenderTarget->GetTexture());
	refractionLayer.SetTexture(m_pRefractionRenderTarget->GetTexture());
	m_WaterRenderPass.AddTextureLayer(refractionLayer);
	m_WaterRenderPass.AddTextureLayer(reflectionLayer);

	//
	// TERRAIN STUFFS
	//
	
	// Init shaders.
	m_TerrainRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("TerrainVS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_TerrainRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("TerrainPS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_TerrainVSParams = m_TerrainRenderPass.GetVertexShader()->VCreateShaderParams("constantBuffer");
	m_TerrainVSParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_TerrainVSParams->SetConstant("gProjectionMatrix", m_ProjectionMatrix);
	m_TerrainVSParams->SetConstant("gWorldMatrix", m_TerrainWorldTransform);
	m_TerrainVSParams->SetConstant("gTexMatrix", CMatrix4x4::BuildScale(10.0f, 10.0f, 1.0f));
	m_TerrainRenderPass.GetPixelShader()->VCreateShaderParams("constantBuffer");

	// Init texture layers.
	CTextureLayer terrainGrass;
	CTextureLayer terrainCliff;
	CTextureLayer terrainSpecular;
	terrainGrass.SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	terrainCliff.SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	terrainSpecular.SetTextureFilter(ETextureFilterType::ANISOTROPIC);
	terrainGrass.SetTexture(g_pApp->GetRenderer()->VLoadTexture("terraingrass.jpg"));
	terrainCliff.SetTexture(g_pApp->GetRenderer()->VLoadTexture("terraincliff.jpg"));
	terrainSpecular.SetTexture(g_pApp->GetRenderer()->VLoadTexture("defaultspec.dds"));
	m_TerrainRenderPass.AddTextureLayer(terrainGrass);
	m_TerrainRenderPass.AddTextureLayer(terrainCliff);
	m_TerrainRenderPass.AddTextureLayer(terrainSpecular);

	m_TerrainVSParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_TerrainVSParams->SetConstant("gProjectionMatrix", m_ProjectionMatrix);

	//
	// SKY BOX STUFFS
	//
	m_SkyBoxVertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_SkyBoxVertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);

	CTextureLayer skyboxTexture;
	skyboxTexture.SetTexture(g_pApp->GetRenderer()->VLoadTexture("skybox2.dds", ETextureType::TYPE_CUBIC));
	m_SkyBoxRenderPass.AddTextureLayer(skyboxTexture);

	m_SkyBoxRenderPass.SetVertexShader(g_pApp->GetRenderer()->VCreateShaderProgram("SkyBoxVS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_SkyBoxRenderPass.SetPixelShader(g_pApp->GetRenderer()->VCreateShaderProgram("SkyBoxPS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));
	m_SkyBoxRenderPass.SetDepthCompareFunction(EComparisonFunction::LESS_EQUAL);
	m_SkyBoxRenderPass.SetCullingMode(ECullingMode::NONE);

	m_pSkyBoxShaderParams = m_SkyBoxRenderPass.GetVertexShader()->VCreateShaderParams("constantBuffer");
	m_pSkyBoxShaderParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_pSkyBoxShaderParams->SetConstant("gProjectionMatrix", m_ProjectionMatrix);
	m_SkyBoxRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_pSkyBoxShaderParams);

	g_pApp->GetRenderer()->VSetBackgroundColour(CColourValue(0.3f, 0.7f, 0.5f));

	LoadTerrain(CVector3(400.0f, 120.0f, 400.0f));
	BuildWater(40, 40);
	BuildSkySphere(10, 10);

	return true;
}

void CTestProjLogic::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;
	m_WaterWaveTransform1 = CMatrix4x4::BuildTranslation(m_ElapsedTime * -0.02f, m_ElapsedTime * 0.0f, 0.0f) *		CMatrix4x4::BuildScale(6.0f, 6.0f, 6.0f);
	m_WaterWaveTransform2 = CMatrix4x4::BuildTranslation(m_ElapsedTime * -0.05f, m_ElapsedTime * -0.05f, 0.0f) *	CMatrix4x4::BuildScale(6.0f, 6.0f, 6.0f);
	m_WaterWaveTransform3 = CMatrix4x4::BuildTranslation(m_ElapsedTime * -0.07f, m_ElapsedTime * 0.05f, 0.0f) *		CMatrix4x4::BuildScale(6.0f, 6.0f, 6.0f);
	m_WaterWaveTransform4 = CMatrix4x4::BuildTranslation(m_ElapsedTime * 0.1f, m_ElapsedTime * 0.07f, 0.0f) *		CMatrix4x4::BuildScale(6.0f, 6.0f, 6.0f);
}

void CTestProjLogic::Render()
{
	CMatrix4x4 reflect = CMatrix4x4::BuildReflect(CVector3(0.0f, 1.0f, 0.0f), 0.0f);

	g_pApp->GetRenderer()->VSetRenderTarget(m_pRefractionRenderTarget.get());
	g_pApp->GetRenderer()->VPreRender();

	m_pSkyBoxShaderParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_pSkyBoxShaderParams->SetConstant("gWorldMatrix", m_SkyBoxWorldTransform);
	m_SkyBoxRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_pSkyBoxShaderParams);

	// Draw the skybox.
	g_pApp->GetRenderer()->SetRenderPass(&m_SkyBoxRenderPass);
	g_pApp->GetRenderer()->VRender(m_SkyBoxVertexDeclaration, EPrimitiveType::TRIANGLELIST, m_pSkyBoxVertices, m_pSkyBoxIndices);

	// Update terrain parameters.
	m_TerrainVSParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_TerrainVSParams->SetConstant("gWorldMatrix", m_TerrainWorldTransform);
	m_TerrainRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_TerrainVSParams);
	m_TerrainRenderPass.GetPixelShader()->VUpdateShaderParams("constantBuffer", m_WaterPSParams);

	// Draw the terrain.
	g_pApp->GetRenderer()->SetRenderPass(&m_TerrainRenderPass);
	g_pApp->GetRenderer()->VRender(m_WaterVertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pTerrainVertices, m_pTerrainIndices);

	//
	// Pass 2 draw the reflection.
	//
	g_pApp->GetRenderer()->VSetRenderTarget(m_pReflectionRenderTarget.get());
	g_pApp->GetRenderer()->VPreRender();

	float pPlane[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
	float pNotAPlane[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_pSkyBoxShaderParams->SetConstant("gViewMatrix", m_ReflectionViewMatrix);
	m_pSkyBoxShaderParams->SetConstant("gWorldMatrix", m_SkyBoxWorldTransform);
	m_pSkyBoxShaderParams->SetConstant("gClipPlane", pPlane, 4);
	m_SkyBoxRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_pSkyBoxShaderParams);

	// Draw the skybox.
	g_pApp->GetRenderer()->SetRenderPass(&m_SkyBoxRenderPass);
	g_pApp->GetRenderer()->VRender(m_SkyBoxVertexDeclaration, EPrimitiveType::TRIANGLELIST, m_pSkyBoxVertices, m_pSkyBoxIndices);

	// Update terrain parameters.
	m_TerrainVSParams->SetConstant("gViewMatrix", m_ReflectionViewMatrix);
	m_TerrainVSParams->SetConstant("gWorldMatrix", m_TerrainWorldTransform);
	m_TerrainVSParams->SetConstant("gClipPlane", pPlane, 4);
	m_TerrainRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_TerrainVSParams);
	m_TerrainRenderPass.GetPixelShader()->VUpdateShaderParams("constantBuffer", m_WaterPSParams);

	// Draw the terrain.
	g_pApp->GetRenderer()->SetRenderPass(&m_TerrainRenderPass);
	g_pApp->GetRenderer()->VRender(m_WaterVertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pTerrainVertices, m_pTerrainIndices);

	// Remove render target.
	g_pApp->GetRenderer()->VSetRenderTarget(nullptr);
	g_pApp->GetRenderer()->VPreRender();

	// Draw the water.
	m_WaterVSParams->SetConstant("gWorldMatrix", m_WaterTransform);
	m_WaterVSParams->SetConstant("gWaveMatrix0", m_WaterWaveTransform1);
	m_WaterVSParams->SetConstant("gWaveMatrix1", m_WaterWaveTransform2);
	m_WaterVSParams->SetConstant("gWaveMatrix2", m_WaterWaveTransform3);
	m_WaterVSParams->SetConstant("gWaveMatrix3", m_WaterWaveTransform4);
	m_WaterVSParams->SetConstant("gReflectionMatrix", m_ReflectionViewMatrix);
	m_WaterVSParams->SetConstant("gElapsedTime", m_ElapsedTime);
	m_WaterVSParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_WaterRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_WaterVSParams);

	m_WaterPSParams->SetConstant("gEyePosition", m_EyePosition);
	m_WaterRenderPass.GetPixelShader()->VUpdateShaderParams("constantBuffer", m_WaterPSParams);

	m_WaterRenderPass.SetCullingMode(ECullingMode::NONE);

	// Draw the water.
	g_pApp->GetRenderer()->SetRenderPass(&m_WaterRenderPass);
	g_pApp->GetRenderer()->VRender(m_WaterVertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pWaterVertices, m_pWaterIndices);

	//
	// Draw the refraction map to the back buffer.
	//

	m_pSkyBoxShaderParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_pSkyBoxShaderParams->SetConstant("gWorldMatrix", m_SkyBoxWorldTransform);
	m_pSkyBoxShaderParams->SetConstant("gClipPlane", pNotAPlane, 4);
	m_SkyBoxRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_pSkyBoxShaderParams);

	// Draw the skybox.
	g_pApp->GetRenderer()->SetRenderPass(&m_SkyBoxRenderPass);
	g_pApp->GetRenderer()->VRender(m_SkyBoxVertexDeclaration, EPrimitiveType::TRIANGLELIST, m_pSkyBoxVertices, m_pSkyBoxIndices);

	// Update terrain parameters.
	m_TerrainVSParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_TerrainVSParams->SetConstant("gWorldMatrix", m_TerrainWorldTransform);
	m_TerrainVSParams->SetConstant("gClipPlane", pNotAPlane, 4);
	m_TerrainRenderPass.GetVertexShader()->VUpdateShaderParams("constantBuffer", m_TerrainVSParams);
	m_TerrainRenderPass.GetPixelShader()->VUpdateShaderParams("constantBuffer", m_WaterPSParams);

	// Draw the terrain.
	g_pApp->GetRenderer()->SetRenderPass(&m_TerrainRenderPass);
	g_pApp->GetRenderer()->VRender(m_WaterVertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pTerrainVertices, m_pTerrainIndices);


	/*// Update skybox parameters.
	m_pSkyBoxShaderParams->SetConstant("gViewMatrix", m_ViewMatrix);
	m_pSkyBoxShaderParams->SetConstant("gWorldMatrix", m_SkyBoxWorldTransform);
	m_SkyBoxRenderPass.GetVertexShader()->VUpdateShaderParams("cbPerFrame", m_pSkyBoxShaderParams);

	// Draw the skybox.
	g_pApp->GetRenderer()->SetRenderPass(&m_SkyBoxRenderPass);
	g_pApp->GetRenderer()->VRender(m_SkyBoxVertexDeclaration, m_pSkyBoxVertices, m_pSkyBoxIndices);

	// Update terrain parameters.
	m_TerrainVSParamsPerFrame->SetConstant("gElapsedTime", m_ElapsedTime);
	m_TerrainVSParamsPerFrame->SetConstant("gViewMatrix", m_ViewMatrix);
	m_TerrainRenderPass.GetVertexShader()->VUpdateShaderParams("cbPerFrame", m_TerrainVSParamsPerFrame);

	m_TerrainVSParamsPerObject->SetConstant("gWorldMatrix", m_TerrainWorldTransform);
	m_TerrainRenderPass.GetVertexShader()->VUpdateShaderParams("cbPerObject", m_TerrainVSParamsPerObject);

	m_TerrainRenderPass.GetPixelShader()->VUpdateShaderParams("cbPerFrame", m_WaterPSParams);

	// Draw the terrain.
	g_pApp->GetRenderer()->SetRenderPass(&m_TerrainRenderPass);
	g_pApp->GetRenderer()->VRender(m_WaterVertexDeclaration, m_pTerrainVertices, m_pTerrainIndices);

	CRenderPass markMirrorPass = m_WaterRenderPass;
	TBlendingMode standardBlendingMode;
	markMirrorPass.SetBlendingMode(standardBlendingMode);
	markMirrorPass.SetColourWritesEnabled(false);
	markMirrorPass.SetDepthWriteEnabled(false);
	markMirrorPass.SetStencilCheckEnabled(true);
	markMirrorPass.SetStencilCompareFunction(EComparisonFunction::ALWAYS);
	markMirrorPass.SetStencilFailOperation(EStencilOperation::KEEP);
	markMirrorPass.SetStencilDepthFailOperation(EStencilOperation::KEEP);
	markMirrorPass.SetStencilPassOperation(EStencilOperation::REPLACE);
	markMirrorPass.SetStencilReferenceValue(1);

	// Draw the water to the stencil buffer.
	g_pApp->GetRenderer()->SetRenderPass(&markMirrorPass);
	g_pApp->GetRenderer()->VRender(m_WaterVertexDeclaration, m_pWaterVertices, m_pWaterIndices);

	// Draw the reflected skybox.
	CMatrix4x4 reflectMatrix = CMatrix4x4::BuildReflect(CVector3(0, 1, 0), 0);
	CMatrix4x4 skyBoxWorld = CMatrix4x4::BuildScale(0.9f, 0.9f, 0.9f) * m_SkyBoxWorldTransform * reflectMatrix;

	m_pSkyBoxShaderParams->SetConstant("gWorldMatrix", skyBoxWorld);
	m_SkyBoxRenderPass.GetVertexShader()->VUpdateShaderParams("cbPerFrame", m_pSkyBoxShaderParams);

	CRenderPass drawReflectionPass = m_SkyBoxRenderPass;
	drawReflectionPass.SetStencilCheckEnabled(true);
	drawReflectionPass.SetDepthCompareFunction(EComparisonFunction::ALWAYS);
	drawReflectionPass.SetCullingMode(ECullingMode::NONE);
	drawReflectionPass.SetStencilCompareFunction(EComparisonFunction::EQUAL);
	drawReflectionPass.SetStencilFailOperation(EStencilOperation::KEEP);
	drawReflectionPass.SetStencilDepthFailOperation(EStencilOperation::KEEP);
	drawReflectionPass.SetStencilPassOperation(EStencilOperation::KEEP);
	drawReflectionPass.SetStencilReferenceValue(1);

	g_pApp->GetRenderer()->SetRenderPass(&drawReflectionPass);
	g_pApp->GetRenderer()->VRender(m_SkyBoxVertexDeclaration, m_pSkyBoxVertices, m_pSkyBoxIndices);

	m_TerrainVSParamsPerObject->SetConstant("gWorldMatrix", m_TerrainWorldTransform * reflectMatrix);
	m_TerrainRenderPass.GetVertexShader()->VUpdateShaderParams("cbPerObject", m_TerrainVSParamsPerObject);

	CRenderPass drawReflectionPass2 = m_TerrainRenderPass;
	drawReflectionPass2.SetStencilCheckEnabled(true);
	drawReflectionPass2.SetDepthCompareFunction(EComparisonFunction::LESS);
	drawReflectionPass2.SetCullingMode(ECullingMode::COUNTER_CLOCKWISE);
	drawReflectionPass2.SetStencilCompareFunction(EComparisonFunction::EQUAL);
	drawReflectionPass2.SetStencilFailOperation(EStencilOperation::KEEP);
	drawReflectionPass2.SetStencilDepthFailOperation(EStencilOperation::KEEP);
	drawReflectionPass2.SetStencilPassOperation(EStencilOperation::KEEP);
	drawReflectionPass2.SetStencilReferenceValue(1);

	g_pApp->GetRenderer()->SetRenderPass(&drawReflectionPass2);
	g_pApp->GetRenderer()->VRender(m_WaterVertexDeclaration, m_pTerrainVertices, m_pTerrainIndices);

	// Update water shader parameters.
	m_WaterVSParamsPerObject->SetConstant("gWorldMatrix", m_WaterTransform);
	m_WaterVSParamsPerObject->SetConstant("gWaveMatrix0", m_WaterWaveTransform1);
	m_WaterVSParamsPerObject->SetConstant("gWaveMatrix1", m_WaterWaveTransform2);
	m_WaterVSParamsPerObject->SetConstant("gWaveMatrix1", m_WaterWaveTransform3);
	m_WaterVSParamsPerObject->SetConstant("gWaveMatrix1", m_WaterWaveTransform4);
	m_WaterRenderPass.GetVertexShader()->VUpdateShaderParams("cbPerObject", m_WaterVSParamsPerObject);

	m_WaterVSParamsPerFrame->SetConstant("gElapsedTime", m_ElapsedTime);
	m_WaterVSParamsPerFrame->SetConstant("gViewMatrix", m_ViewMatrix);
	m_WaterRenderPass.GetVertexShader()->VUpdateShaderParams("cbPerFrame", m_WaterVSParamsPerFrame);

	m_WaterPSParams->SetConstant("gEyePosition", m_EyePosition);
	m_WaterRenderPass.GetPixelShader()->VUpdateShaderParams("cbPerFrame", m_WaterPSParams);

	// Draw the water.
	g_pApp->GetRenderer()->SetRenderPass(&m_WaterRenderPass);
	g_pApp->GetRenderer()->VRender(m_WaterVertexDeclaration, m_pWaterVertices, m_pWaterIndices);*/
}

void CTestProjLogic::HandleInput(const CInput& input, float deltaTime)
{
	int x;
	int y;
	input.GetMousePosition(x, y);

	if (input.IsMouseButtonDown(EMouseButton::MIDDLE)) {
		input.GetMousePosition(x, y);
		m_CameraYaw -= (x - m_LastMouseX) * 0.01f;
		m_CameraPitch -= (y - m_LastMouseY) * 0.01f;

		m_LastMouseX = x;
		m_LastMouseY = y;

		m_ViewMatrix = CMatrix4x4::BuildYawPitchRoll(m_CameraYaw, m_CameraPitch, 0);
		CMatrix4x4 invView = m_ViewMatrix * CMatrix4x4::BuildTranslation(0.0f, 0.0f, 50.0f);
		m_EyePosition = invView.GetPosition();
		m_ViewMatrix = CMatrix4x4::BuildTranslation(0.0f, 0.0f, -50.0f) * m_ViewMatrix.GetTranspose();

		// Create reflection view matrix.
		CVector3 reflectedPosition = m_EyePosition;
		reflectedPosition.SetY(-reflectedPosition.GetY() + (m_WaterTransform.GetPosition().GetY() * 2.0f));

		float reflectedYaw = m_CameraYaw;
		float reflectedPitch = -m_CameraPitch;

		CMatrix4x4 reflectedRotation = CMatrix4x4::BuildYawPitchRoll(reflectedYaw, reflectedPitch, 0);
		CVector3 look = reflectedRotation.TransformNormal(CVector3::s_FORWARD);
		CVector3 up = reflectedRotation.TransformNormal(CVector3::s_UP);

		CVector3 lookAt = reflectedPosition + look;
		m_ReflectionViewMatrix = CMatrix4x4::BuildLookAt(reflectedPosition, lookAt, up);

		/*CVector3 viewRight = m_ViewMatrix.GetRight();
		CVector3 viewUp = m_ViewMatrix.GetUp();
		CVector3 viewForward = m_ViewMatrix.GetDirection();
		m_ReflectionViewMatrix = CMatrix4x4::BuildRotationFromAxis(-viewRight, -viewUp, viewForward);

		CVector3 reflectedPosition = m_ViewMatrix.GetPosition();
		reflectedPosition.SetY(-reflectedPosition.GetY() + (m_WaterTransform.GetPosition().GetY() * 2.0f));
		m_ReflectionViewMatrix.SetPosition(reflectedPosition);*/
	}

	m_LastMouseX = x;
	m_LastMouseY = y;

}

void CTestProjLogic::BuildWater(size_t n, size_t m) 
{
	// Generate vertices.
	std::vector<TVertex> vertices(n * m);
	for (unsigned int z = 0; z < m; ++z) {
		for (unsigned int x = 0; x < n; ++x) {
			TVertex vert;
			vert.m_Position.SetX(static_cast<float>(x) / static_cast<float>(n) - 0.5f);
			vert.m_Position.SetY(0.0f);
			vert.m_Position.SetZ(static_cast<float>(z) / static_cast<float>(m) - 0.5f);
			vert.m_Normal = CVector3::s_UP;
			vert.m_Tangent = CVector3::s_RIGHT;
			vert.m_U = (static_cast<float>(x) / static_cast<float>(n - 1));
			vert.m_V = (static_cast<float>(z) / static_cast<float>(n - 1));

			vertices[x + z * n] = vert;
		}
	}

	m_pWaterVertices = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
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
			for (int x = static_cast<int>(n) - 1; x >= 0; --x) {
				indices[index++] = x + z * n;
				indices[index++] = x + z * n + n;	// Next row.
			}

			// Insert degenerate vertex, if this isn't the last row.
			if (z != m - 2) {
				indices[index++] = z * n;
			}
		}
	}

	m_pWaterIndices = g_pApp->GetRenderer()->CreateIndexBuffer(indices);
}

void CTestProjLogic::BuildSkySphere(int rings, int segments)
{
	std::vector<TSkyBoxVertex> vertices((rings + 1) * (segments + 1));
	std::vector<short> indices(6 * rings * (segments + 1));
	int index = 0;
	int vertexIndex = 0;

	float fDeltaRingAngle = g_PI / rings;
	float fDeltaSegAngle = 2 * g_PI / segments;

	for (int ring = 0; ring <= rings; ++ring) {
		float r0 = 1.0f * std::sinf(ring * fDeltaRingAngle);
		float y0 = 1.0f * std::cosf(ring * fDeltaRingAngle);

		for (int seg = 0; seg <= segments; ++seg) {
			float x0 = r0 * std::sinf(seg * fDeltaSegAngle);
			float z0 = r0 * std::cosf(seg * fDeltaSegAngle);

			vertices[ring * segments + seg].m_Position = CVector3(x0, y0, z0);
			vertices[ring * segments + seg].m_U = static_cast<float>(seg) / static_cast<float>(segments);
			vertices[ring * segments + seg].m_V = static_cast<float>(ring) / static_cast<float>(rings);

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

	m_pSkyBoxVertices = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
	m_pSkyBoxIndices = g_pApp->GetRenderer()->CreateIndexBuffer(indices);
}

void CTestProjLogic::LoadTerrain(const CVector3& scale)
{
	shared_ptr<ATexture> pHeightMap = g_pApp->GetRenderer()->VLoadTexture("TerrainTextures/HeightMap - Island.png", ETextureType::TYPE_2D, ETextureUsage::READ_ONLY);

	if (pHeightMap) {
		CImage image = pHeightMap->VGetImage();

		int numPixels = image.GetSize();
		std::vector<TVertex> vertices(numPixels);

		for (unsigned int z = 0; z < image.GetHeight(); ++z) {
			for (unsigned int x = 0; x < image.GetWidth(); ++x) {
				CColour colour = image[z][x];
				TVertex vert;

				vert.m_Position.SetX((static_cast<float>(x) / static_cast<float>(image.GetWidth()) - 0.5f) * scale.GetX());
				vert.m_Position.SetY((static_cast<float>(colour.m_r) / 255.0f - 0.5f) * scale.GetY());
				vert.m_Position.SetZ((static_cast<float>(z) / static_cast<float>(image.GetHeight()) - 0.5f) * scale.GetZ());
				vert.m_U = (static_cast<float>(x) / static_cast<float>(image.GetWidth() - 1));
				vert.m_V = (static_cast<float>(z) / static_cast<float>(image.GetWidth() - 1));

				vertices[x + z * image.GetWidth()] = vert;
			}
		}

		// Compute normals.
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

					currentNormal = CVector3::CrossProduct(lastLine, currentLine);
					averagedNormal += currentNormal;
					lastLine = currentLine;

					increment(i);
				}

				currentVert.m_Normal = CVector3::Normalise(averagedNormal);
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

	/*shared_ptr<ATexture> pHeightMap = g_pApp->GetRenderer()->VLoadTexture("HighRes.bmp", ETextureType::TYPE_2D, ETextureUsage::READ_ONLY);

	if (pHeightMap) {
		CImage image = pHeightMap->VGetImage();

		int numPolys = 2 * (image.GetWidth() - 1) * (image.GetHeight() - 1);
		std::vector<TVertex> vertices(numPolys * 3);

		auto createVert = [&](int x, int z) {
			CColour colour = image.GetPixel(x, z);
			TVertex vert;

			vert.m_Position.SetX((static_cast<float>(x) / static_cast<float>(image.GetWidth())) - 0.5f);
			vert.m_Position.SetY((static_cast<float>(colour.m_r) / 255.0f) - 0.5f);
			vert.m_Position.SetZ((static_cast<float>(z) / static_cast<float>(image.GetHeight())) - 0.5f);
			vert.m_U = (static_cast<float>(x) / static_cast<float>(image.GetWidth() - 1));
			vert.m_V = (static_cast<float>(z) / static_cast<float>(image.GetHeight() - 1));

			return vert;
		};

		int index = 0;

		auto addPolygon = [&](TVertex& _rVertA, TVertex& _rVertB, TVertex& _rVertC) {
			CVector3 vec3LineAB = _rVertB.m_Position - _rVertA.m_Position;
			CVector3 vec3LineAC = _rVertC.m_Position - _rVertA.m_Position;

			// Get the crossproduct of the lines, to define the normal of the polygon.
			CVector3 vec3Normal = CrossProduct(vec3LineAB, vec3LineAC);
			vec3Normal = Normalise(vec3Normal);

			// Set the normals of the vertices.
			_rVertA.m_Normal = vec3Normal;
			_rVertB.m_Normal = vec3Normal;
			_rVertC.m_Normal = vec3Normal;

			vertices[index++] = _rVertA;
			vertices[index++] = _rVertB;
			vertices[index++] = _rVertC;
		};

		for (int iZ = 0; iZ < image.GetHeight() - 1; ++iZ) {
			// Even rows move left to right, odd rows move right to left.
			for (int iX = 0; iX < image.GetWidth() - 1; ++iX) {
				//				  |\
				// Add a polygon  | \
				//				  |__\
				//
				TVertex vert1 = createVert(iX, iZ);
				TVertex vert2 = createVert(iX + 1, iZ + 1);
				TVertex vert3 = createVert(iX + 1, iZ);
				addPolygon(vert1, vert2, vert3);
				//				  ___
				//				  \  |
				// Add a polygon   \ |
				//				    \|
				TVertex vert4 = createVert(iX, iZ);
				TVertex vert5 = createVert(iX, iZ + 1);
				TVertex vert6 = createVert(iX + 1, iZ + 1);
				addPolygon(vert4, vert5, vert6);
			}
		}

		m_pTerrainVertices = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
		m_pTerrainIndices = nullptr;
	}*/
}
