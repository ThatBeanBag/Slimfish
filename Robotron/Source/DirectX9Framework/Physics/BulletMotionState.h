// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BulletMotionState.h
// Description	: CBulletMotionState declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __BULLETMOTIONSTATE_H__
#define __BULLETMOTIONSTATE_H__

// Library Includes

// Local Includes
#include "LinearMath/btMotionState.h"

class btTransform;

class CBulletMotionState : public btMotionState {
	// Member Functions
public:
	CBulletMotionState(const CMatrix4x4& _krmatInitialTransform, const CMatrix4x4& _krmatRelativeTransform);
	~CBulletMotionState();

	/**
	* Gets the world transform of the motion state.
	*
	* Bullet calls this. 
	* Should only be called by Bullet.
	* Use GetWorldTransform() instead.
	* 
	* @author: 	Hayden Asplet
	* @param:	btTransform & _rbtmatWorldTransform - resultant world transform
	* @return:  void
	*/
	virtual void getWorldTransform(btTransform& _rbtmatWorldTransform) const override;

	/**
	* Sets the world transform of the motion state.
	*
	* Bullet calls this. 
	* Should only be called by Bullet.
	* 
	* @author: 	Hayden Asplet
	* @param:	const btTransform & _krbtmatWorldTransform - world transform to set.
	* @return:  void
	*/
	virtual void setWorldTransform(const btTransform& _krbtmatWorldTransform) override;

	/**
	* Gets the world transform as a CMatrix4x4.
	*
	* This is called by the physics system and not the Bullet SDK.
	* 
	* @author: 	Hayden Asplet
	* @return:  const CMatrix4x4
	*/
	const CMatrix4x4 GetWorldTransform() const;

	/**
	* Sets the world transform
	*
	* This is called by the physics system and not the Bullet SDK.
	* 
	* @author: 	Hayden Asplet
	*/
	void SetWorldTransform(const CMatrix4x4& _krmatWorldTransform);

	/**
	* Gets the scale.
	*
	* @author: Hayden Asplet
	* @return: const CVec3 - scale;
	*/
	const CVec3 GetScale() const;

	/**
	* Sets the scale.
	*
	* @author: Hayden Asplet
	* @param: const CVec3 & _krvec3Scale - scale to set.
	* @return: void
	*/
	void SetScale(const CVec3& _krvec3Scale);
protected:
private:
	
	// Member Variables
public:
protected:
private:
	CMatrix4x4 m_matWorldTransform;
	CMatrix4x4 m_matRelativeTransform;
	CVec3 m_vec3Scale;
};

#endif	// __BULLETMOTIONSTATE_H__
