// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BulletSDKPhysics.h
// Description	: CBulletSDKPhysics declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __BULLETSDKPHYSICS_H__
#define __BULLETSDKPHYSICS_H__

// Library Includes
#include <set>

// Local Includes
#include "Physics.h"

// Bullet Forward Declaration
class btVector3;
class btTransform;
class btRigidBody;
class btCollisionShape;
class btCollisionObject;
class btPersistentManifold;
class btDynamicsWorld;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;

// Forward Declaration
class CBulletDebugDrawer;
class ACollisionShape;

namespace BulletConversions {
	// Conversion helper functions to help interface with the Bullet SDK.

	/**
	* Converts a CVec3 to a btVector3
	*
	* This is a helper function for easy conversions between the framework
	* and the Bullet SDK.
	*
	* @author: 	Hayden Asplet
	* @param:	const CVec3 & _krvec3 - CVec3 to convert.
	* @return:  const btVector3 - converted btVector.
	*/
	const btVector3 Vec3ToBulletVector3(const CVec3& _krvec3);

	/**
	* Converts a btVector3 to CVec3
	*
	* This is a helper function for easy conversions between the framework
	* and the Bullet SDK.
	*
	* @author: 	Hayden Asplet
	* @param:	const btVector3 & _krBulletVec3 - btVector3 to convert.
	* @return:  const CVec3 - converted CVec3.
	*/
	const CVec3 BulletVector3ToVec3(const btVector3& _krBulletVec3);

	/**
	* Converts a CMatrix4x4 to a btTransform
	*
	* This is a helper function for easy conversions between the framework
	* and the Bullet SDK.
	*
	* @author: 	Hayden Asplet
	* @param:	const CMatrix4x4 & _krmatTransform - CMatrix4x4 to convert.
	* @return:  const btTransform - converted btTransform.
	*/
	const btTransform Matrix4x4ToBulletTransform(const CMatrix4x4& _krmatTransform);

	/**
	* Converts a btTransform to a CMatrix4x4
	*
	* This is a helper function for easy conversions between the framework
	* and the Bullet SDK.
	*
	* @author: 	Hayden Asplet
	* @param:	const btTransform & _krTransform - btTransform to convert.
	* @return:  const CMatrix4x4 - converted CMatrix4x4.
	*/
	const CMatrix4x4 BulletTransformToMatrix4x4(const btTransform& _krTransform);
}


class CBulletSDKPhysics : public IPhysics {
	typedef std::map<TActorID, btRigidBody*> TActorIDToRigidBodyMap;
	typedef std::map<const btRigidBody*, TActorID> TRigidBodyToActorIDMap;

	typedef std::pair<const btRigidBody*, const btRigidBody*> TCollision;
	typedef std::set<TCollision> TCollisions;

	// Member Functions
public:
	CBulletSDKPhysics();
	virtual ~CBulletSDKPhysics();

	/**
	* Initialises the physics system.
	* 
	* @author: 	Hayden Asplet
	* @return:  bool - true if initialisation was successful.
	*/
	virtual bool VInitialize() override;

	/**
	* Syncs any scenes of the game with the physics system.
	*
	* For any actors that have move since last frame a move event will be sent out will the 
	* new transform (CEventData_MoveActor).
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	virtual void VSyncScene() override;

	/**
	* Updates the bullet dynamic world.
	* 
	* @author: 	Hayden Asplet
	* @param:	float _fDeltaTime - time to update dynamic world for.
	* @return:  void
	*/
	virtual void VUpdate(float _fDeltaTime) override;

	virtual void VAddSphere(CActor* _pActor, const CCollisionSphere& _krCollisionSphere) override;
	virtual void VAddBox(CActor* _pActor, const CCollisionBox& _krCollisionBox) override;
	virtual void VAddCapsule(CActor* _pActor, const CCollisionCapsule& _krCollisionCapsule) override;
	virtual void VAddPlane(CActor* _pActor, const CCollisionPlane& _krCollisionPlane) override;
	virtual void VRemoveActor(TActorID _actorID) override;

