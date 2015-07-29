// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimQuaternion.h
// Description	: CSlimQuaternion declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMQUATERNION_H__
#define __SLIMQUATERNION_H__

// Library Includes

// Local Includes
#include "SlimMatrix4x4.h"

namespace Slim {

// Forward Declaration
class CVec3;

/************************************************************************/
/* CQuaternion
/*
/* Implementation of a standard quaternion for rotations in both physics
/* and 3D graphics.
/************************************************************************/
class CQuaternion {
	// Member Functions
public:
	/**
	* Default constructor
	*
	* Initializes to identity rotation (0 degrees).
	* 
	* @author: 	Hayden Asplet
	*/
	CQuaternion();

	/**
	* Constructs a quaternion from explicit real, i, j and k values.
	* 
	* @author: 	Hayden Asplet
	* @param:	float real
	* @param:	float i
	* @param:	float j
	* @param:	float k
	*/
	CQuaternion(float real, float i, float j,  float k);

	/**
	* Constructs a quaternion from an axis/angle.
	* 
	* @author: 	Hayden Asplet
	* @param:	axis
	* @param:	radAngle - angle in radians
	*/
	CQuaternion(const CVec3& axis, float radAngle);

	/**
	* Constructs a quaternion from 3 perpendicular axis.
	* 
	* @author: 	Hayden Asplet
	* @param:	right 
	* @param:	up 
	* @param:	forward
	*/
	CQuaternion(const CVec3& right, const CVec3& up, const CVec3& forward);

	/**
	* Destructs a quaternion
	* 
	* @author: 	Hayden Asplet
	*/
	~CQuaternion();


	// Assignment Operators
	CQuaternion& operator*=(const CQuaternion& other);
	CQuaternion& operator+=(const CQuaternion& other);
	CQuaternion& operator-=(const CQuaternion& other);
	CQuaternion& operator*=(float scalar);

	// Operators
	bool operator==(const CQuaternion& other);
	bool operator!=(const CQuaternion& other);

	const CQuaternion operator+(const CQuaternion& other) const;
	const CQuaternion operator-(const CQuaternion& other) const;
	const CQuaternion operator*(const CQuaternion& other) const;
	const CQuaternion operator*(float scalar) const;

	const CQuaternion GetNormalise() const;
	const CQuaternion GetConjugate() const;
	const CQuaternion GetInverse() const;
	const float GetMagnitude() const;

	const CMatrix4x4 ToRotationMatrix() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	float m_w;
	float m_i;
	float m_j;
	float m_k;
};

const CQuaternion operator*(float scalar, const CQuaternion& other);

}
#endif	// __SLIMQUATERNION_H__