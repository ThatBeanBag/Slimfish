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
	ELightType type;
	CColourValue diffuse;
	CColourValue specular;
	CVector3 position;
	CVector3 direction;
	float range;
	float falloff;
	float attenuationConst;
	float attenuationLinear;
	float attenuationQuad;
	float spotTheta;
	float spotPhi;
};

class CLight {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet 
	*/
	CLight();
	/** Destruct
	 	@author Hayden Asplet
	*/
	~CLight();

	/** Set the type of light e.g. DIRECTIONAL, POINT or SPOT. @author Hayden Asplet */
	void SetType(ELightType type);
	/** Get the type of light e.g. DIRECTIONAL, POINT or SPOT. @author Hayden Asplet */
	ELightType GetType() const;

	/** Set the diffuse colour of the light. @author Hayden Asplet */
	void SetDiffuse(const CColourValue& diffuse);
	/** Get the diffuse colour of the light. @author Hayden Asplet */
	const CColourValue& GetDiffuse() const;

	/** Set the specular colour of the light. @author Hayden Asplet */
	void SetSpecular(const CColourValue& specular);
	/** Get the specular colour of the light. @author Hayden Asplet */
	const CColourValue& GetSpecular() const;

	/** Set the position of the light in world space.
		@note Only applicable for point and spot lights.
	 	@author Hayden Asplet
	*/
	void SetPosition(const CVector3& position);
	/** Get the position of the light in world space.
		@note Only applicable for point and spot lights.
		@author Hayden Asplet
	*/
	const CVector3& GetPosition() const;

	/** Set the direction of the light in world space.
		@note Only applicable for directional and spot lights.
	 	@author Hayden Asplet
	*/
	void SetDirection(const CVector3& direction);

	/** Get the direction of the light in world space.
		@note Only applicable for directional and spot lights.
		@author Hayden Asplet
	*/
	const CVector3& GetDirection() const;

	/** Set the range of the light in world space.
		@note Only applicable for point and spot lights.
	 	@author Hayden Asplet
	*/
	void SetRange(float range);
	/** Get the range of the light in world space.
		@note Only applicable for point and spot lights.
		@author Hayden Asplet
	*/
	const float GetRange() const;

	/** Set the falloff of the light in world space.
		@note Only applicable for point and spot lights.
		@author Hayden Asplet
	*/
	void SetFalloff(float falloff);
	/** Get the falloff of the light in world space.
		@note Only applicable for point and spot lights.
		@author Hayden Asplet
	*/
	const float GetFalloff() const;

	/** Set the constant attenuation of the light.
		@note Only applicable for point and spot lights.
	 	@author Hayden Asplet
	*/
	void SetAttenuationConstant(float attenuationConst);
	/** Get the constant attenuation of the light.
		@note Only applicable for point and spot lights.
		@author Hayden Asplet
	*/
	const float GetAttenuationConstant() const;

	/** Set the linear attenuation of the light.
		@note Only applicable for point and spot lights.
		@author Hayden Asplet
	*/
	void SetAttenuationLinear(float attenuationLinear);
	/** Get the linear attenuation of the light.
		@note Only applicable for point and spot lights.
		@author Hayden Asplet
	*/
	const float GetAttenuationLinear() const;

	/** Set the quadratic attenuation of the light.
		@note Only applicable for point and spot lights.
		@author Hayden Asplet
	*/
	void SetAttenuationQuadratic(float attenuationQuad);
	/** Get the quadratic attenuation of the light.
		@note Only applicable for point and spot lights.
		@author Hayden Asplet
	*/
	const float GetAttenuationQuadratic() const;

	/** Get the constant, linear and quadratic attenuation of the light.
	 	@author Hayden Asplet
	 	@return Vector3 with x being constant, y being linear and z being quadratic attenuation.
	*/
	CVector3 GetAttenuation() const;

	/** Set the outside angle of the spot cone. @author Hayden Asplet */
	void SetSpotTheta(float spotTheta);
	/** Get the outside angle of the spot cone. @author Hayden Asplet */
	const float GetSpotTheta() const;

	/** Set the inside angle of the spot cone. @author Hayden Asplet */
	void SetSpotPhi(float spotPhi);
	/** Get the inside angle of the spot cone. @author Hayden Asplet */
	const float GetSpotPhi() const;

	/** Get all the properties of the light. @author Hayden Asplet */
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