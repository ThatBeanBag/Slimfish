// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Vec3.h
// Description	: CVec3 declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __VEC3_H__
#define __VEC3_H__

// Library Includes
#include <d3dx9.h>

// Local Includes

class CVec3 : public D3DXVECTOR3 {
	// Member Functions
public:
	CVec3();
	CVec3(float _fX, float _fY, float _fZ);
	CVec3(const D3DXVECTOR3& _krOther);

	~CVec3();

	CVec3 operator*(float _fScale);
	CVec3 operator*(const CVec3& _krOther) const;

	CVec3& operator*=(const CVec3& _krOther);
	CVec3& operator*=(float _fScale);

	void Normalize();

	std::string ToText(int _iPrecision = g_kiDEF_PRECISION) const;

	float GetLength() const;
	float GetSquaredLength();
protected:
private:
	// Member Variables
public:
protected:
private:
};

float DotProduct(const CVec3& _krvec3A, const CVec3& _krvec3B);
CVec3 CrossProduct(const CVec3& _krvec3A, const CVec3& _krvec3B);
DWORD VectorToDWORD(const CVec3& _krvec3);

extern const CVec3 g_kvec3Right;
extern const CVec3 g_kvec3Up;
extern const CVec3 g_kvec3Foward;

std::ostream& operator<<(std::ostream& _os, const CVec3& _krvec3);
std::istream& operator>>(std::istream& _is, CVec3& _rvec3);

#endif	// __VEC3_H__