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
#include "2DDrawer.h"
#include "CollisionShape.h"
#include "Cloth.h"

enum class ECollisionObject {
	NONE,
	PYRAMID,
	CAPSULE,
	SPHERE
};

class CClothSimulatorLogic : public CGameLogic {
	// Member Functions
public:
	CClothSimulatorLogic();
	~CClothSimulatorLogic();

	virtual bool Initialise() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	void RenderUserInterface();
	virtual void HandleInput(const CInput& input, float deltaTime) override;
	void UpdateClothVertices();
	void UpdateClothIndices();

	void RenderToShadowMap();
protected:
private:
	void CreateCloth();
	//void CreatePointMasses(int width, int height, float hungFrom, float restingDistance, float stiffness, float breakingForce);
	//void CreatePointMasses(float width, float height, float hungFrom, float stiffness, float breakingForce);
	//void PinCloth();
	//CPointMass* GetPointMass(size_t x, size_t y);

	// Member Variables
public:
	static const size_t s_SHADOW_MAP_WIDTH;
	static const size_t s_SHADOW_MAP_HEIGHT;

	static const size_t s_MAX_CLOTH_SIZE;
	static const size_t s_MIN_CLOTH_SIZE;
	static const size_t s_MAX_HOOKS;
	static const size_t s_MIN_HOOKS;
	static const int s_SLIDER_MIN_X;
	static const int s_SLIDER_MAX_X;
protected:
private:
	// 2D Rendering.
	C2DDrawer m_2DDrawer;
	std::shared_ptr<ATexture> m_pSliderBarsImage;
	std::shared_ptr<ATexture> m_pSliderImage;
	CRect m_SliderRectClothWidth;
	CRect m_SliderRectClothHeight;
	CRect m_SliderRectNumHooks;

	CRect* m_pDraggingRect;

	std::unique_ptr<ARenderTexture> m_pShadowMap;
	CRenderPass m_DepthRenderPass;

	CVertexDeclaration m_VertexDeclaration;
	CRenderPass m_ClothRenderPass;
	CRenderPass m_GroundRenderPass;
	CRenderPass m_SphereRenderPass;
	shared_ptr<CShaderParams> m_pPSParams;
	shared_ptr<CShaderParams> m_pVSParams;

	// Meshes.
	std::shared_ptr<AVertexGpuBuffer> m_pClothVertexBuffer;
	std::shared_ptr<AIndexGpuBuffer> m_pClothIndexBuffer;
	std::shared_ptr<AVertexGpuBuffer> m_pGroundVertexBuffer;
	std::shared_ptr<AVertexGpuBuffer> m_pPyramidVertexBuffer;
	std::shared_ptr<AVertexGpuBuffer> m_pSphereVertexBuffer;
	std::shared_ptr<AIndexGpuBuffer> m_pSphereIndexBuffer;
	std::shared_ptr<AVertexGpuBuffer> m_pCapsuleVertexBuffer;
	std::shared_ptr<AIndexGpuBuffer> m_pCapsuleIndexBuffer;

	// Cameras and lighting.
	CCamera m_Camera;
	float m_CameraYaw;
	float m_CameraPitch;
	CPoint m_lastMousePosition;

	CLight m_Light;
	CCamera m_LightCamera;

	// Physics.
	CCloth m_Cloth;

	CPointMass* m_pGrabbedMass;
	float m_fTimeScale;

	CCollisionSphere m_Sphere;
	CCollisionPlane m_GroundPlane;
	CCollisionPyramid m_Pyrimad;
	CCollisionCapsule m_Capsule;
	ECollisionObject m_CurrentObject;
};

#endif // __CLOTHSIMULATORLOGIC_H__