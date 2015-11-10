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

D3DXCOLOR D3D10Conversions::GetColour(const CColourValue& colour)
{
	return D3DXCOLOR(colour.m_r, colour.m_g, colour.m_b, colour.m_a);
}


CColourValue D3D10Conversions::GetColour(const D3DXCOLOR& colour)
{
	return CColourValue(colour.a, colour.r, colour.g, colour.b);
}

D3D10_USAGE D3D10Conversions::GetUsage(EGpuBufferUsage usage)
{
	if (usage == EGpuBufferUsage::DYNAMIC || usage == EGpuBufferUsage::WRITE_ONLY) {
		return D3D10_USAGE_DYNAMIC;
	}
	else {
		return D3D10_USAGE_DEFAULT;
	}
}

UINT D3D10Conversions::GetCPUAccessFlags(ETextureUsage usage)
{
	UINT d3dCpuAccess = 0;
	//Access

	if (usage == ETextureUsage::DYNAMIC) {
		d3dCpuAccess |= D3D10_CPU_ACCESS_WRITE;
	}

	if (usage == ETextureUsage::WRITE_ONLY) {
		d3dCpuAccess |= D3D10_CPU_ACCESS_WRITE;
	}
		
	if (usage == ETextureUsage::READ_ONLY) {
		d3dCpuAccess |= D3D10_CPU_ACCESS_READ;
	}

	return d3dCpuAccess;
}

D3D10_USAGE D3D10Conversions::GetUsage(ETextureUsage usage)
{
	if (usage == ETextureUsage::DYNAMIC ||
		usage == ETextureUsage::WRITE_ONLY ||
		usage == ETextureUsage::DISCARDABLE) {
		return D3D10_USAGE_DYNAMIC;
	}
	else if (usage == ETextureUsage::READ_ONLY) {
		return D3D10_USAGE_STAGING;
	}
	else {
		return D3D10_USAGE_DEFAULT;
	}
}

D3D10_MAP D3D10Conversions::GetLockType(EGpuBufferLockType lockType)
{
	switch (lockType) {
		case EGpuBufferLockType::NORMAL: {
			return D3D10_MAP_READ_WRITE;
		}
		case EGpuBufferLockType::DISCARD: {
			return D3D10_MAP_WRITE_DISCARD;
		}
		case EGpuBufferLockType::READ_ONLY: {
			return D3D10_MAP_READ;
		}
		case EGpuBufferLockType::NO_OVERWRITE: {
			return D3D10_MAP_WRITE_NO_OVERWRITE;
		}
		default: {
			break;
		}
	}

	return D3D10_MAP_READ;
}

