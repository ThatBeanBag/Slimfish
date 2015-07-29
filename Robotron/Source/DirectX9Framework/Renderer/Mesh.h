// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Mesh.h
// Description	: IMesh declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MESH_H__
#define __MESH_H__

// Library Includes

// Local Includes

class CMaterial;

class IMesh {
	// Member Functions
public:
	virtual ~IMesh();

	virtual void VRender() = 0;
	virtual const CAABB VCalculateBoundingBox() = 0;
	virtual std::vector<CMaterial> VGetMaterials() = 0;
	virtual void VSetAlpha(float _fAlpha) = 0;

	static shared_ptr<IMesh> LoadResource(const std::string& _krstrFileName);
protected:
private:
	// Member Variables
public:
protected:
private:
};

#endif	// __MESH_H__