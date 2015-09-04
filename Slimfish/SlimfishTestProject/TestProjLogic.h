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
protected:
private:
	// Member Variables
public:
protected:
private:
	shared_ptr<AVertexGpuBuffer> m_pVertexBuffer;
	shared_ptr<AIndexGpuBuffer> m_pIndexBuffer;

	shared_ptr<AShaderProgram> m_pVertexShader;
	shared_ptr<AShaderProgram> m_pPixelShader;
	shared_ptr<CShaderParams> m_pVertexParamsPerObject;
	shared_ptr<CShaderParams> m_pVertexParamsPerFrame;
	shared_ptr<CShaderParams> m_pPixelParamsPerFrame;

	CVertexDeclaration m_VertexDeclaration;

	CMatrix4x4 m_ViewMatrix;
	CMatrix4x4 m_ProjectionMatrix;
	CMatrix4x4 m_WorldMatrix;
	shared_ptr<ATexture> m_pTexture;
	std::vector<shared_ptr<ATexture>> m_pTextures;
	CTextureLayer m_specularLayer;
	size_t m_Frame;

	float m_TexAnim;
};

#endif // __TESTPROJLOGIC_H__