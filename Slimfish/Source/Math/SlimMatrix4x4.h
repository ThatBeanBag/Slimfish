// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimMatrix4x4.h
// Description	: CMatrix4x4 declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMMATRIX4X4_H__
#define __SLIMMATRIX4X4_H__

// Library Includes

// Local Includes
#include "SlimVector.h"

namespace Slim {

/** Standard 4 by 4 homogeneous matrix class for affine transformations of vectors.
	@remarks
		All matrices are column-major and should be concatenated appropriately.
	@par
		Transformation ordering is Translation * Rotation * Scale.
	@par
		Coordinate system is right-handed. Thus rotation matrices are of the form:
			 Rx =  1       0       0		0
				   0     cos(t) -sin(t)		0
				   0     sin(t)  cos(t)		0
				   0	   0	   0		1
		where t is a counterclockwise rotation about the yz-plane.
	@par
			 Ry =  cos(t)  0    sin(t)		0
				   0       1	   0		0
				   -sin(t) 0    cos(t)		0
				   0	   0	   0		1
		where t is a counterclockwise rotation about the xz-plane.
	@par
			 Rz =  cos(t) -sin(t)	0		0
				   sin(t)  cos(t)	0		0
				   0	   0		1	    0
				   0	   0		0		1
		where t is a counterclockwise rotation about the xy-plane.
	@par
		Elements of the matrix can be indexed by matrix[row][column].
		A visualization of the layout is as follows:
			|	[0][0]	[0][1]	[0][2]	[0][3]	|	
			|	[1][0]	[1][1]	[1][2]	[1][3]	|
			|	[2][0]	[2][1]	[2][2]	[2][3]	|
			|	[3][0]	[3][1]	[3][2]	[3][3]	|

			|	0		1		2		3		|
			|	4		5		6		7		|
			|	8		9		10		11		|
			|	12		13		14		15		|
*/
class CMatrix4x4 {
	// Member Functions
public:
	/** Construct a 4 by 4 matrix with all values set to 0.
	 	@author
	 		Hayden Asplet		
	*/
	CMatrix4x4();

	/** Construct a 4 by 4 matrix from a 4 by 4 array.
	 	@author Hayden Asplet
	 	@param
			pMatrix[4][4] Array of floats that make up the matrix.	
	*/
	CMatrix4x4(float pMatrix[4][4]);

	/** Construct a 4 by 4 matrix from 16 floats.
	 	@author Hayden Asplet
	*/
	CMatrix4x4(float v11, float v12, float v13, float v14,
			   float v21, float v22, float v23, float v24,
			   float v31, float v32, float v33, float v34, 
			   float v41, float v42, float v43, float v44);

	/** Construct a 4 by 4 matrix by copying another 4 by 4 matrix.
	 	@author Hayden Asplet	
	*/
	CMatrix4x4(const CMatrix4x4& other);

	/** Destruct a 4 by 4 matrix.
	 	@author Hayden Asplet
	*/
	~CMatrix4x4();

	/** Assign a matrix
	 	@author Hayden Asplet
	*/
	CMatrix4x4& operator=(const CMatrix4x4& other);

	/** Multiply and assign a matrix
		@author Hayden Asplet
	*/
	CMatrix4x4& operator*=(const CMatrix4x4& other);

	/** Add and assign a matrix
		@author Hayden Asplet
	*/
	CMatrix4x4& operator+=(const CMatrix4x4& other);

	/** Subtract and assign a matrix
		@author Hayden Asplet
	*/
	CMatrix4x4& operator-=(const CMatrix4x4& other);

	/** Transform a vector 3
	 	@author Hayden Asplet
		@return Resultant transformed and homogenized vector.
	*/
	const CVector3 Transform(const CVector3& vector);

	/************************************************************************/
	/* Accessor Methods
	/************************************************************************/

	/** Access a row of the matrix, allows use of operator matrix[row][column];
	 	@author Hayden Asplet
	*/
	const float* operator[](size_t row) const;

	/** Access a row of the matrix, allows use of operator matrix[row][column];
	 	@author Hayden Asplet
	*/
	float* operator[](size_t row);

	/** Set the elements of the matrix from a 4 by 4 array.
		@author Hayden Asplet
	*/
	void SetElements(float pMatrix[4][4]);

	/** Set the translational component of the transform.
	 	@author Hayden Asplet
	*/
	void SetPosition(float x, float y, float z);

	/** Set the translational component of the transform.
	 	@author Hayden Asplet
	*/
	void SetPosition(const CVector3& position);

	/** Set the scale of the transform.
		@remarks
			BEWARE: This will only produce a valid result if the transform is an identity matrix
			or only contains translational information. If the transform has rotational information
			setting the scale will yield undefined behavior.
		@par
			If you want to add scale to a rotation transform DO NOT use this method, first build a
			scale matrix using BuildScale() then concatenate the two transforms by multiplying the
			two matrices like so: Rotation * Scale.
	 	@author
	 		Hayden Asplet
	*/
	void SetScale(float xScale, float yScale, float zScale);

