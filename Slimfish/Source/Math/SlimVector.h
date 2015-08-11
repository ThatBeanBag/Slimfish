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
class CVec3 {
	// Member Functions
public:
	/** Construct a vector with the x, y and z values being set to 0.
		@author Hayden Asplet
	*/
	CVec3();

	/** Construct a vector from x, y, and z values.
		@author Hayden Asplet
	*/
	CVec3(float x, float y, float z);

	/** Destruct a vector
		@author Hayden Asplet
	*/
	~CVec3();

	/** Add two vectors together and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVec3& operator+=(const CVec3& other);

	/** Subtract a vector from the vector and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVec3& operator-=(const CVec3& other);

	/** Multiply the vector by a scalar and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVec3& operator*=(float scalar);

	/** Add a scalar to a vector and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVec3& operator+=(float scalar);

	/** Subtract a scalar from the vector and assign the vector to the result.
		@author Hayden Asplet
	*/
	CVec3& operator-=(float scalar);

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
protected:
private:
	// Member Variables
public:
	static const CVec3 s_FORWARD;
	static const CVec3 s_RIGHT;
	static const CVec3 s_UP;
protected:
private:
	float m_x;
	float m_y;
	float m_z;
};

/** Add two vectors together
	@author Hayden Asplet
*/
const CVec3 operator+(const CVec3& vec3A, const CVec3& vec3B);

/** Subtract a vector from another.
	@author Hayden Asplet
*/
const CVec3 operator-(const CVec3& vec3A, const CVec3& vec3B);

/** Multiply a vector by a scalar. @author Hayden Asplet */
const CVec3 operator*(const CVec3& vec3, float scalar);
/** Multiply a vector by a scalar. @author Hayden Asplet */
const CVec3 operator*(float scalar, const CVec3& vec3);
/** Add a scalar to a vector. @author Hayden Asplet */
const CVec3 operator+(const CVec3& vec3, float scalar);
/** Subtract a scalar from a vector. @author Hayden Asplet */
const CVec3 operator-(const CVec3& vec3, float scalar);
/** Negate a vector @author Hayden Asplet */
const CVec3 operator-(const CVec3& vec3);

/** Normalise a vector3 so that the length of the vector is 1. @author Hayden Asplet */
const CVec3 Normalise(const CVec3& vec3);

/** Get the cross product of two vectors.
	@remarks
		This operation is not commutative as in CrossProduct(A,B) != CrossProduct(B,A).
	@author Hayden Asplet
*/
const CVec3 CrossProduct(const CVec3& vec3A, const CVec3& vec3B);

/** Retrieve the dot product of two vectors.
	@author Hayden Asplet
*/
const float DotProduct(const CVec3& vec3A, const CVec3& vec3B);

}

#endif	// __SLIMVECTOR_H__