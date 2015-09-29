// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TestProjLogic.h
// Description	: CTestProjLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __TESTPROJLOGIC_H__
#define __TESTPROJLOGIC_H__

// Library Includes
#include <GameBase\SlimGameLogic.h>
#include <Graphics\SlimVertexGpuBuffer.h>
#include <Graphics\SlimIndexGpuBuffer.h>
#include <Graphics\SlimShaderProgram.h>
#include <Graphics\SlimTexture.h>
#include <Graphics\SlimTextureLayer.h>
#include <Graphics\SlimRenderPass.h>
#include <Graphics\SlimRenderTexture.h>
#include <Graphics\SlimCamera.h>

// Local Includes

class CTestProjLogic : public CGameLogic {
	// Member Functions
public:
	CTestProjLogic();
	~CTestProjLogic();

	virtual bool Initialise();
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void HandleInput(const CInput& input, float deltaTime) override;

	void RenderToShadowMap();
	void CreateRenderTextures();

	void BuildWater(size_t n, size_t m);
	void BuildSkySphere(int latLines, int longLines);
	void LoadTerrain(const CVector3& scale);
protected:
private:
	// Member Variables
public:
protected:
private:
	// Terrain.
	shared_ptr<AVertexGpuBuffer> m_pTerrainVertices;
	shared_ptr<AIndexGpuBuffer> m_pTerrainIndices;
	CTextureLayer m_TerrainTextureLayer;
	CVertexDeclaration m_TerrainVertexDeclaration;
	shared_ptr<CShaderParams> m_TerrainVSParams;
	shared_ptr<CShaderParams> m_TerrainPSParams;
	CMatrix4x4 m_TerrainWorldTransform;
	CRenderPass m_TerrainRenderPass;

	// Water.
	shared_ptr<AVertexGpuBuffer> m_pWaterVertices;
	shared_ptr<AIndexGpuBuffer> m_pWaterIndices;
	CVertexDeclaration m_WaterVertexDeclaration;
	shared_ptr<CShaderParams> m_WaterVSParams;
	shared_ptr<CShaderParams> m_WaterPSParams;
	CMatrix4x4 m_WaterTransform;
	CMatrix4x4 m_WaterWaveTransform1;
	CMatrix4x4 m_WaterWaveTransform2;
	CMatrix4x4 m_WaterWaveTransform3;
	CMatrix4x4 m_WaterWaveTransform4;
	CRenderPass m_WaterRenderPass;

	// SkyBox.
	CRenderPass m_SkyBoxRenderPass;
	CVertexDeclaration m_SkyBoxVertexDeclaration;
	shared_ptr<AVertexGpuBuffer> m_pSkyBoxVertices;
	shared_ptr<AIndexGpuBuffer> m_pSkyBoxIndices;
	shared_ptr<CShaderParams> m_pSkyBoxShaderParams;
	CMatrix4x4 m_SkyBoxWorldTransform;

	CMatrix4x4 m_ViewMatrix;
	CMatrix4x4 m_ReflectionViewMatrix;
	CMatrix4x4 m_ProjectionMatrix;

	std::unique_ptr<ARenderTexture> m_pRefractionRenderTarget;
	std::unique_ptr<ARenderTexture> m_pReflectionRenderTarget;
	CTextureLayer* m_pRefractionLayer;
	CTextureLayer* m_pReflectionLayer;

	// Shadow mapping stuffs.
	CRenderPass m_RenderDepth;
	std::shared_ptr<CShaderParams> m_pRenderDepthShaderParams;
	std::unique_ptr<ARenderTexture> m_pShadowMap;
	static const size_t s_SHADOW_MAP_WIDTH;
	static const size_t s_SHADOW_MAP_HEIGHT;
	CCamera m_lightCamera;

	// Camera
	std::unique_ptr<CCamera> m_pCamera;
	CVector3 m_EyePosition;

	float m_ElapsedTime;

	float m_CameraPitch;
	float m_CameraYaw;
	float m_CameraDistance;
	CPoint m_lastMousePosition;
	CPoint m_lastScreenSize;
};

#endif // __TESTPROJLOGIC_H__