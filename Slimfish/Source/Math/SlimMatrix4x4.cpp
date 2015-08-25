// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimMatrix4x4.cpp
// Description	: CMatrix4x4 implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes
#include <memory.h>
#include <math.h>

// This Include
#include "SlimMatrix4x4.h"

// Local Includes

namespace Slim {

const CMatrix4x4 CMatrix4x4::s_IDENTITY = { 1.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f,
											0.0f, 0.0f, 0.0f, 1.0f };

const CMatrix4x4 CMatrix4x4::s_ZERO = { 0.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 0.0f };

CMatrix4x4::CMatrix4x4()
{
	// Set all elements of the matrix to 0.
	memset(m_pMatrix, 0, sizeof(m_pMatrix));
}

CMatrix4x4::CMatrix4x4(float pMatrix[4][4])
{
	Copy(pMatrix);
}

CMatrix4x4::CMatrix4x4(float v11, float v12, float v13, float v14, 
					   float v21, float v22, float v23, float v24, 
					   float v31, float v32, float v33, float v34, 
					   float v41, float v42, float v43, float v44)
{
	m_pMatrix[0][0] = v11;
	m_pMatrix[0][1] = v12;
	m_pMatrix[0][2] = v13;
	m_pMatrix[0][3] = v14;

	m_pMatrix[1][0] = v21;
	m_pMatrix[1][1] = v22;
	m_pMatrix[1][2] = v23;
	m_pMatrix[1][3] = v24;

	m_pMatrix[2][0] = v31;
	m_pMatrix[2][1] = v32;
	m_pMatrix[2][2] = v33;
	m_pMatrix[2][3] = v34;

	m_pMatrix[3][0] = v41;
	m_pMatrix[3][1] = v42;
	m_pMatrix[3][2] = v43;
	m_pMatrix[3][3] = v44;
}

CMatrix4x4::CMatrix4x4(const CMatrix4x4& other)
{
	Copy(other.m_pMatrix);
}

CMatrix4x4::~CMatrix4x4()
{

}

CMatrix4x4& CMatrix4x4::operator=(const CMatrix4x4& other)
{
	Copy(other.m_pMatrix);

	return *this;
}

CMatrix4x4& CMatrix4x4::operator*=(const CMatrix4x4& other)
{
	*this = *this * other;

	return *this;
}

CMatrix4x4& CMatrix4x4::operator+=(const CMatrix4x4& other)
{
	*this = *this + other;

	return *this;
}

CMatrix4x4& CMatrix4x4::operator-=(const CMatrix4x4& other)
{
	*this = *this - other;

	return *this;
}

const CVector3 CMatrix4x4::Transform(const CVector3& vector)
{
	CVector3 result(0.0f, 0.0f, 0.0f);
	float w = 0.0f;

	// Loop through row by row to minimize cache misses.
	for (int i = 0; i < 4; ++i) {
		int row = i * 4;
		result.SetX(result.GetX() + vector.GetX() * m_pMatrix[row][0]);
		result.SetY(result.GetY() + vector.GetX() * m_pMatrix[row][1]);
		result.SetZ(result.GetZ() + vector.GetX() * m_pMatrix[row][2]);
		w += m_pMatrix[row][3];
	}

	if (w != 0.0f && w != 1.0f) {
		result.SetX(result.GetX() / w);
		result.SetY(result.GetY() / w);
		result.SetZ(result.GetZ() / w);
	}

	return result;
}

/************************************************************************/
/* Accessor Methods
/************************************************************************/

const float* CMatrix4x4::operator[](size_t row) const
{
	assert(row >= 0 && row < 4);

	return m_pMatrix[row];
}

float* CMatrix4x4::operator[](size_t row)
{
	// Call const version of operator[] then cast off the constness of
	// the result to avoid code duplication.
	return const_cast<float*>(static_cast<const CMatrix4x4&>(*this)[row]);
}

void CMatrix4x4::SetElements(float pMatrix[4][4])
{
	Copy(pMatrix);
}

void CMatrix4x4::SetPosition(float x, float y, float z)
{
	m_pMatrix[0][3] = x;
	m_pMatrix[1][3] = y;
	m_pMatrix[2][3] = z;
}

void CMatrix4x4::SetPosition(const CVector3& position)
{
	SetPosition(position.GetX(), position.GetY(), position.GetZ());
}

void CMatrix4x4::SetScale(const CVector3& scale)
{
	SetScale(scale.GetX(), scale.GetY(), scale.GetZ());
}

void CMatrix4x4::SetScale(float xScale, float yScale, float zScale)
{
	m_pMatrix[0][0] = xScale;
	m_pMatrix[1][1] = yScale;
	m_pMatrix[2][2] = zScale;
}

const CMatrix4x4 CMatrix4x4::GetTranspose() const
{
	CMatrix4x4 transpose;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			transpose[i][j] = m_pMatrix[j][i];
		}
	}

	return transpose;
}

