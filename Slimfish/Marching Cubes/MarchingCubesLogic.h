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
#include <array>
#include <unordered_map>

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

class CMarchingCubesLogic : public CGameLogic {
	// Member Functions
public:
	CMarchingCubesLogic();
	~CMarchingCubesLogic();

	virtual bool Initialise() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void HandleInput(const CInput& input, float deltaTime) override;
protected:
private:
	// Member Variables
public:
protected:
private:
	CChunkManager m_ChunkManager;

	CCamera m_Camera;
	float m_CameraYaw;
	float m_CameraPitch;
	CPoint m_LastMousePosition;

	CLight m_Light;
	CVector3 m_TestChunkPosition;

	// Declarations

	// Buffers
	// For building densities.
	
	// Shader params.

	// Render targets.

	// Textures

	// Render passes.

};

#endif // __MARCHINGCUBESLOGIC_H__