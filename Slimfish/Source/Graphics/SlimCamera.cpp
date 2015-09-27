// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimCamera.cpp
// Description	: CSlimCamera implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimCamera.h"

// Local Includes

namespace Slim {

	CCamera::CCamera(CScene* pSceneCreator)
		:CSceneNode(pSceneCreator),
		m_pTarget(nullptr),
		m_TargetTranslationOffset(CVector3::s_ZERO),
		m_TargetRotationOffset(CVector3::s_ZERO),
		m_TrackRotation(true)
	{

	}

	CCamera::~CCamera()
	{

	}

	void CCamera::SetTarget(CSceneNode* pTarget, bool trackRotation /*= true*/, const CVector3& translationOffset /*= CVector3::s_ZERO*/, const CQuaternion& rotationOffset /*= CQuaternion::s_IDENTITY*/)
	{
		m_pTarget = pTarget;
		m_TargetTranslationOffset = translationOffset;
		m_TargetRotationOffset = rotationOffset;
		m_TrackRotation = trackRotation;
	}

	void CCamera::SetTargetTranslationOffset(const CVector3& targetTranslationOffset)
	{
		m_TargetTranslationOffset = targetTranslationOffset;
	}

	const CVector3& CCamera::GetTargetTranslationOffset() const
	{
		return m_TargetTranslationOffset;
	}

	void CCamera::SetTargetRotationOffset(const CQuaternion& targetRotationOffset)
	{
		m_TargetRotationOffset = targetRotationOffset;
	}

	const CQuaternion& CCamera::GetTargetRotationOffset() const
	{
		return m_TargetRotationOffset;
	}

	void CCamera::SetTrackRotation(bool trackRotation)
	{
		m_TrackRotation = trackRotation;
	}

	const bool CCamera::GetTrackRotation() const
	{
		return m_TrackRotation;
	}

	void CCamera::UpdateViewTransform()
	{
		// Update our position so if we are following a target.
		if (m_pTarget) {
			CVector3 targetPosition = m_pTarget->GetPosition() + m_TargetTranslationOffset;
			SetPosition(targetPosition);

			if (m_TrackRotation) {
				// Concatenate rotations.
				CQuaternion targetRotation = m_pTarget->GetRotation() * m_TargetRotationOffset;
				SetRotation(targetRotation);
			}
		}

		// Inverse the position and rotation of the camera to construct a view matrix.
		CQuaternion viewRotation = GetRotation().GetInverse();
		CVector3 viewPosition = -GetPosition();

		// Set the view matrix internally.
		CFrustum::SetViewMatrix(CMatrix4x4::BuildTransform(viewPosition, CVector3::s_ONE, viewRotation));
	}

	void CCamera::SetFieldOfView(float radFieldOfView)
	{
		CFrustum::SetFieldOfView(radFieldOfView);
	}

	const float CCamera::GetFieldOfView() const
	{
		return CFrustum::GetFieldOfView();
	}

	void CCamera::SetAspectRatio(float aspectRatio)
	{
		CFrustum::SetAspectRatio(aspectRatio);
	}

	const float CCamera::GetAspectRatio() const
	{
		return CFrustum::GetAspectRatio();
	}

	void CCamera::SetNearClipDistance(float zNear)
	{
		CFrustum::SetNearClipDistance(zNear);
	}

	const float CCamera::GetNearClipDistance() const
	{
		return CFrustum::GetNearClipDistance();
	}

	void CCamera::SetFarClipDistance(float zFar)
	{
		CFrustum::SetFarClipDistance(zFar);
	}

	const float CCamera::GetFarClipDistance() const
	{
		return CFrustum::GetFarClipDistance();
	}

	void CCamera::SetOrthographicSize(float orthographicSize)
	{
		CFrustum::SetOrthographicSize(orthographicSize);
	}

	const float CCamera::GetOrthographicSize() const
	{
		return CFrustum::GetOrthographicSize();
	}

	void CCamera::SetProjectionMode(EProjectionMode projectionMode)
	{
		CFrustum::SetProjectionMode(projectionMode);
	}

	const EProjectionMode CCamera::GetProjectionMode() const
	{
		return CFrustum::GetProjectionMode();
	}

	const CMatrix4x4& CCamera::GetProjectionMatrix() const
	{
		return CFrustum::GetProjectionMatrix();
	}

	const CMatrix4x4& CCamera::GetViewMatrix() const
	{
		return CFrustum::GetViewMatrix();
	}

}

