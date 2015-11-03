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

// Local Includes

namespace Slim {

class CRect {
	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CRect();
	/** Construct a rectangle from a x, y coordinate and a width and height.
		@author Hayden Asplet
	*/
	CRect(int x, int y, int width, int height);
	
	/** Destructor
		@author Hayden Asplet
	*/
	~CRect();

	/** Check to see if a point is inside the rectangle.
	 	@author Hayden Asplet
		@return True if the point is inside the rectangle.
	*/
	bool IsInside(const CPoint& point);
	
	/** Set the x component of the position. @author Hayden Asplet */
	void SetX(int x);
	/** Get the x component of the position. @author Hayden Asplet */
	const int GetX() const;
	/** Set the y component of the position. @author Hayden Asplet */
	void SetY(int y);
	/** Get the y component of the position. @author Hayden Asplet */
	const int GetY() const;
	/** Set the width. @author Hayden Asplet */
	void SetWidth(int width);
	/** Get the width. @author Hayden Asplet */
	const int GetWidth() const;
	/** Set the height. @author Hayden Asplet */
	void SetHeight(int height);
	/** Get the height. @author Hayden Asplet */
	const int GetHeight() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;
};
}

#endif	// __SLIMRECT_H__