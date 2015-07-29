// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9StaticBuffer.h
// Description	: CD3D9StaticBuffer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __D3D9STATICBUFFER_H__
#define __D3D9STATICBUFFER_H__

// Library Includes
#include <d3d9.h>

// Local Includes
#include "StaticBuffer.h"


class CD3D9StaticBuffer : public IStaticBuffer {
	// Member Functions
public:
	CD3D9StaticBuffer(IDirect3DDevice9* _pDevice);	// Default constructor
	CD3D9StaticBuffer(CD3D9StaticBuffer&& _rrOther);				// Move constructor.
	CD3D9StaticBuffer(const CD3D9StaticBuffer& _krOther) = delete;	// Deleted copy constructor.

	// Releases vertex and index buffers.
	~CD3D9StaticBuffer();	// Destructor

	CD3D9StaticBuffer& operator=(CD3D9StaticBuffer&& _rrOther);					// Move assignment operator.
	CD3D9StaticBuffer& operator=(const CD3D9StaticBuffer& _krOther) = delete;	// Deleted assignment operator.

	virtual bool VInitialise(DWORD _dwFVF,
							 EPrimitiveType _ePrimitiveType,
							 EIndexType _eIndexType,
							 unsigned int _uiNumVertices,
							 unsigned int _uiNumIndices,
							 unsigned int _iStride,
							 void* _pVertexData,
							 void* _pIndexData);

	virtual void VRender();
	virtual const CAABB VCalculateBoundingBox();
protected:
private:
	// Initialize from an rvalue reference.
	void Move(CD3D9StaticBuffer&& _rrOther);

	// Member Variables
public:
protected:
private:
	IDirect3DDevice9* m_pD3DDevice;				// A pointer to the directx9 device, owned by the renderer.
	IDirect3DVertexBuffer9* m_pVertexBuffer;	// A pointer to the vertex buffer, owned by us.
	IDirect3DIndexBuffer9* m_pIndexBuffer;		// A pointer to the index buffer, owned by us.

	DWORD m_dwFVF;
	EPrimitiveType m_ePrimitiveType;
	EIndexType m_eIndexType;
	unsigned int m_uiNumVertices;
	unsigned int m_uiNumIndices;
	unsigned int m_uiStride;

	CAABB m_AABB;
};

#endif	// __D3D9STATICBUFFER_H__