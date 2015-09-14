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
#include <GameBase/SlimGameLogic.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimIndexGpuBuffer.h>
#include <Graphics/SlimShaderProgram.h>
#include <Graphics/SlimTexture.h>
#include <Graphics/SlimTextureLayer.h>

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

	void LoadTerrain();


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
	shared_ptr<AShaderProgram> m_TerrainVertexShader;
	shared_ptr<AShaderProgram> m_TerrainPixelShader;
	CMatrix4x4 m_TerrainWorldTransform;

	// Water.
	shared_ptr<AVertexGpuBuffer> m_pWaterVertices;
	shared_ptr<AIndexGpuBuffer> m_pWaterIndices;
	CTextureLayer m_WaterTexture;
	CTextureLayer m_WaterSpecular;
	CTextureLayer m_WaterNormalMap;
	CVertexDeclaration m_WaterVertexDeclaration;
	shared_ptr<AShaderProgram> m_WaterVertexShader;
	shared_ptr<AShaderProgram> m_WaterPixelShader;
	CMatrix4x4 m_WaterWorldTransform;

	CMatrix4x4 m_ViewMatrix;
	CMatrix4x4 m_ProjectionMatrix;

	float m_ElapsedTime;

	float m_CameraPitch;
	float m_CameraYaw;
};

#endif // __TESTPROJLOGIC_H__