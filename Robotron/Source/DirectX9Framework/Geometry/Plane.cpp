// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Plane.cpp
// Description	: CPlane implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include 
#include "Plane.h"

// Local Includes



CPlane::CPlane()
	:m_fA(0.0f), m_fB(0.0f), m_fC(0.0f), m_fD(0.0f)
{

}

CPlane::CPlane(float _fA, float _fB, float _fC, float _fD)
	:m_fA(_fA), m_fB(_fB), m_fC(_fC), m_fD(_fD)
{

}

//CPlane::CPlane(const CVec3& _krPoint, const CVec3&_krNormal)
//	: m_fA(_krNormal.x), m_fB(_krNormal.y), m_fC(_krNormal.y), 
//	m_fD(-_krPoint.DotProduct(_krNormal))	// D = -(N . P), as D = -(a*x + b*y + c*z), where N = (a,b,c) and
//											// P = (x,y,x).
//{
//
//}

void CPlane::Initialise(const CVec3& _krPoint, const CVec3& _krNormal)
{
	// Assume _krNormal has a magnitude of 1.
	m_fA = _krNormal.x;
	m_fB = _krNormal.y;
	m_fC = _krNormal.z;

	// D = -(N . P), as D = -(a*x + b*y + c*z), where N = (a,b,c) and P = (x,y,x).
	m_fD = -(DotProduct(_krNormal, _krPoint));
}

void CPlane::Initialise(const CVec3& _krPointA, const CVec3& _krPointB, const CVec3& _krPointC)
{
	CVec3 vec3AB = _krPointB - _krPointA;
	CVec3 vec3AC = _krPointC - _krPointA;

	// Initialize from a point and a normal.
	CVec3 planeNormal = CrossProduct(vec3AB, vec3AC);
	planeNormal.Normalize();

	Initialise(_krPointC, planeNormal);
}

void CPlane::Normalise()
{
	float fMag = sqrt(m_fA * m_fA + m_fB * m_fB + m_fC * m_fC);

	if (fMag == 0) {
		return;
	}
	else {
		m_fA /= fMag;
		m_fB /= fMag;
		m_fC /= fMag;
		m_fD /= fMag;
	}
}

const CVec3 CPlane::GetNormal() const
{
	return CVec3(m_fA, m_fB, m_fC);
}

float CPlane::GetConstant() const
{
	return m_fD;
}

bool IsInsidePlane(const CPlane& _krPlane, const CVec3& _krPoint)
{
	CVec3 vec3Normal = _krPlane.GetNormal();

	// Inside the plane is defined as the direction of the normal.
	float fDotProduct = DotProduct(vec3Normal, _krPoint);
	fDotProduct += _krPlane.GetConstant();

	return (fDotProduct >= 0.0f);
}

CPlane TransformPlane(const CPlane& _krPlane, const CMatrix4x4& _krmatTransform)
{
	CVec3 vec3Normal = _krPlane.GetNormal();
	CVec3 vec3Point = vec3Normal * _krPlane.GetConstant();

	vec3Normal = _krmatTransform.Transform(vec3Normal, 0);
	vec3Point = _krmatTransform.Transform(vec3Point, 1);

	float fD = DotProduct(vec3Normal, vec3Point);

	return CPlane(vec3Normal.x, vec3Normal.y, vec3Normal.z, fD);
}
