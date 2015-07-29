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

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Material.h"

// Local Includes

CMaterial::CMaterial()
	:m_diffuse(CreateColourValueRGB(1.0f, 1.0f, 1.0f)),
	m_ambient(CreateColourValueRGB(1.0f, 1.0f, 1.0f)),
	m_specular(CreateColourValueRGB(1.0f, 1.0f, 1.0f))
{

}

CMaterial::~CMaterial()
{

}

void CMaterial::SetDiffuse(const TColourValue& _krDiffuse)
{
	m_diffuse = _krDiffuse;
}

void CMaterial::SetAmbient(const TColourValue& _krAmbient)
{
	m_ambient = _krAmbient;
}

void CMaterial::SetSpecular(const TColourValue& _krSpecular)
{
	m_specular = _krSpecular;
}

void CMaterial::SetEmissive(const TColourValue& _krEmissive)
{
	m_emissive = _krEmissive;
}

void CMaterial::SetPower(float _fPower)
{
	m_fPower = _fPower;
}

const TColourValue CMaterial::GetDiffuse() const
{
	return m_diffuse;
}

const TColourValue CMaterial::GetAmbient() const
{
	return m_ambient;
}

const TColourValue CMaterial::GetSpecular() const
{
	return m_specular;
}

const TColourValue CMaterial::GetEmissive() const
{
	return m_emissive;
}

const float CMaterial::GetPower() const
{
	return m_fPower;
}
