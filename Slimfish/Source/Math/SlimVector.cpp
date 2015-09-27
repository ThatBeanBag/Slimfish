// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimVector.cpp
// Description	: CSlimVector implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimVector.h"

// Local Includes

namespace Slim {

const CVector3 CVector3::s_FORWARD = CVector3(0, 0, -1);
const CVector3 CVector3::s_RIGHT = CVector3(1, 0, 0);
const CVector3 CVector3::s_UP = CVector3(0, 1, 0);
const CVector3 CVector3::s_ZERO = CVector3(0, 0, 0);
const CVector3 CVector3::s_ONE = CVector3(1.0f, 1.0f, 1.0f);

CVector3::CVector3()
	:m_x(0), m_y(0), m_z(0)
{

}

CVector3::CVector3(float x, float y, float z)
	:m_x(x), m_y(y), m_z(z)
{

}

CVector3::~CVector3()
{

}


CVector3& CVector3::operator+=(const CVector3& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;

	return *this;
}

CVector3& CVector3::operator-=(const CVector3& other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;
	m_z -= other.m_z;

	return *this;
}

CVector3& CVector3::operator*=(float scalar)
{
	m_x *= scalar;
	m_y *= scalar;
	m_z *= scalar;

	return *this;
}

CVector3& CVector3::operator+=(float scalar)
{
	m_x += scalar;
	m_y += scalar;
	m_z += scalar;

	return *this;
}

CVector3& CVector3::operator-=(float scalar)
{
	m_x -= scalar;
	m_y -= scalar;
	m_z -= scalar;

	return *this;
}

const CVector3 CVector3::Normalise(const CVector3& vec3)
{
	float magnitude = vec3.GetLength();

	if (magnitude == 0.0f) {
		return CVector3();
	}
	
	return CVector3(vec3.GetX() / magnitude, vec3.GetY() / magnitude, vec3.GetZ() / magnitude);
}

const CVector3 CVector3::CrossProduct(const CVector3& vec3A, const CVector3& vec3B)
{
	return CVector3(vec3A.GetY() * vec3B.GetZ() - vec3A.GetZ() * vec3B.GetY(),
				    vec3A.GetZ() * vec3B.GetX() - vec3A.GetX() * vec3B.GetZ(),
				    vec3A.GetX() * vec3B.GetY() - vec3A.GetY() * vec3B.GetX());
}

const float CVector3::DotProduct(const CVector3& vec3A, const CVector3& vec3B)
{
	return (vec3A.GetX() * vec3B.GetX() + 
			vec3A.GetY() * vec3B.GetY() +
			vec3A.GetZ() * vec3B.GetZ());
}

const CVector3 operator+(const CVector3& vec3A, const CVector3& vec3B)
{
	CVector3 addition = vec3A;
	addition += vec3B;

	return addition;
}

const CVector3 operator+(const CVector3& vec3, float scalar)
{
	CVector3 addition = vec3;
	addition += scalar;

	return addition;
}

const CVector3 operator-(const CVector3& vec3A, const CVector3& vec3B)
{
	CVector3 subtraction = vec3A;
	subtraction -= vec3B;

	return subtraction;
}

const CVector3 operator-(const CVector3& vec3, float scalar)
{
	CVector3 subtraction = vec3;
	subtraction -= scalar;

	return subtraction;
}

const CVector3 operator-(const CVector3& vec3)
{
	CVector3 negated = vec3;
	negated.SetX(-negated.GetX());
	negated.SetY(-negated.GetY());
	negated.SetZ(-negated.GetZ());

	return negated;
}

const CVector3 operator*(const CVector3& vec3, float scalar)
{
	CVector3 scaled = vec3;
	scaled *= scalar;

	return scaled;
}

const CVector3 operator*(float scalar, const CVector3& vec3)
{
	return vec3 * scalar;
}


}
