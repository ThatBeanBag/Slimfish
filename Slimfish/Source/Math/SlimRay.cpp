// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRay.cpp
// Description	: CSlimRay implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimRay.h"

// Local Includes

namespace Slim {

	CRay::CRay(const CVector3& origin, const CVector3& direction)
		:m_Origin(origin),
		m_Direction(direction)
	{

	}

	CRay::~CRay()
	{

	}

	const CVector3 CRay::GetPoint(float distance) const
	{
		return m_Origin + m_Direction * distance;
	}

	const float CRay::GetDistanceToPoint(const CVector3& point) const
	{
		return sqrt(GetDistanceToPointSquared(point));
	}

	const float CRay::GetDistanceToPointSquared(const CVector3& point) const
	{
		return CVector3::CrossProduct(m_Direction, point - m_Origin).GetLengthSquared();
	}

	const CVector3 CRay::GetIntersectionPointOnRay(const CVector3& point) const
	{
		return m_Origin + m_Direction * CVector3::DotProduct(m_Direction, point - m_Origin);
	}

	void CRay::SetOrigin(const CVector3& origin)
	{
		m_Origin = origin;
	}

	const CVector3& CRay::GetOrigin() const
	{
		return m_Origin;
	}

	void CRay::SetDirection(const CVector3& direction)
	{
		m_Direction = direction;
	}

	const CVector3& CRay::GetDirection() const
	{
		return m_Direction;
	}

	const CRay CRay::BuildFromPoints(const CVector3& start, const CVector3& end)
	{
		CVector3 rayLine = end - start;
		return CRay(start, CVector3::Normalise(rayLine));
	}

}



