// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AABB.cpp
// Description	: CAABB implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "AABB.h"

// Local Includes


CAABB::CAABB()
{

}

CAABB::CAABB(const CVec3& _krvec3Min, const CVec3& _krvec3Max)
	:m_vec3Min(_krvec3Min),
	m_vec3Max(_krvec3Max)
{

}

CAABB::~CAABB()
{

}

void CAABB::SetMin(const CVec3& _krvec3Min)
{
	m_vec3Min = _krvec3Min;
}

void CAABB::SetMax(const CVec3& _krvec3Max)
{
	m_vec3Max = _krvec3Max;
}

const CVec3& CAABB::GetMin() const
{
	return m_vec3Min;
}

const CVec3& CAABB::GetMax() const
{
	return m_vec3Max;
}

const CVec3 CAABB::GetCentre() const
{
	return 0.5 * (m_vec3Min + m_vec3Max);
}

const CVec3 CAABB::GetExtent() const
{
	return 0.5 * (m_vec3Max - m_vec3Min);
}

CAABB CombineAABB(const CAABB& _krAABB1, const CAABB& _krAABB2)
{
	CVec3 vec3CombinedMin;
	CVec3 vec3CombinedMax;
	const CVec3& vec3Min1 = _krAABB1.GetMin();
	const CVec3& vec3Min2 = _krAABB2.GetMin();
	const CVec3& vec3Max1 = _krAABB1.GetMax();
	const CVec3& vec3Max2 = _krAABB2.GetMax();

	// Get the minimum x, y and z values from the two minimums.
	vec3CombinedMin.x = std::min(vec3Min1.x, vec3Min2.x);
	vec3CombinedMin.y = std::min(vec3Min1.y, vec3Min2.y);
	vec3CombinedMin.z = std::min(vec3Min1.z, vec3Min2.z);

	// Get the maximum x, y and z values from the two maximums.
	vec3CombinedMax.x = std::max(vec3Max1.x, vec3Max2.x);
	vec3CombinedMax.y = std::max(vec3Max1.y, vec3Max2.y);
	vec3CombinedMax.z = std::max(vec3Max1.z, vec3Max2.z);

	return CAABB(vec3CombinedMin, vec3CombinedMax);
}

CAABB TransformAABB(const CMatrix4x4& _krMatrix, const CAABB& _krAABB)
{
	const CVec3& vec3Min = _krAABB.GetMin();
	const CVec3& vec3Max = _krAABB.GetMax();
	CVec3 vec3MinNew = _krMatrix.GetPosition();
	CVec3 vec3MaxNew = _krMatrix.GetPosition();

	float fA = 0.0f;
	float fB = 0.0f;

	for (int i = 0; i < 3; ++i) {
		fA = _krMatrix.m[0][i] * vec3Min.x;
		fB = _krMatrix.m[0][i] * vec3Max.x;

		if (fA < fB) {
			vec3MinNew.x += fA;
			vec3MaxNew.x += fB;
		}
		else {
			vec3MinNew.x += fB;
			vec3MaxNew.x += fA;
		}

		fA = _krMatrix.m[1][i] * vec3Min.y;
		fB = _krMatrix.m[1][i] * vec3Max.y;

		if (fA < fB) {
			vec3MinNew.y += fA;
			vec3MaxNew.y += fB;
		}
		else {
			vec3MinNew.y += fB;
			vec3MaxNew.y += fA;
		}

		fA = _krMatrix.m[2][i] * vec3Min.z;
		fB = _krMatrix.m[2][i] * vec3Max.z;

		if (fA < fB) {
			vec3MinNew.z += fA;
			vec3MaxNew.z += fB;
		}
		else {
			vec3MinNew.z += fB;
			vec3MaxNew.z += fA;
		}
	}

	return CAABB(vec3MinNew, vec3MaxNew);

	/*CVec3 vec3Min = _krMatrix.Transform(_krAABB.GetMin());
	CVec3 vec3Max = _krMatrix.Transform(_krAABB.GetMax());

	CVec3 vec3MinNew;
	CVec3 vec3MaxNew;

	vec3MinNew.x = min(vec3Min.x, vec3Max.x);
	vec3MinNew.y = min(vec3Min.y, vec3Max.y);
	vec3MinNew.z = min(vec3Min.z, vec3Max.z);

	vec3MaxNew.x = max(vec3Min.x, vec3Max.x);
	vec3MaxNew.y = max(vec3Min.y, vec3Max.y);
	vec3MaxNew.z = max(vec3Min.z, vec3Max.z);

	return CAABB(vec3MinNew, vec3MaxNew);*/
}

