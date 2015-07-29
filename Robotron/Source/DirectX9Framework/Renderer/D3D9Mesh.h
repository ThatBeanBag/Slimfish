// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9Mesh.h
// Description	: CD3D9Mesh declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __D3D9MESH_H__
#define __D3D9MESH_H__

// Library Includes

// Local Includes
#include "Mesh.h"

class CD3D9Mesh : public IMesh {
	// Member Functions
public:
	CD3D9Mesh(IDirect3DDevice9* _pD3DDevice);
	CD3D9Mesh(CD3D9Mesh&& _rrOther);
	CD3D9Mesh(const CD3D9Mesh& _krOther) = delete;

	virtual ~CD3D9Mesh();

	CD3D9Mesh& operator=(CD3D9Mesh&& _rrOther);
	CD3D9Mesh& operator=(const CD3D9Mesh& _krOther) = delete;

	bool LoadFromFile(const std::string& _krstrFileName);
	virtual void VRender();
	virtual const CAABB VCalculateBoundingBox();
	virtual std::vector<CMaterial> VGetMaterials() override;
	virtual void VSetAlpha(float _fAlpha) override;
protected:
private:
	void Move(CD3D9Mesh&& _rrOther);



	// Member Variables
public:
protected:
private:
	IDirect3DDevice9* m_pD3DDevice;
	ID3DXMesh* m_pMesh;
	std::vector<CMaterial> m_vecMaterials;
	std::vector<shared_ptr<ITexture>> m_vecTextures;
};

#endif	// __D3D9MESH_H__