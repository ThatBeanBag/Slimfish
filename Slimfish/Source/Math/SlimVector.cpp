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

const CVec3 CVec3::s_FORWARD = CVec3(0, 0, 1);
const CVec3 CVec3::s_RIGHT = CVec3(1, 0, 0);
const CVec3 CVec3::s_UP = CVec3(0, 1, 0);

CVec3::CVec3()
	:m_x(0), m_y(0), m_z(0)
{

}

CVec3::CVec3(float x, float y, float z)
	:m_x(x), m_y(y), m_z(z)
{

}

CVec3::~CVec3()
{

}


CVec3& CVec3::operator+=(const CVec3& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;

	return *this;
}

CVec3& CVec3::operator-=(const CVec3& other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;
	m_z -= other.m_z;

	return *this;
}

CVec3& CVec3::operator*=(float scalar)
{
	m_x *= scalar;
	m_y *= scalar;
	m_z *= scalar;

	return *this;
}

CVec3& CVec3::operator+=(float scalar)
{
	m_x += scalar;
	m_y += scalar;
	m_z += scalar;

	return *this;
}

CVec3& CVec3::operator-=(float scalar)
{
	m_x -= scalar;
	m_y -= scalar;
	m_z -= scalar;

	return *this;
}

const CVec3 operator+(const CVec3& vec3A, const CVec3& vec3B)
{
	CVec3 addition = vec3A;
	addition += vec3B;

	return addition;
}

const CVec3 operator+(const CVec3& vec3, float scalar)
{
	CVec3 addition = vec3;
	addition += scalar;

	return addition;
}

const CVec3 operator-(const CVec3& vec3A, const CVec3& vec3B)
{
	CVec3 subtraction = vec3A;
	subtraction -= vec3B;

	return subtraction;
}

const CVec3 operator-(const CVec3& vec3, float scalar)
{
	CVec3 subtraction = vec3;
	subtraction -= scalar;

	return subtraction;
}

const CVec3 operator-(const CVec3& vec3)
{
	CVec3 negated = vec3;
	negated.SetX(-negated.GetX());
	negated.SetY(-negated.GetY());
	negated.SetZ(-negated.GetZ());

	return negated;
}

const CVec3 operator*(const CVec3& vec3, float scalar)
{
	CVec3 scaled = vec3;
	scaled *= scalar;

	return scaled;
}

const CVec3 operator*(float scalar, const CVec3& vec3)
{
	return vec3 * scalar;
}

const CVec3 Normalise(const CVec3& vec3)
{
	float magnitude = vec3.GetLength();

	if (magnitude == 0.0f) {
		return CVec3();
	}
	
	return CVec3(vec3.GetX() / magnitude, vec3.GetY() / magnitude, vec3.GetZ() / magnitude);
}

const CVec3 CrossProduct(const CVec3& vec3A, const CVec3& vec3B)
{
	return CVec3(vec3A.GetY() * vec3B.GetZ() - vec3A.GetZ() * vec3B.GetY(),
				 vec3A.GetZ() * vec3B.GetX() - vec3A.GetX() * vec3B.GetZ(),
				 vec3A.GetX() * vec3B.GetY() - vec3A.GetY() * vec3B.GetX());
}

const float DotProduct(const CVec3& vec3A, const CVec3& vec3B)
{
	return (vec3A.GetX() * vec3B.GetX() + 
			vec3A.GetY() * vec3B.GetY() +
			vec3A.GetZ() * vec3B.GetZ());
}

}
