// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9StaticBuffer.cpp
// Description	: CD3D9StaticBuffer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "D3D9StaticBuffer.h"

// Local Includes

CD3D9StaticBuffer::CD3D9StaticBuffer(IDirect3DDevice9* _pDevice)
	:m_pD3DDevice(_pDevice),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_dwFVF(0),
	m_uiNumVertices(0),
	m_uiNumIndices(0),
	m_uiStride(0)
{

}

CD3D9StaticBuffer::CD3D9StaticBuffer(CD3D9StaticBuffer&& _rrOther)
{
	// Forward the rvalue reference and initialize ourselves from it.
	Move(std::forward<CD3D9StaticBuffer>(_rrOther));
}

CD3D9StaticBuffer& CD3D9StaticBuffer::operator=(CD3D9StaticBuffer&& _rrOther)
{
	// Forward the rvalue reference and initialize ourselves from it.
	Move(std::forward<CD3D9StaticBuffer>(_rrOther));
	return *this;
}

CD3D9StaticBuffer::~CD3D9StaticBuffer()
{
	// Release the vertex buffer.
	if (m_pVertexBuffer) {
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	// Release the index buffer.
	if (m_pIndexBuffer) {
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

	m_pD3DDevice = nullptr;
}

bool CD3D9StaticBuffer::VInitialise(DWORD _dwFVF,
								    EPrimitiveType _ePrimitiveType,
								    EIndexType _eIndexType,
								    unsigned int _uiNumVertices,
								    unsigned int _uiNumIndices,
								    unsigned int _uiStride,
								    void* _pVertexData,
								    void* _pIndexData)
{
	if (!m_pD3DDevice) {
		return false;
	}

	m_uiNumVertices = _uiNumVertices;
	m_uiNumIndices = _uiNumIndices;
	m_uiStride = _uiStride;
	m_dwFVF = _dwFVF;
	m_ePrimitiveType = _ePrimitiveType;
	m_eIndexType = _eIndexType;

	// Create the vertex buffer on the device.
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(m_uiNumVertices * m_uiStride,
												0,
												m_dwFVF,
												D3DPOOL_MANAGED,
												&m_pVertexBuffer,
												NULL))) { // GR: Vertex/Index Buffer
	// Failed to create on device.
		return false;
	}

	D3DFORMAT d3dIndexFormat = D3DFMT_INDEX16;
	unsigned int uiIndexLength = 0;

	if (m_eIndexType == IT_FORMAT16) {
		uiIndexLength = m_uiNumIndices * sizeof(USHORT);
		d3dIndexFormat = D3DFMT_INDEX16;
	}
	else if(m_eIndexType == IT_FORMAT32) {
		uiIndexLength = m_uiNumIndices * sizeof(UINT);
		d3dIndexFormat = D3DFMT_INDEX32;
	}

	if (m_eIndexType != IT_NONE) {
		if (FAILED(m_pD3DDevice->CreateIndexBuffer(uiIndexLength,
												   D3DUSAGE_WRITEONLY,
												   d3dIndexFormat,
												   D3DPOOL_MANAGED,
												   &m_pIndexBuffer,
												   NULL))) {
		// Failed to create index buffer on device.
			return false;
		}
	}
	else {
		m_uiNumIndices = m_uiNumVertices;
	}

	void* pVertices;

	// Lock the vertex buffer for writing to.
	m_pVertexBuffer->Lock(0, 0, &pVertices, 0);

	// Copy the vertex data to the buffer.
	memcpy(pVertices, _pVertexData, m_uiNumVertices * m_uiStride);

	// Done writing; unlock the  vertex buffer.
	m_pVertexBuffer->Unlock();

	if (m_eIndexType != IT_NONE) {
		void* pIndices;

		// Lock the index buffer for writing to.
		m_pIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

		// Copy the index data to the buffer.
		memcpy(pIndices, _pIndexData, uiIndexLength);

		// Done writing; unlock the index buffer.
		m_pIndexBuffer->Unlock();
	}
	else {
		m_uiNumIndices = 0;
	}

	return true;
}

