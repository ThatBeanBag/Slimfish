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
#include "SlimD3D10Forward.h"

namespace Slim {

namespace D3D10Conversions {
	D3DXVECTOR3 GetVec3(const CVector3& vector);
	CVector3 GetVec3(const D3DXVECTOR3& vector);

	D3DXMATRIX GetMatrix4x4(const CMatrix4x4& matrix);
	CMatrix4x4 GetMatrix4x4(const D3DXMATRIX& matrix);

	D3DXCOLOR GetColour(const TColourValue& colour);
	TColourValue GetColour(const D3DXCOLOR& colour);

	D3D10_USAGE GetUsage(AGpuBuffer::EUsage usage);
	D3D10_USAGE GetUsage(ATexture::EUsage usage);
	UINT GetCPUAccessFlags(ATexture::EUsage usage);
	D3D10_MAP GetLockType(AGpuBuffer::ELockType lockType);

	DXGI_FORMAT GetFormat(CInputElement::EFormat inputElementFormat);
	DXGI_FORMAT GetFormat(AIndexGpuBuffer::EIndexType indexType);

	D3D10_PRIMITIVE_TOPOLOGY GetPrimitiveType(CVertexDeclaration::EPrimitiveType primitiveType);
}

}

#endif // __SLIMD3D10CONVERSIONS_H__