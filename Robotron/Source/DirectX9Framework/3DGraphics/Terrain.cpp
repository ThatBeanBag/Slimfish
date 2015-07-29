// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Terrain.h
// Description	: CTerrain implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Terrain.h"

// Local Includes
#include "../GameStd.h"	// REMOVE: this.

CTerrain::CTerrain()
{

}

CTerrain::CTerrain(float _fWidth, float _fHeight, float _fDepth)
{
	m_matTransform.SetScale(_fWidth, _fHeight, _fDepth);
}

CTerrain::~CTerrain()
{

}

void CTerrain::VRender()
{
	IRenderer* pRenderer = g_pApp->GetRenderer();

	pRenderer->VSetWorldTransform(&m_matTransform);
	pRenderer->VSetTexture(m_strTextureFileName);
	pRenderer->VSetMaterial(m_material);

	// Render the mesh.
}

bool CTerrain::Initialize(const std::string& _krstrHeightMap, const std::string& _krstrTexture)
{
	m_strTextureFileName = _krstrTexture;

	IRenderer* pRenderer = g_pApp->GetRenderer();

	m_iStaticBuffer = pRenderer->VCreateTerrain(_krstrHeightMap.c_str());
	if (m_iStaticBuffer == g_kINVALID_BUFFER_ID) {
		return false;
	}

	if (!pRenderer->VLoadAndCreateTexture(m_strTextureFileName)) {
		return false;
	}

	return true;
}