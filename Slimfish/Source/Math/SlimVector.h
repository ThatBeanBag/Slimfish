// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimVector.h
// Description	: Various vectors declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMVECTOR_H__
#define __SLIMVECTOR_H__

// Library Includes
#include <math.h>

// Local Includes

namespace Slim {

/** Class encapsulating a standard 3D vector.
	@remarks
		Represents a coordinate (x, y, z) in 3D space and implements 
		various useful vector math operators and arithmetic.
*/
class CVector3 {
	// Member Functions
public:
	/** Construct a vector with the x, y and z values being set to 0.
		@author Hayden Asplet
	*/
	CVector3();

	/** Construct a vector from x, y, and z values.
		@author Hayden Asplet
	*/
	CVector3(float x, float y, float z);

	/** Destruct a vector
		@author Hayden Asplet
	*/
	~CVector3();

	/** Add two vectors together and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector3& operator+=(const CVector3& other);

	/** Subtract a vector from the vector and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector3& operator-=(const CVector3& other);

	/** Multiply the vector by a scalar and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector3& operator*=(float scalar);

	/** Divide the vector by a scalar and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector3& operator/=(float scalar);

	/** Add a scalar to a vector and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector3& operator+=(float scalar);

	/** Subtract a scalar from the vector and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector3& operator-=(float scalar);

	/** Set the X value of the vector.
		@author Hayden Asplet
	*/
	void SetX(float x) { m_x = x; }

	/** Set the Y value of the vector.
		@author
			Hayden Asplet
	*/
	void SetY(float y) { m_y = y; }

	/** Set the Z value of the vector.
	 	@author
	 		Hayden Asplet
	*/
	void SetZ(float z) { m_z = z; }


	/** Get the X value of vector.
		@author
			Hayden Asplet
	*/
	const float GetX() const { return m_x; }

	/** Get the Y value of vector.
		@author
			Hayden Asplet
	*/
	const float GetY() const { return m_y; }

	/** Get the Z value of vector.
	 	@author
	 		Hayden Asplet
	*/
	const float GetZ() const { return m_z; }

	/** Get the length (magnitude) of the vector.
		@remarks
			This method uses the sqrt() function to determine the length if you just want
			the length for comparison purposes, use GetLengthSquared() instead.
	 	@author
	 		Hayden Asplet
	 	@return   
	 		const float
	*/
	const float GetLength() const { return sqrt(GetLengthSquared()); }

	/** Calculate the length (magnitude) of the vector without square rooting it.
		@remarks
			This method is useful for comparisons with the length in order to maintain
			performance in performance critical code.
			
			You can use this method to improve performance like so:
			if(vector.GetLengthSquared() < minDistance * minDistance) {
					// Do something.
			}
			This is preferable to:
			if(vector.GetLength() < minDistance) {
					// Do something.
			}
	 	@author
	 		Hayden Asplet
	*/
	const float GetLengthSquared() const { return (m_x * m_x + m_y * m_y + m_z * m_z); }

	/** Normalise a vector3 so that the length of the vector is 1. @author Hayden Asplet */
	static const CVector3 Normalise(const CVector3& vec3);

	/** Get the cross product of two vectors.
		@remarks
			This operation is not commutative as in CrossProduct(A,B) != CrossProduct(B,A).
		@author Hayden Asplet
	*/
	static const CVector3 CrossProduct(const CVector3& vec3A, const CVector3& vec3B);

	/** Retrieve the dot product of two vectors.
		@author Hayden Asplet
	*/
	static const float DotProduct(const CVector3& vec3A, const CVector3& vec3B);
protected:
private:
	// Member Variables
public:
	static const CVector3 s_FORWARD;
	static const CVector3 s_RIGHT;
	static const CVector3 s_UP;
	static const CVector3 s_ZERO;
	static const CVector3 s_ONE;
protected:
private:
	float m_x;
	float m_y;
	float m_z;
};

/** Add two vectors together
	@author Hayden Asplet
*/
const CVector3 operator+(const CVector3& vec3A, const CVector3& vec3B);

/** Subtract a vector from another.
	@author Hayden Asplet
*/
const CVector3 operator-(const CVector3& vec3A, const CVector3& vec3B);

/** Multiply a vector by a scalar. @author Hayden Asplet */
const CVector3 operator*(const CVector3& vec3, float scalar);
/** Multiply a vector by a scalar. @author Hayden Asplet */
const CVector3 operator*(float scalar, const CVector3& vec3);
/** Divide a vector by a scalar. @author Hayden Asplet */
const CVector3 operator/(const CVector3& vec3, float scalar);
/** Divide a vector by a scalar. @author Hayden Asplet */
const CVector3 operator/(float scalar, const CVector3& vec3);
/** Add a scalar to a vector. @author Hayden Asplet */
const CVector3 operator+(const CVector3& vec3, float scalar);
/** Subtract a scalar from a vector. @author Hayden Asplet */
const CVector3 operator-(const CVector3& vec3, float scalar);
/** Negate a vector @author Hayden Asplet */
const CVector3 operator-(const CVector3& vec3);

/** Compare two vectors to see if they are equal.
	@author Hayden Asplet.
 	@return True if each component of vectorA and vectorB are equal.
*/
bool operator==(const CVector3& vectorA, const CVector3& vectorB);

/** Compare two vectors to see if they are not equal.
	@author Hayden Asplet.
	@return True if any component of vectorA and vectorB are not equal.
*/
bool operator!=(const CVector3& vectorA, const CVector3& vectorB);


/** Class encapsulating a standard 2D vector.
	@remarks
		Represents a coordinate (x, y) in 2D space and implements
		various useful vector math operators and arithmetic.
*/
class CVector2 {
	// Member Functions
public:
	/** Construct a vector with the x and y values being set to 0.
		@author Hayden Asplet
	*/
	CVector2();

