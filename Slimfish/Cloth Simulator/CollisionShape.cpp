// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: collision_shape.cpp
// Description	: Ccollision_shape implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ClothSimulatorStd.h"

// Library Includes
#include <limits>

// This Include
#include "CollisionShape.h"

// Local Includes

ACollisionShape::ACollisionShape()
	:m_NeedsRebuilding(true),
	m_Transform(CMatrix4x4::s_IDENTITY)
{

}

/************************************************************************/
/* Collision Shape.
/************************************************************************/
ACollisionShape::~ACollisionShape()
{

}

void ACollisionShape::SetTransform(const CMatrix4x4& transform)
{
	m_Transform = transform;
	m_NeedsRebuilding = true;
}

const CMatrix4x4& ACollisionShape::GetTransform() const
{
	return m_Transform;
}

/************************************************************************/
/* Collision Sphere
/************************************************************************/

CCollisionSphere::CCollisionSphere(float _fRadius) 
	:m_fRadius(_fRadius)
{

}

CCollisionSphere::~CCollisionSphere()
{

}

const bool CCollisionSphere::IsInside(const CVector3& point, CVector3& intersectionPoint)
{
	const CMatrix4x4& transform = GetTransform();

	// Calculate the distance between the point and the sphere.
	CVector3 toPoint = point - transform.GetPosition();
	auto distanceSqr = toPoint.GetLengthSquared();

	// Is the point within the radius of the sphere.
	if (distanceSqr <= m_fRadius * m_fRadius) {
		// Get point outside of the sphere.
		if (distanceSqr != 0.0f) {
			auto distance = sqrt(distanceSqr);
			toPoint /= distance;
			intersectionPoint = transform.GetPosition() + toPoint * m_fRadius;
		}
		else {
			intersectionPoint = CVector3::s_FORWARD * m_fRadius;
		}

		return true;
	}

	return false;
}

const ACollisionShape::EType CCollisionSphere::VGetType() const
{
	return ACollisionShape::EType::SPHERE;
}

const float CCollisionSphere::VGetVolume() const
{
	return Math::s_PI * (4.0f / 3.0f) * m_fRadius * m_fRadius * m_fRadius;
}

void CCollisionSphere::SetRadius(float radius)
{
	m_fRadius = radius;
	m_NeedsRebuilding = true;
}

const float CCollisionSphere::GetRadius() const
{
	return m_fRadius;
}


/************************************************************************/
/* Collision Capsule
/************************************************************************/

CCollisionCapsule::CCollisionCapsule(float _fRadius, float _fHeight) 
	:m_fRadius(_fRadius),
	m_fHeight(_fHeight)
{

}

CCollisionCapsule::~CCollisionCapsule()
{

}

const bool CCollisionCapsule::IsInside(const CVector3& point, CVector3& intersectionPoint)
{
	const auto& transform = GetTransform();
	auto lineNormal = transform.GetUp();

	// Project the point onto the line.
	float projection = CVector3::DotProduct(point, lineNormal);
	float centre = CVector3::DotProduct(transform.GetPosition(), lineNormal);

	projection -= centre;

	float halfHeight = m_fHeight / 2.0f;

	if (projection > halfHeight) {
		projection = halfHeight;
	}
	else if (projection < -halfHeight) {
		projection = -halfHeight;
	}

	auto pointOnLine = transform.GetPosition() + (lineNormal * projection);
	auto toPoint = point - pointOnLine;
	auto distanceSqr = toPoint.GetLengthSquared();

	// Is the point within the radius of the capsule.
	if (distanceSqr <= m_fRadius * m_fRadius) {
		// Get point outside of the capsule.
		if (distanceSqr != 0.0f) {
			auto distance = sqrt(distanceSqr);
			toPoint /= distance;
			intersectionPoint = pointOnLine + toPoint * m_fRadius;
		}
		else {
			intersectionPoint = CVector3::s_FORWARD * m_fRadius;
		}

		return true;
	}


	return false;
}

