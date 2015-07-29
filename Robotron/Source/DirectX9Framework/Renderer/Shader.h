// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Shader.h
// Description	: CShader declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

// Library Includes

// Local Includes
#include "../GameStd.h" // TODO: remove this.

class CShader {
	// Member Functions
public:
	CShader();
	~CShader();

	bool OnRestore(CScene* _pScene);
	bool SetupScene(CScene* _pScene);

protected:
private:
	// Member Variables
public:
protected:
private:
	IDirect3DVertexShader9* m_pVertexShader;
	IDirect3DPixelShader9* m_pPixelShader;
	ID3DXBuffer* m_pcbVSMatrix;
	ID3DXBuffer* m_pcbVSLight;
	ID3DXBuffer* m_pcbVSMaterial;
};

#endif	// __SHADER_H__