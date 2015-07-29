// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Plane.h
// Description	: CPlane declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __PLANE_H__
#define __PLANE_H__

// Library Includes

// Local Includes
#include "Vec3.h"

class CPlane {
	// Member Functions
public:
	CPlane();
	CPlane(float _fA, float _fB, float _fC, float _fD);
//	CPlane(const CVec3& _krPoint, const CVec3& _krNormal);

	// Initialises the plane from a point on the plane and a normal vector.
	// Assumes _krNormal is a normalized vector.
	void Initialise(const CVec3& _krPoint, const CVec3& _krNormal);
	// Initialises the plane from 3 points. Clockwise ordering will make the plane face you.
	void Initialise(const CVec3& _krPointA, const CVec3& _krPointB, const CVec3& _krPointC);

	void Normalise();

	const CVec3 GetNormal() const;
	float GetConstant() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	float m_fA;
	float m_fB;
	float m_fC;
	float m_fD;
};

bool IsInsidePlane(const CPlane& _krPlane, const CVec3& _krPoint);
CPlane TransformPlane(const CPlane& _krPlane, const CMatrix4x4& _krmatTransform);

#endif	//__PLANE_H__