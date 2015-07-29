// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionShape.cpp
// Description	: CCollisionShape implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "CollisionShape.h"

// Local Includes


void ACollisionShape::SetRelativeTransform(const CMatrix4x4& _krmatRelativeRigidBodyTransform)
{
	m_matRelativeRigidBodyTransform = _krmatRelativeRigidBodyTransform;
}

const CMatrix4x4 ACollisionShape::GetRelativeTransform() const
{
	return m_matRelativeRigidBodyTransform;
}

void ACollisionShape::SetDensity(float _fDensity)
{
	m_fDensity = _fDensity;
}

void ACollisionShape::SetRestitution(float _fRestitution)
{
	m_fRestitution = _fRestitution;
}

void ACollisionShape::SetFriction(float _fFriction)
{
	m_fFriction = _fFriction;
}

const float ACollisionShape::GetDensity() const
{
	return m_fDensity;
}

const float ACollisionShape::GetRestitution() const
{
	return m_fRestitution;
}

const float ACollisionShape::GetFriction() const
{
	return m_fFriction;
}
