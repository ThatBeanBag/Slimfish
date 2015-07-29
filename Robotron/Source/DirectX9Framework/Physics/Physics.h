// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Physics.h
// Description	: IGamePhysics declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __PHYSICS_H__
#define __PHYSICS_H__

// Library Includes

// Local Includes

class CCollisionSphere;
class CCollisionBox;
class CCollisionCapsule;
class CCollisionPlane;

class IPhysics {
public:
	//enum ECollision

	// Member Functions
public:
	virtual ~IPhysics() {}

	virtual bool VInitialize() = 0;
	virtual void VSyncScene() = 0;	// Syncs the visible scene with the physics system.
	virtual void VUpdate(float _fDeltaTime) = 0;

	virtual void VAddSphere(CActor* _pActor, const CCollisionSphere& _krCollisionSphere) = 0;
	virtual void VAddBox(CActor* _pActor, const CCollisionBox& _krCollisionBox) = 0;
	virtual void VAddCapsule(CActor* _pActor, const CCollisionCapsule& _krCollisionCapsule) = 0;
	virtual void VAddPlane(CActor* _pActor, const CCollisionPlane& _krCollisionPlane) = 0;
	virtual void VRemoveActor(TActorID _actorID) = 0;

	virtual void VApplyForce(TActorID _actorID, const CVec3& _krvec3Direction, float _fNewtonForce) = 0;
	virtual void VApplyImpulse(TActorID _actorID, const CVec3& _krvec3Direction, float _fNewtonForce) = 0;
	virtual void VApplyTorque(TActorID _actorID, const CVec3& _krvec3Direction, float _fNewtonForce) = 0;

	virtual float VGetMass(TActorID _actorID) = 0;
	virtual void VSetCollisionsEnabled(TActorID _actorID, bool m_bIsCollisionEnabled) = 0;

	virtual void VStopActor(TActorID _actorID) = 0;
	virtual void VSetVelocity(TActorID _actorID, const CVec3& _krvcec3Velocity) = 0;
	virtual void VSetAngularVelocity(TActorID _actorID, const CVec3& _krvec3Velocity) = 0;
	virtual const CVec3 VGetVelocity(TActorID _actorID) = 0;
	virtual const CVec3 VGetAngularVelocity(TActorID _actorID) = 0;
	virtual void VSetLinearFactor(TActorID _actorID, const CVec3& _krvec3LinearFactor) = 0;
	virtual void VSetAngularFactor(TActorID _actorID, const CVec3& _krvec3AngularFactor) = 0;

	virtual void VSetTransform(TActorID _actorID, const CMatrix4x4& _krmatTransform) = 0;
	virtual const CMatrix4x4 VGetTransform(TActorID _actorID) = 0;
	virtual void VRotate(TActorID _actorID, const CVec3& _krDirection) = 0;
	virtual void VDrawDebugDiagnostics() = 0;
protected:
private:
	// Member Variables
public:
protected:
private:
};

#endif	// __PHYSICS_H__