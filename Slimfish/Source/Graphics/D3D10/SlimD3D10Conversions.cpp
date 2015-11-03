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

