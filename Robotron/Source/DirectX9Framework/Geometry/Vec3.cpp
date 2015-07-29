// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Vec3.cpp
// Description	: CVec3 implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Vec3.h"

// Local Includes

extern const CVec3 g_kvec3Right(1.0f, 0.0f, 0.0f);
extern const CVec3 g_kvec3Up(0.0f, 1.0f, 0.0f);
extern const CVec3 g_kvec3Foward(0.0f, 0.0f, 1.0f);

CVec3::CVec3()
	:D3DXVECTOR3(0.0f, 0.0f, 0.0f)
{

}

CVec3::CVec3(float _fX, float _fY, float _fZ)
	: D3DXVECTOR3(_fX, _fY, _fZ)
{

}

CVec3::CVec3(const D3DXVECTOR3& _krOther)
	: D3DXVECTOR3(_krOther.x, _krOther.y, _krOther.z)
{

}

CVec3::~CVec3()
{

}

CVec3 CVec3::operator*(float _fScale)
{
	CVec3 vec3Scaled = *this;
	vec3Scaled *= _fScale;
	return vec3Scaled;
}

CVec3 CVec3::operator*(const CVec3& _krOther) const
{
	CVec3 vec3Mult = *this;
	vec3Mult *= _krOther;
	return vec3Mult;
}

CVec3& CVec3::operator*=(const CVec3& _krOther)
{
	x *= _krOther.x;
	y *= _krOther.y;
	z *= _krOther.z;

	return *this;
}

CVec3& CVec3::operator*=(float _fScale)
{
	x *= _fScale;
	y *= _fScale;
	z *= _fScale;

	return *this;
}

void CVec3::Normalize()
{
	D3DXVec3Normalize(this, this);
}

std::string CVec3::ToText(int _iPrecision) const
{
	std::string strText;
	strText = ToString(x, _iPrecision) + ", " + ToString(y, _iPrecision) + ", " + ToString(z, _iPrecision);

	return std::move(strText);
}

float CVec3::GetLength() const
{
	return D3DXVec3Length(this);
}

float CVec3::GetSquaredLength()
{
	return D3DXVec3LengthSq(this);
}

float DotProduct(const CVec3& _krvec3A, const CVec3& _krvec3B)
{
	return D3DXVec3Dot(&_krvec3A, &_krvec3B);
}

CVec3 CrossProduct(const CVec3& _krvec3A, const CVec3& _krvec3B)
{
	CVec3 vec3Cross;
	D3DXVec3Cross(&vec3Cross, &_krvec3A, &_krvec3B);

	return vec3Cross;
}

DWORD VectorToDWORD(const CVec3& _krvec3)
{
	DWORD dwR = static_cast<DWORD>(127 * _krvec3.x + 128);
	DWORD dwG = static_cast<DWORD>(127 * _krvec3.y + 128);
	DWORD dwB = static_cast<DWORD>(127 * _krvec3.z + 128);
	return (0xff000000 + (dwR << 16) + (dwG << 8) + dwB);
}

std::ostream& operator<<(std::ostream& _os, const CVec3& _krvec3)
{
	_os << _krvec3.x << " " << _krvec3.y << " " << _krvec3.z;
	return _os;
}

std::istream& operator>>(std::istream& _is, CVec3& _rvec3)
{
	_is >> _rvec3.x >> _rvec3.y >> _rvec3.z;
	return _is;
}
