// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GeometryShaderLogic.h
// Description	: CGeometryShaderLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __GEOMETRYSHADERLOGIC_H__
#define __GEOMETRYSHADERLOGIC_H__

// Library Includes

// Local Includes
#include <GameBase/SlimGameLogic.h>
#include <Graphics/SlimRenderPass.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimCamera.h>
#include <Graphics/SlimShaderParams.h>

class CGeometryShaderLogic : public CGameLogic {
	// Member Functions
public:
	CGeometryShaderLogic();
	~CGeometryShaderLogic();

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
	std::shared_ptr<AVertexGpuBuffer> m_pStarVertexBuffer;
	std::shared_ptr<CShaderParams> m_pGSParams;
	CRenderPass m_StarRenderPass;

	CCamera m_Camera;
	float m_CameraYaw;
	float m_CameraPitch;
	CPoint m_lastMousePosition;
};

#endif // __GEOMETRYSHADERLOGIC_H__