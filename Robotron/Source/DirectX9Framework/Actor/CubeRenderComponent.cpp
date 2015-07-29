// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CubeRenderComponent.cpp
// Description	: CCubeRenderComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "CubeRenderComponent.h"

// Local Includes

const std::string CCubeRenderComponent::s_kstrNAME = "CubeRenderComponent";

CCubeRenderComponent::CCubeRenderComponent()
{

}

CCubeRenderComponent::~CCubeRenderComponent()
{

}

bool CCubeRenderComponent::VInitialiseDelegate(TiXmlElement* _pXmlData)
{
	return true;
}

shared_ptr<CSceneNode> CCubeRenderComponent::VCreateSceneNode()
{
	TLitColouredVertex pCubeVerts[36];

	CVec3 normals[6];
	TLitColouredVertex verts[8];

	int iCount = 0;

	auto addPolygon = [&](int i1, int i2, int i3, int iNormal) {
		verts[i1].m_vec3Normal = normals[iNormal];
		verts[i2].m_vec3Normal = normals[iNormal];
		verts[i3].m_vec3Normal = normals[iNormal];

		pCubeVerts[iCount++] = verts[i1];
		pCubeVerts[iCount++] = verts[i2];
		pCubeVerts[iCount++] = verts[i3];
	};

	normals[0] = CVec3(0, 0, -1);	// Front.
	normals[1] = CVec3(-1, 0, 0);	// Left.
	normals[2] = CVec3(0, 1, 0);	// Top.
	normals[3] = CVec3(0, -1, 0);	// Bottom.
	normals[4] = CVec3(1, 0, 0);	// Right.
	normals[5] = CVec3(0, 0, 1);	// Back.

	verts[0].m_vec3Position = CVec3(-0.5f, -0.5f, -0.5f);	// Bottom Left.
	verts[1].m_vec3Position = CVec3(-0.5f, 0.5f, -0.5f);	// Top Left.
	verts[2].m_vec3Position = CVec3(0.5f, 0.5f, -0.5f);		// Top Right.
	verts[3].m_vec3Position = CVec3(0.5f, -0.5f, -0.5f);	// Bottom Right.
	verts[4].m_vec3Position = CVec3(-0.5f, -0.5f, 0.5f);	// Bottom Left.
	verts[5].m_vec3Position = CVec3(-0.5f, 0.5f, 0.5f);		// Top Left.
	verts[6].m_vec3Position = CVec3(0.5f, 0.5f, 0.5f);		// Top Right.
	verts[7].m_vec3Position = CVec3(0.5f, -0.5f, 0.5f);		// Bottom Right.

	verts[0].m_dwDiffuse = D3DCOLOR_ARGB(50, 0, 0, 0);		// BLACK.
	verts[1].m_dwDiffuse = D3DCOLOR_ARGB(50, 255, 0, 0);	// RED
	verts[2].m_dwDiffuse = D3DCOLOR_ARGB(50, 255, 0, 255);	// MAGENTA
	verts[3].m_dwDiffuse = D3DCOLOR_ARGB(50, 0, 0, 255);	// BLUE
	verts[4].m_dwDiffuse = D3DCOLOR_ARGB(50, 0, 255, 0);	// GREEN
	verts[5].m_dwDiffuse = D3DCOLOR_ARGB(50, 255, 255, 0);	// YELLOW
	verts[6].m_dwDiffuse = D3DCOLOR_ARGB(50, 255, 255, 255);// WHITE
	verts[7].m_dwDiffuse = D3DCOLOR_ARGB(50, 0, 255, 255);	// CYAN

	addPolygon(3, 1, 2, 0);
	addPolygon(0, 1, 3, 0);

	addPolygon(4, 1, 0, 1);
	addPolygon(5, 1, 4, 1);

	addPolygon(2, 5, 6, 2);
	addPolygon(1, 5, 2, 2);

	addPolygon(7, 4, 0, 3);
	addPolygon(3, 7, 0, 3);

	addPolygon(6, 3, 2, 4);
	addPolygon(3, 6, 7, 4);

	addPolygon(6, 5, 4, 5);
	addPolygon(4, 7, 6, 5);

	shared_ptr<CMeshNode> pCube(new CMeshNode(CActor::s_kINVALID_ACTOR_ID));

	pCube->SetStaticBuffer(g_pApp->GetRenderer()->VCreateStaticBuffer(TLitColouredVertex::s_kFVF,
																	  PT_TRIANGLELIST, IT_NONE,
																	  36, 0,
																	  sizeof(TLitColouredVertex),
																	  pCubeVerts, nullptr));
	return pCube;
}
