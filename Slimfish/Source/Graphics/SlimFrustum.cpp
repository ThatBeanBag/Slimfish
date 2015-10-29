// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimFrustum.cpp
// Description	: CFrustum implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimFrustum.h"

// Local Includes

namespace Slim {

	CFrustum::CFrustum()
		:m_ProjectionMode(EProjectionMode::PERSPECTIVE),
		m_FarClipDistance(1000.0f),
		m_NearClipDistance(1.0f),
		m_AspectRatio(1.0f),
		m_OrthographicSize(1.0f),
		m_radFieldOfView(90),
		m_CachedProjectionMatrixDirty(true)
	{

	}

	CFrustum::~CFrustum()
	{

	}

	bool CFrustum::IsInside(const CVector3& vertex) const
	{
		// Calculate the frustums in case they are invalid.
		CalculateFrustumSides();

		for (int i = 0; i < static_cast<int>(ESide::FS_MAX); ++i) {
			// If the point is outside any one of the planes then it's outside the frustum.
			if (!m_Sides[i].IsInside(vertex)) {
				return false;
			}
		}

		return true;
	}

	void CFrustum::SetFieldOfView(float radFieldOfView)
	{
		m_radFieldOfView = radFieldOfView;
		InvalidateProjectionMatrix();
	}

	const float CFrustum::GetFieldOfView() const
	{
		return m_radFieldOfView;
	}

