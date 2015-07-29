// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: StaticBuffer.h
// Description	: IStaticBuffer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __STATICBUFFER_H__
#define __STATICBUFFER_H__

// Library Includes

// Local Includes
#include "Types.h"
#include "../Geometry/AABB.h"

class IStaticBuffer {
	// Member Functions
public:
	virtual ~IStaticBuffer() {};

	virtual bool VInitialise(DWORD _dwFVF,
							 EPrimitiveType _ePrimitiveType,
							 EIndexType _eIndexType,
							 unsigned int _uiNumVertices,
							 unsigned int _uiNumIndices,
							 unsigned int _iStride,
							 void* _pVertexData,
							 void* _pIndexData) = 0;
	virtual void VRender() = 0;
	virtual const CAABB VCalculateBoundingBox() = 0;
protected:
private:
	// Member Variables
public:
protected:
private:
};

#endif	// __STATICBUFFER_H__