const CMatrix4x4 CMatrix4x4::GetInverse() const
{
	CMatrix4x4 inverse;

	inverse[0][0] = m_pMatrix[1][1]	* m_pMatrix[2][2]	* m_pMatrix[3][3] - 
					m_pMatrix[1][1]	* m_pMatrix[2][3]	* m_pMatrix[3][2] -
					m_pMatrix[2][1]	* m_pMatrix[1][2]	* m_pMatrix[3][3] +
					m_pMatrix[2][1]	* m_pMatrix[1][3]	* m_pMatrix[3][2] +
					m_pMatrix[3][1]	* m_pMatrix[1][2]	* m_pMatrix[2][3] -
					m_pMatrix[3][1]	* m_pMatrix[1][3]	* m_pMatrix[2][2];

	inverse[1][0] = -m_pMatrix[1][0]	* m_pMatrix[2][2]	* m_pMatrix[3][3] +
					 m_pMatrix[1][0]	* m_pMatrix[2][3]	* m_pMatrix[3][2] +
					 m_pMatrix[2][0]	* m_pMatrix[1][2]	* m_pMatrix[3][3] -
					 m_pMatrix[2][0]	* m_pMatrix[1][3]	* m_pMatrix[3][2] -
					 m_pMatrix[3][0]	* m_pMatrix[1][2]	* m_pMatrix[2][3] +
					 m_pMatrix[3][0]	* m_pMatrix[1][3]	* m_pMatrix[2][2];

	inverse[2][0] = m_pMatrix[1][0]	* m_pMatrix[2][1]	* m_pMatrix[3][3] -
					m_pMatrix[1][0]	* m_pMatrix[2][3]	* m_pMatrix[3][1] -
					m_pMatrix[2][0]	* m_pMatrix[1][1]	* m_pMatrix[3][3] +
					m_pMatrix[2][0]	* m_pMatrix[1][3]	* m_pMatrix[3][1] +
					m_pMatrix[3][0]	* m_pMatrix[1][1]	* m_pMatrix[2][3] -
					m_pMatrix[3][0]	* m_pMatrix[1][3]	* m_pMatrix[2][1];

	inverse[3][0] = -m_pMatrix[1][0]	* m_pMatrix[2][1]	* m_pMatrix[3][2] +
					 m_pMatrix[1][0]	* m_pMatrix[2][2]	* m_pMatrix[3][1] +
					 m_pMatrix[2][0]	* m_pMatrix[1][1]	* m_pMatrix[3][2] -
					 m_pMatrix[2][0]	* m_pMatrix[1][2]	* m_pMatrix[3][1] -
					 m_pMatrix[3][0]	* m_pMatrix[1][1]	* m_pMatrix[2][2] +
					 m_pMatrix[3][0]	* m_pMatrix[1][2]	* m_pMatrix[2][1];

	inverse[0][1] = -m_pMatrix[0][1]	* m_pMatrix[2][2]	* m_pMatrix[3][3] +
					 m_pMatrix[0][1]	* m_pMatrix[2][3]	* m_pMatrix[3][2] +
					 m_pMatrix[2][1]	* m_pMatrix[0][2]	* m_pMatrix[3][3] -
					 m_pMatrix[2][1]	* m_pMatrix[0][3]	* m_pMatrix[3][2] -
					 m_pMatrix[3][1]	* m_pMatrix[0][2]	* m_pMatrix[2][3] +
					 m_pMatrix[3][1]	* m_pMatrix[0][3]	* m_pMatrix[2][2];

	inverse[1][1] = m_pMatrix[0][0]	* m_pMatrix[2][2]	* m_pMatrix[3][3] -
					m_pMatrix[0][0]	* m_pMatrix[2][3]	* m_pMatrix[3][2] -
					m_pMatrix[2][0]	* m_pMatrix[0][2]	* m_pMatrix[3][3] +
					m_pMatrix[2][0]	* m_pMatrix[0][3]	* m_pMatrix[3][2] +
					m_pMatrix[3][0]	* m_pMatrix[0][2]	* m_pMatrix[2][3] -
					m_pMatrix[3][0]	* m_pMatrix[0][3]	* m_pMatrix[2][2];

	inverse[2][1] = -m_pMatrix[0][0]	* m_pMatrix[2][1]	* m_pMatrix[3][3] +
					 m_pMatrix[0][0]	* m_pMatrix[2][3]	* m_pMatrix[3][1] +
					 m_pMatrix[2][0]	* m_pMatrix[0][1]	* m_pMatrix[3][3] -
					 m_pMatrix[2][0]	* m_pMatrix[0][3]	* m_pMatrix[3][1] -
					 m_pMatrix[3][0]	* m_pMatrix[0][1]	* m_pMatrix[2][3] +
					 m_pMatrix[3][0]	* m_pMatrix[0][3]	* m_pMatrix[2][1];

	inverse[3][1] = m_pMatrix[0][0] * m_pMatrix[2][1]	* m_pMatrix[3][2] -
					m_pMatrix[0][0] * m_pMatrix[2][2]	* m_pMatrix[3][1] -
				    m_pMatrix[2][0]	* m_pMatrix[0][1]	* m_pMatrix[3][2] +
				    m_pMatrix[2][0]	* m_pMatrix[0][2]	* m_pMatrix[3][1] +
				    m_pMatrix[3][0]	* m_pMatrix[0][1]	* m_pMatrix[2][2] -
				    m_pMatrix[3][0]	* m_pMatrix[0][2]	* m_pMatrix[2][1];

	inverse[0][2] = m_pMatrix[0][1]	* m_pMatrix[1][2]	* m_pMatrix[3][3] -
					m_pMatrix[0][1]	* m_pMatrix[1][3]	* m_pMatrix[3][2] -
					m_pMatrix[1][1]	* m_pMatrix[0][2]	* m_pMatrix[3][3] +
					m_pMatrix[1][1]	* m_pMatrix[0][3]	* m_pMatrix[3][2] +
					m_pMatrix[3][1]	* m_pMatrix[0][2]	* m_pMatrix[1][3] -
					m_pMatrix[3][1]	* m_pMatrix[0][3]	* m_pMatrix[1][2];

	inverse[1][2] = -m_pMatrix[0][0]	* m_pMatrix[1][2]	* m_pMatrix[3][3] +
					 m_pMatrix[0][0]	* m_pMatrix[1][3]	* m_pMatrix[3][2] +
					 m_pMatrix[1][0]	* m_pMatrix[0][2]	* m_pMatrix[3][3] -
					 m_pMatrix[1][0]	* m_pMatrix[0][3]	* m_pMatrix[3][2] -
					 m_pMatrix[3][0]	* m_pMatrix[0][2]	* m_pMatrix[1][3] +
					 m_pMatrix[3][0]	* m_pMatrix[0][3]	* m_pMatrix[1][2];

	inverse[2][2] = m_pMatrix[0][0]	* m_pMatrix[1][1]	* m_pMatrix[3][3] -
					m_pMatrix[0][0]	* m_pMatrix[1][3]	* m_pMatrix[3][1] -
					m_pMatrix[1][0]	* m_pMatrix[0][1]	* m_pMatrix[3][3] +
					m_pMatrix[1][0]	* m_pMatrix[0][3]	* m_pMatrix[3][1] +
					m_pMatrix[3][0] * m_pMatrix[0][1]	* m_pMatrix[1][3] -
					m_pMatrix[3][0] * m_pMatrix[0][3]	* m_pMatrix[1][1];

	inverse[3][2] = -m_pMatrix[0][0]	* m_pMatrix[1][1] * m_pMatrix[3][2] +
					 m_pMatrix[0][0]	* m_pMatrix[1][2] * m_pMatrix[3][1] +
					 m_pMatrix[1][0]	* m_pMatrix[0][1] * m_pMatrix[3][2] -
					 m_pMatrix[1][0]	* m_pMatrix[0][2] * m_pMatrix[3][1] -
					 m_pMatrix[3][0]	* m_pMatrix[0][1] * m_pMatrix[1][2] +
					 m_pMatrix[3][0]	* m_pMatrix[0][2] * m_pMatrix[1][1];

	inverse[0][3] =-m_pMatrix[0][1]	* m_pMatrix[1][2] * m_pMatrix[2][3] +
				  m_pMatrix[0][1]	* m_pMatrix[1][3] * m_pMatrix[2][2] +
				  m_pMatrix[1][1]	* m_pMatrix[0][2]* m_pMatrix[2][3] -
				  m_pMatrix[1][1]	* m_pMatrix[0][3]* m_pMatrix[2][2] -
				  m_pMatrix[2][1]	* m_pMatrix[0][2]* m_pMatrix[1][3]  +
				  m_pMatrix[2][1]	* m_pMatrix[0][3]* m_pMatrix[1][2];

	inverse[1][3] = m_pMatrix[0][0]	* m_pMatrix[1][2]	* m_pMatrix[2][3] -
					m_pMatrix[0][0]	* m_pMatrix[1][3]	* m_pMatrix[2][2] -
					m_pMatrix[1][0]	* m_pMatrix[0][2]	* m_pMatrix[2][3] +
					m_pMatrix[1][0]	* m_pMatrix[0][3]	* m_pMatrix[2][2] +
					m_pMatrix[2][0]	* m_pMatrix[0][2]	* m_pMatrix[1][3]  -
					m_pMatrix[2][0]	* m_pMatrix[0][3]	* m_pMatrix[1][2];

	inverse[2][3] = -m_pMatrix[0][0] * m_pMatrix[1][1] * m_pMatrix[2][3] +
					 m_pMatrix[0][0] * m_pMatrix[1][3] * m_pMatrix[2][1] +
					 m_pMatrix[1][0] * m_pMatrix[0][1] * m_pMatrix[2][3] -
					 m_pMatrix[1][0] * m_pMatrix[0][3] * m_pMatrix[2][1] -
					 m_pMatrix[2][0] * m_pMatrix[0][1] * m_pMatrix[1][3] +
					 m_pMatrix[2][0] * m_pMatrix[0][3] * m_pMatrix[1][1];

	inverse[3][3] = m_pMatrix[0][0]	* m_pMatrix[1][1]	* m_pMatrix[2][2] -
					m_pMatrix[0][0]	* m_pMatrix[1][2]	* m_pMatrix[2][1] -
					m_pMatrix[1][0]	* m_pMatrix[0][1]	* m_pMatrix[2][2] +
					m_pMatrix[1][0]	* m_pMatrix[0][2]	* m_pMatrix[2][1] +
					m_pMatrix[2][0]	* m_pMatrix[0][1]	* m_pMatrix[1][2] -
					m_pMatrix[2][0]	* m_pMatrix[0][2]	* m_pMatrix[1][1];

	// Calculate the determinant.
	float determinant = (m_pMatrix[0][0] * inverse[0][0]) + 
						(m_pMatrix[0][1] * inverse[1][0]) + 
						(m_pMatrix[0][2] * inverse[2][0]) + 
						(m_pMatrix[0][3] * inverse[3][0]);

	if (determinant == 0) {
	// Is the matrix invertible?
		return s_IDENTITY;
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			inverse[i][j] /= determinant;
		}
	}

	return inverse;
}

