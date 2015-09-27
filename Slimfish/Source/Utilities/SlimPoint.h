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

// Local Includes

namespace Slim {

	/** Simple class for representing a point on the screen.
	*/
	class CPoint {
		// Member Functions
	public:
		/** Default constructor.
			@author Hayden Asplet
		*/
		CPoint();

		/** Construct a point from a position on the screen defined by x, y coordinates.
			@author Hayden Asplet
		*/
		CPoint(int x, int y);

		/** Destructor.
		 	@author Hayden Asplet
		*/
		~CPoint();

		/** Add two points together and assign the result. @author Hayden Asplet */
		CPoint& operator+=(const CPoint& other);
		/** Subtract a point from another and assign the result. @author Hayden Asplet */
		CPoint& operator-=(const CPoint& other);

		/** Set the x component of the point. @author Hayden Asplet */
		void SetX(int x);
		/** Get the x component of the point. @author Hayden Asplet */
		const int GetX() const;
		/** Set the y component of the point. @author Hayden Asplet */
		void SetY(int y);
		/** Get the y component of the point. @author Hayden Asplet */
		const int GetY() const;
	protected:
	private:
		// Member Variables
	public:
	protected:
	private:
		int m_X;
		int m_Y;
	};

	/** Add two points together. @author Hayden Asplet */
	CPoint operator+(const CPoint& pointA, const CPoint& pointB);

	/** Subtract a point from another. @author Hayden Asplet */
	CPoint operator-(const CPoint& pointA, const CPoint& pointB);

}

#endif	// __SLIMPOINT_H__