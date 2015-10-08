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

class CTechnique;

/** Class representing the graphical material of an object in the scene. 
	@remarks
	
*/
class CMaterial {
	using TTechniques = std::vector<std::unique_ptr<CTechnique>>;

	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CMaterial();

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CMaterial();

	/** Create a technique and adds it to the material at the back.
	 	@author Hayden Asplet
	 	@return Pointer to the new technique for further modification.
	*/
	CTechnique* CreateTechnique();

	/** Add a technique to the list of the techniques. 
	 	@author Hayden Asplet
	 	@param const CTechnique & technique
	 	@return
	*/
	CTechnique* AddTechnique(const CTechnique& technique);

	CTechnique* GetBestTechnique();
	CTechnique* GetTechnique(size_t lod);
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