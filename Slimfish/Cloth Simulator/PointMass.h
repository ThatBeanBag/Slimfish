// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: PointMass.h
// Description	: CPointMass declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __POINTMASS_H__
#define __POINTMASS_H__

// Library Includes

// Local Includes

const float g_GRAVITY = 9.81f;

// Forward Declaration.
class CLink;

class CPointMass {
	// Member Functions
public:
	CPointMass(const CVector3& position);
	~CPointMass();

	void Update(float timeStep);
	void SolveConstraints();

	void Attach(CPointMass* pointMass, float restingDistance, float stiffness, float breakForce);

	void ApplyForce(const CVector3& force);
	void Pin(const CVector3& pinPosition);

	/** Set the position. @author Hayden Asplet */
	void SetPosition(const CVector3& position);
	/** Get the position. @author Hayden Asplet */
	const CVector3& GetPosition() const;
	/** Set the mass. @author Hayden Asplet */
	void SetMass(float mass);
	/** Get the mass. @author Hayden Asplet */
	const float GetMass() const;

	const std::vector<CLink>& GetLinks() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	CVector3 m_Position;
	CVector3 m_LastPosition;
	CVector3 m_Acceleration;
	std::vector<CLink> m_Links;

	float m_Mass;
	float m_Damping;

	CVector3 m_PinPosition;
	bool m_IsPinned;
};

#endif	// __POINTMASS_H__