	/** Set the scale of the transform.
		@remarks
			BEWARE: This will only produce a valid result if the transform is an identity matrix
			or only contains translational information. If the transform has rotational information
			setting the scale will yield undefined behavior.
		@par
			If you want to add scale to a rotation transform DO NOT use this method, first build a
			scale matrix using BuildScale() then concatenate the two transforms by multiplying the
			two matrices like so: Rotation * Scale.
	 	@author
	 		Hayden Asplet
	*/
	void SetScale(const CVector3& scale);

	/** Create a new matrix by transposing the rows and columns of this matrix.
	 	@author
	 		Hayden Asplet
	*/
	const CMatrix4x4 GetTranspose() const;

	/** Create a new matrix by inversing this matrix.
		@remarks
			This method is particularly slow and should be called as frequently as possible.
			There are several situations where this method can be avoided resulting is higher
			performance code, which are described as follows:
		@par
			The inverse of a rotation only matrix is its transpose. Transposing a matrix is
			much less of a performance hit and thus GetTranpose() should be used inplace of 
			GetInverse() in situations with rotation only matrices.
		@par
			The inverse of a scale only matrix is a matrix built from the reciprocals of the
			Scale components which can be made from BuildScaleMatrix(1 / x, 1 / y, 1 / z).
		@par
			The inverse of a translation only matrix is simply a translation in the other direction.
			Building a matrix from the negative of the position will result in an inverted matrix.
			Use BuildTranslationMatrix( -position ) for translational only matrices.
		@author
			Hayden Asplet
	*/
	const CMatrix4x4 GetInverse() const;

	/** Get the position of the transform. @author Hayden Asplet */
	const CVector3 GetPosition() const;
	/** Retrieve the direction (look or forward) of the transform. @author Hayden Asplet */
	const CVector3 GetDirection() const;
	/** Retrieve the right vector of the transform. @author Hayden Asplet */
	const CVector3 GetRight() const;
	/** Get the up vector of the transform. @author Hayden Asplet */
	const CVector3 GetUp() const;

	// Convenience Build Functions

	/** Build a scale transform. @author Hayden Asplet */
	static const CMatrix4x4 BuildScale(float xScale, float yScale, float zScale);
	/** Build a scale transform. @author Hayden Asplet */
	static const CMatrix4x4 BuildScale(const CVector3& scale);
	/** Build a rotation transform to rotate a vector around the X axis. @author Hayden Asplet */
	static const CMatrix4x4 BuildRotationX(float radians);
	/** Build a rotation transform to rotate a vector around the Y axis. @author Hayden Asplet */
	static const CMatrix4x4 BuildRotationY(float radians);
	/** Build a rotation transform to rotate a vector around the Z axis. @author Hayden Asplet */
	static const CMatrix4x4 BuildRotationZ(float radians);
	/** Build a rotation transform from 3 perpendicular axis. @author Hayden Asplet */
	static const CMatrix4x4 BuildRotationFromAxis(const CVector3& right, const CVector3& up, const CVector3& forward);
	/** Build a rotation transform from yaw pitch and roll. @author Hayden Asplet */
	static const CMatrix4x4 BuildYawPitchRoll(float radYaw, float radPitch, float radRoll);
	/** Build a translation transform. @author Hayden Asplet */
	static const CMatrix4x4 BuildTranslation(float x, float y, float z);
	/** Build a translation transform. @author Hayden Asplet */
	static const CMatrix4x4 BuildTranslation(const CVector3& position);
	/** Build a view matrix from a camera position, a look at position and an up direction. @author Hayden Asplet */
	static const CMatrix4x4 BuildLookAt(const CVector3& eye, const CVector3& at, const CVector3& up);

	/** Build a perspective projection matrix.
	 	@author Hayden Asplet
		@param radFOV Field of view in radians
		@param aspectRatio The aspect ratio of the viewport.
		@param near The near clipping plane.
		@param far	The far clipping plane.
	*/
	static const CMatrix4x4 BuildProjection(float radFOV, float aspectRatio, float near, float far);

	/** Build an orthographic projection matrix.
	 	@author Hayden Asplet
		@param width The width of the viewport.
		@param height The height of viewport.
		@param near The near clipping plane.
		@param far The far clipping plane.
	*/
	static const CMatrix4x4 BuildOrthographic(float width, float height, float near, float far);
protected:
private:
	/** Internal helper function to copy a matrix to this matrix.
		@remarks
			Assignment operator and copy constructor call this method to avoid code duplication.
	 	@author Hayden Asplet
	*/
	void Copy(const float pMatrix[4][4]);

	// Member Variables
public:
	static const CMatrix4x4 s_IDENTITY;
	static const CMatrix4x4 s_ZERO;
protected:
private:
	float m_pMatrix[4][4];	// Sixteen floating-point values that make up the matrix.
};

/************************************************************************/
/* Arithmetic operators
/************************************************************************/
const CMatrix4x4 operator*(const CMatrix4x4& matrixA, const CMatrix4x4& matrixB);
const CMatrix4x4 operator+(const CMatrix4x4& matrixA, const CMatrix4x4& matrixB);
const CMatrix4x4 operator-(const CMatrix4x4& matrixA, const CMatrix4x4& matrixB);


}

#endif	// __SLIMMATRIX4X4_H__