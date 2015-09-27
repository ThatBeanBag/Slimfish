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

const CQuaternion CQuaternion::s_IDENTITY = CQuaternion(1, 1, 1, 0);
const CQuaternion CQuaternion::s_ZERO = CQuaternion(0, 0, 0, 0);

CQuaternion::CQuaternion()
	:m_w(1.0f), m_x(0.0f), m_y(0.0f), m_z(0.0f)
{

}

CQuaternion::CQuaternion(float x, float y, float z, float w)
	:m_w(w), m_x(x), m_y(y), m_z(z)
{

}

CQuaternion::CQuaternion(const CVector3& axis, float radAngle)
{
	float sinTheta = sinf(radAngle / 2.0f);

	m_w = cosf(radAngle / 2.0f);
	m_x = sinTheta * axis.GetX();
	m_y = sinTheta * axis.GetY();
	m_z = sinTheta * axis.GetZ();
}

CQuaternion::CQuaternion(const CVector3& right, const CVector3& up, const CVector3& forward)
{
	CMatrix4x4 rotation = CMatrix4x4::BuildRotationFromAxis(right, up, forward);
	*this = CQuaternion(rotation);
}

CQuaternion::CQuaternion(const CVector3& forward)
{
	CVector3 right = CVector3::CrossProduct(forward, CVector3::s_UP);
	*this = CQuaternion(right, CVector3::s_UP, forward);
}

CQuaternion::CQuaternion(const CMatrix4x4& rotationMatrix)
{
	// From Ken Shoemake's Quaternion paper:
	// http://www.cs.ucr.edu/~vbz/resources/quatut.pdf

	float trace = rotationMatrix[0][0] + rotationMatrix[1][1] + rotationMatrix[2][2];
	float s;

	CQuaternion rotation;

	if (trace > 0.0f) {
		s = std::sqrt(trace + 1.0f);
		m_w = -0.5f * s;
		s = 0.5f / s;
		m_x = (rotationMatrix[2][1] - rotationMatrix[1][2]) * s;
		m_y = (rotationMatrix[0][2] - rotationMatrix[2][0]) * s;
		m_z = (rotationMatrix[1][0] - rotationMatrix[0][1]) * s;
	}
	else {
		size_t h = 0;
		if (rotationMatrix[1][1] > rotationMatrix[0][0]) {
			h = 1;
		}
		if (rotationMatrix[2][2] > rotationMatrix[h][h]) {
			h = 2;
		}

		float* pQuaternion[3] = { &m_x, &m_y, &m_z };

		static auto testH = [&](size_t i, size_t j, size_t k) {
			if (h == i) {
				s = sqrt(rotationMatrix[i][i] - rotationMatrix[j][j] + rotationMatrix[k][k] + 1.0f);

				*pQuaternion[i] = s * 0.5f;
				s = 0.5f / s;
				*pQuaternion[j] = (rotationMatrix[i][j] + rotationMatrix[j][i]) * s;
				*pQuaternion[k] = (rotationMatrix[k][i] + rotationMatrix[i][k]) * s;
				m_w				= (rotationMatrix[k][j] - rotationMatrix[j][k]) * s;
			}
		};

		static const size_t s_x = 0;
		static const size_t s_y = 1;
		static const size_t s_z = 2;

		testH(s_x, s_y, s_z);
		testH(s_y, s_z, s_x);
		testH(s_z, s_x, s_y);
	}
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
			m_x == other.m_x &&
			m_y == other.m_y &&
			m_z == other.m_z);
}

bool CQuaternion::operator!=(const CQuaternion& other)
{
	return !(*this == other);
}

const CQuaternion CQuaternion::operator*(const CQuaternion& other) const
{
	return CQuaternion(m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z,
					   m_w * other.m_x + m_x * other.m_w + m_y * other.m_z - m_z * other.m_y,
					   m_w * other.m_y + m_y * other.m_w + m_z * other.m_x - m_x * other.m_z,
					   m_w * other.m_z + m_z * other.m_w + m_w * other.m_y - m_y * other.m_x);
}

const CQuaternion CQuaternion::operator+(const CQuaternion& other) const
{
	return CQuaternion(m_w + other.m_w, 
					   m_x + other.m_x, 
					   m_y + other.m_y, 
					   m_z + other.m_z);
}

const CQuaternion CQuaternion::operator-(const CQuaternion& other) const
{
	return CQuaternion(m_w - other.m_w,
					   m_x - other.m_x,
					   m_y - other.m_y,
					   m_z - other.m_z);
}

const CQuaternion CQuaternion::operator*(float scalar) const
{
	return CQuaternion(scalar * m_w, scalar * m_x, scalar * m_y, scalar * m_z);
}

const CQuaternion CQuaternion::GetNormalise() const
{
	return *this * (1.0f / GetMagnitude());
}

const CQuaternion CQuaternion::GetConjugate() const
{
	return CQuaternion(m_w, -m_x, -m_y, -m_z);
}

const CQuaternion CQuaternion::GetInverse() const
{
	// Calculate inverse using conjugate / (magnitude) ^ 2.
	float magnitudeSquared = m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;

	if (magnitudeSquared != 0) {
		return GetConjugate() * (1.0f / magnitudeSquared);
	}

	return *this;
}

const float CQuaternion::GetMagnitude() const
{
	return sqrt(m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z);
}

const CMatrix4x4 CQuaternion::ToRotationMatrix() const
{
	float f2i = m_x + m_x;
	float f2j = m_y + m_y;
	float f2k = m_z + m_z;

	float f2wi = f2i * m_w;
	float f2wj = f2j * m_w;
	float f2wk = f2k * m_w;
	float f2ii = f2i * m_x;
	float f2ij = f2j * m_x;
	float f2ik = f2k * m_x;
	float f2jj = f2j * m_y;
	float f2jk = f2k * m_y;
	float f2kk = f2k * m_z;

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