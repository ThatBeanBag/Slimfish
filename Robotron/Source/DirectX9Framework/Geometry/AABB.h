// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AABB.h
// Description	: CAABB declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __AABB_H__
#define __AABB_H__

// Library Includes

// Local Includes
#include "Matrix4x4.h"

class CAABB {
	// Member Functions
public:
	CAABB();
	CAABB(const CVec3& _krvec3Min, const CVec3& _krvec3Max);

	~CAABB();
	
	void SetMin(const CVec3& _krvec3Min);
	void SetMax(const CVec3& _krvec3Max);

	const CVec3& GetMin() const;
	const CVec3& GetMax() const;

	const CVec3 GetCentre() const;
	const CVec3 GetExtent() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	CVec3 m_vec3Min;
	CVec3 m_vec3Max;
};

// Creates a new AABB that contains both AABB's.
CAABB CombineAABB(const CAABB& _krAABB1, const CAABB& _krAABB2);
CAABB TransformAABB(const CMatrix4x4& _krMatrix, const CAABB& _krAABB);

#endif	// __AABB_H__