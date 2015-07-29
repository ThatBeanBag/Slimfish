// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9Mesh.cpp
// Description	: CD3D9Mesh implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "D3D9Mesh.h"

// Local Includes


CD3D9Mesh::CD3D9Mesh(IDirect3DDevice9* _pD3DDevice)
	:m_pD3DDevice(_pD3DDevice)
{

}

CD3D9Mesh::CD3D9Mesh(CD3D9Mesh&& _rrOther)
{
	Move(std::forward<CD3D9Mesh>(_rrOther));
}


CD3D9Mesh::~CD3D9Mesh()
{
	// Release the mesh.
	SafeRelease(m_pMesh);

	m_pD3DDevice = nullptr;
}

CD3D9Mesh& CD3D9Mesh::operator=(CD3D9Mesh&& _rrOther)
{
	Move(std::forward<CD3D9Mesh>(_rrOther));

	return *this;
}

bool CD3D9Mesh::LoadFromFile(const std::string& _krstrFileName)
{
	std::string strFilePath = ExtractPathFromResource(_krstrFileName);

	ID3DXBuffer* pMaterialBuffer;
	DWORD dwNumMaterials;

	HRESULT hResult;
	hResult = D3DXLoadMeshFromXA(_krstrFileName.c_str(),
								 D3DXMESH_MANAGED,
								 m_pD3DDevice,
								 nullptr,
								 &pMaterialBuffer,
								 nullptr,
								 &dwNumMaterials,
								 &m_pMesh);
	if (FAILED(hResult)) {
		DEBUG_ERROR("Failed to load mesh from file \"" + _krstrFileName + "\"");
		return false;
	}

	D3DXMATERIAL* pMaterials = reinterpret_cast<D3DXMATERIAL*>(pMaterialBuffer->GetBufferPointer());
	m_vecMaterials = std::vector<CMaterial>(dwNumMaterials);
	m_vecTextures = std::vector<shared_ptr<ITexture>>(dwNumMaterials, nullptr);

	for (unsigned int i = 0; i < m_vecMaterials.size(); ++i) {
		// Copy the material.
		D3DXCOLOR diffuse = pMaterials[i].MatD3D.Diffuse;
		D3DXCOLOR specular = pMaterials[i].MatD3D.Specular;
		D3DXCOLOR emissive = pMaterials[i].MatD3D.Emissive;
		m_vecMaterials[i].SetDiffuse(CreateColourValueARGB(diffuse.a, diffuse.r, diffuse.g, diffuse.b));
		m_vecMaterials[i].SetSpecular(CreateColourValueARGB(specular.a, specular.r, specular.g, specular.b));
		m_vecMaterials[i].SetEmissive(CreateColourValueARGB(emissive.a, emissive.r, emissive.g, emissive.b));
		m_vecMaterials[i].SetPower(pMaterials[i].MatD3D.Power);

		// Set the ambient colour for the material (D3DX doesn't do this). 
		m_vecMaterials[i].SetAmbient(m_vecMaterials[i].GetDiffuse());

		if (pMaterials[i].pTextureFilename != nullptr) {
			std::string strTextureFilePath = strFilePath + pMaterials[i].pTextureFilename;

			shared_ptr<ITexture> pCurrentTexture = CResourceManager<ITexture>::GetInstance()->Load(strTextureFilePath);

			if (!pCurrentTexture) {
				DEBUG_ERROR("Failed to load texture \"" + strTextureFilePath + "\"");
				return false;
			}

			m_vecTextures[i] = pCurrentTexture;
		}
		else {
			m_vecTextures[i] = nullptr;
		}
	}

	// Make sure there are normals for lighting.
	if (!(m_pMesh->GetFVF() & D3DFVF_NORMAL)) {
	// Does the mesh not have normals?
		ID3DXMesh* pTempMesh;
		hResult = m_pMesh->CloneMeshFVF(m_pMesh->GetOptions(), m_pMesh->GetFVF() | D3DFVF_NORMAL, m_pD3DDevice, &pTempMesh);

		if (FAILED(hResult)) {
			DEBUG_ERROR("Failed to clone mesh");
			return false;
		}

		SafeRelease(m_pMesh);
		m_pMesh = pTempMesh;
	}

	// Optimize the mesh for the vertex cache.
	unique_ptr<DWORD[]> rgdwAdjacency(new DWORD[m_pMesh->GetNumFaces() * 3]);
	if (!rgdwAdjacency) {
		m_pMesh->ConvertPointRepsToAdjacency(nullptr, rgdwAdjacency.get());
		m_pMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency.get(), nullptr, nullptr, nullptr);
	}

	return true;
}

void CD3D9Mesh::VRender()
{
	DEBUG_ASSERT(m_pD3DDevice);

	for (unsigned int i = 0; i < m_vecMaterials.size(); ++i) {
		g_pApp->GetRenderer()->VSetMaterial(m_vecMaterials[i]);

		g_pApp->GetRenderer()->VSetTexture(TEXSTAGE_0, m_vecTextures[i]);
		g_pApp->GetRenderer()->VSetTexture(TEXSTAGE_1, nullptr);

		m_pMesh->DrawSubset(i);
	}
}

const CAABB CD3D9Mesh::VCalculateBoundingBox()
{
	D3DXVECTOR3* pVerts;
	CVec3 vec3Min;
	CVec3 vec3Max;
	unsigned int uiStride = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// Lock the vertex buffer for reading.
	m_pMesh->LockVertexBuffer(0, reinterpret_cast<void**>(&pVerts));

	HRESULT hResult = D3DXComputeBoundingBox(pVerts, m_pMesh->GetNumVertices(), uiStride, &vec3Min, &vec3Max);

	// Done reading; unlock the vertex buffer.
	m_pMesh->UnlockVertexBuffer();

	if (FAILED(hResult)) {
		DEBUG_ERROR("Failed to compute bounding box for mesh");
		return CAABB();
	}
	else {
		return CAABB(vec3Min, vec3Max);
	}

	return CAABB();
}

void CD3D9Mesh::Move(CD3D9Mesh&& _rrOther)
{
	// Steal the mesh.
	m_pMesh = _rrOther.m_pMesh;
	_rrOther.m_pMesh = nullptr;

	// Move the vector of materials
	m_vecMaterials = std::move(_rrOther.m_vecMaterials);
}

void CD3D9Mesh::VSetAlpha(float _fAlpha)
{
	// Set the alpha of the diffuse for all material.
	for (unsigned int i = 0; i < m_vecMaterials.size(); ++i) {
		TColourValue diffuse = m_vecMaterials[i].GetDiffuse();
		diffuse.m_fA = _fAlpha;
		m_vecMaterials[i].SetDiffuse(diffuse);
	}
}

std::vector<CMaterial> CD3D9Mesh::VGetMaterials()
{
	return m_vecMaterials;
}
