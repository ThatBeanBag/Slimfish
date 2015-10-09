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
	 	@return Pointer to the new technique for further modification.
	*/
	CTechnique* AddTechnique(const CTechnique& technique);

	/** Get the best possible technique for the current level of detail.
	 	@author Hayden Asplet
	*/
	CTechnique* GetBestTechnique();
	/** 
	 	@author Hayden Asplet
	 	@param size_t lod
	 	@return
	*/
	CTechnique* GetTechnique(size_t lod);
	/** 
	 	@author Hayden Asplet
	 	@return
	*/
	size_t GetNumTechniques() const { return m_Techniques.size(); }

	void RemoveTechnique(size_t lod);
	void ClearTechniques();

	/** Set the current level of detail. @author Hayden Asplet */
	void SetLevelOfDetail(size_t lod) { m_CurrentLOD = lod; }
	/** Get the current level of detail. @author Hayden Asplet */
	const size_t SetLevelOfDetail() const { return m_CurrentLOD; }
protected:
private:
	// Member Variables
public:
protected:
private:
	TTechniques m_Techniques;

	size_t m_CurrentLOD;
	CColourValue m_Diffuse;
	CColourValue m_Ambient;
	CColourValue m_Specular;
	CColourValue m_Emissive;
	float m_Power;

};

}
#endif	// __SLIMMATERIAL_H__