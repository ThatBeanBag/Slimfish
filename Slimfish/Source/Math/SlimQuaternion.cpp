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

const CQuaternion CQuaternion::s_IDENTITY = CQuaternion(0, 0, 0, 1);
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
	float sinTheta = std::sinf(radAngle * 0.5f);

	m_w = std::cosf(radAngle * 0.5f);
	m_x = sinTheta * axis.GetX();
	m_y = sinTheta * axis.GetY();
	m_z = sinTheta * axis.GetZ();
}

CQuaternion::CQuaternion(const CVector3& right, const CVector3& up, const CVector3& forward)
{
	CMatrix4x4 rotation = CMatrix4x4::BuildRotationFromAxis(right, up, forward);
	*this = CQuaternion(rotation);
}

CQuaternion::CQuaternion(const CVector3& forward, const CVector3& up)
{
	CVector3 right = CVector3::CrossProduct(forward, up);
	CVector3 localUp = CVector3::CrossProduct(right, forward);
	*this = CQuaternion(right, localUp, forward);
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

CQuaternion::CQuaternion(float yaw, float pitch, float roll)
{
	*this = /*CQuaternion(CVector3::s_UP, yaw) **/ CQuaternion(CVector3::s_RIGHT, pitch)/* * CQuaternion(CVector3::s_FORWARD, roll)*/;

	// Implementation from:
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/
	float c1 = std::cosf(yaw * 0.5f);
	float s1 = std::sinf(yaw * 0.5f);
	float c2 = std::cosf(roll * 0.5f);
	float s2 = std::sinf(roll * 0.5f);
	float c3 = std::cosf(pitch * 0.5f);
	float s3 = std::sinf(pitch * 0.5f);
	m_w = (c1 * c2 * c3) - (s1 * s2 * s3);
	m_x = (c1 * c2 * s3) + (s1 * s2 * c3);
	m_y = (s1 * c2 * c3) + (c1 * s2 * s3);
	m_z = (c1 * s2 * c3) - (s1 * c2 * s3);
}

CQuaternion::~CQuaternion()
{

}


CQuaternion& CQuaternion::operator*=(const CQuaternion& other)
{
	*this = *this * other;

	return *this;
}

const CVector3 CQuaternion::operator*(const CVector3& vector) const
{
	CVector3 quaternionVector(m_x, m_y, m_z);
	CVector3 qCrossV = CVector3::CrossProduct(quaternionVector, vector);
	CVector3 qCrossVCrossV = CVector3::CrossProduct(quaternionVector, qCrossV);
	qCrossV *= 2.0f * m_w;
	qCrossVCrossV *= 2.0f;

	return (vector + qCrossV + qCrossVCrossV);
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
	return (*this * (1.0f / GetMagnitude()));
}

const CQuaternion CQuaternion::GetConjugate() const
{
	return CQuaternion(-m_x, -m_y, -m_z, m_w);
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
	float xx = m_x + m_x;
	float yy = m_y + m_y;
	float zz = m_z + m_z;

	float f2wx = xx * m_w;
	float f2wy = yy * m_w;
	float f2wz = zz * m_w;
	float f2xx = xx * m_x;
	float f2xy = yy * m_x;
	float f2xz = zz * m_x;
	float f2yy = yy * m_y;
	float f2yz = zz * m_y;
	float f2zz = zz * m_z;

	CMatrix4x4 rotation(CMatrix4x4::s_IDENTITY);

	rotation[0][0] = 1.0f - (f2yy + f2zz);
	rotation[0][1] = f2xy - f2wz;
	rotation[0][2] = f2xz + f2wy;
	rotation[1][0] = f2xy + f2wz;
	rotation[1][1] = 1.0f - (f2xx + f2zz);
	rotation[1][2] = f2yz - f2wx;
	rotation[2][0] = f2xz - f2wy;
	rotation[2][1] = f2yz + f2wx;
	rotation[2][2] = 1.0f - (f2xx + f2yy);

	return rotation;
}

const CVector3 CQuaternion::GetDirection() const
{
	CVector3 direction = *this * CVector3::s_FORWARD;
	return direction;
}

const CVector3 CQuaternion::GetRight() const
{
	CVector3 right = *this * CVector3::s_RIGHT;
	return right;
}

const CVector3 CQuaternion::GetUp() const
{
	CVector3 up = *this * CVector3::s_UP;
	return up;
}

const CQuaternion operator*(float scalar, const CQuaternion& other)
{
	return (other * scalar);
}

const CVector3 operator*(const CVector3& vector, const CQuaternion& rotation)
{
	return (rotation * vector);
}

}