void CD3D9StaticBuffer::VRender()
{
	if (m_pD3DDevice == nullptr) {
		return;
	}
	
	if (m_pVertexBuffer == nullptr) {
		return;
	}

	if (m_uiNumVertices <= 0) {
		return;
	}
	else {
		D3DPRIMITIVETYPE eD3DPrimitiveType;
		unsigned int uiNumPrimitives = 0;

		switch (m_ePrimitiveType) {
			case PT_POINTLIST: {
				eD3DPrimitiveType = D3DPT_POINTLIST;
				uiNumPrimitives = m_uiNumVertices;

				break;
			}
			case PT_LINELIST: {
				eD3DPrimitiveType = D3DPT_LINELIST;
				uiNumPrimitives = static_cast<unsigned int>(m_uiNumVertices / 2.0f);

				break;
			}
			case PT_LINESTRIP: {
				eD3DPrimitiveType = D3DPT_LINESTRIP;
				uiNumPrimitives = m_uiNumVertices - 1;

				break;
			}
			case PT_TRIANGLELIST: {
				eD3DPrimitiveType = D3DPT_TRIANGLELIST;
				uiNumPrimitives = static_cast<unsigned int>(m_uiNumVertices / 3.0f);

				break;
			}
			case PT_TRIANGLESTRIP: {
				eD3DPrimitiveType = D3DPT_TRIANGLESTRIP;

				if (m_uiNumIndices > 2) {
					uiNumPrimitives = m_uiNumIndices - 2;
				}
				else if (m_uiNumVertices > 2) {
					uiNumPrimitives = m_uiNumVertices - 2;
				}
				else {
					return;
				}

				break;
			}
			case PT_TRIANGLEFAN: {
				eD3DPrimitiveType = D3DPT_TRIANGLEFAN;
				uiNumPrimitives = m_uiNumVertices;

				if (m_uiNumVertices > 2) {
					uiNumPrimitives = m_uiNumVertices - 2;
				}
				else {
					return;
				}

				break;
			}

			default: break;
		}

		m_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, m_uiStride);
		m_pD3DDevice->SetIndices(m_pIndexBuffer);
		m_pD3DDevice->SetFVF(m_dwFVF);

		if (m_uiNumIndices > 0) {
			m_pD3DDevice->DrawIndexedPrimitive(eD3DPrimitiveType, 0, 0, m_uiNumVertices, 0, uiNumPrimitives); // GR: Draw Primitive
		}
		else {
			m_pD3DDevice->DrawPrimitive(eD3DPrimitiveType, 0, uiNumPrimitives);
		}
	}
}

const CAABB CD3D9StaticBuffer::VCalculateBoundingBox()
{
	D3DXVECTOR3* pVerts; // MF: D3DX Usage.
	CVec3 vec3Min;
	CVec3 vec3Max;

	// Lock the vertex buffer for reading.
	m_pVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVerts), 0);

	HRESULT hResult = D3DXComputeBoundingBox(pVerts, m_uiNumVertices, m_uiStride, &vec3Min, &vec3Max);

	// Done reading; unlock the vertex buffer.
	m_pVertexBuffer->Unlock();
	
	if (FAILED(hResult)) {
		return CAABB();
	}
	else {
		return CAABB(vec3Min, vec3Max);
	}
}

void CD3D9StaticBuffer::Move(CD3D9StaticBuffer&& _rrOther)
{
	if (this == &_rrOther) {
	// Are we assigning to ourselves?
		// No need to assign to ourselves.
		return;
	}
	else {
		if (m_pVertexBuffer != nullptr) {
			// Do we already have a vertex buffer?
			// Release it.
			m_pVertexBuffer->Release();
			m_pVertexBuffer = nullptr;
		}

		if (m_pIndexBuffer != nullptr) {
			// Do we already have a index buffer?
			// Release it.
			m_pIndexBuffer->Release();
			m_pIndexBuffer = nullptr;
		}

		// Steal _rrOther's vertex and index buffers and device.
		m_pD3DDevice = _rrOther.m_pD3DDevice;
		m_pVertexBuffer = _rrOther.m_pVertexBuffer;
		m_pIndexBuffer = _rrOther.m_pIndexBuffer;
		m_dwFVF = _rrOther.m_dwFVF;
		m_ePrimitiveType = _rrOther.m_ePrimitiveType;
		m_eIndexType = _rrOther.m_eIndexType;
		m_uiNumVertices = _rrOther.m_uiNumVertices;
		m_uiNumIndices = _rrOther.m_uiNumIndices;
		m_uiStride = _rrOther.m_uiStride;

		_rrOther.m_pD3DDevice = nullptr;
		_rrOther.m_pVertexBuffer = nullptr;
		_rrOther.m_pIndexBuffer = nullptr;
	}
}