const CVector3 CMatrix4x4::GetPosition() const
{
	// Return the forth column of the matrix which is just the translational information.
	return CVector3(m_pMatrix[0][3], m_pMatrix[1][3], m_pMatrix[2][3]);
}

const CVector3 CMatrix4x4::GetDirection() const
{
	// Return third column of the rotation component of the matrix.
	return CVector3(m_pMatrix[0][2], m_pMatrix[1][2], m_pMatrix[2][2]);
}

const CVector3 CMatrix4x4::GetRight() const 
{
	// Return first column of the rotation component of the matrix.
	return CVector3(m_pMatrix[0][0], m_pMatrix[1][0], m_pMatrix[2][0]);
}

const CVector3 CMatrix4x4::GetUp() const
{
	// Return second column of the rotation component of the matrix.
	return CVector3(m_pMatrix[0][1], m_pMatrix[1][1], m_pMatrix[2][1]);
}

void CMatrix4x4::Copy(const float pMatrix[4][4])
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m_pMatrix[i][j] = pMatrix[i][j];
		}
	}
}

/************************************************************************/
/* Arithmetic operators
/************************************************************************/

const CMatrix4x4 operator*(const CMatrix4x4& matrixA, const CMatrix4x4& matrixB)
{
	CMatrix4x4 matrixC;	// Resultant matrix.
	float columnB[4];	// Temporary array for the columns of matrixB.

	for (int i = 0; i < 4; ++i) {
		// Get the ith column of matrixB.
		for (int j = 0; j < 4; ++j) {
			columnB[j] = matrixB[j][i];
		}

		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				// Multiply the row of matrixA by the column of matrixB.
				matrixC[j][i] += matrixA[j][k] * columnB[k];
			}
		}
	}

	return matrixC;
}

