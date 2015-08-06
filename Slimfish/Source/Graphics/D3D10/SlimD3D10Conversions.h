// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10Conversions.h
// Description	: CSlimD3D10Conversions declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10CONVERSIONS_H__
#define __SLIMD3D10CONVERSIONS_H__

// Library Includes

// Local Includes
#include "SlimD3D10Forward.h"

namespace Slim {

namespace D3D10Conversions {
	D3DXVECTOR3 Get(const CVec3& vector);
	CVec3 Get(const D3DXVECTOR3& vector);

	D3DXMATRIX Get(const CMatrix4x4& matrix);
	CMatrix4x4 Get(const D3DXMATRIX& matrix);

	D3DXCOLOR Get(const TColourValue& colour);
	TColourValue Get(const D3DXCOLOR& colour);
}

}

#endif // __SLIMD3D10CONVERSIONS_H__