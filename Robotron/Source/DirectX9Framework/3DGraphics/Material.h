// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Material.h
// Description	: CMaterial implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

// Library Includes

// Local Includes

class CMaterial {
	// Member Functions
public:
	CMaterial();
	~CMaterial();

	void SetDiffuse(const TColourValue& _krDiffuse);
	void SetAmbient(const TColourValue& _krAmbient);
	void SetSpecular(const TColourValue& _krSpecular);
	void SetEmissive(const TColourValue& _krEmissive);
	void SetPower(float m_fPower);

	const TColourValue GetDiffuse() const;
	const TColourValue GetAmbient() const;
	const TColourValue GetSpecular() const;
	const TColourValue GetEmissive() const;
	const float GetPower() const;
protected:
private:

	// Member Variables
public:
protected:
private:
	TColourValue m_diffuse;		// The diffuse RGBA colour value.
	TColourValue m_ambient;		// The ambient RGB colour value.
	TColourValue m_specular;	// The specular RGB shine colour value.
	TColourValue m_emissive;	// The emmisive RGB colour value.
	float m_fPower;				// The sharpness of the specular highlight. 
};

#endif	// __MATERIAL_H__