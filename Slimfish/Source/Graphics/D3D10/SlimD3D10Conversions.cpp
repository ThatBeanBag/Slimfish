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

	CVector3 D3D10Conversions::GetVec3(const D3DXVECTOR3& vector)
	{
		return CVector3(vector.x, vector.y, vector.z);
	}

	D3DXVECTOR3 D3D10Conversions::GetVec3(const CVector3& vector)
	{
		return D3DXVECTOR3(vector.GetX(), vector.GetY(), vector.GetZ());
	}

	CMatrix4x4 D3D10Conversions::GetMatrix4x4(const D3DXMATRIX& d3dMatrix)
	{
		CMatrix4x4 matrix;

		// Transpose the matrix, D3D uses row-major.
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				matrix[i][j] = d3dMatrix.m[j][i];
			}
		}

		return matrix;
	}

	D3DXMATRIX D3D10Conversions::GetMatrix4x4(const CMatrix4x4& matrix)
	{
		D3DXMATRIX d3dMatrix;

		// Transpose the matrix, D3D uses row-major.
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				d3dMatrix.m[i][j] = matrix[j][i];
			}
		}

		return d3dMatrix;
	}

	D3DXCOLOR D3D10Conversions::GetColour(const TColourValue& colour)
	{
		return D3DXCOLOR(colour.m_r, colour.m_g, colour.m_b, colour.m_a);
	}


	TColourValue D3D10Conversions::GetColour(const D3DXCOLOR& colour)
	{
		return CreateColourValueARGB(colour.a, colour.r, colour.g, colour.b);
	}

	D3D10_USAGE D3D10Conversions::GetUsage(AGpuBuffer::EUsage usage)
	{
		if (usage & AGpuBuffer::USAGE_DYNAMIC) {
			return D3D10_USAGE_DYNAMIC;
		}
		else {
			return D3D10_USAGE_DEFAULT;
		}
	}

	UINT D3D10Conversions::GetCPUAccessFlags(ATexture::EUsage usage)
	{
		if (usage & AGpuBuffer::USAGE_DYNAMIC) {
			return D3D10_CPU_ACCESS_WRITE;
		}
		else {
			return 0;
		}
	}

	D3D10_USAGE D3D10Conversions::GetUsage(ATexture::EUsage usage)
	{
		if (usage & AGpuBuffer::USAGE_DYNAMIC) {
			return D3D10_USAGE_DYNAMIC;
		}
		else {
			return D3D10_USAGE_DEFAULT;
		}
	}

	D3D10_MAP D3D10Conversions::GetLockType(AGpuBuffer::ELockType lockType)
	{
		switch (lockType) {
			case AGpuBuffer::LOCK_NORMAL: {
				return D3D10_MAP_READ_WRITE;
			}
			case AGpuBuffer::LOCK_DISCARD: {
				return D3D10_MAP_WRITE_DISCARD;
			}
			case AGpuBuffer::LOCK_READ_ONLY: {
				return D3D10_MAP_READ;
			}
			case AGpuBuffer::LOCK_NO_OVERWRITE: {
				return D3D10_MAP_WRITE_NO_OVERWRITE;
			}
			default: {
				break;
			}
		}

		return D3D10_MAP_READ;
	}

	DXGI_FORMAT D3D10Conversions::GetFormat(CInputElement::EFormat inputElementFormat)
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

		return DXGI_FORMAT_R32_FLOAT;
	}

	D3D10_PRIMITIVE_TOPOLOGY D3D10Conversions::GetPrimitiveType(CVertexDeclaration::EPrimitiveType primitiveType)
	{
		switch (primitiveType) {
			case CVertexDeclaration::PRIMITIVE_TYPE_POINTLIST: {
				return D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
			}
			case CVertexDeclaration::PRIMITIVE_TYPE_LINELIST: {
				return D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			}
			case CVertexDeclaration::PRIMITIVE_TYPE_LINESTRIP: {
				return D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
			};
			case CVertexDeclaration::PRIMITIVE_TYPE_TRIANGLELIST: {
				return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			}
			case CVertexDeclaration::PRIMITIVE_TYPE_TRIANGLESTRIP: {
				return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			}
			default: {
				break;
			}
		}

		return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	DXGI_FORMAT D3D10Conversions::GetFormat(AIndexGpuBuffer::EIndexType indexType)
	{
		switch (indexType) {
			case AIndexGpuBuffer::INDEX_TYPE_16: {
				return DXGI_FORMAT_R16_UINT;
			}
			case AIndexGpuBuffer::INDEX_TYPE_32: {
				return DXGI_FORMAT_R32_UINT;
			}
			default: {
				break;
			}
		}

		return DXGI_FORMAT_R16_UINT;
	}

}

