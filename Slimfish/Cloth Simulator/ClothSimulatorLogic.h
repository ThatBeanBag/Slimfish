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

class CClothSimulatorLogic : public CGameLogic {
	// Member Functions
public:
	CClothSimulatorLogic();
	~CClothSimulatorLogic();

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
	CVertexDeclaration m_VertexDeclaration;
	std::shared_ptr<AVertexGpuBuffer> m_pClothVertexBuffer;
	CRenderPass m_ClothRenderPass;

	CCamera m_Camera;
	float m_CameraYaw;
	float m_CameraPitch;
	CPoint m_lastMousePosition;
	shared_ptr<CShaderParams> m_pVSParams;
};

#endif // __CLOTHSIMULATORLOGIC_H__