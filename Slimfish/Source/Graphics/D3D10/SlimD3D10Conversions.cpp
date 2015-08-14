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

	D3D10_USAGE D3D10Conversions::Get(AGpuBuffer::EUsage usage)
	{
		if (usage & AGpuBuffer::USAGE_DYNAMIC) {
			return D3D10_USAGE_DYNAMIC;
		}
		else {
			return D3D10_USAGE_DEFAULT;
		}
	}

	D3D10_MAP D3D10Conversions::Get(AGpuBuffer::ELockType lockType)
	{
		switch (lockType) {
			case LOCK_NORMAL: {
				return D3D10_MAP_READ_WRITE;
			}
			case LOCK_DISCARD: {
				return D3D10_MAP_WRITE_DISCARD;
			}
			case LOCK_READ_ONLY: {
				return D3D10_MAP_READ;
			}
			case LOCK_NO_OVERWRITE: {
				return D3D10_MAP_WRITE_NO_OVERWRITE;
			}
			default: {
				break;
			}
		}

		// Only to make the compiler happy.
		return D3D10_MAP_READ;
	}

	DXGI_FORMAT D3D10Conversions::Get(CInputElement::EFormat inputElementFormat)
	{
		switch (inputElementFormat) {
			case CInputElement::FORMAT_FLOAT: {
				return DXGI_FORMAT_R32_FLOAT;
			}
			case CInputElement::FORMAT_FLOAT2: {
				return DXGI_FORMAT_R32G32_FLOAT;
			}
			case CInputElement::FORMAT_FLOAT3: {
				return DXGI_FORMAT_R32G32B32_FLOAT;
			}
			case CInputElement::FORMAT_FLOAT4: {
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			case CInputElement::FORMAT_SHORT: {
				return DXGI_FORMAT_R32_SINT;
			}
			case CInputElement::FORMAT_SHORT2: {
				return DXGI_FORMAT_R32G32_SINT;
			}
			case CInputElement::FORMAT_SHORT3: {
				return DXGI_FORMAT_R32G32B32_SINT;
			}
			case CInputElement::FORMAT_SHORT4: {
				return DXGI_FORMAT_R32G32B32A32_SINT;
			}
			case CInputElement::FORMAT_COLOUR_RGBA: {
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			default: {
				break;
			}
		}
	}
}

