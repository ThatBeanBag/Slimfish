// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SteeringBehaviours.cpp
// Description	: CSteeringBehaviours implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "SteeringBehaviours.h"

// Local Includes
#include "..\Actor\PhysicsComponent.h"

CSteeringBehaviours::CSteeringBehaviours()
{
	CEventManager::GetInstance()->AddListener<CEventData_DestroyActor>(MakeDelegate(this, &CSteeringBehaviours::DestroyActorDelegate));
}

CSteeringBehaviours::~CSteeringBehaviours()
{
	CEventManager::GetInstance()->RemoveListener<CEventData_DestroyActor>(MakeDelegate(this, &CSteeringBehaviours::DestroyActorDelegate));
}

void CSteeringBehaviours::Update(float _fDeltaTime)
{

}

TSteeringGroupPtr CSteeringBehaviours::AddToFlock(std::string& _krstrFlockType, CActor* _pActor)
{
	if (!_pActor) {
		DEBUG_ERROR("Attempting to add a non existent actor to a flock.");
		return nullptr;
	}

	// Try to find a flock of the provided flock type.
	auto findIter = m_flockMap.find(_krstrFlockType);
	if (findIter != m_flockMap.end()) {
		// Add the actor to the flock.
		findIter->second->Add(_pActor);
		// Return the flock.
		return findIter->second;
	}
	else {
	// No flock of the specified type.
		// Create a group for the new flock.
		TSteeringGroupPtr pFlock(new CSteeringGroup());
		if (!pFlock) {
			return nullptr;
		}

		// Add the flock to our map of flocks.
		m_flockMap[_krstrFlockType] = pFlock;

		// Add the actor to the flock.
		pFlock->Add(_pActor);

		return pFlock;
	}

	return nullptr;
}

TSteeringGroupPtr CSteeringBehaviours::Follow(TActorID _actorTarget, CActor* _pActor)
{
	if (!_pActor) {
		DEBUG_ERROR("Attempting to add a non existent actor to a flock.");
		return nullptr;
	}

	auto findIter = m_followingMap.find(_actorTarget);
	if (findIter != m_followingMap.end()) {
		findIter->second->Add(_pActor);
		return findIter->second;
	}
	else {
	// No followers of the specified type.
		// Create a group for the new follower.
		TSteeringGroupPtr pFollowers(new CSteeringGroup());
		if (!pFollowers) {
			return nullptr;
		}

		// Add the flock to our map of flocks.
		m_followingMap[_actorTarget] = pFollowers;

		// Add the actor to the followers.
		pFollowers->Add(_pActor);

		return pFollowers;
	}

	return nullptr;
}

void CSteeringBehaviours::DestroyActorDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_DestroyActor> pDestoyActorData = static_pointer_cast<CEventData_DestroyActor>(_pEventData);

	for (auto iter = m_followingMap.begin(); iter != m_followingMap.end(); ++iter) {
		iter->second->Remove(pDestoyActorData->GetActorID());
	}

	for (auto iter = m_flockMap.begin(); iter != m_flockMap.end(); ++iter) {
		iter->second->Remove(pDestoyActorData->GetActorID());
	}
}

CVec3 CSteeringBehaviours::Seek(const CVec3& _krPosition, const CVec3& _krTargetPosition, float _fArrivalDistance)
{
	CVec3 vec3ToTarget = _krTargetPosition - _krPosition;
	float fVelocity = 1.0f;

	float fDist = vec3ToTarget.GetLength();
	if (fDist < _fArrivalDistance && _fArrivalDistance != 0) {
		fVelocity *= fDist / _fArrivalDistance;
	}

	if (fDist != 0) {
		// Normalize the vector.
		vec3ToTarget /= fDist;
	}

	CVec3 vec3DesiredVelocity = fVelocity * vec3ToTarget;
	return  vec3DesiredVelocity;
}

CVec3 CSteeringBehaviours::Evade(const CVec3& _krPosition, const CVec3& _krTargetPosition, const CVec3& _krTargetVelocity, float _fMaxVelocity)
{
	if (_fMaxVelocity == 0) {
		return CVec3();
	}

	CVec3 vec3Distance = _krTargetPosition - _krPosition;
	float fTime = vec3Distance.GetLength() / _fMaxVelocity;
	CVec3 vec3Future = _krTargetPosition + fTime * _krTargetVelocity;

	return Flee(_krPosition, vec3Future);
}

CVec3 CSteeringBehaviours::Pursue(const CVec3& _krPosition, const CVec3& _krTargetPosition, const CVec3& _krTargetVelocity, float _fMaxVelocity)
{
	if (_fMaxVelocity == 0) {
		return CVec3();
	}

	CVec3 vec3Distance = _krTargetPosition - _krPosition;
	float fTime = vec3Distance.GetLength() / _fMaxVelocity;
	CVec3 vec3Future = _krTargetPosition + fTime * _krTargetVelocity;

	return Seek(_krPosition, vec3Future);
}


CSteeringGroup::CSteeringGroup()
{

}

CSteeringGroup::~CSteeringGroup()
{

}

void CSteeringGroup::Add(CActor* _pActor)
{
	if (!_pActor) {
		DEBUG_ERROR("Attempting to add a non existent actor to a group.");
		return;
	}

	CPhysicsComponent* pPhysicsComponent = _pActor->GetComponent<CPhysicsComponent>();
	if (!pPhysicsComponent) {
		DEBUG_ERROR("Attempting to add an actor without a physics component to a group.");
		return;
	}

	// Test to see if the actor already exists.
	for (unsigned int i = 0; i < m_vecGroup.size(); ++i) {
		DEBUG_ASSERT(m_vecGroup[i]);
		if (m_vecGroup[i] == pPhysicsComponent) {
			DEBUG_ERROR("Attempting to add an actor more than once to a steering group.");
			return;
		}
	}

	m_vecGroup.push_back(pPhysicsComponent);
}

void CSteeringGroup::Remove(TActorID _actorID)
{
	for (auto iter = m_vecGroup.begin(); iter != m_vecGroup.end(); ++iter) {
		if ((*iter)->GetOwnerID() == _actorID) {
			iter = m_vecGroup.erase(iter);
			break;
		}
	}
}

CPhysicsComponent* CSteeringGroup::Get(unsigned int _uiIndex)
{
	return m_vecGroup[_uiIndex];
}

const CPhysicsComponent* CSteeringGroup::Get(unsigned int _uiIndex) const
{
	return m_vecGroup[_uiIndex];
}

unsigned int CSteeringGroup::GetSize() const
{
	return m_vecGroup.size();
}