	/**
	* Applies a force to an actor.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to apply the force to.
	* @param:	const CVec3 & _krvec3Direction - direction of the force. Assumed to be normalised.
	* @param:	float _fNewtonForce - amount of force.
	* @return:  void
	*/
	virtual void VApplyForce(TActorID _actorID, const CVec3& _krvec3Direction, float _fNewtonForce) override;

	/**
	* Applies an impulse to an actor.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to apply the impulse to.
	* @param:	const CVec3 & _krvec3Direction - direction of the impulse. Assumed to be normalised.
	* @param:	float _fNewtonForce - amount of force.
	* @return:  void
	*/
	virtual void VApplyImpulse(TActorID _actorID, const CVec3& _krvec3Direction, float _fNewtonForce) override;

	/**
	* Applies a torque to an actor
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to apply the torque to.
	* @param:	const CVec3 & _krvec3Direction - direction of the torque.
	* @param:	float _fNewtonForce - force of the torque.
	* @return:  void
	*/
	virtual void VApplyTorque(TActorID _actorID, const CVec3& _krvec3Direction, float _fNewtonForce) override;

	/**
	* Gets the mass of an actor.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to get the mass of.
	* @return:  float - mass of the actor.
	*/
	virtual float VGetMass(TActorID _actorID) override;

	virtual void VSetCollisionsEnabled(TActorID _actorID, bool m_bIsCollisionEnabled) override;

	/**
	* Stops an actor by settings its velocity to 0.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to stop.
	* @return:  void
	*/
	virtual void VStopActor(TActorID _actorID) override;

	/**
	* Sets the linear velocity of an actor.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to set the velocity of.
	* @param:	const CVec3 & _krvcec3Velocity - velocity vector.
	* @return:  void
	*/
	virtual void VSetVelocity(TActorID _actorID, const CVec3& _krvcec3Velocity) override;

	/**
	* Sets the angular velocity of an actor.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - Id of the actor to set the angular velocity of.
	* @param:	const CVec3 & _krvec3Velocity - velocity vector.
	* @return:  void
	*/
	virtual void VSetAngularVelocity(TActorID _actorID, const CVec3& _krvec3Velocity) override;

	/**
	* Returns the velocity of an actor.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to get the velocity of.
	* @return:  const CVec3 - velocity vector.
	*/
	virtual const CVec3 VGetVelocity(TActorID _actorID) override;

	/**
	* Returns the angular velocity of an actor.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to get the aungular velocity of.
	* @return:  const CVec3 - angular velocity vector.
	*/
	virtual const CVec3 VGetAngularVelocity(TActorID _actorID) override;


	/**
	* Sets the linear factor of the actor.
	*
	* This allows you to constrain the movement of an actor to specified axis(s).
	* Example usage: VSetAngularFactor(ID, CVec3(1, 0, 1)) will constrain the actor to only move on the XZ plane.
	*
	* @author: Hayden Asplet
	* @param: TActorID _actorID - ID of the actor.
	* @param: const CVec3 & _krvec3LinearFactor - linear factor.
	* @return: void
	*/
	virtual void VSetLinearFactor(TActorID _actorID, const CVec3& _krvec3LinearFactor) override;

	/**
	* Sets the angular factor of the actor.
	*
	* This allows you to constrain the angular movement of an actor to specified axis(s).
	* Example usage: VSetAngularFactor(ID, CVec3(0, 1, 0)) will constrain the actor to only rotate around Y.
	*
	* @author: Hayden Asplet
	* @param: TActorID _actorID - ID of the actor
	* @param: const CVec3 & _krvec3AngularFactor - angular factor.
	* @return: void
	*/
	virtual void VSetAngularFactor(TActorID _actorID, const CVec3& _krvec3AngularFactor) override;

	/**
	* Sets the transform of an actor.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to set the transform of.
	* @param:	const CMatrix & _krmatTransform - transform to set.
	* @return:  void
	*/
	virtual void VSetTransform(TActorID _actorID, const CMatrix4x4& _krmatTransform) override;

	/**
	* Gets the transform of an actor.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to get the transform of.
	* @return:  const CMatrix4x4 - transform of the actor.
	*/
	virtual const CMatrix4x4 VGetTransform(TActorID _actorID) override;