const ACollisionShape::EType CCollisionCapsule::VGetType() const
{
	return ACollisionShape::EType::CAPSULE;
}

void CCollisionCapsule::SetRadius(float radius)
{
	m_fRadius = radius;
	m_NeedsRebuilding = true;
}

const float CCollisionCapsule::GetRadius() const
{
	return m_fRadius;
}

void CCollisionCapsule::SetHeight(float height)
{
	m_fHeight = height;
	m_NeedsRebuilding = true;
}

const float CCollisionCapsule::GetHeight() const
{
	return m_fHeight;
}

const float CCollisionCapsule::VGetVolume() const
{
	return Math::s_PI * m_fRadius * m_fRadius * ((4.0f / 3.0f) * m_fRadius + m_fHeight);
}

/************************************************************************/
/* Collision Box
/************************************************************************/

CCollisionBox::CCollisionBox(float _fWidth, float _fHeight, float _fLength) 
	:m_fWidth(_fWidth),
	m_fHeight(_fHeight),
	m_fLength(_fLength)
{

}

CCollisionBox::~CCollisionBox()
{

}

const bool CCollisionBox::IsInside(const CVector3& point, CVector3& intersectionPoint)
{
	return false;
}

const ACollisionShape::EType CCollisionBox::VGetType() const
{
	return ACollisionShape::EType::BOX;
}

const float CCollisionBox::VGetVolume() const
{
	return m_fWidth * m_fHeight * m_fLength;
}

void CCollisionBox::SetWidth(float width)
{
	m_fWidth = width;
	m_NeedsRebuilding = true;
}

const float CCollisionBox::GetWidth() const
{
	return m_fWidth;
}

void CCollisionBox::SetHeight(float height)
{
	m_fHeight = height;
	m_NeedsRebuilding = true;
}

const float CCollisionBox::GetHeight() const
{
	return m_fHeight;
}

void CCollisionBox::SetLength(float length)
{
	m_fLength = length;
	m_NeedsRebuilding = true;
}

const float CCollisionBox::GetLength() const
{
	return m_fLength;
}


/************************************************************************/
/* Collision Cylinder
/************************************************************************/

CCollisionCylinder::CCollisionCylinder(float _fRadius, float _fHeight) 
	:m_fRadius(_fRadius),
	m_fHeight(_fHeight)
{

}

CCollisionCylinder::~CCollisionCylinder()
{

}

const bool CCollisionCylinder::IsInside(const CVector3& point, CVector3& intersectionPoint)
{
	return false;
}

const ACollisionShape::EType CCollisionCylinder::VGetType() const
{
	return ACollisionShape::EType::CYLINDER;
}

const float CCollisionCylinder::VGetVolume() const
{
	return Math::s_PI * m_fRadius * m_fRadius * m_fHeight;
}

void CCollisionCylinder::SetRadius(float radius)
{
	m_fRadius = radius;
}

const float CCollisionCylinder::GetRadius() const
{
	return m_fRadius;
}

void CCollisionCylinder::SetHeight(float height)
{
	m_fHeight = height;
}

const float CCollisionCylinder::GetHeight() const
{
	return m_fHeight;
}

/************************************************************************/
/* Collision Plane
/************************************************************************/

CCollisionPlane::CCollisionPlane(const CPlane& plane) 
	:m_Plane(plane)
{

}

CCollisionPlane::~CCollisionPlane()
{

}

const bool CCollisionPlane::IsInside(const CVector3& point, CVector3& intersectionPoint)
{
	if (m_Plane.IsInside(point)) {
		// Get the collision point
		auto planeNormal = m_Plane.GetNormal();
		auto pointOnPlane = planeNormal * -m_Plane.GetD();

		// Project the point onto the plane.
		intersectionPoint = point - (planeNormal * CVector3::DotProduct(point - pointOnPlane, planeNormal));

		return true;
	}

	return false;
}

