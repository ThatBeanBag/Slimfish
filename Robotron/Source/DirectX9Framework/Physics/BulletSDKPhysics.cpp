// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BulletSDKPhysics.cpp
// Description	: CBulletSDKPhysics implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

// This Include
#include "BulletSDKPhysics.h"

// Local Includes
#include "../Actor/TransformComponent.h"
#include "BulletMotionState.h"
#include "BulletDebugDrawer.h"
#include "CollisionSphere.h"
#include "CollisionBox.h"
#include "CollisionCapsule.h"
#include "CollisionPlane.h"


const btVector3 BulletConversions::Vec3ToBulletVector3(const CVec3& _krvec3)
{
	return 	btVector3(_krvec3.x, _krvec3.y, _krvec3.z);
}

const CVec3 BulletConversions::BulletVector3ToVec3(const btVector3& _krBulletVec3)
{
	return CVec3(_krBulletVec3.x(), _krBulletVec3.y(), _krBulletVec3.z());
}

const btTransform BulletConversions::Matrix4x4ToBulletTransform(const CMatrix4x4& _krmatTransform)
{
	// Bullet matrices are column major, ours are row major.
//	CMatrix4x4 matTransposed = _krmatTransform.Tranposed();
	btMatrix3x3 btmatRotation;

	for (int iRow = 0; iRow < 3; ++iRow) {
		for (int iColumn = 0; iColumn < 3; ++iColumn) {
			btmatRotation[iRow][iColumn] = _krmatTransform.m[iColumn][iRow];
		}
	}

	btVector3 btvec3Translation;
	btvec3Translation.setX(_krmatTransform.m[3][0]);
	btvec3Translation.setY(_krmatTransform.m[3][1]);
	btvec3Translation.setZ(_krmatTransform.m[3][2]);

	return btTransform(btmatRotation, btvec3Translation);
}

const CMatrix4x4 BulletConversions::BulletTransformToMatrix4x4(const btTransform& _krTransform)
{
	CMatrix4x4 matTransform(CMatrix4x4::s_kIdentity);
	const btMatrix3x3& btmatRotation = _krTransform.getBasis();
	const btVector3& btvec3Translation = _krTransform.getOrigin();

	for (int iRow = 0; iRow < 3; ++iRow) {
		for (int iColumn = 0; iColumn < 3; ++iColumn) {
			matTransform.m[iRow][iColumn] = btmatRotation[iColumn][iRow];
		}
	}

	// Get the translation.
	matTransform.m[3][0] = btvec3Translation.x();
	matTransform.m[3][1] = btvec3Translation.y();
	matTransform.m[3][2] = btvec3Translation.z();

	return matTransform;
}

using namespace BulletConversions;

CBulletSDKPhysics::CBulletSDKPhysics()
{

}

CBulletSDKPhysics::~CBulletSDKPhysics()
{
	for (int iObjectIndex = m_pDynamicsWorld->getNumCollisionObjects() - 1; iObjectIndex >= 0; --iObjectIndex) {
		btCollisionObject* pObject = m_pDynamicsWorld->getCollisionObjectArray()[iObjectIndex];
		RemoveCollisionObject(pObject);
	}

	// All rigid bodies should have been cleaned up by now.
	DEBUG_ASSERT(m_mapActorIDToRigidBody.empty());
	DEBUG_ASSERT(m_mapRigidBodyToActorID.empty());

	// Delete any left over rigid bodies.
	for (auto iter = m_mapRigidBodyToActorID.begin(); iter != m_mapRigidBodyToActorID.end(); ++iter) {
		SafeDelete(iter->first);
	}
}

