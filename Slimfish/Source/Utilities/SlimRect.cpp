// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRect.cpp
// Description	: CSlimRect implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimRect.h"

// Local Includes

namespace Slim {

	CRect::CRect()
		:m_X(0), m_Y(0), m_Width(0), m_Height(0)
	{

	}

	CRect::CRect(int x, int y, int width, int height)
		:m_X(x), m_Y(y), m_Width(width), m_Height(height)
	{

	}

	CRect::~CRect()
	{

	}

	bool CRect::IsInside(const CPoint& point)
	{
		if (point.GetX() < m_X) {
			return false;
		}
		else if (point.GetY() < m_Y) {
			return false;
		}
		else if (point.GetX() > m_X + m_Width) {
			return false;
		}
		else if (point.GetY() > m_Y + m_Height) {
			return false;
		}

		return true;
	}

	void CRect::SetX(int x)
	{
		m_X = x;
	}

	const int CRect::GetX() const
	{
		return m_X;
	}

	void CRect::SetY(int y)
	{
		m_Y = y;
	}

	const int CRect::GetY() const
	{
		return m_Y;
	}

	void CRect::SetWidth(int width)
	{
		m_Width = width;
	}

	const int CRect::GetWidth() const
	{
		return m_Width;
	}

	void CRect::SetHeight(int height)
	{
		m_Height = height;
	}

	const int CRect::GetHeight() const
	{
		return m_Height;
	}

}

