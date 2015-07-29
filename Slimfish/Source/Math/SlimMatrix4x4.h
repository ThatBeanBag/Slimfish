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
#include <cstdint>

// Local Includes
#include "SlimVector.h"

namespace Slim {

/************************************************************************/
/* CMatrix4x4
/*
/* A standard 4 by 4 matrix class.
/*
/* All matrices are column-major and should be concatenated appropriately.
/*
/* Standard transformation ordering: 
/* Translation * Rotation * Scale.
/*
/* Coordinate system is right-handed.
/* Rotation matrices are of the form:
/*	 Rx =  1       0       0		0
/*	       0     cos(t) -sin(t)		0
/*	       0     sin(t)  cos(t)		0
/*		   0	   0	   0		1
/* where t is a counterclockwise rotation about the yz-plane.
/*
/*	 Ry =  cos(t)  0    sin(t)		0
/*	       0       1	   0		0
/*		   -sin(t) 0    cos(t)		0
/*		   0	   0	   0		1
/* where t is a counterclockwise rotation about the xz-plane.
/*
/*	 Rz =  cos(t) -sin(t)	0		0
/*	       sin(t)  cos(t)	0		0
/*		   0	   0		1	    0
/*		   0	   0		0		1
/* where t is a counterclockwise rotation about the xy-plane.
/*
/* Elements of the matrix can be indexed by matrix[row][column].
/* A visualization of the layout is as follows:
/*	|	[0][0]	[0][1]	[0][2]	[0][3]	|	
/*	|	[1][0]	[1][1]	[1][2]	[1][3]	|
/*	|	[2][0]	[2][1]	[2][2]	[2][3]	|
/*	|	[3][0]	[3][1]	[3][2]	[3][3]	|
/*
/************************************************************************/
class CMatrix4x4 {
	// Member Functions
public:
	/**
	* Constructs a 4 by 4 matrix with all 16 values set to 0.
	* 
	* @author: 	Hayden Asplet
	*/
	CMatrix4x4();

	/**
	* Constructs a 4 by 4 matrix from an array of 16 floats in a row first ordering.
	* 
	* @author: 	Hayden Asplet
	* @param:	pMatrix[4][4] - elements of the matrix.
	*/
	CMatrix4x4(float pMatrix[4][4]);

	/**
	* Constructs a 4 by 4 matrix from 16 floats.
	* 
	* @author: 	Hayden Asplet
	* @param:	float v11 - row 1 column 1.
	* @param:	float v12 - row 1 column 2.
	* @param:	float v13 - row 1 column 3.
	* @param:	float v14 - row 1 column 4.
	* @param:	float v21 - row 2 column 1.
	* @param:	float v22 - row 2 column 2.
	* @param:	float v23 - row 2 column 3.
	* @param:	float v24 - row 2 column 4.
	* @param:	float v31 - row 3 column 1.
	* @param:	float v32 - row 3 column 2.
	* @param:	float v33 - row 3 column 3.
	* @param:	float v34 - row 3 column 4.
	* @param:	float v41 - row 4 column 1.
	* @param:	float v42 - row 4 column 2.
	* @param:	float v43 - row 4 column 3.
	* @param:	float v44 - row 4 column 4.
	*/
	CMatrix4x4(float v11, float v12, float v13, float v14,
			   float v21, float v22, float v23, float v24,
			   float v31, float v32, float v33, float v34, 
			   float v41, float v42, float v43, float v44);

	/**
	* Constructs a 4 by 4 matrix from another 4 by 4 matrix.
	* 
	* @author: 	Hayden Asplet
	* @param:	other - another matrix to copy from.
	*/
	CMatrix4x4(const CMatrix4x4& other);

	/**
	* Destructs a 4 by 4 matrix.
	* 
	* @author: 	Hayden Asplet
	*/
	~CMatrix4x4();

	CMatrix4x4& operator=(const CMatrix4x4& other);
	CMatrix4x4& operator*=(const CMatrix4x4& other);
	CMatrix4x4& operator+=(const CMatrix4x4& other);
	CMatrix4x4& operator-=(const CMatrix4x4& other);

	/**
	* Transforms a vector by the matrix.
	* 
	* @author: 	Hayden Asplet
	* @param:	vector - vector to transform.
	* @return:  const CVec3 - transformed vector.
	*/
	const CVec3 Transform(const CVec3& vector);

	/************************************************************************/
	/* Accessor Methods
	/************************************************************************/

	/**
	* Access a row of the matrix, allows use of operator matrix[r][c];
	* 
	* @author: 	Hayden Asplet
	* @param:	size_t row - row to retrieve.
	* @return:  const float* - row;
	*/
	const float* operator[](size_t row) const;

	/**
	* Access a row of the matrix, allows use of operator matrix[r][c];
	*
	* @author: 	Hayden Asplet
	* @param:	size_t row - row to retrieve.
	* @return:  const float* - row;
	*/
	float* operator[](size_t row);

