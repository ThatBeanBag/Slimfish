// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Camera.cpop
// Description	: CCameraNode implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH 
#include "GameStd.h"

// Library Includes

// This Include
#include "CameraNode.h"

// Local Includes

CCameraNode::CCameraNode()
	:CSceneNode(CActor::s_kINVALID_ACTOR_ID),
	m_matProjection(CMatrix4x4::s_kIdentity),
	m_fVolumeWidth(0.0f),
	m_fVolumeHeight(0.0f),
	m_eProjectionMode(PROJECTION_ORTHOGRAPHIC),
	m_matView(CMatrix4x4::s_kIdentity),
	m_pTarget(shared_ptr<CSceneNode>()),
	m_bIsAbsoluteRotation(false)
{

}

CCameraNode::~CCameraNode()
{

}

bool CCameraNode::VOnRestore()
{
	SetProjectionTransform(m_frustum.GetFOV(), 
						   static_cast<float>(g_pApp->GetScreenWidth()),
						   static_cast<float>(g_pApp->GetScreenHeight()),
						   m_frustum.GetNear(), 
						   m_frustum.GetFar(),
						   m_eProjectionMode);

	return true;
}

void CCameraNode::SetViewTransform()
{
	CMatrix4x4 matTarget = m_matTransform;
	CMatrix4x4 matRelativeRotation = matTarget;
	CVec3 vec3Position = matTarget.GetPosition();
	CVec3 vec3Direction = matTarget.GetForward();

	if (m_pTarget) {
	// Do we have a target?
		matTarget = m_pTarget->GetTransformMatrix();

		CVec3 vec3WorldOffset;
		matRelativeRotation = BuildYawPitchRoll(m_vec3RelativeRotation.x,
												m_vec3RelativeRotation.y,
												m_vec3RelativeRotation.z);

		if (m_bIsAbsoluteRotation) {
		// Is the relative rotation actually absolute?
			matRelativeRotation = matRelativeRotation;
		}
		else {
		// Relative rotation is relative.
			// Get the rotation only matrix of the target.
			CMatrix4x4 matRotationOnly = matTarget;
			matRotationOnly.SetPosition(0.0f, 0.0f, 0.0f);

			// Concatenate the rotations.
			matRelativeRotation = matRotationOnly * matRelativeRotation;
		}

		// Transform the offset by the target's rotation matrix.
		vec3WorldOffset = matRelativeRotation.Transform(m_vec3RelativeOffset);
		vec3Position = matTarget.GetPosition() + vec3WorldOffset;
		vec3Direction = matRelativeRotation.GetForward();
	}
	else {
	// No target.
	}

	m_matTransform = matRelativeRotation;
	m_matTransform.SetPosition(vec3Position);

	CVec3 vec3LookAt = vec3Position + vec3Direction;

	// Build the view matrix.
	m_matView = BuildLookAt(vec3Position, vec3LookAt, g_kvec3Up);
	
	// Set the view transform of the renderer.
	g_pApp->GetRenderer()->VSetViewTransform(&m_matView);
}

void CCameraNode::SetTarget(shared_ptr<CSceneNode> _pTarget)
{
	m_pTarget = _pTarget;
}

void CCameraNode::SetOffsetFromTarget(const CVec3& _krOffset)
{
	m_vec3RelativeOffset = _krOffset;
}

void CCameraNode::SetOffsetFromTarget(float _fX, float _fY, float _fZ)
{
	m_vec3RelativeOffset = CVec3(_fX, _fY, _fZ);
}

void CCameraNode::SetRotationFromTarget(const CVec3& _krRotation)
{
	m_vec3RelativeRotation = _krRotation;
}

void CCameraNode::SetRotationFromTarget(float _fX, float _fY, float _fZ)
{
	m_vec3RelativeRotation = CVec3(_fX, _fY, _fZ);
}

void CCameraNode::SetAbsoluteRotation(bool _bIsAbsoluteRotation)
{
	m_bIsAbsoluteRotation = _bIsAbsoluteRotation;
}

void CCameraNode::SetProjectionTransform(float _fFOV,
										 float _fVolumeWidth, float _fVolumeHeight,
										 float _fNear, float _fFar,
										 ECameraProjectionMode _eProjectionMode)
{
	// Set internal values.
	m_eProjectionMode = _eProjectionMode;
	m_fVolumeWidth = _fVolumeWidth;
	m_fVolumeHeight = _fVolumeHeight;

	if (m_fVolumeHeight == 0) {
		// Don't divide by 0.
		return;
	}

	// Get the aspect ratio from the view volume width and height.
	float fAspectRatio = m_fVolumeWidth / m_fVolumeHeight;
	m_frustum.Initialize(_fFOV, fAspectRatio, _fNear, _fFar);

	if (m_eProjectionMode == PROJECTION_ORTHOGRAPHIC) {
	// Is the projection mode orthographic?
		// Build an orthographic projection matrix.
		m_matProjection = BuildOrthographic(m_fVolumeWidth, m_fVolumeHeight, _fNear, _fFar);
	}
	else if (m_eProjectionMode == PROJECTION_PERSPECTIVE) {
	// Is the projection mode perspective.

		// Build a perspective projection matrix.
		m_matProjection = BuildPerspective(_fFOV, fAspectRatio, _fNear, _fFar);
	}
	else {
	// The projection mode is invalid.
		return;
	}

	// Set the projection transform of the renderer.
	g_pApp->GetRenderer()->VSetProjectionTransform(&m_matProjection);
}

const CMatrix4x4& CCameraNode::GetProjectionMatrix()
{
	return m_matProjection;
}

const CFrustum& CCameraNode::GetFrustum()
{
	return m_frustum;
}

float CCameraNode::GetFOV() const
{
	return m_frustum.GetFOV();
}

float CCameraNode::GetViewVolumeWidth() const
{
	return m_fVolumeWidth; // CS: Viewport.
}

float CCameraNode::GetViewVolumeHeight() const
{
	return m_fVolumeHeight;
}

float CCameraNode::GetNearClippingPlane() const
{
	return m_frustum.GetNear();
}

float CCameraNode::GetFarClippingPlane() const
{
	return m_frustum.GetFar();
}

ECameraProjectionMode CCameraNode::GetProjectionMode() const
{
	return m_eProjectionMode;
}

const CMatrix4x4& CCameraNode::GetViewMatrix()
{
	return m_matView;
}