bool CBulletSDKPhysics::VInitialize()
{
	// Create the Bullet SDK components
	m_pCollisionConfiguration.reset(new btDefaultCollisionConfiguration());
	if (!m_pCollisionConfiguration) {
		DEBUG_ERROR("Bullets Physics - failed to create collision configuration");
		return false;
	}

	m_pDispatcher.reset(new btCollisionDispatcher(m_pCollisionConfiguration.get()));
	if (!m_pDispatcher) {
		DEBUG_ERROR("Bullets Physics - failed to create dispatcher");
		return false;
	}

	m_pBroadphase.reset(new btDbvtBroadphase);
	if (!m_pBroadphase) {
		DEBUG_ERROR("Bullets Physics - failed to create broadphase");
		return false;
	}

	m_pSolver.reset(new btSequentialImpulseConstraintSolver);
	if (!m_pSolver) {
		DEBUG_ERROR("Bullets Physics - failed to create solver");
		return false;
	}

	m_pDynamicsWorld.reset(new btDiscreteDynamicsWorld(m_pDispatcher.get(), 
													   m_pBroadphase.get(), 
													   m_pSolver.get(),
													   m_pCollisionConfiguration.get()));
	if (!m_pDynamicsWorld) {
		DEBUG_ERROR("Bullets Physics - failed to create dynamics world");
		return false;
	}

	m_pDebugDrawer.reset(new CBulletDebugDrawer());
	if (!m_pDebugDrawer) {
		DEBUG_ERROR("Bullets Physics - failed to create debug drawer");
		return false;
	}

	m_pDynamicsWorld->setDebugDrawer(m_pDebugDrawer.get());
	m_pDynamicsWorld->setInternalTickCallback(InternalTickCallback);
	m_pDynamicsWorld->setWorldUserInfo(this);

	return true;
}

void CBulletSDKPhysics::VSyncScene()
{
	// Check all rigid bodies for transformation changes.
	TActorIDToRigidBodyMap::const_iterator iter = m_mapActorIDToRigidBody.begin();
	for (; iter != m_mapActorIDToRigidBody.end(); ++iter) {
		// Get the motion state of the rigid body.
		const CBulletMotionState* const kpkMotionState = static_cast<CBulletMotionState*>(iter->second->getMotionState());
		DEBUG_ASSERT(kpkMotionState);

		// Get the actor from the current actorID.
		CActor* pActor = g_pApp->GetGame()->GetActor(iter->first);
		if (pActor) {
		// Did we retrieve the actor?
			CTransformComponent* pTransformComponent = pActor->GetComponent<CTransformComponent>();

			if (pTransformComponent) {
			// Does the actor have a transform component?
				CMatrix4x4 matTransform = kpkMotionState->GetWorldTransform();

				if (pTransformComponent->GetTransform() != matTransform) {
				// Has the transformation been changed?
					// Update the transform in the transform component.
					pTransformComponent->SetTransform(matTransform);

					// Send out a move event to sync the scene with the game logic.
					shared_ptr<CEventData_MoveActor> pEvent(new CEventData_MoveActor(iter->first, matTransform));
					CEventManager::GetInstance()->QueueEvent(pEvent);
				}
			}
			else {
			// No transform component.
				// An actor in the physics engine must have a transform component.
				DEBUG_ERROR("Couldn't retrieve transform component when syncing the scene");
			}
		}
	}
}

void CBulletSDKPhysics::VUpdate(float _fDeltaTime)
{
	DEBUG_ASSERT(m_pDynamicsWorld);

	m_pDynamicsWorld->stepSimulation(_fDeltaTime, 4);
}

void CBulletSDKPhysics::VAddSphere(CActor* _pActor, const CCollisionSphere& _krCollisionSphere)
{
	if (!_pActor) {
		DEBUG_ERROR("No actor when adding a sphere rigid body");
		return;
	}

	float fRadius = _krCollisionSphere.GetRaidus();

	// Create the Bullet shape.
	unique_ptr<btSphereShape> pSphereShape(new btSphereShape(fRadius));

	// Volume of sphere = 4/3 * pi * R ^ 3.
	float fVolume = g_kfPI * (4.0f / 3.0f) * fRadius * fRadius * fRadius;
	float fMass = fVolume * _krCollisionSphere.GetDensity();

	AddBulletShape(_pActor, pSphereShape.release(), fMass, _krCollisionSphere);
}

