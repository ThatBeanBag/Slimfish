// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10Conversions.h
// Description	: Convenient D3D10 conversions declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10CONVERSIONS_H__
#define __SLIMD3D10CONVERSIONS_H__

// Library Includes

// Local Includes
#include "../SlimGpuBuffer.h"
#include "../SlimIndexGpuBuffer.h"
#include "../SlimVertexDeclaration.h"
#include "../SlimTexture.h"
#include "../SlimTextureLayer.h"
#include "../SlimShaderParams.h"
#include "../SlimTypes.h"
#include "SlimD3D10Forward.h"

namespace Slim {

namespace D3D10Conversions {
	D3DXVECTOR3 GetVec3(const CVector3& vector);
	CVector3 GetVec3(const D3DXVECTOR3& vector);

	D3DXMATRIX GetMatrix4x4(const CMatrix4x4& matrix);
	CMatrix4x4 GetMatrix4x4(const D3DXMATRIX& matrix);

	D3DXCOLOR GetColour(const CColourValue& colour);
	CColourValue GetColour(const D3DXCOLOR& colour);

	D3D10_USAGE GetUsage(EGpuBufferUsage usage);
	D3D10_USAGE GetUsage(ETextureUsage usage);
	UINT GetCPUAccessFlags(ETextureUsage usage);
	D3D10_MAP GetLockType(EGpuBufferLockType lockType);

	DXGI_FORMAT GetPixelFormat(ETexturePixelFormat format);
	ETexturePixelFormat GetPixelFormat(DXGI_FORMAT format);

	DXGI_FORMAT GetFormat(CInputElement::EFormat inputElementFormat);
	DXGI_FORMAT GetFormat(AIndexGpuBuffer::EIndexType indexType);

	D3D10_PRIMITIVE_TOPOLOGY GetPrimitiveType(EPrimitiveType primitiveType);
	D3D10_TEXTURE_ADDRESS_MODE GetAddressMode(ETextureAddressMode addressMode);
	D3D10_FILTER GetFilter(ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter);

	EShaderConstantType GetShaderConstantType(D3D11_SHADER_TYPE_DESC variableTypeDesc);

	D3D10_BLEND_OP GetBlendOperation(EBlendOperation blendOperation);
	D3D10_BLEND GetBlendFactor(EBlendFactor blendFactor);

	D3D10_CULL_MODE GetCullingMode(ECullingMode cullingMode);
	D3D10_FILL_MODE GetFillMode(EFillMode fillMode);

	D3D10_COMPARISON_FUNC GetComparisonFunction(EComparisonFunction function);
	D3D10_STENCIL_OP GetStencilOperation(EStencilOperation operation);
}

}

#endif // __SLIMD3D10CONVERSIONS_H__