	void CFrustum::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		InvalidateProjectionMatrix();
	}

	const float CFrustum::GetAspectRatio() const
	{
		return m_AspectRatio;
	}

	void CFrustum::SetNearClipDistance(float zNear)
	{
		m_NearClipDistance = zNear;
		InvalidateProjectionMatrix();
	}

	const float CFrustum::GetNearClipDistance() const
	{
		return m_NearClipDistance;
	}

	void CFrustum::SetFarClipDistance(float zFar)
	{
		m_FarClipDistance = zFar;
		InvalidateProjectionMatrix();
	}

	const float CFrustum::GetFarClipDistance() const
	{
		return m_FarClipDistance;
	}

	void CFrustum::SetOrthographicSize(float orthographicSize)
	{
		m_OrthographicSize = orthographicSize;
		InvalidateProjectionMatrix();
	}

	const float CFrustum::GetOrthographicSize() const
	{
		return m_OrthographicSize;
	}

	void CFrustum::SetProjectionMode(EProjectionMode projectionMode)
	{
		m_ProjectionMode = projectionMode;
		InvalidateProjectionMatrix();
	}

	const EProjectionMode CFrustum::GetProjectionMode() const
	{
		return m_ProjectionMode;
	}

	const CMatrix4x4& CFrustum::GetProjectionMatrix() const
	{
		if (m_CachedProjectionMatrixDirty) {
		// Do we need to rebuild the matrix?
			if (m_ProjectionMode == EProjectionMode::ORTHOGRAPHIC) {
				float aspectRatioReciprocal = 0.0f;

				if (m_AspectRatio != 0) {
					aspectRatioReciprocal = 1.0f / m_AspectRatio;
				}

				m_CachedProjectionMatrix = CMatrix4x4::BuildOrthographic(
					m_OrthographicSize * m_AspectRatio,
					m_OrthographicSize * aspectRatioReciprocal,
					m_NearClipDistance,
					m_FarClipDistance);
			}
			else {
				m_CachedProjectionMatrix = CMatrix4x4::BuildProjection(
					m_radFieldOfView, 
					m_AspectRatio, 
					m_NearClipDistance, 
					m_FarClipDistance);
			}

			// Projection matrix has been rebuilt.
			m_CachedProjectionMatrixDirty = false;
		}

		return m_CachedProjectionMatrix;
	}

	void CFrustum::SetViewMatrix(const CMatrix4x4& viewMatrix)
	{
		m_ViewMatrix = viewMatrix;
		InvalidateFrustumSides();
	}

	const CMatrix4x4& CFrustum::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	void CFrustum::InvalidateProjectionMatrix()
	{
		m_CachedProjectionMatrixDirty = true; 
		InvalidateFrustumSides();
	}

	void CFrustum::InvalidateFrustumSides()
	{
		m_FrustumSidesDirty = true;
	}

	void CFrustum::CalculateFrustumSides() const
	{
		if (m_FrustumSidesDirty) {
		// Only rebuild them if neccessary.
			// Concatenate projection and view using the function so they can 
			// rebuild themselves if necessary.
			CMatrix4x4 viewProj = GetProjectionMatrix() * GetViewMatrix();

			m_Sides[static_cast<int>(ESide::FS_NEAR)].SetA(viewProj[3][0] + viewProj[2][0]);
			m_Sides[static_cast<int>(ESide::FS_NEAR)].SetB(viewProj[3][1] + viewProj[2][1]);
			m_Sides[static_cast<int>(ESide::FS_NEAR)].SetC(viewProj[3][2] + viewProj[2][2]);
			m_Sides[static_cast<int>(ESide::FS_NEAR)].SetD(viewProj[3][3] + viewProj[2][3]);

			m_Sides[static_cast<int>(ESide::FS_FAR)].SetA(viewProj[3][0] - viewProj[2][0]);
			m_Sides[static_cast<int>(ESide::FS_FAR)].SetB(viewProj[3][1] - viewProj[2][1]);
			m_Sides[static_cast<int>(ESide::FS_FAR)].SetC(viewProj[3][2] - viewProj[2][2]);
			m_Sides[static_cast<int>(ESide::FS_FAR)].SetD(viewProj[3][3] - viewProj[2][3]);

			m_Sides[static_cast<int>(ESide::FS_LEFT)].SetA(viewProj[3][0] + viewProj[0][0]);
			m_Sides[static_cast<int>(ESide::FS_LEFT)].SetB(viewProj[3][1] + viewProj[0][1]);
			m_Sides[static_cast<int>(ESide::FS_LEFT)].SetC(viewProj[3][2] + viewProj[0][2]);
			m_Sides[static_cast<int>(ESide::FS_LEFT)].SetD(viewProj[3][3] + viewProj[0][3]);

			m_Sides[static_cast<int>(ESide::FS_RIGHT)].SetA(viewProj[3][0] - viewProj[0][0]);
			m_Sides[static_cast<int>(ESide::FS_RIGHT)].SetB(viewProj[3][1] - viewProj[0][1]);
			m_Sides[static_cast<int>(ESide::FS_RIGHT)].SetC(viewProj[3][2] - viewProj[0][2]);
			m_Sides[static_cast<int>(ESide::FS_RIGHT)].SetD(viewProj[3][3] - viewProj[0][3]);

			m_Sides[static_cast<int>(ESide::FS_TOP)].SetA(viewProj[3][0] - viewProj[1][0]);
			m_Sides[static_cast<int>(ESide::FS_TOP)].SetB(viewProj[3][1] - viewProj[1][1]);
			m_Sides[static_cast<int>(ESide::FS_TOP)].SetC(viewProj[3][2] - viewProj[1][2]);
			m_Sides[static_cast<int>(ESide::FS_TOP)].SetD(viewProj[3][3] - viewProj[1][3]);

			m_Sides[static_cast<int>(ESide::FS_BOTTOM)].SetA(viewProj[3][0] + viewProj[1][0]);
			m_Sides[static_cast<int>(ESide::FS_BOTTOM)].SetB(viewProj[3][1] + viewProj[1][1]);
			m_Sides[static_cast<int>(ESide::FS_BOTTOM)].SetC(viewProj[3][2] + viewProj[1][2]);
			m_Sides[static_cast<int>(ESide::FS_BOTTOM)].SetD(viewProj[3][3] + viewProj[1][3]);

			// Normalise the planes
			for (int i = 0; i < static_cast<int>(ESide::FS_MAX); ++i) {
				m_Sides[i] = CPlane::Normalise(m_Sides[i]);
			}

			// Frustum sides have been rebuilt.
			m_FrustumSidesDirty = false;
		}
	}

}