const CMatrix4x4 operator+(const CMatrix4x4& matrixA, const CMatrix4x4& matrixB)
{
	CMatrix4x4 matrixC;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			matrixC[i][j] = matrixA[i][j] + matrixB[i][j];
		}
	}

	return matrixC;
}

const CMatrix4x4 operator-(const CMatrix4x4& matrixA, const CMatrix4x4& matrixB)
{
	CMatrix4x4 matrixC;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			matrixC[i][j] = matrixA[i][j] - matrixB[i][j];
		}
	}

	return matrixC;
}

/************************************************************************/
/* Helper Build Functions
/************************************************************************/

const CMatrix4x4 CMatrix4x4::BuildScale(float xScale, float yScale, float zScale)
{
	return BuildScale(CVector3(xScale, yScale, zScale));
}

const CMatrix4x4 CMatrix4x4::BuildScale(const CVector3& scale)
{
	CMatrix4x4 scaleMatrix = CMatrix4x4::s_IDENTITY;
	scaleMatrix.SetScale(scale);

	return scaleMatrix;
}

const CMatrix4x4 CMatrix4x4::BuildRotationX(float radians)
{
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);
	CMatrix4x4 rotation = CMatrix4x4::s_IDENTITY;

	// Rx =  1       0       0		0
	//       0     cos(t) -sin(t)	0
	//       0     sin(t)  cos(t)	0
	//	     0		 0		 0		1
	rotation[1][1] = cosTheta;
	rotation[1][2] = -sinTheta;
	rotation[2][1] = sinTheta;
	rotation[2][2] = cosTheta;

	return rotation;
}

