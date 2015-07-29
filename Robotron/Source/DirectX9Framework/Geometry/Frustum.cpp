// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Frustum.cpp
// Description	: CFrustum implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Frustum.h"

// Local Includes


CFrustum::CFrustum()
	:m_fFOV(ToRadians(90.0f)),	// Default FOV, of 90 degrees.
	m_fAspectRatio(1.0f),		// Default aspect of 1.0f
	m_fNear(0.1f),			// Default near plane of 1 metre.
	m_fFar(1000.0f)		// Default far plane of 1000 metres.
{

}

CFrustum::~CFrustum()
{

}

void CFrustum::Initialize(float _fFOV, float _fAspectRatio, float _fNear, float _fFar)
{
	m_fFOV = _fFOV;
	m_fAspectRatio = _fAspectRatio;
	m_fNear = _fNear;
	m_fFar = _fFar;

	float tanFOVOver2 = tan(m_fFOV / 2.0f);
	CVec3 vec3NearRight = (m_fNear * tanFOVOver2) * m_fAspectRatio * g_kvec3Right;
	CVec3 vec3NearUp = (m_fNear * tanFOVOver2) * g_kvec3Up;
	CVec3 vec3FarRight = (m_fFar * tanFOVOver2) * m_fAspectRatio * g_kvec3Right;
	CVec3 vec3FarUp = (m_fFar * tanFOVOver2) * g_kvec3Up;

	// T = Top, R = Right, L = Left, B = Bottom.
	CVec3 vec3NearClipTL = (m_fNear * g_kvec3Foward) - vec3NearRight + vec3NearUp;
	CVec3 vec3NearClipTR = (m_fNear * g_kvec3Foward) + vec3NearRight + vec3NearUp;
	CVec3 vec3NearClipBR = (m_fNear * g_kvec3Foward) + vec3NearRight - vec3NearUp;
	CVec3 vec3NearClipBL = (m_fNear * g_kvec3Foward) - vec3NearRight - vec3NearUp;

	CVec3 vec3FarClipTL = (m_fFar * g_kvec3Foward) - vec3FarRight + vec3FarUp;
	CVec3 vec3FarClipTR = (m_fFar * g_kvec3Foward) + vec3FarRight + vec3FarUp;
	CVec3 vec3FarClipBR = (m_fFar * g_kvec3Foward) + vec3FarRight - vec3FarUp;
	CVec3 vec3FarClipBL = (m_fFar * g_kvec3Foward) - vec3FarRight - vec3FarUp;

	m_pVerts[0] = vec3NearClipTL;
	m_pVerts[1] = vec3NearClipTR;
	m_pVerts[2] = vec3NearClipBR;
	m_pVerts[3] = vec3NearClipBL;

	m_pVerts[4] = vec3FarClipTL;
	m_pVerts[5] = vec3FarClipTR;
	m_pVerts[6] = vec3FarClipBR;
	m_pVerts[7] = vec3FarClipBL;

	CVec3 vec3Origin(0.0f, 0.0f, 0.0f);

	// Construct six planes to define the frustum.
	m_pPlanes[SIDE_NEAR].Initialise(m_fNear * g_kvec3Foward, g_kvec3Foward);	// Point away from us.
	m_pPlanes[SIDE_FAR].Initialise(m_fFar * g_kvec3Foward, -g_kvec3Foward);		// Point towards us.
	m_pPlanes[SIDE_LEFT].Initialise(vec3FarClipTL, vec3FarClipBL, vec3Origin);
	m_pPlanes[SIDE_RIGHT].Initialise(vec3FarClipBR, vec3FarClipTR, vec3Origin);
	m_pPlanes[SIDE_TOP].Initialise(vec3FarClipTR, vec3FarClipTL, vec3Origin);
	m_pPlanes[SIDE_BOTTOM].Initialise(vec3FarClipBL, vec3FarClipBR, vec3Origin);
}

void CFrustum::SetFOV(float _fFOV)
{
	Initialize(_fFOV, m_fAspectRatio, m_fNear, m_fFar);
}

void CFrustum::SetAspectRatio(float _fAspectRatio)
{
	Initialize(m_fFOV, _fAspectRatio, m_fNear, m_fFar);
}

void CFrustum::SetNear(float _fNear)
{
	Initialize(m_fFOV, m_fAspectRatio, _fNear, m_fFar);
}

void CFrustum::SetFar(float _fFar)
{
	Initialize(m_fFOV, m_fAspectRatio, m_fNear, _fFar);
}


const CPlane& CFrustum::GetPlane(ESide _eSide) const
{
	return m_pPlanes[_eSide];
}

const CVec3* CFrustum::GetVerts() const
{
	return m_pVerts;
}

float CFrustum::GetFOV() const
{
	return m_fFOV;
}

float CFrustum::GetAspectRatio() const
{
	return m_fAspectRatio;
}

float CFrustum::GetNear() const
{
	return m_fNear;
}

float CFrustum::GetFar() const
{
	return m_fFar;
}

bool IsInsideFrustum(const CFrustum& _krFrustum, const CVec3& _krPoint)
{
	for (int i = 0; i < CFrustum::NUM_SIDES; i++) {
		const CPlane& side = _krFrustum.GetPlane(static_cast<CFrustum::ESide>(i));

		if (!IsInsidePlane(side, _krPoint)) {
		// Is the point outside a side?
			return false;
		}
	}

	return true;
}

bool IsInsideFrustum(const CFrustum& _krFrustum, const CAABB& _krAABB, const CMatrix4x4& _krmatView)
{
	for (int i = 0; i < CFrustum::NUM_SIDES; i++) {
		CPlane side = _krFrustum.GetPlane(static_cast<CFrustum::ESide>(i));
		const CVec3& vec3Plane = side.GetNormal();

		CVec3 vec3PlaneAbs;
		vec3PlaneAbs.x = abs(vec3Plane.x);
		vec3PlaneAbs.y = abs(vec3Plane.y);
		vec3PlaneAbs.z = abs(vec3Plane.z);

		float fD = DotProduct(vec3Plane, _krAABB.GetCentre());
		fD += DotProduct(vec3PlaneAbs, _krAABB.GetExtent());
		fD += side.GetConstant();

		if (fD < 0) {
			std::string output = "Failed frustum check on side: ";
			CFrustum::ESide eSide = static_cast<CFrustum::ESide>(i);

			switch (eSide) {
				case CFrustum::SIDE_NEAR: {
					output += "Near\n";
					break;
				}
				case CFrustum::SIDE_FAR: {
					output += "Far\n";
					break;
				}
				case CFrustum::SIDE_LEFT: {
					output += "Left\n";
					break;
				}
				case CFrustum::SIDE_RIGHT: {
					output += "Right\n";
					break;
				}
				case CFrustum::SIDE_TOP: {
					output += "Top\n";
					break;
				}
				case CFrustum::SIDE_BOTTOM: {
					output += "Bottom\n";
					break;
				}
			}

			//OutputDebugStringA(output.c_str());
 			return false;
		}
	}

	return true;
}
