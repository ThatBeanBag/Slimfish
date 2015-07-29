// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BulletMotionState.cpp
// Description	: CBulletMotionState implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "BulletMotionState.h"

// Local Includes
#include "BulletSDKPhysics.h"

using namespace BulletConversions;

CBulletMotionState::CBulletMotionState(const CMatrix4x4& _krmatInitialTransform, const CMatrix4x4& _krmatRelativeTransform)
	:m_matWorldTransform(_krmatInitialTransform)
{
	// Extract the scale from the transform as rigid bodies do not have scale so we have to store this for our own retrieval.
	m_vec3Scale = m_matWorldTransform.GetScale();

	// Remove the scale from the transform, the rigid body never moves the scale will stay 
	// and the object will grow or shrink undesirably.
	CMatrix4x4 matScaleInversed(CMatrix4x4::s_kIdentity);
	if (m_vec3Scale.x != 0.0f && m_vec3Scale.y != 0.0f && m_vec3Scale.z != 0) {
		matScaleInversed = BuildScale(1.0f / m_vec3Scale.x, 1.0f / m_vec3Scale.y, 1.0f / m_vec3Scale.z);
	}

	CVec3 vec3Position = _krmatRelativeTransform.GetPosition() + m_matWorldTransform.GetPosition();
	CMatrix4x4 matRotOnly = _krmatRelativeTransform;
	CMatrix4x4 matRotOnly2 = m_matWorldTransform;
	matRotOnly.SetPosition(CVec3(0, 0, 0));
	matRotOnly2.SetPosition(CVec3(0, 0, 0));

	m_matWorldTransform = matScaleInversed * matRotOnly * matRotOnly2 * BuildTranslation(vec3Position);
	m_matRelativeTransform = _krmatRelativeTransform.Inversed();
}

CBulletMotionState::~CBulletMotionState()
{

}

void CBulletMotionState::getWorldTransform(btTransform& _rbtmatWorldTransform) const
{
	_rbtmatWorldTransform = Matrix4x4ToBulletTransform(m_matWorldTransform);
}

void CBulletMotionState::setWorldTransform(const btTransform& _krbtmatWorldTransform)
{
	m_matWorldTransform = BulletTransformToMatrix4x4(_krbtmatWorldTransform);
}

const CMatrix4x4 CBulletMotionState::GetWorldTransform() const
{
	CVec3 vec3Position = m_matRelativeTransform.GetPosition() + m_matWorldTransform.GetPosition();
	CMatrix4x4 matRotOnly = m_matRelativeTransform;
	CMatrix4x4 matRotOnly2 = m_matWorldTransform;
	matRotOnly.SetPosition(CVec3(0, 0, 0));
	matRotOnly2.SetPosition(CVec3(0, 0, 0));
	
	return BuildScale(m_vec3Scale) * matRotOnly * matRotOnly2 * BuildTranslation(vec3Position);
	//return BuildScale(m_vec3Scale) * m_matWorldTransform /** m_matRelativeTransform*/;
}

void CBulletMotionState::SetWorldTransform(const CMatrix4x4& _krmatWorldTransform)
{
	// Extract the scale from the transform as rigid bodies do not have scale so we have to store this for our own retrieval.
	m_vec3Scale = _krmatWorldTransform.GetScale();

	CMatrix4x4 matRelativeTransform = m_matRelativeTransform.Inversed();
	m_matWorldTransform = _krmatWorldTransform;

	// Remove the scale from the transform, the rigid body never moves the scale will stay 
	// and the object will grow or shrink undesirably.
	CMatrix4x4 matScaleInversed(CMatrix4x4::s_kIdentity);
	if (m_vec3Scale.x != 0.0f && m_vec3Scale.y != 0.0f && m_vec3Scale.z != 0) {
		matScaleInversed = BuildScale(1.0f / m_vec3Scale.x, 1.0f / m_vec3Scale.y, 1.0f / m_vec3Scale.z);
	}

	CVec3 vec3Position = matRelativeTransform.GetPosition() + m_matWorldTransform.GetPosition();
	CMatrix4x4 matRotOnly = matRelativeTransform;
	CMatrix4x4 matRotOnly2 = m_matWorldTransform;
	matRotOnly.SetPosition(CVec3(0, 0, 0));
	matRotOnly2.SetPosition(CVec3(0, 0, 0));

	m_matWorldTransform = matScaleInversed * matRotOnly * matRotOnly2 * BuildTranslation(vec3Position);
}

const CVec3 CBulletMotionState::GetScale() const
{
	return m_vec3Scale;
}

void CBulletMotionState::SetScale(const CVec3& _krvec3Scale)
{
	m_vec3Scale = _krvec3Scale;
}
