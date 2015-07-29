// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionShape.h
// Description	: ACollisionShape declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __COLLISIONSHAPE_H__
#define __COLLISIONSHAPE_H__

// Library Includes

// Local Includes

class ACollisionShape {
	// Member Functions
public:
	virtual ~ACollisionShape() {}

	virtual const std::string VGetType() = 0;
	virtual bool VInitializeFromXML(TiXmlElement* _pXmlData) = 0;

	void SetRelativeTransform(const CMatrix4x4& _krmatRelativeRigidBodyTransform);
	const CMatrix4x4 GetRelativeTransform() const;

	void SetDensity(float _fDensity);
	void SetRestitution(float _fRestitution);
	void SetFriction(float _fFriction);

	const float GetDensity() const;
	const float GetRestitution() const;
	const float GetFriction() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	CMatrix4x4 m_matRelativeRigidBodyTransform;
	float m_fDensity;
	float m_fRestitution;
	float m_fFriction;
};

#endif	// __COLLISIONSHAPE_H__