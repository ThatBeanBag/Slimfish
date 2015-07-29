// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CameraNode.h
// Description	: CCameraNode declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __CAMERANODE_H__
#define __CAMERANODE_H__

// Library Includes

// Local Includes
#include "SceneNode.h"
#include "../GameStd.h"	// REMOVE: this

enum ECameraProjectionMode {
	PROJECTION_ORTHOGRAPHIC,
	PROJECTION_PERSPECTIVE
};

class CCameraNode : public CSceneNode {
	// Member Functions
public:
	CCameraNode();
	virtual ~CCameraNode();
	
	virtual bool VOnRestore();
	virtual bool VIsVisible(CScene* _pScene) const { return true; } // TODO: Remove this.

	// Sets the view transform of the renderer.
	void SetViewTransform();

	// Target accessor methods.
	void SetTarget(shared_ptr<CSceneNode> _pTarget);
	void SetOffsetFromTarget(const CVec3& _krOffset);
	void SetOffsetFromTarget(float _fX, float _fY, float _fZ);
	void SetRotationFromTarget(const CVec3& _krRotation);
	void SetRotationFromTarget(float _fX, float _fY, float _fZ);
	void SetAbsoluteRotation(bool _bIsAbsoluteRotation);

	// Projection 
	void SetProjectionTransform(float _fFOV, 
								float _fVolumeWidth, float _fVolumeHeight, 
								float _fNear, float _fFar,
								ECameraProjectionMode _eProjectionMode);
	const CMatrix4x4& GetProjectionMatrix();
	const CFrustum& GetFrustum();
	float GetFOV() const;
	float GetViewVolumeWidth() const;
	float GetViewVolumeHeight() const;
	float GetNearClippingPlane() const;
	float GetFarClippingPlane() const;
	ECameraProjectionMode GetProjectionMode() const;

	const CMatrix4x4& GetViewMatrix();
protected:
private:

	// Member Variables
public:
protected:
private:
	// Projection
	CMatrix4x4 m_matProjection;	// The matrix for the projection.
	CFrustum m_frustum;

	// The view volumes height and width give the aspect ratio by width / height.
	float m_fVolumeHeight;
	float m_fVolumeWidth;

	ECameraProjectionMode m_eProjectionMode;

	// View
	CMatrix4x4 m_matView;

	shared_ptr<CSceneNode> m_pTarget;
	CVec3 m_vec3RelativeOffset;		// Translational offset from target.
	CVec3 m_vec3RelativeRotation;	// Rotational offset from target.
	bool m_bIsAbsoluteRotation;	// True if the rotation of the camera is detached from rotation of target.
};

#endif	// __CAMERA_H__
