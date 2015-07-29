// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Images.h
// Description	: CImage declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __IMAGE_H__
#define __IMAGE_H__

// Library Includes
#include <exception>

// Local Includes
#include "Types.h"

class OutOfBounds : public std::exception {
	virtual const char* what() const throw()
	{
		return "Out of bounds access";
	}
};

class CImage {
	// Member Functions
public:
	CImage(int _iWidth, int _iHeight);
	CImage(const CImage& _krOther);
	CImage(CImage&& _rrOther);

	~CImage();

	CImage& operator=(const CImage& _krOther);
	CImage& operator=(CImage&& _krOther);

	void SetPixel(int _iX, int _iY, TColour _pixel);

	TColour GetPixel(int _iX, int _iY) const;
	TColour* GetPixels();
	int GetWidth() const;
	int GetHeight() const;
	int GetSize() const;
protected:
private:

	// Member Variables
public:
protected:
private:
	TColour* m_pPixels;

	int m_iWidth;
	int m_iHeight;
};

#endif	//__IMAGE_H__