	/**
	* Sets the elements of the matrix from an array of 16 floating point values.
	* 
	* @author: 	Hayden Asplet
	* @param:	pMatrix[16] - elements of the matrix to set.
	* @return:  void
	*/
	void SetElements(float pMatrix[4][4]);

	/**
	* Sets the translational component of the transform.
	* 
	* @author: 	Hayden Asplet
	* @param:	x - x component of the position to set.
	* @param:	y - y component of the position to set.
	* @param:	z - z component of the position to set.
	* @return:  void
	*/
	void SetPosition(float x, float y, float z);

	/**
	* Sets the translational component of the transform.
	* 
	* @author: 	Hayden Asplet
	* @param:	position - position vector to set.
	* @return:  void
	*/
	void SetPosition(const CVec3& position);

	/**
	* Sets the scale of the transform.
	*
	* BEWARE: This will only produce a valid result if the transform is an identity matrix
	* or only contains translational information. If the transform has rotational information
	* setting the scale will not work as intended.
	*
	* If you want to add scale to a rotation transform DO NOT use this method, first build a
	* scale matrix using BuildScale() then concatenate the two transforms by multiplying the 
	* two matrices like so: Scale * Rotation.
	* 
	* @author: 	Hayden Asplet
	* @param:	float xScale - x component of the scale to set.
	* @param:	float yScale - y component of the scale to set.
	* @param:	float zScale - z component of the scale to set.
	* @return:  void
	*/
	void SetScale(float xScale, float yScale, float zScale);

	/**
	* Sets the scale of the transform.
	*
	* BEWARE: This will only produce a valid result if the transform is an identity matrix
	* or only contains translational information. If the transform has rotational information
	* setting the scale will not work as intended.
	*
	* If you want to add scale to a rotation transform DO NOT use this method, first build a
	* scale matrix using BuildScale() then concatenate the two transforms by multiplying the
	* two matrices like so: Scale * Rotation.
	*
	* @author: 	Hayden Asplet
	* @param:	scale - scale to set.
	* @return:  void
	*/
	void SetScale(const CVec3& scale);

	/**
	* Retrieves the transposed matrix.
	* 
	* @author: 	Hayden Asplet
	* @return:  const CMatrix4x4 - transposed matrix.
	*/
	const CMatrix4x4 GetTranspose() const;

	/**
	* Retrieves the inverse matrix.
	* 
	* @author: 	Hayden Asplet
	* @return:  const CMatrix4x4 - inverted matrix.
	*/
	const CMatrix4x4 GetInverse() const;

	/**
	* Retrieves the position of the transform.
	* 
	* @author: 	Hayden Asplet
	* @return:  const CVec3 - position of the transform.
	*/
	const CVec3 GetPosition() const;
	/**
	* Retrieves the rotational direction (look or forward) of the transform.
	* 
	* @author: 	Hayden Asplet
	* @return:  const CVec3 - direction of the transform.
	*/
	const CVec3 GetDirection() const;

	/**
	* Retrieves the right vector of the transform.
	* 
	* @author: 	Hayden Asplet
	* @return:  const CVec3 - right vector of the transform.
	*/
	const CVec3 GetRight() const;

	/**
	* Get the up vector of the transform.
	* 
	* @author: 	Hayden Asplet
	* @return:  const CVec3 - up vector of the transform.
	*/
	const CVec3 GetUp() const;
protected:
private:
	/**
	* Helper function that copies a matrix to another.
	*
	* Assignment operator and copy constructor call this method to avoid code duplication.
	* 
	* @author: 	Hayden Asplet
	* @param:	other - matrix to copy.
	* @return:  void
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

/************************************************************************/
/* Helper Build Functions
/************************************************************************/
const CMatrix4x4 BuildScaleMatrix(float xScale, float yScale, float zScale);
const CMatrix4x4 BuildScaleMatrix(const CVec3& scale);
const CMatrix4x4 BuildRotationXMatrix(float radians);
const CMatrix4x4 BuildRotationYMatrix(float radians);
const CMatrix4x4 BuildRotationZMatrix(float radians);
const CMatrix4x4 BuildYawPitchRollMatrix(float radYaw, float radPitch, float radRoll);
const CMatrix4x4 BuildTranslationMatrix(float x, float y, float z);
const CMatrix4x4 BuildTranslationMatrix(const CVec3& position);
const CMatrix4x4 BuildLookAtMatrix(const CVec3& eye, const CVec3& at, const CVec3& up);
const CMatrix4x4 BuildProjectionMatrix(float radFOV, float aspectRatio, float near, float far);
const CMatrix4x4 BuildOrthographicMatrix(float width, float height, float near, float far);

}

#endif	// __SLIMMATRIX4X4_H__