void CBulletSDKPhysics::VAddBox(CActor* _pActor, const CCollisionBox& _krCollisionBox)
{
	if (!_pActor) {
		DEBUG_ERROR("No actor when adding a box rigid body");
		return;
	}

	CVec3 vec3Dimensions = _krCollisionBox.GetDimensions();

	// Create the Bullet shape.
	unique_ptr<btBoxShape> pBoxShape(new btBoxShape(Vec3ToBulletVector3(vec3Dimensions)));

	// Volume of box = width * height * length;
	float fVolume = vec3Dimensions.x * vec3Dimensions.y * vec3Dimensions.z;
	float fMass = fVolume * _krCollisionBox.GetDensity();

	AddBulletShape(_pActor, pBoxShape.release(), fMass, _krCollisionBox);
}

void CBulletSDKPhysics::VAddCapsule(CActor* _pActor, const CCollisionCapsule& _krCollisionCapsule)
{
	if (!_pActor) {
		DEBUG_ERROR("No actor when adding a capsule rigid body");
		return;
	}

	float fRadius = _krCollisionCapsule.GetRadius();
	float fHeight = _krCollisionCapsule.GetHeight();

	// Create the Bullet shape.
	unique_ptr<btCapsuleShape> pCapsuleShape(new btCapsuleShape(fRadius, fHeight));

	// Volume of capsule = pi * (R ^ 2) * ((4/3) * R + height);
	float fVolume = g_kfPI * fRadius * fRadius * ((4.0f / 3.0f) * fRadius + fHeight);
	float fMass = fVolume * _krCollisionCapsule.GetDensity();

	AddBulletShape(_pActor, pCapsuleShape.release(), fMass, _krCollisionCapsule);
}

void CBulletSDKPhysics::VAddPlane(CActor* _pActor, const CCollisionPlane& _krCollisionPlane)
{
	if (!_pActor) {
		DEBUG_ERROR("No actor when adding a plane rigid body");
		return;
	}

	btVector3 btvec3PlaneNormal = Vec3ToBulletVector3(_krCollisionPlane.GetPlane().GetNormal());
	float fConstant = _krCollisionPlane.GetPlane().GetConstant();

	// Create the Bullet shape.
	unique_ptr<btStaticPlaneShape> pPlaneShape(new btStaticPlaneShape(btvec3PlaneNormal, fConstant));

	// Volume and mass of a plane is infinite to express this to Bullet we just set the mass to 0.
	float fMass = 0.0f;

	AddBulletShape(_pActor, pPlaneShape.release(), fMass, _krCollisionPlane);
}

void CBulletSDKPhysics::VRemoveActor(TActorID _actorID)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		RemoveCollisionObject(pRigidBody);
		m_mapRigidBodyToActorID.erase(pRigidBody);
	}

	m_mapActorIDToRigidBody.erase(_actorID);
}

void CBulletSDKPhysics::VApplyForce(TActorID _actorID, const CVec3& _krvec3Direction, float _fNewtonForce)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		btVector3 btvec3Force = Vec3ToBulletVector3(_fNewtonForce * _krvec3Direction);

		pRigidBody->applyCentralForce(btvec3Force);
	}
}

void CBulletSDKPhysics::VApplyImpulse(TActorID _actorID, const CVec3& _krvec3Direction, float _fNewtonForce)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		btVector3 btvec3Force = Vec3ToBulletVector3(_fNewtonForce * _krvec3Direction);

		pRigidBody->applyCentralImpulse(btvec3Force);
	}
}

void CBulletSDKPhysics::VApplyTorque(TActorID _actorID, const CVec3& _krvec3Direction, float _fNewtonForce)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		btVector3 btvec3Torque = Vec3ToBulletVector3(_fNewtonForce * _krvec3Direction);

		pRigidBody->applyTorqueImpulse(btvec3Torque);
	}
}

