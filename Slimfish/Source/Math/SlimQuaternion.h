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

	/** Construct a quaternion from explicit x, y, z and w values.
	 	@author Hayden Asplet
	*/
	CQuaternion(float x, float y, float z,  float w);

	/** Construct a quaternion from an axis/angle.
	 	@author Hayden Asplet
	*/
	CQuaternion(const CVector3& axis, float radAngle);

	CQuaternion(float yaw, float pitch, float roll);

	/** Construct a quaternion from a rotation matrix.
	 	@author Hayden Asplet
	*/
	CQuaternion(const CMatrix4x4& rotationMatrix);

	/** Construct a quaternion from 3 perpendicular axis.
	 	@author Hayden Asplet
	*/
	CQuaternion(const CVector3& right, const CVector3& up, const CVector3& forward);

	/** Construct a quaternion from a singular forward, assuming the up vector points directly up (0, 1, 0).
		@remarks This is useful for settings characters direction.
	 	@author Hayden Asplet
	 	@param 
			forward The forward or look vector that describes the direction to face. The y component of this
			should be 0 in order to produce an expected result. 
	*/
	CQuaternion(const CVector3& forward, const CVector3& up = CVector3::s_UP);

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

	const CVector3 operator*(const CVector3& vector) const;

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

	const CVector3 GetDirection() const;
	const CVector3 GetRight() const;
	const CVector3 GetUp() const;

	/** Set the w (real) component of the quaternion. @author Hayden Asplet */
	void SetW(float w) { m_w = w; }
	/** Get the w (real) component of the quaternion. @author Hayden Asplet */
	const float GetW() const { return m_w; }
	/** Set the x (i) component of the quaternion. @author Hayden Asplet */
	void SetX(float x) { m_x = x; }
	/** Get the i (i) component of the quaternion. @author Hayden Asplet */
	const float GetX() const { return m_x; }
	/** Set the y (j) component of the quaternion. @author Hayden Asplet */
	void SetY(float y) { m_y = y; }
	/** Get the y (j) component of the quaternion. @author Hayden Asplet */
	const float GetY() const { return m_y; }
	/** Set the z (k) component of the quaternion. @author Hayden Asplet */
	void SetZ(float z) { m_z = z; }
	/** Get the z (k) component of the quaternion. @author Hayden Asplet */
	const float GetZ() const { return m_z; }
protected:
private:
	// Member Variables
public:
	static const CQuaternion s_IDENTITY;
	static const CQuaternion s_ZERO;
protected:
private:
	float m_w;
	float m_x;
	float m_y;
	float m_z;
};

/** Multiply a quaternion by a scalar. @author Hayden Asplet */
const CQuaternion operator*(float scalar, const CQuaternion& other);

/** Multiply a vector by a quaternion. @author Hayden Asplet */
const CVector3 operator*(const CVector3& vector, const CQuaternion& rotation);

}
#endif	// __SLIMQUATERNION_H__