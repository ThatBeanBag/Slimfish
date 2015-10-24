// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ClothSimulatorLogic.h
// Description	: CClothSimulatorLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __CLOTHSIMULATORLOGIC_H__
#define __CLOTHSIMULATORLOGIC_H__

// Library Includes

// Local Includes
#include <GameBase/SlimGameLogic.h>
#include <Graphics/SlimRenderPass.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimCamera.h>
#include <Graphics/SlimShaderParams.h>
#include <Graphics/SlimIndexGpuBuffer.h>
#include <Graphics/SlimRenderTexture.h>
#include <Graphics/SlimLight.h>
#include "PointMass.h"
#include "Link.h"

class CClothSimulatorLogic : public CGameLogic {
	// Member Functions
public:
	CClothSimulatorLogic();
	~CClothSimulatorLogic();

	virtual bool Initialise() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void HandleInput(const CInput& input, float deltaTime) override;
	void UpdateClothVertices();
	void CreateSphereVertices(float rings, float segments);

	void RenderToShadowMap();
protected:
private:
	void CreateCloth(int width, int height, float restingDistance, float stiffness, float breakingForce);

	// Member Variables
public:
	static const size_t s_SHADOW_MAP_WIDTH;
	static const size_t s_SHADOW_MAP_HEIGHT;
protected:
private:
	std::unique_ptr<ARenderTexture> m_pShadowMap;

	CRenderPass m_DepthRenderPass;

	CVertexDeclaration m_VertexDeclaration;
	std::shared_ptr<AVertexGpuBuffer> m_pClothVertexBuffer;
	std::shared_ptr<AIndexGpuBuffer> m_pClothIndexBuffer;
	CRenderPass m_ClothRenderPass;

	CCamera m_Camera;
	float m_CameraYaw;
	float m_CameraPitch;
	CPoint m_lastMousePosition;
	shared_ptr<CShaderParams> m_pVSParams;

	CLight m_Light;
	CCamera m_LightCamera;

	// Physics.
	std::vector<std::unique_ptr<CPointMass> > m_PointMasses;
	float m_LeftOverDeltaTime;
	unsigned int m_ClothWidth;
	unsigned int m_ClothHeight;
	shared_ptr<AVertexGpuBuffer> m_pGroundVertexBuffer;
	CPointMass* m_pGrabbedMass;
	float m_AccumulatedTime;

	shared_ptr<AIndexGpuBuffer> m_pSphereIndices;
	shared_ptr<AVertexGpuBuffer> m_pSphereVertices;
	shared_ptr<CShaderParams> m_pPSParams;
};

#endif // __CLOTHSIMULATORLOGIC_H__