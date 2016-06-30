// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRect.h
// Description	: CSlimRect declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMRECT_H__
#define __SLIMRECT_H__

// Library Includes
#include <type_traits> 

// Local Includes

namespace Slim {

template <typename T = int>
class CRect {
	static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
		"Rectangle must be of a numeric type.");

	// Member Functions
public:
	/** Default constructor.
		@remarks
			Sets the coordinates, width and height of the rectangle to 0.
		@author Hayden Asplet
	*/
	CRect()
		:m_X(0), m_Y(0), m_Width(0), m_Height(0)
	{

	}

	/** Construct a rectangle from a x, y coordinate and a width and height.
		@author Hayden Asplet
	*/
	CRect(T x, T y, T width, T height)
		:m_X(x), m_Y(y), m_Width(width), m_Height(height)
	{

	}

	/** Construct a rectangle from a position point and a width and height.
	 	@author Hayden Asplet
	*/
	CRect(const CPoint<T>& position, T width, T height)
		:CRect(position.GetX(), position.GetY(), width, height)
	{

	}

	/** Construct a rectangle from a position point and a point specifying the width and height.
	 	@author Hayden Asplet
	 	@param 
			position Position of the rectangle.
	 	@param 
			dimensions Width and height of the rectangle. X being width Y being height.
	*/
	CRect(const CPoint<T>& position, const CPoint<T>& dimensions)
		:CRect(position.GetX(), position.GetY(), dimensions.GetX(), dimensions.GetY())
	{

	}

	/** Construct a rectangle from a x, y coordinate and a point specifying the width and height.
	 	@author Hayden Asplet
		@param 
			x X Component of the position of the rectangle.
		@param 
			y Y Component of the position of the rectangle.
		@param 
			dimensions Width and height of the rectangle. X being width Y being height.
	*/
	CRect(T x, T y, const CPoint<T>& dimensions)
		:CRect(x, y, dimensions.GetX(), dimensions.GetY())
	{

	}
	
	/** Destructor
		@author Hayden Asplet
	*/
	~CRect() {}

	/** Check to see if a point is inside the rectangle.
	 	@author Hayden Asplet
		@return True if the point is inside the rectangle.
	*/
	bool IsInside(const CPoint<T>& point) const
	{
		if (point.GetX() < m_X) {
			return false;
		}
		else if (point.GetY() < m_Y) {
			return false;
		}
		else if (point.GetX() > GetRight()) {
			return false;
		}
		else if (point.GetY() > GetBottom()) {
			return false;
		}

		return true;
	}

	/** Check to see if another rectangle is overlapping or intersecting with the rectangle.
	 	@author Hayden Asplet
	 	@return True if the rectangles are intersecting or overlapping.
	*/
	bool IsIntersecting(const CRect<T>& rhs) const
	{
		if (this->GetLeft() < rhs.GetRight() && this->GetRight() > rhs.GetLeft() &&
			this->GetBottom() < rhs.GetTop() && this->GetTop() > rhs.GetBottom()) {
			return false;
		}

		return true;
	}
	
	/** Set the x component of the rectangle. @author Hayden Asplet */
	void SetX(T x) { m_X = x; }
	/** Get the x component of the rectangle. @author Hayden Asplet */
	const T GetX() const { return m_X; }
	/** Set the y component of the rectangle. @author Hayden Asplet */
	void SetY(T y) { m_Y = y; }
	/** Get the y component of the rectangle. @author Hayden Asplet */
	const T GetY() const { return m_Y; }
	/** Set the width. @author Hayden Asplet */
	void SetWidth(T width) { m_Width = width; }
	/** Get the width. @author Hayden Asplet */
	const T GetWidth() const { return m_Width; }
	/** Set the height. @author Hayden Asplet */
	void SetHeight(T height) { m_Height = height; }
	/** Get the height. @author Hayden Asplet */
	const T GetHeight() const { return m_Height; }

	/** Get the position of the rectangle.
	 	@author Hayden Asplet
	*/
	const CPoint<T> GetPosition() const { return CPoint<T>(m_X, m_Y); }

	/** Get the width and height of the rectangle.
	 	@author Hayden Asplet
	 	@return A point with the x component being the width and the y component being the height.
	*/
	const CPoint<T> GetDimensions() const { return CPoint<T>(m_Width, m_Height); }

	/** Get the x component of the left side of the rectangle.
		@note This is equivalent to GetX().
		@author Hayden Asplet 
	*/
	const T GetLeft() const { return GetX(); }

	/** Get the y component of the top edge of the rectangle.
		@note This is equivalent to GetY().
		@author Hayden Asplet
	*/
	const T GetTop() const { return GetY(); }

	/** Get the x component of the right side of the rectangle. @author Hayden Asplet */
	const T GetRight() const { return GetX() + GetWidth(); }
	/** Get the y component of the bottom side of the rectangle. @author Hayden Asplet */
	const T GetBottom() const { return GetY() + GetHeight(); }
protected:
private:

	// Member Variables
public:
protected:
private:
	T m_X;
	T m_Y;
	T m_Width;
	T m_Height;
};

}

#endif	// __SLIMRECT_H__