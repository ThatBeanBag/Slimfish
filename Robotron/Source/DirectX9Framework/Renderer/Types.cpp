// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Types.cpp
// Description	: Types implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include 
#include "../GameStd.h"
#include "Types.h"

// Local Includes

const TStaticBufferID g_kINVALID_BUFFER_ID = -1;
const std::string g_kstrINVALID_TEXTURE = "";

const DWORD TUnlitColouredVertex::s_kFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD TLitColouredVertex::s_kFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
const DWORD TLitTexturedVertex::s_kFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

TUnlitColouredVertex CreateVertex(float _fX, float _fY, float _fZ, TColour _diffuse)
{
	TUnlitColouredVertex vert;
	vert.m_vec3Position.x = _fX;
	vert.m_vec3Position.y = _fY;
	vert.m_vec3Position.z = _fZ;
	vert.dwDiffuse = ColourToDWORD(_diffuse);

	return vert;
}

TLitColouredVertex CreateVertex(float _fX, float _fY, float _fZ,
								float _fnX, float _fnY, float _fnZ,
								TColour _diffuse)
{
	TLitColouredVertex vert;
	vert.m_vec3Position = CVec3(_fX, _fY, _fZ);
	vert.m_vec3Normal = CVec3(_fnX, _fnY, _fnZ);
	vert.m_dwDiffuse = ColourToDWORD(_diffuse);

	return vert;
}
TLitTexturedVertex CreateVertex(float _fX, float _fY, float _fZ,
								float _fnX, float _fnY, float _fnZ,
								float _fU, float _fV)
{
	TLitTexturedVertex vert;
	vert.m_vec3Position = CVec3(_fX, _fY, _fZ);
	vert.m_vec3Normal = CVec3(_fnX, _fnY, _fnZ);
	vert.m_fU = _fU;
	vert.m_fV = _fV;

	return vert;
}