float CBulletSDKPhysics::VGetMass(TActorID _actorID)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		float fInverseMass = pRigidBody->getInvMass();

		if (fInverseMass != 0) {
			return 1.0f / fInverseMass;
		}
		else {
			return fInverseMass;
		}
	}

	return 0;
}

void CBulletSDKPhysics::VSetCollisionsEnabled(TActorID _actorID, bool m_bIsCollisionEnabled)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		if (m_bIsCollisionEnabled) {
			pRigidBody->setCollisionFlags(pRigidBody->getCollisionFlags() ^ btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
		else {
			pRigidBody->setCollisionFlags(pRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
	}
}

void CBulletSDKPhysics::VStopActor(TActorID _actorID)
{
	VSetVelocity(_actorID, CVec3(0.0f, 0.0f, 0.0f));
}

void CBulletSDKPhysics::VSetVelocity(TActorID _actorID, const CVec3& _krvec3Velocity)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		pRigidBody->setLinearVelocity(Vec3ToBulletVector3(_krvec3Velocity));
	}
}

void CBulletSDKPhysics::VSetAngularVelocity(TActorID _actorID, const CVec3& _krvec3Velocity)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		pRigidBody->setAngularVelocity(Vec3ToBulletVector3(_krvec3Velocity));
	}
}

const CVec3 CBulletSDKPhysics::VGetVelocity(TActorID _actorID)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		const btVector3& btvec3Velocity = pRigidBody->getLinearVelocity();
		return BulletVector3ToVec3(btvec3Velocity);
	}
	else {
		DEBUG_ERROR("Bullet Physics - failed to get velocity. No actor found with ID: " + ToString(_actorID));
	}

	return CVec3();
}

const CVec3 CBulletSDKPhysics::VGetAngularVelocity(TActorID _actorID)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		const btVector3& btvec3Velocity = pRigidBody->getAngularVelocity();
		return BulletVector3ToVec3(btvec3Velocity);
	}
	else {
		DEBUG_ERROR("Bullet Physics - failed to get angular velocity. No actor found with ID: " + ToString(_actorID));
	}

	return CVec3();
}

void CBulletSDKPhysics::VSetLinearFactor(TActorID _actorID, const CVec3& _krvec3LinearFactor)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		btVector3 btvec3LinearFactor = Vec3ToBulletVector3(_krvec3LinearFactor);
		pRigidBody->setLinearFactor(btvec3LinearFactor);
	}
	else {
		DEBUG_ERROR("Bullet Physics - failed to set linear factor. No actor found with ID: " + ToString(_actorID));
	}
}

void CBulletSDKPhysics::VSetAngularFactor(TActorID _actorID, const CVec3& _krvec3AngularFactor)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		btVector3 btvec3AngularFactor = Vec3ToBulletVector3(_krvec3AngularFactor);
		pRigidBody->setAngularFactor(btvec3AngularFactor);
	}
	else {
		DEBUG_ERROR("Bullet Physics - failed to set linear factor. No actor found with ID: " + ToString(_actorID));
	}
}

void CBulletSDKPhysics::VSetTransform(TActorID _actorID, const CMatrix4x4& _krmatTransform)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		CBulletMotionState* pMotionState = static_cast<CBulletMotionState*>(pRigidBody->getMotionState());
		pMotionState->SetWorldTransform(_krmatTransform);

		// Move the body the new position.
		btTransform btmatWorldTransform;
		pMotionState->getWorldTransform(btmatWorldTransform);
		pRigidBody->setWorldTransform(btmatWorldTransform);
	}
}

const CMatrix4x4 CBulletSDKPhysics::VGetTransform(TActorID _actorID)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		const btTransform& btmatTransform = pRigidBody->getWorldTransform();
		return BulletTransformToMatrix4x4(btmatTransform);
	}
	else {
		DEBUG_ERROR("Bullet Physics - failed to get transform. No actor found with ID: " + ToString(_actorID));
	}

	return CMatrix4x4::s_kIdentity;
}

