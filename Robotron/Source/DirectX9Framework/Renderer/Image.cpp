// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Images.cpp
// Description	: CImage implemenation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes
#include <utility>

// This Include
#include "Image.h"

// Local Includes


CImage::CImage(int _iWidth, int _iHeight)
	:m_pPixels(new TColour[_iWidth * _iHeight]),
	m_iWidth(_iWidth),
	m_iHeight(_iHeight)
{
}

CImage::CImage(const CImage& _krOther)
	:m_pPixels(new TColour[_krOther.m_iWidth * _krOther.m_iHeight]),
	m_iWidth(_krOther.m_iWidth),
	m_iHeight(_krOther.m_iHeight)
{
	// Copy over the data.
	for (int iY = 0; iY < m_iHeight; ++iY) {
		for (int iX = 0; iX < m_iWidth; ++iX) {
			m_pPixels[iX + iY * m_iWidth] = _krOther.m_pPixels[iX + iY * m_iWidth];
		}
	}
}

CImage::CImage(CImage&& _krOther)
	:m_pPixels(NULL),
	m_iWidth(0),
	m_iHeight(0)
{
	// Invoke the move assignment operator.
	*this = std::move(_krOther);
}

CImage::~CImage()
{
	if (m_pPixels) {
		delete[] m_pPixels;
	}
}

CImage& CImage::operator=(const CImage& _krOther)
{
	if (m_pPixels) {
	// Do we already have an existing image?
		delete[] m_pPixels;
	}

	m_iWidth = _krOther.m_iWidth;
	m_iHeight = _krOther.m_iHeight;
	m_pPixels = new TColour[m_iWidth * m_iHeight];

	// Copy over the data.
	for (int iY = 0; iY < m_iHeight; ++iY) {
		for (int iX = 0; iX < m_iWidth; ++iX) {
			m_pPixels[iX + iY * m_iWidth] = _krOther.m_pPixels[iX + iY * m_iWidth];
		}
	}

	return *this;
}

CImage& CImage::operator=(CImage&& _krOther)
{
	// Make sure we are not assigning to ourselves.
	if (this != &_krOther) {
		if (m_pPixels) {
		// Do we already have an existing image?
			delete[] m_pPixels;
		}

		// Steal the other image's data.
		m_pPixels = _krOther.m_pPixels;
		m_iWidth = _krOther.m_iWidth;
		m_iHeight = _krOther.m_iHeight;

		_krOther.m_pPixels = NULL;
		_krOther.m_iWidth = 0;
		_krOther.m_iHeight = 0;
	}
	else {
	// We are assigning to ourselves.
	}

	return *this;
}


void CImage::SetPixel(int _iX, int _iY, TColour _pixel)
{
	if (_iX < 0 || _iX >= m_iWidth || _iY < 0 || _iY > m_iHeight) {
		throw OutOfBounds();
	}

	m_pPixels[_iX + _iY * m_iWidth] = _pixel;
}

TColour CImage::GetPixel(int _iX, int _iY) const
{
	if (_iX < 0 || _iX >= m_iWidth || _iY < 0 || _iY >= m_iHeight) {
		throw OutOfBounds();
	}

	return m_pPixels[_iX + _iY * m_iWidth];
}

TColour* CImage::GetPixels()
{
	return m_pPixels;
}

int CImage::GetWidth() const
{
	return m_iWidth;
}

int CImage::GetHeight() const
{
	return m_iHeight;
}

int CImage::GetSize() const
{
	return m_iWidth * m_iHeight;
}
