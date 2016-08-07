// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ProceduralTerrainGenerationLogic.h
// Description	: CProceduralTerrainGenerationLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __PROCEDURALTERRAINGENERATIONLOGIC_H__
#define __PROCEDURALTERRAINGENERATIONLOGIC_H__

// Library Includes

// Local Includes
#include <GameBase/SlimGameLogic.h>
#include <Graphics/SlimRenderPass.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimIndexGpuBuffer.h>
#include <Graphics/SlimCamera.h>
#include <Graphics/SlimShaderParams.h>
#include <Graphics/SlimLight.h>
#include "ProceduralTerrain.h"

class CProceduralTerrainGenerationLogic : public CGameLogic {
	// Member Functions
public:
	CProceduralTerrainGenerationLogic();
	~CProceduralTerrainGenerationLogic();

	virtual bool Initialise() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void HandleInput(const CInput& input, float deltaTime) override;

	void CreateTerrainVertices(const CVector3& scale, bool smooth = false);
	void CreateTerrainIndices();
protected:
private:
	// Member Variables
public:
protected:
private:
	CVertexDeclaration m_VertexDeclaration;

	CProceduralTerrain m_Terrain;
	CMatrix4x4 m_TerrainWorldTransform;
	std::vector<int> m_TerrainIndices;
	std::shared_ptr<AVertexGpuBuffer> m_pTerrainVertices;
	std::shared_ptr<AIndexGpuBuffer> m_pTerrainIndices;
	std::shared_ptr<CShaderParams> m_pTerrainVSParams;
	std::shared_ptr<CShaderParams> m_pTerrainPSParams;
	CRenderPass m_TerrainRenderPass;

	CLight m_Light;

	CCamera m_Camera;
	float m_CameraYaw;
	float m_CameraPitch;
	CPoint<> m_lastMousePosition;
};

#endif // __PROCEDURALTERRAINGENERATIONLOGIC_H__
