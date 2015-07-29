// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Matrix4x4.h
// Description	: CMatrix4x4 declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Matrix4x4.h"

// Local Includes

const CMatrix4x4 CMatrix4x4::s_kIdentity(D3DXMATRIX(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

CMatrix4x4::CMatrix4x4()
	:D3DXMATRIX()
{
}

CMatrix4x4::CMatrix4x4(const D3DXMATRIX& _krMatrix)
{
	memcpy(&m, &_krMatrix.m, sizeof(_krMatrix.m));
}

CVec3 CMatrix4x4::Transform(const CVec3& _krVec3) const
{
	D3DXVECTOR4 vec4Out;
	D3DXVec3Transform(&vec4Out, &_krVec3, this);
	return CVec3(vec4Out.x, vec4Out.y, vec4Out.z);
}

CVec3 CMatrix4x4::Transform(const CVec3& _krVec3, float _fW) const
{
	D3DXVECTOR4 vec4InOut(_krVec3.x, _krVec3.y, _krVec3.z, _fW);
	D3DXVec4Transform(&vec4InOut, &vec4InOut, this);
	return CVec3(vec4InOut.x, vec4InOut.y, vec4InOut.z);
}

std::string CMatrix4x4::ToText(int _iPrecision) const
{
	std::string strMatrix;

	for (int i = 0; i < 4; ++i) {
		strMatrix += "[";

		for (int j = 0; j < 4; ++j) {
			// It's useful to fix the precision here so that 
			// the matrix keeps it's shape, in text form.
			strMatrix += ToStringFixed(m[i][j], _iPrecision);

			if (j != 3) {
				strMatrix += ", ";
			}
		}

		strMatrix += "]\n";
	}

	return std::move(strMatrix);
}

void CMatrix4x4::SetPosition(float _fX, float _fY, float _fZ)
{
	m[3][0] = _fX;
	m[3][1] = _fY;
	m[3][2] = _fZ;
	m[3][3] = 1.0f;
}

void CMatrix4x4::SetPosition(const CVec3& _krvec3Position)
{
	m[3][0] = _krvec3Position.x;
	m[3][1] = _krvec3Position.y;
	m[3][2] = _krvec3Position.z;
	m[3][3] = 1.0f;
}

void CMatrix4x4::SetScaleRotationless(float _fX, float _fY, float _fZ)
{
	m[0][0] = _fX;
	m[1][1] = _fY;
	m[2][2] = _fZ;
}

void CMatrix4x4::SetScaleRotationless(const CVec3& _krvec3Scale)
{
	m[0][0] = _krvec3Scale.x;
	m[1][1] = _krvec3Scale.y;
	m[2][2] = _krvec3Scale.z;
}

const CVec3 CMatrix4x4::GetPosition() const
{
	return CVec3(m[3][0], m[3][1], m[3][2]);
}

const CVec3 CMatrix4x4::GetScaleRotationless() const
{
	return CVec3(m[0][0], m[1][1], m[2][2]);
}

const CVec3 CMatrix4x4::GetScale() const
{
	CVec3 vec3Scale;
	vec3Scale.x = GetRight().GetLength();
	vec3Scale.y = GetUp().GetLength();
	vec3Scale.z = GetForward().GetLength();
	return vec3Scale;
}

const CVec3 CMatrix4x4::GetRight() const
{
	CMatrix4x4 matRotationOnly = *this;
	matRotationOnly.SetPosition(0.0f, 0.0f, 0.0f);
	return matRotationOnly.Transform(g_kvec3Right);
}

const CVec3 CMatrix4x4::GetUp() const
{
	CMatrix4x4 matRotationOnly = *this;
	matRotationOnly.SetPosition(0.0f, 0.0f, 0.0f);
	return matRotationOnly.Transform(g_kvec3Up);
}

const CVec3 CMatrix4x4::GetForward() const
{
	CMatrix4x4 matRotationOnly = *this;
	matRotationOnly.SetPosition(0.0f, 0.0f, 0.0f);
	return matRotationOnly.Transform(g_kvec3Foward);
}

CMatrix4x4 CMatrix4x4::Inversed() const
{
	D3DXMATRIX matInverse;
	FLOAT fDeterminant = D3DXMatrixDeterminant(this);
	D3DXMatrixInverse(&matInverse, &fDeterminant, this);
	return matInverse;
}

CMatrix4x4 CMatrix4x4::Tranposed() const
{
	D3DXMATRIX matTranspose;
	D3DXMatrixTranspose(&matTranspose, this);
	return CMatrix4x4(matTranspose);
}

CMatrix4x4 BuildScale(float _fX, float _fY, float _fZ)
{
	CMatrix4x4 matScale;
	D3DXMatrixScaling(&matScale, _fX, _fY, _fZ);
	return matScale;
}

CMatrix4x4 BuildScale(const CVec3& _krvec3Scale)
{
	CMatrix4x4 matScale;
	D3DXMatrixScaling(&matScale, _krvec3Scale.x, _krvec3Scale.y, _krvec3Scale.z);
	return matScale;
}

CMatrix4x4 BuildRotationX(float _fRadians)
{
	CMatrix4x4 matRotation;
	D3DXMatrixRotationX(&matRotation, _fRadians);
	return matRotation;
}

CMatrix4x4 BuildRotationY(float _fRadians)
{
	CMatrix4x4 matRotation;
	D3DXMatrixRotationY(&matRotation, _fRadians);
	return matRotation;
}

CMatrix4x4 BuildRotationZ(float _fRadians)
{
	CMatrix4x4 matRotation;
	D3DXMatrixRotationZ(&matRotation, _fRadians);
	return matRotation;
}

CMatrix4x4 BuildYawPitchRoll(float _fRadYaw, float _fRadPitch, float _fRadRoll)
{
	CMatrix4x4 matRotation;
	D3DXMatrixRotationYawPitchRoll(&matRotation, _fRadYaw, _fRadPitch, _fRadRoll);
	return matRotation;
}

CMatrix4x4 BuildTranslation(float _fX, float _fY, float _fZ)
{
	CMatrix4x4 matTranslation = CMatrix4x4::s_kIdentity;

	matTranslation.m[3][0] = _fX;
	matTranslation.m[3][1] = _fY;
	matTranslation.m[3][2] = _fZ;

	return matTranslation;
}

CMatrix4x4 BuildTranslation(const CVec3& _krPosition)
{
	CMatrix4x4 matTranslation = CMatrix4x4::s_kIdentity;

	matTranslation.m[3][0] = _krPosition.x;
	matTranslation.m[3][1] = _krPosition.y;
	matTranslation.m[3][2] = _krPosition.z;

	return matTranslation;
}

CMatrix4x4 BuildLookAt(const CVec3& _krEye, const CVec3& _krAt, const CVec3& _krUp)
{
	CMatrix4x4 matLookAt;
	D3DXMatrixLookAtLH(&matLookAt, &_krEye, &_krAt, &_krUp);
	return matLookAt;
}

CMatrix4x4 BuildOrthographic(float _fVolumeWidth, float _fVolumeHeight, float _fZNear, float _fZFar)
{
	CMatrix4x4 matOthrographicProj;
	D3DXMatrixOrthoLH(&matOthrographicProj, _fVolumeWidth, _fVolumeHeight, _fZNear, _fZFar);
	return matOthrographicProj;
}

CMatrix4x4 BuildPerspective(float _fFOV, float _fAspectRatio, float _fZNear, float _fZFar)
{
	CMatrix4x4 matPerspective;
	D3DXMatrixPerspectiveFovLH(&matPerspective, _fFOV, _fAspectRatio, _fZNear, _fZFar);
	return matPerspective;
}

CMatrix4x4 BuildRotationDirection(const CVec3& _krvec3Direction)
{
	CVec3 vec3Forward = _krvec3Direction;
	CVec3 vec3Right = CrossProduct(g_kvec3Up, vec3Forward);
	CVec3 vec3Up = CrossProduct(vec3Forward, vec3Right);
	CMatrix4x4 matRotation(CMatrix4x4::s_kIdentity);

	matRotation.m[0][0] = vec3Right.x;
	matRotation.m[0][1] = vec3Right.y;
	matRotation.m[0][2] = vec3Right.z;
	matRotation.m[1][0] = vec3Up.x;
	matRotation.m[1][1] = vec3Up.y;
	matRotation.m[1][2] = vec3Up.z;
	matRotation.m[2][0] = vec3Forward.x;
	matRotation.m[2][1] = vec3Forward.y;
	matRotation.m[2][2] = vec3Forward.z;

	return matRotation;
}

std::ostream& operator<<(std::ostream& _os, const CMatrix4x4& _krMatrix)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			_os << _krMatrix.m[i][j] << " ";
		}
	}

	return _os;
}

std::istream& operator>>(std::istream& _is, CMatrix4x4& _krMatrix)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			_is >> _krMatrix.m[i][j];
		}
	}

	return _is;
}
