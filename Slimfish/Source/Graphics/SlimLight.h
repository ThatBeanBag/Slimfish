// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimLight.h
// Description	: CSlimLight declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMLIGHT_H__
#define __SLIMLIGHT_H__

// Library Includes

// Local Includes

namespace Slim {

/** List of light types.
*/
enum ELightType {
	// Parallel light, like a sun in a scene. Only has a direction, no position.
	LIGHT_DIRECTIONAL,
	// Light emitting from a point. Only has a position, no direction.
	LIGHT_POINT,
	// Light emitting out in a cone shape. Has both a position and direction.
	LIGHT_SPOT
};

struct TLightProperties {
	ELightType m_Type;
	CColourValue m_Diffuse;
	CColourValue m_Specular;
	CVector3 m_Position;
	CVector3 m_Direction;
	float m_Range;
	float m_Falloff;
	float m_AttenuationConst;
	float m_AttenuationLinear;
	float m_AttenuationQuad;
	float m_SpotTheta;
	float m_SpotPhi;
};

class CLight {
	// Member Functions
public:
	CLight();
	~CLight();

	void SetType(ELightType type);
	ELightType GetType() const;

	void SetDiffuse(const CColourValue& diffuse);
	const CColourValue& GetDiffuse() const;

	void SetSpecular(const CColourValue& specular);
	const CColourValue& GetSpecular() const;

	void SetPosition(const CVector3& position);
	const CVector3& GetPosition() const;

	void SetDirection(const CVector3& direction);
	const CVector3& GetDirection() const;

	void SetRange(float range);
	const float GetRange() const;

	void SetFalloff(float falloff);
	const float GetFalloff() const;

	void SetAttenuationConstant(float attenuationConst);
	const float GetAttenuationConstant() const;

	void SetAttenuationLinear(float attenuationLinear);
	const float GetAttenuationLinear() const;

	void SetAttenuationQuadratic(float attenuationQuad);
	const float GetAttenuationQuadratic() const;

	void SetSpotTheta(float spotTheta);
	const float GetSpotTheta() const;

	void SetSpotPhi(float spotPhi);
	const float GetSpotPhi() const;

	const TLightProperties& GetLightProperties() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	TLightProperties m_LightProperties;
};

}
#endif	// __SLIMLIGHT_H__