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

#pragma once

#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

// Library Includes

// Local Includes
#include "Vec3.h"
#include "../GameStd.h"	// REMOVE: this.

class CMatrix4x4 : public D3DXMATRIX {
	// Member Functions
public:
	CMatrix4x4();
	CMatrix4x4(const D3DXMATRIX& _krMatrix);

	CVec3 Transform(const CVec3& _krVec3) const;
	CVec3 Transform(const CVec3& _krVec3, float fW) const;

	std::string ToText(int _iPrecision = g_kiDEF_PRECISION) const;

	// Accessors
	void SetPosition(float _fX, float _fY, float _fZ);
	void SetPosition(const CVec3& _krvec3Position);
	void SetScaleRotationless(float _fX, float _fY, float _fZ);
	void SetScaleRotationless(const CVec3& _krvec3Scale);
	
	const CVec3 GetPosition() const;
	const CVec3 GetScaleRotationless() const;
	const CVec3 GetScale() const;
	const CVec3 GetRight() const;
	const CVec3 GetUp() const;
	const CVec3 GetForward() const;

	CMatrix4x4 Inversed() const;
	CMatrix4x4 Tranposed() const;
protected:
private:
	// Member Variables
public:
	static const CMatrix4x4 s_kIdentity;
protected:
private:
};

// Build methods.
CMatrix4x4 BuildScale(float _fX, float _fY, float _fZ);
CMatrix4x4 BuildScale(const CVec3& _krvec3Scale);
CMatrix4x4 BuildRotationX(float _fRadians);
CMatrix4x4 BuildRotationY(float _fRadians);
CMatrix4x4 BuildRotationZ(float _fRadians);
CMatrix4x4 BuildYawPitchRoll(float _fRadYaw, float _fRadPitch, float _fRadRoll);
CMatrix4x4 BuildRotationDirection(const CVec3& _krvec3Direction);
CMatrix4x4 BuildTranslation(float _fX, float _fY, float _fZ);
CMatrix4x4 BuildTranslation(const CVec3& _krPosition);
CMatrix4x4 BuildLookAt(const CVec3& _krEye, const CVec3& _krAt, const CVec3& _krUp);
CMatrix4x4 BuildOrthographic(float _fVolumeWidth, float _fVolumeHeight, float _fZNear, float _fZFar);
CMatrix4x4 BuildPerspective(float _fFOV, float _fAspectRatio, float _fZNear, float _fZFar);

// Serialisation/Deserialisation
std::ostream& operator<<(std::ostream& _os, const CMatrix4x4& _krMatrix);
std::istream& operator>>(std::istream& _is, CMatrix4x4& _krMatrix);

#endif	// __MATRIX4X4_H__