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

// This Include
#include "CollisionShape.h"

// Local Includes

ACollisionShape::ACollisionShape()
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

const bool CCollisionSphere::IsInside(const CVector3& point)
{
	const CMatrix4x4& transform = GetTransform();
	CVector3 toPoint = transform.GetPosition() - point;
	return point.GetLengthSquared() <= m_fRadius * m_fRadius;
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

const bool CCollisionCapsule::IsInside(const CVector3& point)
{
	return false;
}

const ACollisionShape::EType CCollisionCapsule::VGetType() const
{
	return ACollisionShape::EType::CAPSULE;
}

void CCollisionCapsule::SetRadius(float radius)
{
	m_fRadius = radius;
}

const float CCollisionCapsule::GetRadius() const
{
	return m_fRadius;
}

void CCollisionCapsule::SetHeight(float height)
{
	m_fHeight = height;
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

const bool CCollisionBox::IsInside(const CVector3& point)
{
	float halfHeight = m_fHeight / 2.0f;
	float halfWidth = m_fWidth / 2.0f;
	float halfLength = m_fLength / 2.0f;

	CPlane s_Plane[] = {
			{ 0.0f, 1.0f, 0.0f, halfHeight	},
			{ 0.0f, -1.0f, 0.0f, -halfHeight},
			{ 1.0f, 0.0f, 0.0f,  halfWidth	},
			{ -1.0f, 0.0f, 0.0f, -halfWidth },
			{ 0.0f, 0.0f, 1.0f, -halfHeight },
			{ 0.0f, 0.0f, -1.0f, halfHeight }
	};


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
}

const float CCollisionBox::GetWidth() const
{
	return m_fWidth;
}

void CCollisionBox::SetHeight(float height)
{
	m_fHeight = height;
}

const float CCollisionBox::GetHeight() const
{
	return m_fHeight;
}

void CCollisionBox::SetLength(float length)
{
	m_fLength = length;
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

const bool CCollisionCylinder::IsInside(const CVector3& point)
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

const bool CCollisionPlane::IsInside(const CVector3& point)
{
	//return m_Plane.IsInside(point);
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
{

}

CCollisionPyramid::~CCollisionPyramid()
{

}

const bool CCollisionPyramid::IsInside(const CVector3& point)
{

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
}

const float CCollisionPyramid::GetWidth() const
{
	return m_Width;
}

void CCollisionPyramid::SetHeight(float height)
{
	m_Height = height;
}

const float CCollisionPyramid::GetHeight() const
{
	return m_Height;
}

void CCollisionPyramid::SetLength(float length)
{
	m_Length = length;
}

const float CCollisionPyramid::GetLength() const
{
	return m_Length;
}
