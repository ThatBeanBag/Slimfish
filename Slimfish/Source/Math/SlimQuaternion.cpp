// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimQuaternion.cpp
// Description	: CSlimQuaternion implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes
#include <math.h>

// This Include
#include "SlimQuaternion.h"

// Local Includes

namespace Slim {

CQuaternion::CQuaternion()
	:m_w(1.0f), m_i(0.0f), m_j(0.0f), m_k(0.0f)
{

}

CQuaternion::CQuaternion(float real, float i, float j, float k)
	:m_w(real), m_i(i), m_j(j), m_k(k)
{

}

CQuaternion::CQuaternion(const CVec3& axis, float radAngle)
{
	float sinTheta = sinf(radAngle / 2.0f);

	m_w = cosf(radAngle / 2.0f);
	m_i = sinTheta * axis.GetX();
	m_j = sinTheta * axis.GetY();
	m_k = sinTheta * axis.GetZ();
}

CQuaternion::CQuaternion(const CVec3& right, const CVec3& up, const CVec3& forward)
{

}

CQuaternion::~CQuaternion()
{

}


CQuaternion& CQuaternion::operator*=(const CQuaternion& other)
{
	*this = *this * other;

	return *this;
}

CQuaternion& CQuaternion::operator+=(const CQuaternion& other)
{
	*this = *this + other;

	return *this;
}

CQuaternion& CQuaternion::operator-=(const CQuaternion& other)
{
	*this = *this - other;

	return *this;
}

CQuaternion& CQuaternion::operator*=(float scalar)
{
	*this = *this * scalar;

	return *this;
}

bool CQuaternion::operator==(const CQuaternion& other)
{
	return (m_w == other.m_w &&
			m_i == other.m_i &&
			m_j == other.m_j &&
			m_k == other.m_k);
}

bool CQuaternion::operator!=(const CQuaternion& other)
{
	return !(*this == other);
}

const CQuaternion CQuaternion::operator*(const CQuaternion& other) const
{
	return CQuaternion(m_w * other.m_w - m_i * other.m_i - m_j * other.m_j - m_k * other.m_k,
					   m_w * other.m_i + m_i * other.m_w + m_j * other.m_k - m_k * other.m_j,
					   m_w * other.m_j + m_j * other.m_w + m_k * other.m_i - m_i * other.m_k,
					   m_w * other.m_k + m_k * other.m_w + m_w * other.m_j - m_j * other.m_i);
}

const CQuaternion CQuaternion::operator+(const CQuaternion& other) const
{
	return CQuaternion(m_w + other.m_w, 
					   m_i + other.m_i, 
					   m_j + other.m_j, 
					   m_k + other.m_k);
}

const CQuaternion CQuaternion::operator-(const CQuaternion& other) const
{
	return CQuaternion(m_w - other.m_w,
					   m_i - other.m_i,
					   m_j - other.m_j,
					   m_k - other.m_k);
}

const CQuaternion CQuaternion::operator*(float scalar) const
{
	return CQuaternion(scalar * m_w, scalar * m_i, scalar * m_j, scalar * m_k);
}

const CQuaternion CQuaternion::GetNormalise() const
{
	return *this * (1.0f / GetMagnitude());
}

const CQuaternion CQuaternion::GetConjugate() const
{
	return CQuaternion(m_w, -m_i, -m_j, -m_k);
}

const CQuaternion CQuaternion::GetInverse() const
{
	// Calculate inverse using conjugate / (magnitude) ^ 2.
	float magnitudeSquared = m_w * m_w + m_i * m_i + m_j * m_j + m_k * m_k;

	if (magnitudeSquared != 0) {
		return GetConjugate() * (1.0f / magnitudeSquared);
	}

	return *this;
}

const float CQuaternion::GetMagnitude() const
{
	return sqrt(m_w * m_w + m_i * m_i + m_j * m_j + m_k * m_k);
}

const CMatrix4x4 CQuaternion::ToRotationMatrix() const
{
	float f2i = m_i + m_i;
	float f2j = m_j + m_j;
	float f2k = m_k + m_k;

	float f2wi = f2i * m_w;
	float f2wj = f2j * m_w;
	float f2wk = f2k * m_w;
	float f2ii = f2i * m_i;
	float f2ij = f2j * m_i;
	float f2ik = f2k * m_i;
	float f2jj = f2j * m_j;
	float f2jk = f2k * m_j;
	float f2kk = f2k * m_k;

	CMatrix4x4 rotation(CMatrix4x4::s_IDENTITY);

	rotation[0][0] = 1.0f - f2jj - f2kk;
	rotation[0][1] = f2ij - f2wk;
	rotation[0][2] = f2ik + f2wj;
	rotation[1][0] = f2ij + f2wk;
	rotation[1][1] = 1.0f - f2ii - f2kk;
	rotation[1][2] = f2jk - f2wi;
	rotation[2][0] = f2ik - f2wj;
	rotation[2][1] = f2jk - f2wi;
	rotation[2][2] = 1.0f - f2ii - f2jj;

	return rotation;
}

const CQuaternion operator*(float scalar, const CQuaternion& other)
{
	return (other * scalar);
}

}