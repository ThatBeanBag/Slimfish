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
class CVector3;

/** Class implementing a standard quaternion for rotations in both physics
	and 3D graphics.
*/
class CQuaternion {
	// Member Functions
public:
	/** Default Constructor.
		@remarks Initialize to identity rotation (0 degrees).
	 	@author Hayden Asplet
	*/
	CQuaternion();

	/** Construct a quaternion from explicit real, i, j and k values.
	 	@author Hayden Asplet
	*/
	CQuaternion(float real, float i, float j,  float k);

	/** Construct a quaternion from an axis/angle.
	 	@author Hayden Asplet
	*/
	CQuaternion(const CVector3& axis, float radAngle);

	/** Construct a quaternion from 3 perpendicular axis.
	 	@author Hayden Asplet
	*/
	CQuaternion(const CVector3& right, const CVector3& up, const CVector3& forward);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CQuaternion();

	// Assignment Operators

	/** Add two quaternions together and assign the result. @author Hayden Asplet */
	CQuaternion& operator*=(const CQuaternion& other);
	/** Subtract a quaternion from this one and assign the result. @author Hayden Asplet */
	CQuaternion& operator+=(const CQuaternion& other);
	/** Multiply two quaternions together and assign the result. @author Hayden Asplet */
	CQuaternion& operator-=(const CQuaternion& other);
	/** Multiply a quaternion by a scalar and assign the result. @author Hayden Asplet */
	CQuaternion& operator*=(float scalar);

	// Operators

	/** Equate a quaternion to another. @author Hayden Asplet. */
	bool operator==(const CQuaternion& other);
	/** Determine if a quaternion is not equal to another. @author Hayden Asplet */
	bool operator!=(const CQuaternion& other);

	/** Add two quaternions together. @author Hayden Asplet */
	const CQuaternion operator+(const CQuaternion& other) const;
	/** Subtract a quaternion from this one. @author Hayden Asplet */
	const CQuaternion operator-(const CQuaternion& other) const;
	/** Multiply two quaternions together. @author Hayden Asplet */
	const CQuaternion operator*(const CQuaternion& other) const;
	/** Multiply a quaternion by a scalar @author Hayden Asplet */
	const CQuaternion operator*(float scalar) const;

	/** Get the normalised version of the quaternion (magnitude 1). @author Hayden Asplet */
	const CQuaternion GetNormalise() const;
	/** Get the conjugate of the quaternion. @author Hayden Asplet */
	const CQuaternion GetConjugate() const;
	/** Get the inverse of the quaternion. @author Hayden Asplet */
	const CQuaternion GetInverse() const;
	/** Get the magnitude of the quaternion. @author Hayden Asplet */
	const float GetMagnitude() const;

	/** Convert the quaternion to a 4x4 rotation matrix. @author Hayden Asplet */
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

/** Multiply a quaternion by a scalar. @author Hayden Asplet */
const CQuaternion operator*(float scalar, const CQuaternion& other);

}
#endif	// __SLIMQUATERNION_H__