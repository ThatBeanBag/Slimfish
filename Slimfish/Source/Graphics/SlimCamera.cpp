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
#include "SlimRenderer.h"

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

	const CRay CCamera::ScreenPointToRay(const CPoint<>& point) const
	{
		auto screenSize = g_pApp->GetRenderer()->GetWindowSize();
		const auto& projectionMatrix = CFrustum::GetProjectionMatrix();

		// Convert mouse cursor to normalised device coordinates (between -1, 1) and then to view space.
		CVector3 viewMousePos = {
			(((2.0f * point.GetX()) / static_cast<float>(screenSize.GetX())) - 1) / projectionMatrix[0][0],
			-(((2.0f * point.GetY()) / static_cast<float>(screenSize.GetY())) - 1) / projectionMatrix[1][1],
			-1.0f };

		const auto& inverseViewMatrix = GetWorldTransform();
		//inverseViewMatrix = inverseViewMatrix.GetInverse();

		CVector3 rayOrigin;
		CVector3 rayDirection;

		if (CFrustum::GetProjectionMode() == EProjectionMode::ORTHOGRAPHIC) {
			// Transform the mouse position to get into world space.
			rayOrigin = inverseViewMatrix.Transform(viewMousePos);

			// In orthographic the direction of the ray is always the camera's direction.
			rayDirection = inverseViewMatrix.GetDirection();
		}
		else {	// Projection mode perspective.
			// Project the world space mouse position onto the orthogonal basis of the inverse view matrix.
			// To get the direction of the ray, then normalize the result.
			rayDirection.SetX(viewMousePos.GetX() * inverseViewMatrix[0][0] + viewMousePos.GetY() * inverseViewMatrix[0][1] + viewMousePos.GetZ() * inverseViewMatrix[0][2]);
			rayDirection.SetY(viewMousePos.GetX() * inverseViewMatrix[1][0] + viewMousePos.GetY() * inverseViewMatrix[1][1] + viewMousePos.GetZ() * inverseViewMatrix[1][2]);
			rayDirection.SetZ(viewMousePos.GetX() * inverseViewMatrix[2][0] + viewMousePos.GetY() * inverseViewMatrix[2][1] + viewMousePos.GetZ() * inverseViewMatrix[2][2]);
			rayDirection = CVector3::Normalise(rayDirection);
			/*auto viewMatrix = inverseViewMatrix.GetTranspose();
			viewMatrix.SetPosition(CVector3::s_ZERO);*/
			/*rayDirection.SetX(CVector3::DotProduct(viewMousePos, inverseViewMatrix.GetRight()));
			rayDirection.SetY(CVector3::DotProduct(viewMousePos, inverseViewMatrix.GetUp()));
			rayDirection.SetZ(CVector3::DotProduct(viewMousePos, inverseViewMatrix.GetDirection()));
			rayDirection = CVector3::Normalise(rayDirection);*/

			// In perspective no matter where you click the origin of the ray is always the camera's position.
			rayOrigin = inverseViewMatrix.GetPosition();
		}

		return CRay(rayOrigin, rayDirection);
	}

	bool CCamera::IsInView(const CAxisAlignedBoundingBox& aabb) const
	{
		return CFrustum::IsInside(aabb);
	}

	void CCamera::SetPerspective(float radFieldOfView, float aspectRatio, float nearClipDistance, float farClipDistance)
	{
		CFrustum::SetFieldOfView(radFieldOfView);
		CFrustum::SetAspectRatio(aspectRatio);
		CFrustum::SetNearClipDistance(nearClipDistance);
		CFrustum::SetFarClipDistance(farClipDistance);
		CFrustum::SetProjectionMode(EProjectionMode::PERSPECTIVE);
	}

	void CCamera::SetOrthographic(float orthoSize, float aspectRatio, float nearClipDistance, float farClipDistance)
	{
		CFrustum::SetOrthographicSize(orthoSize);
		CFrustum::SetAspectRatio(aspectRatio);
		CFrustum::SetNearClipDistance(nearClipDistance);
		CFrustum::SetFarClipDistance(farClipDistance);
		CFrustum::SetProjectionMode(EProjectionMode::ORTHOGRAPHIC);
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
			CVector3 targetPosition = m_pTarget->GetPosition() + (m_TargetTranslationOffset * m_pTarget->GetRotation());
			SetPosition(targetPosition);

			if (m_TrackRotation) {
				// Concatenate rotations.
				CQuaternion targetRotation = m_pTarget->GetRotation() * m_TargetRotationOffset;
				SetRotation(targetRotation);
			}
		}

		// Inverse the position and rotation of the camera to construct a view matrix.
		CQuaternion viewRotation = GetRotation();
		CVector3 viewPosition = -GetPosition();

		CMatrix4x4 rotation = viewRotation.ToRotationMatrix().GetTranspose();
		CMatrix4x4 translation = CMatrix4x4::BuildTranslation(viewPosition);

		// Set the view matrix internally.
		//CFrustum::SetViewMatrix(CMatrix4x4::BuildTransform(viewPosition, CVector3::s_ONE, viewRotation));
		CFrustum::SetViewMatrix(rotation * translation);	// Have to use opposite ordering for inverse-ness.
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

	const CMatrix4x4 CCamera::GetViewProjMatrix() const
	{
		return CFrustum::GetProjectionMatrix() * CFrustum::GetViewMatrix();
	}

	const bool CCamera::HasTarget() const
	{
		return m_pTarget != nullptr;
	}

}