const ACollisionShape::EType CCollisionPlane::VGetType() const
{
	return EType::PLANE;
}

const float CCollisionPlane::VGetVolume() const
{
	return 0.0f;
}

void CCollisionPlane::SetPlane(const CPlane& plane)
{
	m_Plane = plane;
}

const CPlane& CCollisionPlane::GetPlane() const
{
	return m_Plane;
}

CCollisionPyramid::CCollisionPyramid(float width, float height, float length)
	:m_Width(width), m_Height(height), m_Length(length)
{

}

CCollisionPyramid::~CCollisionPyramid()
{

}

const bool CCollisionPyramid::IsInside(const CVector3& point, CVector3& intersectionPoint)
{
	// Rebuild the planes if necessary.
	if (m_NeedsRebuilding) {
		const auto& transform = GetTransform();

		auto halfHeight = (m_Height / 2.0f) * transform.GetUp();
		auto halfWidth = (m_Width / 2.0f) * transform.GetRight();
		auto halfLength = (m_Length / 2.0f) * transform.GetDirection();

		CVector3 topPoint = transform.GetPosition() + halfHeight;
		CVector3 bottomCentre = transform.GetPosition() - halfHeight;

		CVector3 cornerLeftFront = bottomCentre - halfWidth + halfLength;
		CVector3 cornerLeftBack = bottomCentre - halfWidth - halfLength;
		CVector3 cornerRightFront = bottomCentre + halfWidth + halfLength;
		CVector3 cornerRightBack = bottomCentre + halfWidth - halfLength;

		// Create the planes.
		m_Planes[0] = CPlane(topPoint, cornerLeftFront, cornerLeftBack );		// Left side.
		m_Planes[1] = CPlane(topPoint, cornerRightFront,cornerLeftFront );		// Front side.
		m_Planes[2] = CPlane(topPoint, cornerRightBack ,cornerRightFront );	// Right side.
		m_Planes[3] = CPlane(topPoint, cornerLeftBack, cornerRightBack) ;	// Back side.
		m_Planes[4] = CPlane(cornerLeftFront, cornerRightFront, cornerLeftBack); // Bottom.

		m_NeedsRebuilding = false;
	}

	// Check to see if the point is outside any one of the planes.
	for (const auto& plane : m_Planes) {
		if (!plane.IsInside(point)) {
			return false;
		}
	}

	// If we get here the point is inside each plane.
	// Calculate the intersection point.
	CVector3 closestPlaneNormal = m_Planes[0].GetNormal();
	float distance = std::numeric_limits<float>::max();

	// Find the closest plane to the point.
	for (const auto& plane : m_Planes) {
		auto planeNormal = plane.GetNormal();
		auto pointOnPlane = planeNormal * -plane.GetD();
		auto currentDistance = CVector3::DotProduct(point - pointOnPlane, planeNormal);
		if (abs(currentDistance) < abs(distance)) {
			distance = currentDistance;
			closestPlaneNormal = planeNormal;
		}
	}

	// Project the point onto the closest plane.
	intersectionPoint = point - (closestPlaneNormal * distance);

	return true;
}

const ACollisionShape::EType CCollisionPyramid::VGetType() const
{
	return ACollisionShape::EType::PYRAMID;
}

const float CCollisionPyramid::VGetVolume() const
{
	return (m_Length * m_Width * m_Height) / 3.0f;
}

void CCollisionPyramid::SetWidth(float width)
{
	m_Width = width;
	m_NeedsRebuilding = true;
}

const float CCollisionPyramid::GetWidth() const
{
	return m_Width;
}

void CCollisionPyramid::SetHeight(float height)
{
	m_Height = height;
	m_NeedsRebuilding = true;
}

const float CCollisionPyramid::GetHeight() const
{
	return m_Height;
}

void CCollisionPyramid::SetLength(float length)
{
	m_Length = length;
	m_NeedsRebuilding = true;
}

const float CCollisionPyramid::GetLength() const
{
	return m_Length;
}