const CMatrix4x4 CMatrix4x4::BuildRotationY(float radians)
{
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);
	CMatrix4x4 rotation = CMatrix4x4::s_IDENTITY;

	// Ry = cos(t)   0    sin(t)	0
	//       0       1	   0		0
	//	   -sin(t)	 0    cos(t)	0
	//		 0		 0	   0		1
	rotation[0][0] = cosTheta;
	rotation[0][2] = sinTheta;
	rotation[2][0] = -sinTheta;
	rotation[2][2] = cosTheta;

	return rotation;
}

const CMatrix4x4 CMatrix4x4::BuildRotationZ(float radians)
{
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);
	CMatrix4x4 rotation = CMatrix4x4::s_IDENTITY;

	// Rz = cos(t) -sin(t)	0		0
	//      sin(t)  cos(t)	0		0
	//		 0		0		1	    0
	//		 0		0		0		1
	rotation[0][0] = cosTheta;
	rotation[0][1] = -sinTheta;
	rotation[1][0] = sinTheta;
	rotation[1][1] = cosTheta;

	return rotation;
}

const CMatrix4x4 CMatrix4x4::BuildRotationFromAxis(const CVector3& right, const CVector3& up, const CVector3& forward)
{
	CMatrix4x4 matrix = CMatrix4x4::s_IDENTITY;

	matrix[0][0] = right.GetX();
	matrix[0][1] = right.GetY();
	matrix[0][2] = right.GetZ();

	matrix[1][0] = up.GetX();
	matrix[1][1] = up.GetY();
	matrix[1][2] = up.GetZ();

	matrix[2][0] = forward.GetX();
	matrix[2][1] = forward.GetY();
	matrix[2][2] = forward.GetZ();

	return matrix;
}

