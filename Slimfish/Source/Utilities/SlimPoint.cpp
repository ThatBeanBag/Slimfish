// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimPoint.cpp
// Description	: CSlimPoint implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimPoint.h"

// Local Includes

namespace Slim {

	CPoint::CPoint()
		:m_X(0), m_Y(0)
	{

	}

	CPoint::CPoint(int x, int y) 
		:m_X(x), m_Y(y)
	{

	}

	CPoint::~CPoint()
	{

	}

	CPoint& CPoint::operator+=(const CPoint& other)
	{
		*this = *this + other;
		return *this;
	}

	CPoint& CPoint::operator-=(const CPoint& other)
	{
		*this = *this - other;
		return *this;
	}

	void CPoint::SetX(int x)
	{
		m_X = x;
	}

	const int CPoint::GetX() const
	{
		return m_X;
	}

	void CPoint::SetY(int y)
	{
		m_Y = y;
	}

	const int CPoint::GetY() const
	{
		return m_Y;
	}

	CPoint operator+(const CPoint& pointA, const CPoint& pointB)
	{
		return CPoint(pointA.GetX() + pointB.GetX(), pointA.GetY() + pointB.GetY());
	}

	CPoint operator-(const CPoint& pointA, const CPoint& pointB)
	{
		return CPoint(pointA.GetX() - pointB.GetX(), pointA.GetY() - pointB.GetY());
	}

	bool operator==(const CPoint& pointA, const CPoint& pointB)
	{
		return pointA.GetX() == pointB.GetX() &&
			pointA.GetY() == pointB.GetY();
	}

	bool operator!=(const CPoint& pointA, const CPoint& pointB)
	{
		return !(pointA == pointB);
	}
}