DXGI_FORMAT D3D10Conversions::GetPixelFormat(ETexturePixelFormat format)
{
	switch (format) {
	case ETexturePixelFormat::UNKNOWN:			return DXGI_FORMAT_UNKNOWN;
	case ETexturePixelFormat::FLOAT_32_RGBA:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case ETexturePixelFormat::TYPELESS_32_RGBA: return DXGI_FORMAT_R32G32B32A32_TYPELESS;
	case ETexturePixelFormat::UINT_32_RGBA:		return DXGI_FORMAT_R32G32B32A32_UINT;
	case ETexturePixelFormat::INT_32_RGBA:		return DXGI_FORMAT_R32G32B32A32_SINT;
	case ETexturePixelFormat::FLOAT_32_RGB:		return DXGI_FORMAT_R32G32B32_FLOAT;
	case ETexturePixelFormat::TYPELESS_32_RGB:	return DXGI_FORMAT_R32G32B32_TYPELESS;
	case ETexturePixelFormat::UINT_32_RGB:		return DXGI_FORMAT_R32G32B32_UINT;
	case ETexturePixelFormat::INT_32_RGB:		return DXGI_FORMAT_R32G32B32_SINT;
	case ETexturePixelFormat::FLOAT_32_RG:		return DXGI_FORMAT_R32G32_FLOAT;
	case ETexturePixelFormat::TYPELESS_32_RG:	return DXGI_FORMAT_R32G32_TYPELESS;
	case ETexturePixelFormat::UINT_32_RG:		return DXGI_FORMAT_R32G32_UINT;
	case ETexturePixelFormat::INT_32_RG:		return DXGI_FORMAT_R32G32_SINT;
	case ETexturePixelFormat::FLOAT_32_R:		return DXGI_FORMAT_R32_FLOAT;
	case ETexturePixelFormat::TYPELESS_32_R:	return DXGI_FORMAT_R32_TYPELESS;
	case ETexturePixelFormat::UINT_32_R:		return DXGI_FORMAT_R32_UINT;
	case ETexturePixelFormat::INT_32_R:			return DXGI_FORMAT_R32_SINT;
	case ETexturePixelFormat::FLOAT_16_RGBA:	return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case ETexturePixelFormat::TYPELESS_16_RGBA:	return DXGI_FORMAT_R16G16B16A16_TYPELESS;
	case ETexturePixelFormat::UINT_16_RGBA:		return DXGI_FORMAT_R16G16B16A16_UINT;
	case ETexturePixelFormat::INT_16_RGBA:		return DXGI_FORMAT_R16G16B16A16_SINT;
	case ETexturePixelFormat::UNORM_16_RGBA:	return DXGI_FORMAT_R16G16B16A16_UNORM;
	case ETexturePixelFormat::NORM_16_RGBA:		return DXGI_FORMAT_R16G16B16A16_SNORM;
	case ETexturePixelFormat::FLOAT_16_RG:		return DXGI_FORMAT_R16G16_FLOAT;
	case ETexturePixelFormat::TYPELESS_16_RG:	return DXGI_FORMAT_R16G16_TYPELESS;
	case ETexturePixelFormat::UINT_16_RG:		return DXGI_FORMAT_R16G16_UINT;
	case ETexturePixelFormat::INT_16_RG:		return DXGI_FORMAT_R16G16_SINT;
	case ETexturePixelFormat::UNORM_16_RG:		return DXGI_FORMAT_R16G16_UNORM;
	case ETexturePixelFormat::NORM_16_RG:		return DXGI_FORMAT_R16G16_SNORM;
	case ETexturePixelFormat::FLOAT_16_R:		return DXGI_FORMAT_R16_FLOAT;
	case ETexturePixelFormat::TYPELESS_16_R:	return DXGI_FORMAT_R16_TYPELESS;
	case ETexturePixelFormat::UINT_16_R:		return DXGI_FORMAT_R16_UINT;
	case ETexturePixelFormat::INT_16_R:			return DXGI_FORMAT_R16_SINT;
	case ETexturePixelFormat::UNORM_16_R:		return DXGI_FORMAT_R16_UNORM;
	case ETexturePixelFormat::NORM_16_R:		return DXGI_FORMAT_R16_SNORM;
	case ETexturePixelFormat::TYPELESS_8_RGBA:	return DXGI_FORMAT_R8G8B8A8_TYPELESS;
	case ETexturePixelFormat::UINT_8_RGBA:		return DXGI_FORMAT_R8G8B8A8_UINT;
	case ETexturePixelFormat::INT_8_RGBA:		return DXGI_FORMAT_R8G8B8A8_SINT;
	case ETexturePixelFormat::UNORM_8_RGBA:		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case ETexturePixelFormat::NORM_8_RGBA:		return DXGI_FORMAT_R8G8B8A8_SNORM;
	case ETexturePixelFormat::TYPELESS_8_RG:	return DXGI_FORMAT_R8G8_TYPELESS;
	case ETexturePixelFormat::UINT_8_RG:		return DXGI_FORMAT_R8G8_UINT;
	case ETexturePixelFormat::INT_8_RG:			return DXGI_FORMAT_R8G8_SINT;
	case ETexturePixelFormat::UNORM_8_RG:		return DXGI_FORMAT_R8G8_UNORM;
	case ETexturePixelFormat::NORM_8_RG:		return DXGI_FORMAT_R8G8_SNORM;
	case ETexturePixelFormat::TYPELESS_8_R:		return DXGI_FORMAT_R8_TYPELESS;
	case ETexturePixelFormat::UINT_8_R:			return DXGI_FORMAT_R8_UINT;
	case ETexturePixelFormat::INT_8_R:			return DXGI_FORMAT_R8_SINT;
	case ETexturePixelFormat::UNORM_8_R:		return DXGI_FORMAT_R8_UNORM;
	case ETexturePixelFormat::NORM_8_R:			return DXGI_FORMAT_R8_SNORM;
	default:									return DXGI_FORMAT_UNKNOWN;
	}
}

