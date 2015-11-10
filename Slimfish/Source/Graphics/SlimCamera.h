// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimCamera.h
// Description	: CSlimCamera declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMCAMERA_H__
#define __SLIMCAMERA_H__

// Library Includes

// Local Includes
#include "SlimFrustum.h"
#include "SlimSceneNode.h"
#include "../Math/SlimRay.h"

namespace Slim {


/** Class representing a camera.
@remarks
	
*/
class CCamera : public CSceneNode, private CFrustum {
public:
	// Member Functions
public:
	/** Constructor.
	 	@author Hayden Asplet
	 	@param pSceneCreator Pointer to the scene that created this camera.
	*/
	CCamera(CScene* pSceneCreator);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CCamera();

	/** Convert a point on the screen to a ray in 3D space.
	 	@author Hayden Asplet
	*/
	const CRay ScreenPointToRay(const CPoint& point) const;

	bool IsInView(const CAxisAlignedBoundingBox& aabb);

	/** Set the projection of the camera to perspective using the given properties.
	 	@author Hayden Asplet
		@param
			radFieldOfView Field of view in radians.
		@param 
			aspectRatio Aspect ratio of the screen.
		@param
			nearClipDistance The distance the near clipping plane is from the camera.
			Describes the distance in front of the camera in which geometry is drawn
			and not clipped.
		@param
			farClipDistance The distance the far clipping plane is from the camera.
			Describes the distance the camera can see before geometry gets clipped.
	*/
	void SetPerspective(float radFieldOfView, float aspectRatio, float nearClipDistance, float farClipDistance);

	/** Set the projection of the camera to orthographic using the given properties.
		@author Hayden Asplet
		@param orthoSize 
			The size of the orthographic projection. The largest this value is the more 
			geometry that's captured in the view.
		@param 
			aspectRatio Aspect ratio of the screen.
		@param 
			nearClipDistance The distance the near clipping plane is from the camera.
			Describes the distance in front of the camera in which geometry is drawn 
			and not clipped.
		@param 
			farClipDistance The distance the far clipping plane is from the camera.
			Describes the distance the camera can see before geometry gets clipped.
	*/
	void SetOrthographic(float orthoSize, float aspectRatio, float nearClipDistance, float farClipDistance);

	/** Set a target for the camera to track.
	 	@author Hayden Asplet
	 	@param 
			pTarget Pointer to the target node.
	 	@param 
			trackRotation True if the camera's rotation will follow the target's.
	 	@param 
			translationOffset The translational offset from the target.
	 	@param 
			rotationOffset The rotational offset from the target's rotation, only 
			applicable if trackRotation is true.
	*/
	void SetTarget(CSceneNode* pTarget,
		bool trackRotation = true,
		const CVector3& translationOffset = CVector3::s_ZERO, 
		const CQuaternion& rotationOffset = CQuaternion::s_IDENTITY);

	/** Set the translational offset from the target. @author Hayden Asplet */
	void SetTargetTranslationOffset(const CVector3& targetTranslationOffset);
	/** Get the translational offset from the target. @author Hayden Asplet */
	const CVector3& GetTargetTranslationOffset() const;
	/** Set the rotational offset from the target's rotation. @author Hayden Asplet */
	void SetTargetRotationOffset(const CQuaternion& targetRotationOffset);
	/** Get the rotational offset from the target's rotation. @author Hayden Asplet */
	const CQuaternion& GetTargetRotationOffset() const;
	/** Set whether to track the rotation of the target or not. @author Hayden Asplet */
	void SetTrackRotation(bool trackRotation);
	/** Get whether the camera is tracking the the target or not. @author Hayden Asplet */
	const bool GetTrackRotation() const;

	/** Update the view transform to correspond to the position and orientation of the camera.
		@remarks
			Updates the position and orientation of the camera to the target, if the camera
			has one. Sets the internal view transform from the inverse of the camera's
			orientation and position. 
		@par 
			This should be called each frame before calling GetViewTransform() by the scene
			owning this camera.
	 	@author Hayden Asplet
	*/
	void UpdateViewTransform();

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

	/** Gets projection matrix of the camera to transform vertices by to get them from view space to screen space.
		@remarks
			Rebuilds the matrix if necessary, this matrix is then cached so subsequent calls are as fast as
			possible.
		@author Hayden Asplet
	*/
	const CMatrix4x4& GetProjectionMatrix() const;

	/** Get the view matrix that transform vertices from world space to view space. @author Hayden Asplet */
	const CMatrix4x4& GetViewMatrix() const;

	/** Convenience method for getting the view and projection matrix concatenated together.
	 	@author Hayden Asplet
	*/
	const CMatrix4x4 GetViewProjMatrix() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	// The target to follow, if we have one.
	CSceneNode* m_pTarget;
	// The translational offset from the target.
	CVector3 m_TargetTranslationOffset;
	// The rotational offset from the target.
	CQuaternion m_TargetRotationOffset;
	// True if rotational offset is relative to the target, false if the rotation offset is absolute.
	bool m_TrackRotation;
};

}

#endif	// __SLIMCAMERA_H__