void CBulletSDKPhysics::VRotate(TActorID _actorID, const CVec3& _krvec3Direction)
{
	btRigidBody* pRigidBody = FindRigidBodyFromActorID(_actorID);
	if (pRigidBody) {
		pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		const btVector3& btvec3Position = pRigidBody->getCenterOfMassPosition();
		btTransform btmatTransform = Matrix4x4ToBulletTransform(BuildRotationDirection(_krvec3Direction));
		btmatTransform.setOrigin(btvec3Position);

		pRigidBody->setCenterOfMassTransform(btmatTransform);
	}
	else {
		DEBUG_ERROR("Bullet Physics - failed. No actor found with ID: " + ToString(_actorID));
	}
}

void CBulletSDKPhysics::VDrawDebugDiagnostics()
{
	m_pDynamicsWorld->debugDrawWorld();
}

void CBulletSDKPhysics::CheckIfNewCollision(const TCollision& _krCollision, const btPersistentManifold* const _pManifold)
{
	DEBUG_ASSERT(_krCollision.first);
	DEBUG_ASSERT(_krCollision.second);
	DEBUG_ASSERT(_pManifold);

	TCollisions::const_iterator findIter = m_collisions.find(_krCollision);
	if (findIter == m_collisions.end()) {
	// New Collision.
		// Send an event.
		TActorID actorID1 = FindActorIDFromRigidBody(_krCollision.first);
		TActorID actorID2 = FindActorIDFromRigidBody(_krCollision.second);

		if (actorID1 == CActor::s_kINVALID_ACTOR_ID || actorID2 == CActor::s_kINVALID_ACTOR_ID) {
			DEBUG_WARNING("Something is colliding with a non actor, we currently don't handle that.");
			return;
		}

		std::vector<CVec3> vecCollisionPoints;
		CVec3 vec3SumOfForceNormal;
		CVec3 vec3SumOfFrictionForce;

		// Loop through all contact points.
		for (int iPointIndex = 0; iPointIndex < _pManifold->getNumContacts(); ++iPointIndex) {
			// Get the current contact point.
			const btManifoldPoint& krPoint = _pManifold->getContactPoint(iPointIndex);

			// Get the normal force and friction force of the contact.
			btVector3 btvec3ForceNormal = krPoint.m_combinedRestitution * krPoint.m_normalWorldOnB;
			btVector3 btvec3FrictionForce = krPoint.m_combinedFriction * krPoint.m_lateralFrictionDir1;

			vec3SumOfForceNormal += BulletVector3ToVec3(btvec3ForceNormal);
			vec3SumOfFrictionForce += BulletVector3ToVec3(btvec3FrictionForce);
			vecCollisionPoints.push_back(BulletVector3ToVec3(krPoint.getPositionWorldOnB()));
		}

		// Queue a collision event to tell other systems about the collision.
		shared_ptr<CEventData_Collision> pEvent(new CEventData_Collision(actorID1, actorID2, 
																	     vec3SumOfForceNormal, 
																	     vec3SumOfFrictionForce, 
																	     vecCollisionPoints));
		CEventManager::GetInstance()->QueueEvent(pEvent);
	}
}