	virtual void VRotate(TActorID _actorID, const CVec3& _krvec3Direction);
	/**
	* Draws the debug diagnostics to the screen.
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	virtual void VDrawDebugDiagnostics();

protected:
private:
	/**
	* Checks if the input collision is new and therefore should send out a start collision event.
	*
	* See EventData_Collision.
	* 
	* @author: 	Hayden Asplet
	* @param:	const TCollision & _krCollision - collision to be checked.
	* @param:	btPersistentManifold * _pManifold - manifold of the collision
	* @return:  void
	*/
	void CheckIfNewCollision(const TCollision& _krCollision, const btPersistentManifold* const _pManifold);

	/**
	* Updates the last tick collisions with the current collisions.
	*
	* If there are any removals, collision separation events will be queued for them.
	* see EventData_CollisionSeparation.
	* 
	* @author: 	Hayden Asplet
	* @param:	const TCollisions & _krCollisions - the current collisions.
	* @return:  void
	*/
	void UpdateCollisions(const TCollisions& _krCollisions);

	/**
	* Detects collisions between actors.
	*
	* This function is a callback, which is called after the Bullet SDK performs its update.
	* 
	* @author: 	Hayden Asplet
	* @param:	btDynamicsWorld * _pDynamicsWorld - the dynamics world
	* @param:	float _fTimeStep - the time step of the update.
	* @return:  void
	*/
	static void InternalTickCallback(btDynamicsWorld* _pDynamicsWorld, float _fTimeStep);

	/**
	* Finds an actorID assigned to the input btRigidBody.
	*
	* Returns CActor::s_kiINVALID_ACTOR_ID if rigid body fails to be found.
	* 
	* @author: 	Hayden Asplet
	* @param:	btRigidBody* _pRigidBody - rigid body.
	* @return:  TActorID - actor ID of the rigid body. Will be CActor::s_kiINVALID_ACTOR_ID if rigid body isn't found.
	*/
	TActorID FindActorIDFromRigidBody(const btRigidBody* _kpRigidBody);

	/**
	* Finds a rigid body assigned to an actor ID.
	*
	* Returns NULL if rigid body fails to be found.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - actor ID.
	* @return:  btRigidBody* - rigid body of the actorID. Will be NULL if actorID isn't found.
	*/
	btRigidBody* FindRigidBodyFromActorID(TActorID _actorID);

	/**
	* Generic function for adding shapes.
	*
	* All VAddShape() functions should call this after creating the specified shape.
	* 
	* @author: 	Hayden Asplet
	* @param:	CActor * _pActor - owner of the rigid body
	* @param:	btCollisionShape * pShape - collisional shape.
	* @param:	float fMass - mass of the shape.
	* @param:	float _fRestitution - restitution of the body
	* @param:	float _fFriction - friction of the body.
	* @return:  void
	*/
	void AddBulletShape(CActor* _pActor, btCollisionShape* pShape, float fMass, const ACollisionShape& _krCollisionShape);

	void RemoveCollisionObject(btCollisionObject* _pCollisionObject);
	void SendSeparationEvent(const TCollision& _krCollision);

	// Member Variables
public:
protected:
private:
	// Keeps track of rigid bodies in the system.
	// To check for positional updates and lifetimes of actors.
	TActorIDToRigidBodyMap m_mapActorIDToRigidBody;	// Keeps track of actors in the system.

	// Keeps track of actor ID's in terms of rigid bodies.
	// This is helpful for getting the actor ID of a rigid body when doing collisions.
	TRigidBodyToActorIDMap m_mapRigidBodyToActorID;

	TCollisions m_collisions;
	unique_ptr<CBulletDebugDrawer> m_pDebugDrawer;

	// Bullet SDK objects that do all the work.
	unique_ptr<btDynamicsWorld> m_pDynamicsWorld;	// The main bullet interface component.
	unique_ptr<btBroadphaseInterface> m_pBroadphase;// The quick and inaccurate collision detection phase.
	unique_ptr<btCollisionDispatcher> m_pDispatcher;// The precise collision detection manager.
	unique_ptr<btConstraintSolver> m_pSolver;		// The constraint solver.
	unique_ptr<btDefaultCollisionConfiguration> m_pCollisionConfiguration;	// The internal memory manager during collision pass for Bullet.
};

#endif	// __BULLETSDKPHYSICS_H__