// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimFrustum.h
// Description	: CSlimFrustum declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMFRUSTUM_H__
#define __SLIMFRUSTUM_H__

// Library Includes

// Local Includes
#include "../Math/SlimPlane.h"

namespace Slim {

// Forward Declaration.
class CAxisAlignedBoundingBox;

/** List of projection modes.
*/
enum class EProjectionMode {
	ORTHOGRAPHIC,
	PERSPECTIVE
};

/** Class representing a square frustum.
@remarks
	This class is intended to be used for CPU-side culling operations. It maintains and
	stores the view and projection transforms for transforming vertices from world space to
	screen space.
@par
	This class is only useful when coupled with a camera as it effectively implements the internal
	representation of a camera, but lacks the interface necessary for most typical camera 
	operations like movement, targeting/following objects etc. For those operations and more
	see CCamera.
*/
class CFrustum {
private:
	/** List of sides of a frustum.
	*/
	enum class ESide : int {
		FS_NEAR,
		FS_FAR,
		FS_LEFT,
		FS_RIGHT,
		FS_TOP,
		FS_BOTTOM,
		FS_MAX
	};

	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CFrustum();

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CFrustum();

	/** Test to see if a vertex is inside the frustum.
	 	@author Hayden Asplet
	 	@return True if the vertex is inside the frustum.
	*/
	bool IsInside(const CVector3& vertex) const;

	/** Test to see if an axis aligned bounding box intersects with the frustum.
	 	@author Hayden Asplet
	 	@param aabb The axis aligned bounding box to check.
	 	@return True if the aabb is inside or intersecting with the frustum.
	*/
	bool IsInside(const CAxisAlignedBoundingBox& aabb) const;

	/** Set the field of view of the perspective.
		@note This only applies to perspective projections and not orthographic projection.
	 	@author Hayden Asplet
	 	@param radFieldOfView The field of view in radians.
	*/
	void SetFieldOfView(float radFieldOfView);

	/** Get the field of view of the perspective.
	 	@author Hayden Asplet
	 	@return field of view in radians.
	*/
	const float GetFieldOfView() const;

	/** Set the aspect ratio. @author Hayden Asplet */
	void SetAspectRatio(float aspectRatio);
	/** Get the aspect ratio. @author Hayden Asplet */
	const float GetAspectRatio() const;
	/** Set the near clipping distance. @author Hayden Asplet */
	void SetNearClipDistance(float zNear);
	/** Get the near clipping distance. @author Hayden Asplet */
	const float GetNearClipDistance() const;
	/** Set the far clipping distance. @author Hayden Asplet */
	void SetFarClipDistance(float zFar);
	/** Get the far clipping distance. @author Hayden Asplet */
	const float GetFarClipDistance() const;
	/** Set the orthographic size. @author Hayden Asplet */
	void SetOrthographicSize(float orthographicSize);
	/** Get the orthographic size. @author Hayden Asplet */
	const float GetOrthographicSize() const;
	/** Set the projection mode. @author Hayden Asplet */
	void SetProjectionMode(EProjectionMode projectionMode);
	/** Get the projection mode. @author Hayden Asplet */
	const EProjectionMode GetProjectionMode() const;

	/** Gets projection matrix of frustum to transform vertices by to get them from view space to screen space.
		@remarks 
			Rebuilds the matrix if necessary, this matrix is then cached so subsequent calls are as fast as 
			possible. 
	 	@author Hayden Asplet
	*/
	const CMatrix4x4& GetProjectionMatrix() const;

	/** Set the view matrix that transform vertices from world space to view space. @author Hayden Asplet */
	void SetViewMatrix(const CMatrix4x4& viewMatrix);
	/** Get the view matrix that transform vertices from world space to view space. @author Hayden Asplet */
	const CMatrix4x4& GetViewMatrix() const;
protected:
private:
	/** Invalidate the projection matrix signifying that it needs to be rebuilt.
		@remarks
			Also invalidates anything relying on the projection matrix.
	 	@author Hayden Asplet
	*/
	void InvalidateProjectionMatrix();

	/** Invalidate the sides of the frustum to signify that they need to be rebuilt.
		@remarks
			Also invalidates anything relying on the frustum sides.
	 	@author Hayden Asplet
	*/
	void InvalidateFrustumSides();

	/** Recalculate the frustum sides. @author Hayden Asplet */
	void CalculateFrustumSides() const;

	// Member Variables
public:
protected:
private:
	// The type of projection to perform, orthographic or perspective.
	EProjectionMode m_ProjectionMode;

	// Field of view in radians.
	float m_radFieldOfView;
	// Aspect ratio of the screen.
	float m_AspectRatio;
	// The distance from the view to begin showing world geometry.
	float m_NearClipDistance;
	// The distance from the view to stop showing world geometry.
	float m_FarClipDistance;
	// The size of the orthographic projection.
	float m_OrthographicSize;

	// The projection matrix of frustum to transform vertices by to get them from view space to screen space.
	mutable CMatrix4x4 m_CachedProjectionMatrix;
	// True if the cached projection matrix requires rebuilding.
	mutable bool m_CachedProjectionMatrixDirty;

	// The view matrix that transform vertices from world space to view space.
	CMatrix4x4 m_ViewMatrix;

	// The planes/sides of the frustum for calculating is geometry is inside or outside the frustum for culling.
	mutable CPlane m_Sides[static_cast<int>(ESide::FS_MAX)];
	// True if the sides of the frustum need to be rebuilt.
	mutable bool m_FrustumSidesDirty;
};

}

#endif	// __SLIMFRUSTUM_H__