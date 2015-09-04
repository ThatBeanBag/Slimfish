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

	CLight::CLight()
	{

	}

	CLight::~CLight()
	{

	}

	void CLight::SetType(ELightType type)
	{
		m_LightProperties.m_Type = type;
	}

	Slim::ELightType CLight::GetType() const
	{
		return m_LightProperties.m_Type;
	}

	void CLight::SetDiffuse(const CColourValue& diffuse)
	{
		m_LightProperties.m_Diffuse = diffuse;
	}

	const CColourValue& CLight::GetDiffuse() const
	{
		return m_LightProperties.m_Diffuse;
	}

	void CLight::SetSpecular(const CColourValue& specular)
	{
		m_LightProperties.m_Specular = specular;
	}

	const CColourValue& CLight::GetSpecular() const
	{
		return m_LightProperties.m_Specular;
	}

	void CLight::SetPosition(const CVector3& position)
	{
		m_LightProperties.m_Position = position;
	}

	const CVector3& CLight::GetPosition() const
	{
		return m_LightProperties.m_Position;
	}

	void CLight::SetDirection(const CVector3& direction)
	{
		m_LightProperties.m_Direction = direction;
	}

	const CVector3& CLight::GetDirection() const
	{
		return m_LightProperties.m_Direction;
	}

	void CLight::SetRange(float range)
	{
		m_LightProperties.m_Range = range;
	}

	const float CLight::GetRange() const
	{
		return m_LightProperties.m_Range;
	}

	void CLight::SetFalloff(float falloff)
	{
		m_LightProperties.m_Falloff = falloff;
	}

	const float CLight::GetFalloff() const
	{
		return m_LightProperties.m_Falloff;
	}

	void CLight::SetAttenuationConstant(float attenuationConst)
	{
		m_LightProperties.m_AttenuationConst = attenuationConst;
	}

	const float CLight::GetAttenuationConstant() const
	{
		return m_LightProperties.m_AttenuationConst;
	}

	void CLight::SetAttenuationLinear(float attenuationLinear)
	{
		m_LightProperties.m_AttenuationLinear = attenuationLinear;
	}

	const float CLight::GetAttenuationLinear() const
	{
		return m_LightProperties.m_AttenuationLinear;
	}

	void CLight::SetAttenuationQuadratic(float attenuationQuad)
	{
		m_LightProperties.m_AttenuationQuad = attenuationQuad;
	}

	const float CLight::GetAttenuationQuadratic() const
	{
		return m_LightProperties.m_AttenuationQuad;
	}

	void CLight::SetSpotTheta(float spotTheta)
	{
		m_LightProperties.m_SpotTheta = spotTheta;
	}

	const float CLight::GetSpotTheta() const
	{
		return m_LightProperties.m_SpotTheta;
	}

	void CLight::SetSpotPhi(float spotPhi)
	{
		m_LightProperties.m_SpotPhi = spotPhi;
	}

	const float CLight::GetSpotPhi() const
	{
		return m_LightProperties.m_SpotPhi;
	}

	const TLightProperties& CLight::GetLightProperties() const
	{
		return m_LightProperties;
	}

}