void CBulletSDKPhysics::UpdateCollisions(const TCollisions& _krCollisions)
{
	TCollisions removedCollisions;

	std::set_difference(m_collisions.begin(), m_collisions.end(), _krCollisions.begin(), _krCollisions.end(),
						std::inserter(removedCollisions, removedCollisions.begin()));

	// Loop through all recently removed collisions.
	TCollisions::const_iterator iter = removedCollisions.begin();
	for (; iter != removedCollisions.end(); ++iter) {
		// Send collision separation event.
		TActorID actorID1 = FindActorIDFromRigidBody(iter->first);
		TActorID actorID2 = FindActorIDFromRigidBody(iter->second);

		if (actorID1 == CActor::s_kINVALID_ACTOR_ID || actorID2 == CActor::s_kINVALID_ACTOR_ID) {
			DEBUG_WARNING("Something is seperating from a non actor, we currently don't handle that.");
			continue;
		}

		shared_ptr<CEventData_CollisionSeparation> pEvent(new CEventData_CollisionSeparation(actorID1, actorID2));
		CEventManager::GetInstance()->QueueEvent(pEvent);
	}

	m_collisions = _krCollisions;
}

void CBulletSDKPhysics::InternalTickCallback(btDynamicsWorld* _pDynamicsWorld, float _fTimeStep)
{
	DEBUG_ASSERT(_pDynamicsWorld);
	DEBUG_ASSERT(_pDynamicsWorld->getWorldUserInfo());

	CBulletSDKPhysics* pPhysics = static_cast<CBulletSDKPhysics*>(_pDynamicsWorld->getWorldUserInfo());

	// Get the collision dispatcher.
	btDispatcher* pDispatcher = _pDynamicsWorld->getDispatcher();
	DEBUG_ASSERT(pDispatcher);

	TCollisions currentCollisions;

	// Iterate through all manifolds.
	for (int iManifoldIndex = 0; iManifoldIndex < pDispatcher->getNumManifolds(); ++iManifoldIndex) {
		// Get the current manifold.
		const btPersistentManifold* const pManifold = pDispatcher->getManifoldByIndexInternal(iManifoldIndex);
		DEBUG_ASSERT(pManifold);

		if (pManifold->getNumContacts() == 0) {
		// If there's no contact points then the collision is only broad phase.
			continue;
		}

		const btRigidBody* const pRigidBody1 = static_cast<const btRigidBody*>(pManifold->getBody0());
		const btRigidBody* const pRigidBody2 = static_cast<const btRigidBody*>(pManifold->getBody1());


		DEBUG_ASSERT(pRigidBody1);
		DEBUG_ASSERT(pRigidBody2);

		TCollision collision;

		if (pRigidBody1 < pRigidBody2) {
			collision = std::make_pair(pRigidBody1, pRigidBody2);
		}
		else {
			collision = std::make_pair(pRigidBody2, pRigidBody1);
		}

		currentCollisions.insert(collision);
		pPhysics->CheckIfNewCollision(collision, pManifold);
	}

	pPhysics->UpdateCollisions(currentCollisions);
}

TActorID CBulletSDKPhysics::FindActorIDFromRigidBody(const btRigidBody* _kpRigidBody)
{
	TRigidBodyToActorIDMap::const_iterator findIter = m_mapRigidBodyToActorID.find(_kpRigidBody);

	if (findIter != m_mapRigidBodyToActorID.end()) {
		return findIter->second;
	}
	
	return CActor::s_kINVALID_ACTOR_ID;
}

btRigidBody* CBulletSDKPhysics::FindRigidBodyFromActorID(TActorID _actorID)
{
	TActorIDToRigidBodyMap::const_iterator findIter = m_mapActorIDToRigidBody.find(_actorID);

	if (findIter != m_mapActorIDToRigidBody.end()) {
		return findIter->second;
	}

	return nullptr;
}