Slim::ETexturePixelFormat D3D10Conversions::GetPixelFormat(DXGI_FORMAT format)
{
	switch (format) {
	case DXGI_FORMAT_UNKNOWN:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:			return ETexturePixelFormat::TYPELESS_32_RGBA;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:			return ETexturePixelFormat::FLOAT_32_RGBA;
	case DXGI_FORMAT_R32G32B32A32_UINT:				return ETexturePixelFormat::UINT_32_RGBA;
	case DXGI_FORMAT_R32G32B32A32_SINT:				return ETexturePixelFormat::INT_32_RGBA;
	case DXGI_FORMAT_R32G32B32_TYPELESS:			return ETexturePixelFormat::TYPELESS_32_RGB;
	case DXGI_FORMAT_R32G32B32_FLOAT:				return ETexturePixelFormat::FLOAT_32_RGB;
	case DXGI_FORMAT_R32G32B32_UINT:				return ETexturePixelFormat::UINT_32_RGB;
	case DXGI_FORMAT_R32G32B32_SINT:				return ETexturePixelFormat::INT_32_RGB;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:			return ETexturePixelFormat::TYPELESS_16_RGBA;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:			return ETexturePixelFormat::FLOAT_16_RGBA;
	case DXGI_FORMAT_R16G16B16A16_UNORM:			return ETexturePixelFormat::UNORM_16_RGBA;
	case DXGI_FORMAT_R16G16B16A16_UINT:				return ETexturePixelFormat::UINT_16_RGBA;
	case DXGI_FORMAT_R16G16B16A16_SNORM:			return ETexturePixelFormat::NORM_16_RGBA;
	case DXGI_FORMAT_R16G16B16A16_SINT:				return ETexturePixelFormat::INT_16_RGBA;
	case DXGI_FORMAT_R32G32_TYPELESS:				return ETexturePixelFormat::TYPELESS_32_RGBA;
	case DXGI_FORMAT_R32G32_FLOAT:					return ETexturePixelFormat::FLOAT_32_RGBA;
	case DXGI_FORMAT_R32G32_UINT:					return ETexturePixelFormat::UINT_32_RGBA;
	case DXGI_FORMAT_R32G32_SINT:					return ETexturePixelFormat::INT_32_RGBA;
	case DXGI_FORMAT_R32G8X24_TYPELESS:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:			return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:		return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:		return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:			return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R10G10B10A2_UNORM:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R10G10B10A2_UINT:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R11G11B10_FLOAT:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:				return ETexturePixelFormat::TYPELESS_8_RGBA;
	case DXGI_FORMAT_R8G8B8A8_UNORM:				return ETexturePixelFormat::UNORM_8_RGBA;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:			return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R8G8B8A8_UINT:					return ETexturePixelFormat::UINT_8_RGBA;
	case DXGI_FORMAT_R8G8B8A8_SNORM:				return ETexturePixelFormat::NORM_8_RGBA;
	case DXGI_FORMAT_R8G8B8A8_SINT:					return ETexturePixelFormat::INT_8_R;
	case DXGI_FORMAT_R16G16_TYPELESS:				return ETexturePixelFormat::TYPELESS_16_RG;
	case DXGI_FORMAT_R16G16_FLOAT:					return ETexturePixelFormat::FLOAT_16_RG;
	case DXGI_FORMAT_R16G16_UNORM:					return ETexturePixelFormat::UNORM_16_RG;
	case DXGI_FORMAT_R16G16_UINT:					return ETexturePixelFormat::UINT_16_RG;
	case DXGI_FORMAT_R16G16_SNORM:					return ETexturePixelFormat::NORM_16_RG;
	case DXGI_FORMAT_R16G16_SINT:					return ETexturePixelFormat::INT_16_RG;
	case DXGI_FORMAT_R32_TYPELESS:					return ETexturePixelFormat::TYPELESS_32_R;
	case DXGI_FORMAT_D32_FLOAT:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R32_FLOAT:						return ETexturePixelFormat::FLOAT_32_R;
	case DXGI_FORMAT_R32_UINT:						return ETexturePixelFormat::UINT_32_R;
	case DXGI_FORMAT_R32_SINT:						return ETexturePixelFormat::INT_32_R;
	case DXGI_FORMAT_R24G8_TYPELESS:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:			return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:			return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R8G8_TYPELESS:					return ETexturePixelFormat::TYPELESS_8_RG;
	case DXGI_FORMAT_R8G8_UNORM:					return ETexturePixelFormat::UNORM_8_RG;
	case DXGI_FORMAT_R8G8_UINT:						return ETexturePixelFormat::UINT_8_RG;
	case DXGI_FORMAT_R8G8_SNORM:					return ETexturePixelFormat::NORM_8_RG;
	case DXGI_FORMAT_R8G8_SINT:						return ETexturePixelFormat::INT_8_RG;
	case DXGI_FORMAT_R16_TYPELESS:					return ETexturePixelFormat::TYPELESS_16_RGBA;
	case DXGI_FORMAT_R16_FLOAT:						return ETexturePixelFormat::FLOAT_16_RGBA;
	case DXGI_FORMAT_D16_UNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R16_UNORM:						return ETexturePixelFormat::UNORM_16_R;
	case DXGI_FORMAT_R16_UINT:						return ETexturePixelFormat::UINT_16_R;
	case DXGI_FORMAT_R16_SNORM:						return ETexturePixelFormat::NORM_16_R;
	case DXGI_FORMAT_R16_SINT:						return ETexturePixelFormat::INT_16_R;
	case DXGI_FORMAT_R8_TYPELESS:					return ETexturePixelFormat::TYPELESS_8_R;
	case DXGI_FORMAT_R8_UNORM:						return ETexturePixelFormat::UNORM_8_R;
	case DXGI_FORMAT_R8_UINT:						return ETexturePixelFormat::UINT_8_R;
	case DXGI_FORMAT_R8_SNORM:						return ETexturePixelFormat::NORM_8_R;
	case DXGI_FORMAT_R8_SINT:						return ETexturePixelFormat::INT_8_R;
	case DXGI_FORMAT_A8_UNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R1_UNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:			return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R8G8_B8G8_UNORM:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_G8R8_G8B8_UNORM:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC1_TYPELESS:					return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC1_UNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC1_UNORM_SRGB:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC2_TYPELESS:					return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC2_UNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC2_UNORM_SRGB:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC3_TYPELESS:					return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC3_UNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC3_UNORM_SRGB:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC4_TYPELESS:					return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC4_UNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC4_SNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC5_TYPELESS:					return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC5_UNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC5_SNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_B5G6R5_UNORM:					return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_B5G5R5A1_UNORM:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_B8G8R8A8_UNORM:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_B8G8R8X8_UNORM:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:	return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:			return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:			return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC6H_TYPELESS:					return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC6H_UF16:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC6H_SF16:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC7_TYPELESS:					return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC7_UNORM:						return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_BC7_UNORM_SRGB:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_AYUV:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_Y410:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_Y416:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_NV12:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_P010:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_P016:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_420_OPAQUE:					return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_YUY2:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_Y210:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_Y216:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_NV11:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_AI44:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_IA44:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_P8:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_A8P8:							return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_B4G4R4A4_UNORM:				return ETexturePixelFormat::UNKNOWN;
	case DXGI_FORMAT_FORCE_UINT:					return ETexturePixelFormat::UNKNOWN;
	default:										return ETexturePixelFormat::UNKNOWN;
	}

	return ETexturePixelFormat::UNKNOWN;
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
		case CInputElement::FORMAT_INT: {
			return DXGI_FORMAT_R32_SINT;
		}
		case CInputElement::FORMAT_INT2: {
			return DXGI_FORMAT_R32G32_SINT;
		}
		case CInputElement::FORMAT_INT3: {
			return DXGI_FORMAT_R32G32B32_SINT;
		}
		case CInputElement::FORMAT_INT4: {
			return DXGI_FORMAT_R32G32B32A32_SINT;
		}
		case CInputElement::FORMAT_UINT: {
			return DXGI_FORMAT_R32_UINT;
		}
		case CInputElement::FORMAT_UINT2: {
			return DXGI_FORMAT_R32G32_UINT;
		}
		case CInputElement::FORMAT_UINT3: {
			return DXGI_FORMAT_R32G32B32_UINT;
		}
		case CInputElement::FORMAT_UINT4: {
			return DXGI_FORMAT_R32G32B32A32_UINT;
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

D3D10_PRIMITIVE_TOPOLOGY D3D10Conversions::GetPrimitiveType(EPrimitiveType primitiveType)
{
	switch (primitiveType) {
		case EPrimitiveType::POINTLIST: {
			return D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
		}
		case EPrimitiveType::LINELIST: {
			return D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
		}
		case EPrimitiveType::LINESTRIP: {
			return D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
		};
		case EPrimitiveType::TRIANGLELIST: {
			return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
		case EPrimitiveType::TRIANGLESTRIP: {
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

D3D10_TEXTURE_ADDRESS_MODE D3D10Conversions::GetAddressMode(ETextureAddressMode addressMode)
{
	switch (addressMode) {
		case ETextureAddressMode::WRAP: {
			return D3D10_TEXTURE_ADDRESS_WRAP;
		}
		case ETextureAddressMode::MIRROR: {
			return D3D10_TEXTURE_ADDRESS_MIRROR;
		}
		case ETextureAddressMode::BORDER: {
			return D3D10_TEXTURE_ADDRESS_BORDER;
		}
		case ETextureAddressMode::CLAMP: {
			return D3D10_TEXTURE_ADDRESS_CLAMP;
		}
		default: {
			break;
		}
	}

	return D3D10_TEXTURE_ADDRESS_WRAP;
}

D3D10_FILTER D3D10Conversions::GetFilter(ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter)
{
	auto testFilters = [&](ETextureFilterType min, ETextureFilterType mag, ETextureFilterType mip) {
		return min == minFilter && mag == magFilter && mip == mipFilter;
	};

	if (testFilters(ETextureFilterType::POINT, ETextureFilterType::POINT, ETextureFilterType::POINT)) {
		return D3D10_FILTER_MIN_MAG_MIP_POINT;
	}
	else if (testFilters(ETextureFilterType::POINT, ETextureFilterType::POINT, ETextureFilterType::LINEAR)) {
		return D3D10_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	}
	else if (testFilters(ETextureFilterType::POINT, ETextureFilterType::LINEAR, ETextureFilterType::POINT)) {
		return D3D10_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	}
	else if (testFilters(ETextureFilterType::POINT, ETextureFilterType::LINEAR, ETextureFilterType::LINEAR)) {
		return D3D10_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	}
	else if (testFilters(ETextureFilterType::LINEAR, ETextureFilterType::POINT, ETextureFilterType::POINT)) {
		return D3D10_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	}
	else if (testFilters(ETextureFilterType::LINEAR, ETextureFilterType::POINT, ETextureFilterType::LINEAR)) {
		return D3D10_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	}
	else if (testFilters(ETextureFilterType::LINEAR, ETextureFilterType::LINEAR, ETextureFilterType::POINT)) {
		return D3D10_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	}
	else if (testFilters(ETextureFilterType::LINEAR, ETextureFilterType::LINEAR, ETextureFilterType::LINEAR)) {
		return D3D10_FILTER_MIN_MAG_MIP_LINEAR;
	}
	else if (testFilters(ETextureFilterType::LINEAR, ETextureFilterType::LINEAR, ETextureFilterType::LINEAR)) {
		return D3D10_FILTER_MIN_MAG_MIP_LINEAR;
	}
	else if (testFilters(ETextureFilterType::ANISOTROPIC, ETextureFilterType::ANISOTROPIC, ETextureFilterType::ANISOTROPIC)) {
		return D3D10_FILTER_ANISOTROPIC;
	}
		
	return D3D10_FILTER_MIN_MAG_MIP_LINEAR;
}

EShaderConstantType D3D10Conversions::GetShaderConstantType(D3D11_SHADER_TYPE_DESC variableTypeDesc)
{
	switch (variableTypeDesc.Type) {
		case D3D10_SVT_INT: {
			switch (variableTypeDesc.Columns) {
				case 1: {
					return EShaderConstantType::INT;
				}
				case 2: {
					return EShaderConstantType::INT2;
				}
				case 3: {
					return EShaderConstantType::INT3;
				}
				case 4: {
					return EShaderConstantType::INT4;
				}
			}

			break;
		}
		case D3D10_SVT_FLOAT: {
			if (variableTypeDesc.Rows == 1) {
				switch (variableTypeDesc.Columns) {
					case 1: {
						return EShaderConstantType::FLOAT;
					}
					case 2: {
						return EShaderConstantType::FLOAT2;
					}
					case 3: {
						return EShaderConstantType::FLOAT3;
					}
					case 4: {
						return EShaderConstantType::FLOAT4;
					}
				}
			}
			else if (variableTypeDesc.Rows == 4 && variableTypeDesc.Columns == 4) {
				return EShaderConstantType::MATRIX4X4;
			}
		}
		default: {
			break;
		}
	}
		
	SLIM_WARNING() << "Unsupported type of shader constant found in shader";
	return EShaderConstantType::INT;
}

D3D10_BLEND_OP D3D10Conversions::GetBlendOperation(EBlendOperation blendOperation)
{
	switch (blendOperation) {
		case EBlendOperation::ADD: {
			return D3D10_BLEND_OP_ADD;
		}
		case EBlendOperation::SUBTRACT: {
			return D3D10_BLEND_OP_SUBTRACT;
		}
		case EBlendOperation::REVERSE_SUBTRACT: {
			return D3D10_BLEND_OP_REV_SUBTRACT;
		}
		case EBlendOperation::MIN: {
			return D3D10_BLEND_OP_MIN;
		}
		case EBlendOperation::MAX: {
			return D3D10_BLEND_OP_MAX;
		}
		default: {
			break;
		}
	}

	return D3D10_BLEND_OP_ADD;
}

D3D10_BLEND D3D10Conversions::GetBlendFactor(EBlendFactor blendFactor)
{
	switch (blendFactor) {
		case EBlendFactor::ZERO: {
			return D3D10_BLEND_ZERO;
		}
		case EBlendFactor::ONE: {
			return D3D10_BLEND_ONE;
		}
		case EBlendFactor::SOURCE_COLOUR: {
			return D3D10_BLEND_SRC_COLOR;
		}
		case EBlendFactor::INVERSE_SOURCE_COLOUR: {
			return D3D10_BLEND_INV_SRC_COLOR;
		}
		case EBlendFactor::SOURCE_ALPHA: {
			return D3D10_BLEND_SRC_ALPHA;
		}
		case EBlendFactor::INVERSE_SOURCE_ALPHA: {
			return D3D10_BLEND_INV_SRC_ALPHA;
		}
		case EBlendFactor::DEST_COLOUR: {
			return D3D10_BLEND_DEST_COLOR;
		}
		case EBlendFactor::INVERSE_DEST_COLOUR: {
			return D3D10_BLEND_INV_DEST_COLOR;
		}
		case EBlendFactor::DEST_ALPHA: {
			return D3D10_BLEND_DEST_ALPHA;
		}
		case EBlendFactor::INVERSE_DEST_ALPHA: {
			return D3D10_BLEND_INV_DEST_ALPHA;
		}
		default: {
			break;
		}
	}

	return D3D10_BLEND_ZERO;
}

D3D10_CULL_MODE D3D10Conversions::GetCullingMode(ECullingMode cullingMode)
{
	switch (cullingMode) {
		case ECullingMode::NONE: {
			return D3D10_CULL_NONE;
		}
		case ECullingMode::CLOCKWISE: {
			return D3D10_CULL_FRONT;
		}
		case ECullingMode::COUNTER_CLOCKWISE: {
			return D3D10_CULL_BACK;
		}
		default: {
			break;
		}
	}

	return D3D10_CULL_NONE;
}

D3D10_FILL_MODE D3D10Conversions::GetFillMode(EFillMode fillMode)
{
	switch (fillMode){
		case Slim::EFillMode::SOLID: {
			return D3D10_FILL_SOLID;
		}
		case Slim::EFillMode::WIREFRAME: {
			return D3D10_FILL_WIREFRAME;
		}
		default: {
			break;
		}
	}

	return D3D10_FILL_SOLID;
}

D3D10_COMPARISON_FUNC D3D10Conversions::GetComparisonFunction(EComparisonFunction function)
{
	switch (function) {
		case EComparisonFunction::NEVER: {
			return D3D10_COMPARISON_NEVER;
		}
		case EComparisonFunction::LESS: {
			return D3D10_COMPARISON_LESS;
		}
		case EComparisonFunction::EQUAL: {
			return D3D10_COMPARISON_EQUAL;
		}
		case EComparisonFunction::LESS_EQUAL: {
			return D3D10_COMPARISON_LESS_EQUAL;
		}
		case EComparisonFunction::GREATER: {
			return D3D10_COMPARISON_GREATER;
		}
		case EComparisonFunction::NOT_EQUAL: {
			return D3D10_COMPARISON_NOT_EQUAL;
		}
		case EComparisonFunction::GREATER_EQUAL: {
			return D3D10_COMPARISON_GREATER_EQUAL;
		}
		case EComparisonFunction::ALWAYS: {
			return D3D10_COMPARISON_ALWAYS;
		}
		default: {
			break;
		}
	}

	return D3D10_COMPARISON_ALWAYS;
}

D3D10_STENCIL_OP D3D10Conversions::GetStencilOperation(EStencilOperation operation)
{
	switch (operation) {
		case EStencilOperation::KEEP: {
			return D3D10_STENCIL_OP_KEEP;
		}
		case EStencilOperation::ZERO: {
			return D3D10_STENCIL_OP_ZERO;
		}
		case EStencilOperation::REPLACE: {
			return D3D10_STENCIL_OP_REPLACE;
		}
		case EStencilOperation::INVERT: {
			return D3D10_STENCIL_OP_INVERT;
		}
		case EStencilOperation::INCREMENT: {
			return D3D10_STENCIL_OP_INCR;
		}
		case EStencilOperation::DECREMENT: {
			return D3D10_STENCIL_OP_DECR;
		}
		case EStencilOperation::INCREMENT_CLAMP: {
			return D3D10_STENCIL_OP_INCR_SAT;
		}
		case EStencilOperation::DECREMENT_CLAMP: {
			return D3D10_STENCIL_OP_DECR_SAT;
		}
		default: {
			break;
		}
	}

	return D3D10_STENCIL_OP_KEEP;
}

}

