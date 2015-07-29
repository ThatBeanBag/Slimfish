// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2014 Media Design School
//
// File Name	: Rect.cpp
// Description	: CRect implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Rect.h"

// Local Includes

CRect::CRect()
	:m_iX(0),
	m_iY(0),
	m_iWidth(0),
	m_iHeight(0)
{

}

CRect::CRect(int _iX, int _iY, int _iW, int _iH)
	:m_iX(_iX),
	m_iY(_iY),
	m_iWidth(_iW),
	m_iHeight(_iH)
{

}

CRect::CRect(const CRect& _krRect)
	:m_iX(_krRect.m_iX),
	m_iY(_krRect.m_iY),
	m_iWidth(_krRect.m_iWidth),
	m_iHeight(_krRect.m_iHeight)
{

}

void CRect::SetX(int _iX)
{
	m_iX = _iX;
}

void CRect::SetY(int _iY)
{
	m_iY = _iY;
}

void CRect::SetWidth(int _iWidth)
{
	m_iWidth = _iWidth;
}

void CRect::SetHeight(int _iHeight)
{
	m_iHeight = _iHeight;
}

CRect& CRect::operator=(const CRect& _krRect)
{
	m_iX = _krRect.m_iX;
	m_iY = _krRect.m_iY;
	m_iWidth = _krRect.m_iWidth;
	m_iHeight = _krRect.m_iHeight;

	return *this;
}

bool CRect::operator==(const CRect& _krRect) const
{
	return m_iX == _krRect.m_iX && 
		   m_iY == _krRect.m_iY &&
		   m_iWidth == _krRect.m_iWidth &&
		   m_iHeight == _krRect.m_iHeight;
}

bool CRect::operator!=(const CRect& _krRect) const
{
	return !(*this == _krRect);
}

int CRect::GetX() const
{
	return m_iX;
}

int CRect::GetY() const
{
	return m_iY;
}

int CRect::GetWidth() const
{
	return m_iWidth;
}

int CRect::GetHeight() const
{
	return m_iHeight;
}

int CRect::GetLeft() const
{
	return m_iX;
}

int CRect::GetRight() const
{
	return m_iX + m_iWidth;
}

int CRect::GetTop() const
{
	return m_iY;
}

int CRect::GetBottom() const
{
	return m_iY + m_iHeight;
}

CPoint CRect::GetPosition() const
{
	return CPoint(m_iX, m_iY);
}

void CRect::SetPosition(const CPoint& _krPosition)
{
	m_iX = _krPosition.GetX();
	m_iY = _krPosition.GetY();
}

bool IsIntersecting(const CRect& _krRectA, const CRect& _krRectB)
{
	// Are any of the sides of _krRectA outside _krRectB. 
	if (_krRectA.GetLeft() > _krRectB.GetRight()) {
		return false;
	}
	else if (_krRectA.GetRight() < _krRectB.GetLeft()) {
		return false;
	}
	else if (_krRectA.GetTop() > _krRectB.GetBottom()) {
		return false;
	}
	else if (_krRectA.GetBottom() < _krRectB.GetTop()) {
		return false;
	}
	else {
		// A is inside B.
		return true;
	}

	// We passed the test.
	return true;
}

bool IsInside(const CRect& _krRectA, const CRect& _krRectB)
// Is _krRectA inside _krRectB
{
	// Are any of the sides of _krRectA outside _krRectB. 
	if (_krRectA.GetLeft() < _krRectB.GetLeft()) {
		return false;
	}
	else if (_krRectA.GetRight() > _krRectB.GetRight()) {
		return false;
	}
	else if (_krRectA.GetTop() < _krRectB.GetTop()) {
		return false;
	}
	else if (_krRectA.GetBottom() > _krRectB.GetBottom()) {
		return false;
	}
	else {
	// A is inside B.
		return true;
	}

	return true;
}

bool IsInside(const CRect& _krRect, const CPoint& _krPoint)
{
	if (_krPoint.GetX() < _krRect.GetLeft()) {
		return false;
	}
	else if (_krPoint.GetY() < _krRect.GetTop()) {
		return false;
	}
	else if (_krPoint.GetX() > _krRect.GetRight()) {
		return false;
	}
	else if (_krPoint.GetY() > _krRect.GetBottom()) {
		return false;
	}

	return true;
}