void CBulletSDKPhysics::AddBulletShape(CActor* _pActor, btCollisionShape* _pShape, float _fMass, const ACollisionShape& _krCollisionShape)
{
	DEBUG_ASSERT(_pActor);
	DEBUG_ASSERT(_pShape);

	TActorID actorID = _pActor->GetID();
	if (FindRigidBodyFromActorID(actorID) != nullptr) {
		DEBUG_WARNING("Actors can't have more than one rigid body, overwriting current rigid body for ID: " + ToString(actorID));
	}

	// If mass is less than or equal 0 the rigid body has infinite mass.
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (_fMass > 0.0f) {
		_pShape->calculateLocalInertia(_fMass, localInertia);
	}

	CTransformComponent* pTransformComponent = _pActor->GetComponent<CTransformComponent>();
	if (!pTransformComponent) {
	// No transform component.
		DEBUG_ERROR("Couldn't retrieve transform component when adding actor to physics system.");
		return;
	}

	// Get the initial transform from the transform component.
	CMatrix4x4 matInitialTransform = pTransformComponent->GetTransform();
	unique_ptr<CBulletMotionState> pMotionState(new CBulletMotionState(matInitialTransform, _krCollisionShape.GetRelativeTransform()));

	if (!pMotionState) {
		DEBUG_ERROR("Failed to create motion state.");
		return;
	}

	// Create the construction info for the rigid body.
	btRigidBody::btRigidBodyConstructionInfo btrbConstructionInfo(_fMass, pMotionState.release(), _pShape, localInertia);

	// Set the material properties of the rigid body.
	btrbConstructionInfo.m_friction = _krCollisionShape.GetFriction();
	btrbConstructionInfo.m_restitution = _krCollisionShape.GetRestitution();

	btRigidBody* pRigidBody = new btRigidBody(btrbConstructionInfo);

	m_pDynamicsWorld->addRigidBody(pRigidBody);

	// Add the actor to the map of actors and rigid bodies.
	m_mapActorIDToRigidBody[actorID] = pRigidBody;
	m_mapRigidBodyToActorID[pRigidBody] = actorID;
}

void CBulletSDKPhysics::RemoveCollisionObject(btCollisionObject* _pCollisionObject)
{
	DEBUG_ASSERT(m_pDynamicsWorld);
	m_pDynamicsWorld->removeCollisionObject(_pCollisionObject);

	// Loop through all collisions - WARNING: Slow for sets.
	TCollisions::iterator iter = m_collisions.begin();
	while(iter != m_collisions.end()) {
		// Keep track of the next iteration as the current might invalidate.
		TCollisions::iterator nextIter = iter;
		nextIter++;

		if (iter->first == _pCollisionObject || iter->second == _pCollisionObject) {
		// Does this collision contain the rigid body to remove?
			// First send a separation even then erase the collision.
			SendSeparationEvent(*iter);
			m_collisions.erase(iter);	// Invalidates the iterator.

			// Keep going, there might be more collisions with the rigid body.
		}

		// Move to next element.
		iter = nextIter;
	}

	btRigidBody* pRigidBody = btRigidBody::upcast(_pCollisionObject);
	if (pRigidBody) {
		// Delete the components of the rigid body.
		SafeDelete(pRigidBody->getMotionState());
		SafeDelete(pRigidBody->getCollisionShape());
		SafeDelete(pRigidBody->getUserPointer());

		// Release all constraints.
		for (int iConstraint = pRigidBody->getNumConstraintRefs() - 1; iConstraint >= 0; --iConstraint) {
			btTypedConstraint* pConstraint = pRigidBody->getConstraintRef(iConstraint);
			m_pDynamicsWorld->removeConstraint(pConstraint);
			SafeDelete(pConstraint);
		}
	}

	SafeDelete(_pCollisionObject);
}

void CBulletSDKPhysics::SendSeparationEvent(const TCollision& _krCollision)
{
	TActorID actorID1 = FindActorIDFromRigidBody(_krCollision.first);
	TActorID actorID2 = FindActorIDFromRigidBody(_krCollision.second);

	if (actorID1 == CActor::s_kINVALID_ACTOR_ID || actorID2 == CActor::s_kINVALID_ACTOR_ID) {
		DEBUG_WARNING("Something is seperating from a non actor, we currently don't handle that.");
		return;
	}

	shared_ptr<CEventData_CollisionSeparation> pEvent(new CEventData_CollisionSeparation(actorID1, actorID2));
	CEventManager::GetInstance()->QueueEvent(pEvent);
}