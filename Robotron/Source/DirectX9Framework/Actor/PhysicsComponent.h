// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: PhysicsComponent.h
// Description	: CPhysicsComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __PHYSICSCOMPONENT_H__
#define __PHYSICSCOMPONENT_H__

// Library Includes

// Local Includes
#include "ActorComponent.h"
#include "..\Physics\Physics.h"

// Forward Declaration
class ACollisionShape;

/************************************************************************/
/* CPhysicsComponent handles all the physics for an actor.
/*
/* It essentially is a gateway into the physics system.
/************************************************************************/
class CPhysicsComponent : public AActorComponent {
	// Member Functions
public:
	CPhysicsComponent();
	virtual ~CPhysicsComponent();

	virtual bool VInitialise(TiXmlElement* _pXmlData) override;
	virtual void VPostInitialise() override;
	virtual void VUpdate(float _fDeltaTime) override;

	void ApplyForce(const CVec3& _krvec3Direction, float _fNewtonForce = 1.0f);
	void ApplyImpulse(const CVec3& _krvec3Direction, float _fNewtonForce = 1.0f);
	void ApplyTorque(const CVec3& _krvec3Direction, float _fNewtonForce = 1.0f);

	void SetCollisionsEnabled(bool _bCollisionEnable);
	float GetMass();

	// Acceleration
	void ApplyAcceleration(const CVec3& _krVec3Direction, float fAcceleration, float _fMaxVelocity);
	void ApplyAcceleration(float fAcceleration, float _fMaxVelocity);
	void RemoveAcceleration();
	void ApplyAngularAcceleration(float _fAcceleration, float _fMaxAngularVelocity);
	void RemoveAngularAcceleration();

	// Velocity
	void SetVelocity(const CVec3& _krvcec3Velocity);
	void SetAngularVelocity(const CVec3& _krvec3Velocity);
	const CVec3 GetVelocity();
	const CVec3 GetAngularVelocity();
	void Stop();

	void SetLinearFactor(const CVec3& _krvec3LinearFactor);
	void SetAngularFactor(const CVec3& _krvec3AngularFactor);

	void SetOnGround(bool _bIsOnGround);
	bool IsOnGround() const;

	CMatrix4x4 GetTransform();
	void SetTransform(const CMatrix4x4& _krmatTransform);

	void Rotate(const CVec3& _krvec3Direction);
protected:
private:
	void CreateShape(ACollisionShape& _rShape);

	virtual bool VIsActiveOnRemote() const override;

	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	IPhysics* m_pGamePhysics;	// The physics engine of the game.

	float m_fAcceleration;
	float m_fAngularAcceleration;
	float m_fMaxVelocity;
	float m_fMaxAngularVelocity;

	CVec3 m_vec3AcceleratingDirection;


	// Initialisation data.
	std::string m_strRigidBodyShape;
	TiXmlElement* m_pShapeXMLData;
	CMatrix4x4 m_matRelativeRigidBodyTransform;	// The transform for the rigid body.
												// This is relative to the transform of the actor, found in the transform component.
	float m_fDensity;
	float m_fRestitution;
	float m_fFriction;

	bool m_bIsNoCollide;	// True, if the actor shouldn't be collided with.
	bool m_bIsOnGround;
};

#endif	// __PHYSICSCOMPONENT_H__