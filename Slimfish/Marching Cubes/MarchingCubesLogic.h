// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MarchingCubesLogic.h
// Description	: CMarchingCubesLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MARCHINGCUBESLOGIC_H__
#define __MARCHINGCUBESLOGIC_H__

// Library Includes

// Local Includes
#include <GameBase/SlimGameLogic.h>
#include <Graphics/SlimRenderPass.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimIndexGpuBuffer.h>
#include <Graphics/SlimCamera.h>
#include <Graphics/SlimShaderParams.h>
#include <Graphics/SlimRenderTexture.h>
#include <Graphics/SlimLight.h>
#include "ChunkManager.h"
#include "2DDrawer.h"

class CMarchingCubesLogic : public CGameLogic {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CMarchingCubesLogic();

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CMarchingCubesLogic();

	/** Intialise the marching cubes logic.
	 	@author Hayden Asplet
	 	@return True if initialisation was successful.
	*/
	virtual bool Initialise() override;

	/** Update the marching cubes logic.
	 	@author Hayden Asplet
	*/
	virtual void Update(float deltaTime) override;

	/** Render stuff to the screen.
	 	@author Hayden Asplet
	*/
	virtual void Render() override;

	/** Allow the logic to handle input.
	 	@author Hayden Asplet
	*/
	virtual void HandleInput(const CInput& input, float deltaTime) override;
protected:
private:
	void CreateSkyDome(size_t rings, size_t segments);

	// Member Variables
public:
protected:
private:
	CChunkManager m_ChunkManager;
	C2DDrawer m_2DRenderer;

	CCamera m_Camera;
	float m_CameraYaw;
	float m_CameraPitch;
	CPoint<> m_LastMousePosition;

	CCamera m_LightCamera;
	CLight m_Light;
	float m_LightPitch;
	float m_LightYaw;

	bool m_DisplayShadowMap;
	bool m_DisplayControls;

	// Declarations
	CVertexDeclaration m_SphereVertexDeclaration;

	// Buffers
	std::shared_ptr<AVertexGpuBuffer> m_pSphereVertices;
	std::shared_ptr<AIndexGpuBuffer> m_pSphereIndices;
	
	// Shader params.

	// Render targets.

	// Textures
	std::shared_ptr<ATexture> m_pControlsImage;

	// Render passes.
	CRenderPass m_DrawSkyDomePass;
	shared_ptr<CShaderParams> m_pSkyBoxParams;
	CVector3 m_SkyDomeScale;
};

#endif // __MARCHINGCUBESLOGIC_H__