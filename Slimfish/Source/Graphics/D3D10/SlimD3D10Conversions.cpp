// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10Conversions.cpp
// Description	: CSlimD3D10Conversions implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D10Conversions.h"

// Local Includes

namespace Slim {

	CVec3 D3D10Conversions::Get(const D3DXVECTOR3& vector)
	{
		return CVec3(vector.x, vector.y, vector.z);
	}

	D3DXVECTOR3 D3D10Conversions::Get(const CVec3& vector)
	{
		return D3DXVECTOR3(vector.GetX(), vector.GetY(), vector.GetZ());
	}

	CMatrix4x4 D3D10Conversions::Get(const D3DXMATRIX& matrix)
	{

	}

	D3DXMATRIX D3D10Conversions::Get(const CMatrix4x4& matrix)
	{

	}

	D3DXCOLOR D3D10Conversions::Get(const TColourValue& colour)
	{
		return D3DXCOLOR(colour.m_r, colour.m_g, colour.m_b, colour.m_a);
	}


	TColourValue D3D10Conversions::Get(const D3DXCOLOR& colour)
	{
		return CreateColourValueARGB(colour.a, colour.r, colour.g, colour.b);
	}

}
