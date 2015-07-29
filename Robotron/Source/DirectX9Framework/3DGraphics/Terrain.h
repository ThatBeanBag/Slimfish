// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Terrain.h
// Description	: CTerrain implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#ifndef __TERRAIN_H__
#define __TERRAIN_H__

// Library Includes

// Local Includes
#include "MeshNode.h"

class CTerrain : public CMeshNode {
	// Member Functions
public:
	CTerrain();
	CTerrain(float _fWidth, float _fHeight, float _fDepth);
	virtual ~CTerrain();

	virtual void VRender();

	bool Initialize(const std::string& _krstrHeightMap, const std::string& _krstrTexture);
protected:
private:

	// Member Variables
public:
protected:
private:
	int m_iStaticBuffer;
	std::string m_strTextureFileName;
};

#endif // __TERRAIN_H__