// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimMaterial.h
// Description	: CMaterial declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMMATERIAL_H__
#define __SLIMMATERIAL_H__

// Library Includes

// Local Includes
#include "SlimColour.h"

namespace Slim {

/************************************************************************/
/* CMaterial
/* 
/* Represents the graphical material of an object in the scene. 
/************************************************************************/
class CMaterial {
	typedef std::vector<std::unique_ptr<CRenderPass>> TTechnique;
	typedef std::vector<TTechnique> TTechniques;

	// Member Functions
public:
	CMaterial();
	~CMaterial();

	TTechnique* CreateTechnique();
	TTechnique* GetBestTechnique();
	TTechnique* GetTechnique(size_t lod);
	size_t GetNumTechniques() const { return m_Techniques.size(); }


	void RemoveTechnique(size_t lod);
	void ClearTechniques();
protected:
private:
	// Member Variables
public:
protected:
private:
	TTechniques m_Techniques;

	CColourValue m_diffuse;
	CColourValue m_ambient;
	CColourValue m_specular;
	CColourValue m_emissive;
	float m_power;

};

}
#endif	// __SLIMMATERIAL_H__