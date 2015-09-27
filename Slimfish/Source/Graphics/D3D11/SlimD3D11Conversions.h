// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11Conversions.h
// Description	: Convenient D3D11 conversions declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D11CONVERSIONS_H__
#define __SLIMD3D11CONVERSIONS_H__

// Library Includes

// Local Includes
#include "../SlimGpuBuffer.h"
#include "../SlimIndexGpuBuffer.h"
#include "../SlimVertexDeclaration.h"
#include "../SlimTexture.h"
#include "../SlimTextureLayer.h"
#include "../SlimShaderParams.h"
#include "../SlimTypes.h"
#include "SlimD3D11Forward.h"

namespace Slim {

namespace D3D11Conversions {
	void GetColour(const CColourValue& colour, float* pColourOut);
	CColourValue GetColour(const float* pColour);

	D3D11_USAGE GetUsage(EGpuBufferUsage usage);
	D3D11_USAGE GetUsage(ETextureUsage usage);
	UINT GetCPUAccessFlags(ETextureUsage usage);
	D3D11_MAP GetLockType(EGpuBufferLockType lockType);

	DXGI_FORMAT GetFormat(CInputElement::EFormat inputElementFormat);
	DXGI_FORMAT GetFormat(AIndexGpuBuffer::EIndexType indexType);

	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType(EPrimitiveType primitiveType);
	D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(ETextureAddressMode addressMode);
	D3D11_FILTER GetFilter(ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter);

	EShaderConstantType GetShaderConstantType(D3D11_SHADER_TYPE_DESC variableTypeDesc);

	D3D11_BLEND_OP GetBlendOperation(EBlendOperation blendOperation);
	D3D11_BLEND GetBlendFactor(EBlendFactor blendFactor);

	D3D11_CULL_MODE GetCullingMode(ECullingMode cullingMode);
	D3D11_FILL_MODE GetFillMode(EFillMode fillMode);

	D3D11_COMPARISON_FUNC GetComparisonFunction(EComparisonFunction function);
	D3D11_STENCIL_OP GetStencilOperation(EStencilOperation operation);
}

}

#endif // __SLIMD3D11CONVERSIONS_H__