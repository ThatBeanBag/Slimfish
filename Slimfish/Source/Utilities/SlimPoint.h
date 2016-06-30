// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimPoint.h
// Description	: CSlimPoint declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMPOINT_H__
#define __SLIMPOINT_H__

// Library Includes
#include <type_traits> 

// Local Includes

namespace Slim {

/** Simple class for representing a point on the screen.
*/
template<typename T = int>
class CPoint {
	static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
		"Point must be of a numeric type.");

	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CPoint()
		:m_X(0), m_Y(0)
	{

	}

	/** Construct a point from a position on the screen defined by x, y coordinates.
		@author Hayden Asplet
	*/
	CPoint(T x, T y)
		:m_X(x), m_Y(y)
	{

	}

	/** Destructor.
		@author Hayden Asplet
	*/
	~CPoint() {}

	/** Add two points together and assign the result. @author Hayden Asplet */
	CPoint& operator+=(const CPoint& other) 
	{
		// Invoke the '+' operator.
		*this = *this + other;
		return *this;
	}

	/** Subtract a point from another and assign the result. @author Hayden Asplet */
	CPoint& operator-=(const CPoint& other)
	{
		// Invoke the '-' operator.
		*this = *this - other;
		return *this;
	}

	/** Set the x component of the point. @author Hayden Asplet */
	void SetX(T x) { m_X = x; }
	/** Get the x component of the point. @author Hayden Asplet */
	const T GetX() const { return m_X; }
	/** Set the y component of the point. @author Hayden Asplet */
	void SetY(T y) { m_Y = y; }
	/** Get the y component of the point. @author Hayden Asplet */
	const T GetY() const { return m_Y; }
protected:
private:
	// Member Variables
public:
protected:
private:
	T m_X;
	T m_Y;
};

/** Add two points together. @author Hayden Asplet */
template<typename T>
CPoint<T> operator+(const CPoint<T>& pointA, const CPoint<T>& pointB)
{
	return CPoint<T>(
		pointA.GetX() + pointB.GetX(),
		pointA.GetY() + pointB.GetY());
}

/** Subtract a point from another. @author Hayden Asplet */
template<typename T>
CPoint<T> operator-(const CPoint<T>& pointA, const CPoint<T>& pointB)
{
	return CPoint<T>(
		pointA.GetX() - pointB.GetX(),
		pointA.GetY() - pointB.GetY());
}

/** Compares a point with another. @author Hayden Asplet */
template<typename T>
bool operator==(const CPoint<T>& pointA, const CPoint<T>& pointB)
{
	return pointA.GetX() == pointB.GetX() &&
		pointA.GetY() == pointB.GetY();
}

/** Compares a point with another and checks for inequality. @author Hayden Asplet */
template<typename T>
bool operator!=(const CPoint<T>& pointA, const CPoint<T>& pointB)
{
	return !(pointA == pointB);
}

}

#endif	// __SLIMPOINT_H__