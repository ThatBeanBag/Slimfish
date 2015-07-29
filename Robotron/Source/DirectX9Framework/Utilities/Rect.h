// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2014 Media Design School
//
// File Name	: Rect.h
// Description	: CRect declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef _RECT_H__
#define _RECT_H__

// Library Includes

// Local Includes
#include "Point.h"

class CRect {
	// Member Functions
public:
	// Constructors
	CRect();
	CRect(int _iX, int _iY, int _iW, int _iH);
	CRect(const CRect& _krRect);

	// Assignments
	CRect& operator=(const CRect& _krRect);

	// Operators
	bool operator==(const CRect& _krRect) const;
	bool operator!=(const CRect& _krRect) const;

	// Accessors
	void SetX(int _iX);
	void SetY(int _iY);
	void SetWidth(int _iWidth);
	void SetHeight(int _iHeight);

	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;

	int GetLeft() const;
	int GetRight() const;
	int GetTop() const;
	int GetBottom() const;

	CPoint GetPosition() const;
	void SetPosition(const CPoint& _krPosition);
protected:
private:

	// Member variables
public:
protected:
private:
	int m_iX;		// x coordinate.
	int m_iY;		// y coordinate.
	int m_iWidth;	// The width.
	int m_iHeight;	// The height.
};

bool IsInside(const CRect& _krRectA, const CPoint& krPoint);
bool IsIntersecting(const CRect& _krRectA, const CRect& _krRectB);
bool IsInside(const CRect& _krRectA, const CRect& _krRectB);

#endif