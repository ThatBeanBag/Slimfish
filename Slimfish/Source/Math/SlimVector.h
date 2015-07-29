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

/************************************************************************/
/* CVec3
/*
/* Implementation of a standard 3D vector.
/* 
/* Represents a coordinate (x, y, z) in 3D space.
/* Implements various useful vector math operators and arithmetic.
/************************************************************************/
class CVec3 {
	// Member Functions
public:
	/**
	* Constructs a vector with x, y and z being set to 0.
	* 
	* @author: 	Hayden Asplet
	*/
	CVec3();

	/**
	* Constructs a vector from x, y and z values.
	* 
	* @author: 	Hayden Asplet
	* @param:	x
	* @param:	y
	* @param:	z
	*/
	CVec3(float x, float y, float z);

	/**
	* Destructs a vector
	* 
	* @author: 	Hayden Asplet
	*/
	~CVec3();

	/**
	* Adds two vectors and assigns the result.
	* 
	* @author: 	Hayden Asplet
	* @param:	other - vector to add.
	* @return:  CVec3& - resultant vector
	*/
	CVec3& operator+=(const CVec3& other);

	/**
	* Subtracts one vector from another and assigns the result.
	* 
	* @author: 	Hayden Asplet
	* @param:	other - vector to subtract.
	* @return:  CVec3& - resultant subtracted vector.
	*/
	CVec3& operator-=(const CVec3& other);
	/**
	* Multiplies a vector by a scalar and assigns the result.
	* 
	* @author: 	Hayden Asplet
	* @param:	float scalar - scalar to multiply.
	* @return:  CVec3& - resultant multiplied vector.
	*/
	CVec3& operator*=(float scalar);

	/**
	* Sets the X value.
	* 
	* @author: 	Hayden Asplet
	* @param:	x
	* @return:  void
	*/
	void SetX(float x) { m_x = x; }

	/**
	* Sets the Y value.
	*
	* @author: 	Hayden Asplet
	* @param:	y
	* @return:  void
	*/
	void SetY(float y) { m_y = y; }

	/**
	* Sets the Z value.
	*
	* @author: 	Hayden Asplet
	* @param:	z
	* @return:  void
	*/
	void SetZ(float z) { m_z = z; }

	/**
	* Gets the X value.
	*
	* @author: 	Hayden Asplet
	* @return:  const float - x value.
	*/
	const float GetX() const { return m_x; }

	/**
	* Gets the Y value.
	*
	* @author: 	Hayden Asplet
	* @return:  const float - y value.
	*/
	const float GetY() const { return m_y; }

	/**
	* Gets the Z value.
	*
	* @author: 	Hayden Asplet
	* @return:  const float - z value.
	*/
	const float GetZ() const { return m_z; }

	/**
	* Gets the length (magnitude) of the vector.
	*
	* This method uses the sqrt() function to determine the length if you just want 
	* the length for comparison purposes use GetLengthSquared() instead.
	* 
	* @author: 	Hayden Asplet
	* @return:  const float - length (magnitude) of the vector.
	*/
	const float GetLength() const { return sqrt(GetLengthSquared()); }

	/**
	* Calculates the length (magnitude) of the vector without square rooting it.
	*
	* This method is useful for comparisons with the length in order to maintain 
	* performance in performance critical code.
	*
	* You can use this method to improve performance like so:
	* if(vector.GetLengthSquared() < minDistance * minDistance) {
	*		// Do something.
	* }
	* This is preferable to:
	* if(vector.GetLength() < minDistance) {
	*		// Do something.
	* }
	* 
	* @author: 	Hayden Asplet
	* @return:  const float - length (magnitude) of the vector squared.
	*/
	const float GetLengthSquared() const { return (m_x * m_x + m_y * m_y + m_z * m_z); }
protected:
private:
	// Member Variables
public:
protected:
private:
	float m_x;
	float m_y;
	float m_z;
};

const CVec3 operator+(const CVec3& vec3A, const CVec3& vec3B);
const CVec3 operator-(const CVec3& vec3A, const CVec3& vec3B);
const CVec3 operator*(const CVec3& vec3, float scalar);
const CVec3 operator*(float scalar, const CVec3& vec3);

/**
* Normalises a CVec3 so that length of the vector is 1 and returns the result.
* 
* @author: 	Hayden Asplet
* @param:	vec3 - vector to normalise.
* @return:  const CVec3 - normalised vector.
*/
const CVec3 Normalise(const CVec3& vec3);

/**
* Retrieves the cross product of two vectors.
* 
* @author: 	Hayden Asplet
* @param:	vec3A
* @param:	vec3B
* @return:  const CVec3 - cross product.
*/
const CVec3 CrossProduct(const CVec3& vec3A, const CVec3& vec3B);

/**
* Retrieves the dot product of two vectors.
* 
* @author: 	Hayden Asplet
* @param:	vec3A 
* @param:	vec3B
* @return:  const float - dot product.
*/
const float DotProduct(const CVec3& vec3A, const CVec3& vec3B);

}

#endif	// __SLIMVECTOR_H__