// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2014 Media Design School
//
// File Name	: Point.cpp
// Description	: CPoint implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Point.h"

// Local Includes

CPoint::CPoint()
	:m_iX(0), m_iY(0)
{
}

CPoint::CPoint(int _iX, int _iY)
	: m_iX(_iX), m_iY(_iY)
{
}

CPoint::CPoint(const CPoint& _krPoint)
	: m_iX(_krPoint.m_iX), m_iY(_krPoint.m_iY)
{
}

CPoint& CPoint::operator=(const CPoint& _krPoint)
{
	m_iX = _krPoint.m_iX;
	m_iY = _krPoint.m_iY;
	return (*this);
}

CPoint& CPoint::operator+=(const CPoint& _krPoint)
{
	m_iX += _krPoint.m_iX;
	m_iY += _krPoint.m_iY;
	return (*this);
}
CPoint& CPoint::operator-=(const CPoint& _krPoint)
{
	m_iX -= _krPoint.m_iX;
	m_iY -= _krPoint.m_iY;
	return (*this);
}

int CPoint::GetX() const
{
	return m_iX;
}

int CPoint::GetY() const
{
	return m_iY;
}

void CPoint::SetX(int _iX)
{
	m_iX = _iX;
}

void CPoint::SetY(int _iY)
{
	m_iY = _iY;
}

CPoint CPoint::operator+(const CPoint& _krPoint) const
{
	CPoint temp(*this);
	temp += _krPoint;
	return temp;
}
CPoint CPoint::operator-(const CPoint& _krPoint) const
{
	CPoint temp(*this);
	temp -= _krPoint;
	return temp;
}

bool CPoint::operator==(const CPoint& _krPoint) const
{
	return (m_iX == _krPoint.m_iX) && (m_iY == _krPoint.m_iY);
}

bool CPoint::operator!=(const CPoint& _krPoint) const
{
	return !(*this == _krPoint);
}

int DistanceSquared(const CPoint& _krPoint1, const CPoint& _krPoint2)
{
	// The distance formula without square rooting.
	return (_krPoint2.m_iX - _krPoint1.m_iX) * (_krPoint2.m_iX - _krPoint1.m_iX) +
		(_krPoint2.m_iY - _krPoint1.m_iY) * (_krPoint2.m_iX - _krPoint1.m_iY);
}
