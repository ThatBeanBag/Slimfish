// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimLight.cpp
// Description	: CSlimLight implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimLight.h"

// Local Includes

namespace Slim {

CLight::CLight(CScene* pCreatorScene)
	:CSceneNode(pCreatorScene)
{

}

CLight::~CLight()
{

}

void CLight::SetType(ELightType type)
{
	m_LightProperties.type = type;
}

Slim::ELightType CLight::GetType() const
{
	return m_LightProperties.type;
}

void CLight::SetDiffuse(const CColourValue& diffuse)
{
	m_LightProperties.diffuse = diffuse;
}

const CColourValue& CLight::GetDiffuse() const
{
	return m_LightProperties.diffuse;
}

void CLight::SetSpecular(const CColourValue& specular)
{
	m_LightProperties.specular = specular;
}

const CColourValue& CLight::GetSpecular() const
{
	return m_LightProperties.specular;
}

void CLight::SetRange(float range)
{
	m_LightProperties.range = range;
}

const float CLight::GetRange() const
{
	return m_LightProperties.range;
}

void CLight::SetFalloff(float falloff)
{
	m_LightProperties.falloff = falloff;
}

const float CLight::GetFalloff() const
{
	return m_LightProperties.falloff;
}

void CLight::SetAttenuationConstant(float attenuationConst)
{
	m_LightProperties.attenuationConst = attenuationConst;
}

const float CLight::GetAttenuationConstant() const
{
	return m_LightProperties.attenuationConst;
}

void CLight::SetAttenuationLinear(float attenuationLinear)
{
	m_LightProperties.attenuationLinear = attenuationLinear;
}

const float CLight::GetAttenuationLinear() const
{
	return m_LightProperties.attenuationLinear;
}

void CLight::SetAttenuationQuadratic(float attenuationQuad)
{
	m_LightProperties.attenuationQuad = attenuationQuad;
}

const float CLight::GetAttenuationQuadratic() const
{
	return m_LightProperties.attenuationQuad;
}

CVector3 CLight::GetAttenuation() const
{
	return CVector3(m_LightProperties.attenuationConst, m_LightProperties.attenuationLinear, m_LightProperties.attenuationQuad);
}

void CLight::SetSpotTheta(float spotTheta)
{
	m_LightProperties.spotTheta = spotTheta;
}

const float CLight::GetSpotTheta() const
{
	return m_LightProperties.spotTheta;
}

void CLight::SetSpotPhi(float spotPhi)
{
	m_LightProperties.spotPhi = spotPhi;
}

const float CLight::GetSpotPhi() const
{
	return m_LightProperties.spotPhi;
}

const TLightProperties& CLight::GetLightProperties() const
{
	return m_LightProperties;
}

}

