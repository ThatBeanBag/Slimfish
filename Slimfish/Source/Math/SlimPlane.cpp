// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimPlane.cpp
// Description	: CSlimPlane implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimPlane.h"

// Local Includes

namespace Slim {

	CPlane::CPlane()
		:m_A(0), m_B(0), m_C(0), m_D(0)
	{

	}

	CPlane::CPlane(float a, float b, float c, float d)
		:m_A(a), m_B(b), m_C(c), m_D(d)
	{

	}

	CPlane::CPlane(const CVector3& point, const CVector3& normal)
		:m_A(normal.GetX()), 
		m_B(normal.GetY()),
		m_C(normal.GetZ()), 
		m_D(-CVector3::DotProduct(point, normal))
	{

	}

	CPlane::CPlane(const CVector3& pointA, const CVector3& pointB, const CVector3& pointC)
	{
		CVector3 lineAB = pointB - pointA;
		CVector3 lineAC = pointC - pointA;

		CVector3 planeNormal = CVector3::CrossProduct(lineAB, lineAC);
		planeNormal = CVector3::Normalise(planeNormal);

		*this = CPlane(pointA, planeNormal);
	}

	CPlane::~CPlane()
	{

	}

	const CVector3 CPlane::GetNormal() const
	{
		return CVector3(m_A, m_B, m_C);
	}

	const float CPlane::GetConstant() const
	{
		return m_D;
	}

	void CPlane::SetA(float a)
	{
		m_A = a;
	}

	const float CPlane::GetA() const
	{
		return m_A;
	}

	void CPlane::SetB(float b)
	{
		m_B = b;
	}

	const float CPlane::GetB() const
	{
		return m_B;
	}

	void CPlane::SetC(float c)
	{
		m_C = c;
	}

	const float CPlane::GetC() const
	{
		return m_C;
	}

	void CPlane::SetD(float d)
	{
		m_D = d;
	}

	const float CPlane::GetD() const
	{
		return m_D;
	}

	const CPlane CPlane::Normalise(const CPlane& plane)
	{
		float magnitude = plane.GetNormal().GetLength();

		if (magnitude == 0) {
			return plane;
		}
		else {
			float a = plane.GetA() / magnitude;
			float b = plane.GetB() / magnitude;
			float c = plane.GetC() / magnitude;
			float d = plane.GetD() / magnitude;

			return CPlane(a, b, c, d);
		}

		return CPlane();
	}

}