	/** Construct a vector from x and y values.
		@author Hayden Asplet
	*/
	CVector2(float x, float y);

	/** Destruct a vector
		@author Hayden Asplet
	*/
	~CVector2();

	/** Add two vectors together and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector2& operator+=(const CVector2& other);

	/** Subtract a vector from the vector and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector2& operator-=(const CVector2& other);

	/** Multiply the vector by a scalar and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector2& operator*=(float scalar);

	/** Divide the vector by a scalar and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector2& operator/=(float scalar);

	/** Add a scalar to a vector and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector2& operator+=(float scalar);

	/** Subtract a scalar from the vector and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVector2& operator-=(float scalar);

	/** Set the X value of the vector.
		@author Hayden Asplet
	*/
	void SetX(float x) { m_x = x; }

	/** Set the Y value of the vector.
		@author Hayden Asplet
	*/
	void SetY(float y) { m_y = y; }

	/** Get the X value of vector.
		@author Hayden Asplet
	*/
	const float GetX() const { return m_x; }

	/** Get the Y value of vector.
		@author Hayden Asplet
	*/
	const float GetY() const { return m_y; }

	/** Get the length (magnitude) of the vector.
		@remarks
			This method uses the sqrt() function to determine the length if you just want
			the length for comparison purposes, use GetLengthSquared() instead.
		@author Hayden Asplet
	*/
	const float GetLength() const { return sqrt(GetLengthSquared()); }

	/** Calculate the length (magnitude) of the vector without square rooting it.
		@remarks
			This method is useful for comparisons with the length in order to maintain
			performance in performance critical code.

			You can use this method to improve performance like so:
			if(vector.GetLengthSquared() < minDistance * minDistance) {
				// Do something.
			}
			This is preferable to:
			if(vector.GetLength() < minDistance) {
				// Do something.
			}
		@author Hayden Asplet
	*/
	const float GetLengthSquared() const { return (m_x * m_x + m_y * m_y); }

	/** Normalise a vector2 so that the length of the vector is 1. @author Hayden Asplet */
	static const CVector2 Normalise(const CVector2& vec3);

	/** Retrieve the dot product of two vectors.
		@author Hayden Asplet
	*/
	static const float DotProduct(const CVector2& vec3A, const CVector2& vec3B);
protected:
private:
	// Member Variables
public:
	static const CVector2 s_RIGHT;
	static const CVector2 s_UP;
	static const CVector2 s_ZERO;
	static const CVector2 s_ONE;
protected:
private:
	float m_x;
	float m_y;
};

/** Add two vectors together
	@author Hayden Asplet
*/
const CVector2 operator+(const CVector2& vec2A, const CVector2& vec2B);

/** Subtract a vector from another.
	@author Hayden Asplet
*/
const CVector2 operator-(const CVector2& vec2A, const CVector2& vec2B);

/** Multiply a vector by a scalar. @author Hayden Asplet */
const CVector2 operator*(const CVector2& vec2, float scalar);
/** Multiply a vector by a scalar. @author Hayden Asplet */
const CVector2 operator*(float scalar, const CVector2& vec2);
/** Divide a vector by a scalar. @author Hayden Asplet */
const CVector2 operator/(const CVector2& vec2, float scalar);
/** Divide a vector by a scalar. @author Hayden Asplet */
const CVector2 operator/(float scalar, const CVector2& vec2);
/** Add a scalar to a vector. @author Hayden Asplet */
const CVector2 operator+(const CVector2& vec2, float scalar);
/** Subtract a scalar from a vector. @author Hayden Asplet */
const CVector2 operator-(const CVector2& vec2, float scalar);
/** Negate a vector @author Hayden Asplet */
const CVector2 operator-(const CVector2& vec2);

/** Compare two vectors to see if they are equal.
	@author Hayden Asplet.
	@return True if each component of vectorA and vectorB are equal.
*/
bool operator==(const CVector2& vectorA, const CVector2& vectorB);

/** Compare two vectors to see if they are not equal.
	@author Hayden Asplet.
	@return True if any component of vectorA and vectorB are not equal.
*/
bool operator!=(const CVector2& vectorA, const CVector2& vectorB);

}

#endif	// __SLIMVECTOR_H__