const CMatrix4x4 CMatrix4x4::BuildYawPitchRoll(float radYaw, float radPitch, float radRoll)
{
	return BuildRotationY(radYaw) * BuildRotationX(radPitch) * BuildRotationZ(radRoll);
}

const CMatrix4x4 CMatrix4x4::BuildTranslation(float x, float y, float z)
{
	return BuildTranslation(CVector3(x, y, z));
}

const CMatrix4x4 CMatrix4x4::BuildTranslation(const CVector3& position)
{
	CMatrix4x4 translation(CMatrix4x4::s_IDENTITY);
	translation.SetPosition(position);
	
	return translation;
}

const CMatrix4x4 CMatrix4x4::BuildLookAt(const CVector3& eye, const CVector3& at, const CVector3& up)
{
	CVector3 zAxis = Normalise(at - eye);
	CVector3 xAxis = Normalise(CrossProduct(CVector3::s_UP, zAxis));
	CVector3 yAxis = CrossProduct(zAxis, xAxis);

	CMatrix4x4 matrix = BuildRotationFromAxis(xAxis, yAxis, zAxis);

	matrix[0][3] = -DotProduct(xAxis, eye);
	matrix[1][3] = -DotProduct(yAxis, eye);
	matrix[2][3] = -DotProduct(zAxis, eye);

	return matrix;
}

const CMatrix4x4 CMatrix4x4::BuildProjection(float radFOV, float aspectRatio, float nearPlane, float farPlane)
{
	float yScale = tan(radFOV * 0.5f);
	if (yScale == 0) {
		return CMatrix4x4::s_IDENTITY;
	}

	yScale = 1.0f / yScale;
	float xScale = yScale / aspectRatio;

	CMatrix4x4 projection;

	projection[0][0] = xScale;
	projection[1][1] = yScale;
	projection[2][2] = farPlane / (farPlane - nearPlane);
	projection[2][3] = -nearPlane * projection[2][2];
	projection[3][2] = 1.0f;

	return projection;
}

const CMatrix4x4 CMatrix4x4::BuildOrthographic(float width, float height, float nearPlane, float farPlane)
{
	CMatrix4x4 ortho(CMatrix4x4::s_IDENTITY);
	ortho[0][0] = 2.0f / width;
	ortho[1][1] = 2.0f / height;
	ortho[2][2] = 1.0f / (farPlane - nearPlane);
	ortho[2][3] = nearPlane / (nearPlane - farPlane);

	return ortho;
}

}