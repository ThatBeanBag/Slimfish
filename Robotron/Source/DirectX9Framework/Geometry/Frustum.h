// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Frustum.h
// Description	: CFrustum declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

// Library Includes

// Local Includes
#include "Plane.h"
#include "AABB.h"

class CFrustum {
public:
	enum ESide {
		SIDE_NEAR,
		SIDE_FAR,
		SIDE_LEFT,
		SIDE_RIGHT,
		SIDE_TOP,
		SIDE_BOTTOM,
		NUM_SIDES
	};

	// Member Functions
public:
	CFrustum();
	~CFrustum();

	void Initialize(float _fFOV, float _fAspectRatio, float _fNearPlane, float _fFarPlane);

	void SetFOV(float _fFOV);
	void SetAspectRatio(float _fAspectRatio);
	void SetNear(float _fNear);
	void SetFar(float _fFar);

	const CPlane& GetPlane(ESide _eSide) const;
	const CVec3* GetVerts() const;
	float GetFOV() const;
	float GetAspectRatio() const;
	float GetNear() const;
	float GetFar() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	CPlane m_pPlanes[NUM_SIDES];
	CVec3 m_pVerts[8];

	float m_fFOV;		// The field of view of the perspective projection matrix.
	float m_fAspectRatio;
	float m_fNear;
	float m_fFar;
};

bool IsInsideFrustum(const CFrustum& _krFrustum, const CVec3& _krPoint);
bool IsInsideFrustum(const CFrustum& _krFrustum, const CAABB& _krAABB, const CMatrix4x4& _krmatView);

#endif	// __FRUSTRUM_H__