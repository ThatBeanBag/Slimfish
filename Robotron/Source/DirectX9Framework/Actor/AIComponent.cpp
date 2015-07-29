// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AIComponent.cpp
// Description	: CAIComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "AIComponent.h"

// Local Includes
#include "ActorFactory.h" // For GetTransform().
#include "PhysicsComponent.h"
#include "..\3rdParty\tinyxml\tinyxml.h"
#include "MeshRenderComponent.h"


const std::string CAIComponent::s_kstrNAME = "AIComponent";

CAIComponent::CAIComponent()
	:m_targetActorID(CActor::s_kINVALID_ACTOR_ID),
	m_fArrivalDistance(0.06f),
	m_eState(AI_NONE),
	m_fWanderDistance(10.0f),
	m_fWanderJitter(1.0f),
	m_fWanderRadius(6.0f),
	m_fMaxForce(7.0),
	m_fMaxSpeed(3.0)
{
	
}

CAIComponent::~CAIComponent()
{
	if (!m_actions.empty()) {
		CEventManager::GetInstance()->RemoveListener<CEventData_MoveActor>(MakeDelegate(this, &CAIComponent::MoveActorDelegate));
	}
}

bool CAIComponent::VInitialise(TiXmlElement* _pXmlData)
{
	TiXmlElement* pMaxForce = _pXmlData->FirstChildElement("MaxForce");
	if (pMaxForce) {
		m_fMaxForce = static_cast<float>(atof(pMaxForce->FirstChild()->Value()));
	}
	else {
		DEBUG_ERROR("No \"MaxForce\" found when creating AI component");
	}

	TiXmlElement* pMaxSpeed = _pXmlData->FirstChildElement("MaxSpeed");
	if (pMaxSpeed) {
		m_fMaxSpeed = static_cast<float>(atof(pMaxSpeed->FirstChild()->Value()));
	}
	else {
		DEBUG_ERROR("No \"MaxSpeed\" found when creating AI component");
	}
	
	// Wandering.
	TiXmlElement* pWander = _pXmlData->FirstChildElement("Wander");
	if(pWander) {
		double dWanderDistance = 0.0;
		double dWanderJitter = 0.0;
		double dWanderRadius = 0.0;

		pWander->Attribute("distance", &dWanderDistance);
		pWander->Attribute("jitter", &dWanderJitter);
		pWander->Attribute("radius", &dWanderRadius);

		m_fWanderDistance = static_cast<float>(dWanderDistance);
		m_fWanderJitter = static_cast<float>(dWanderJitter);
		m_fWanderRadius = static_cast<float>(dWanderRadius);

		m_eState = AI_WANDER;
	}

	// Seek.
	TiXmlElement* pSeek = _pXmlData->FirstChildElement("Seek");
	if (pSeek) {
		double dArrivalDistance = 0.0;

		pSeek->Attribute("arrival_distance", &dArrivalDistance);
		m_fArrivalDistance = static_cast<float>(dArrivalDistance);
	}

	// Containment.
	TiXmlElement* pContainment = _pXmlData->FirstChildElement("Containment");
	if (pContainment) {
		double dWidth = 0;
		double dLength = 0;

		pContainment->Attribute("width", &dWidth);
		pContainment->Attribute("length", &dLength);

		m_fContainerWidth = static_cast<float>(dWidth);
		m_fContainerLength = static_cast<float>(dLength);

		m_bIsContained = true;
	}
	else {
		m_bIsContained = false;
	}

	// Flocking behaviour.
	TiXmlElement* pFlocking = _pXmlData->FirstChildElement("Flocking");
	if (pFlocking) {
		const char* pcType = pFlocking->Attribute("type");
		if (pcType) {
			m_strFlockType = pcType;
		}
		else {
			DEBUG_ERROR("No \"type\" specified for flocking behaviour.");
		}

		// Get the properties of the flocking behavior.
		double dAlignmentVelocity = 0.0;
		double dCohesionVelocity = 0.0;
		double dSeparationVelocity = 0.0;
		double dNeighbourRadius = 0.0;

		pFlocking->Attribute("alignment", &dAlignmentVelocity);
		pFlocking->Attribute("cohesion", &dCohesionVelocity);
		pFlocking->Attribute("separation", &dSeparationVelocity);
		pFlocking->Attribute("neighbour_radius", &dNeighbourRadius);

		m_fAlignmentVelocity = static_cast<float>(dAlignmentVelocity);
		m_fCohesionVelocity = static_cast<float>(dCohesionVelocity);
		m_fSeparationVelocity = static_cast<float>(dSeparationVelocity);
		m_fNeighbourRadiusSquared = static_cast<float>(dNeighbourRadius * dNeighbourRadius);
	}

	TiXmlElement* pActions = _pXmlData->FirstChildElement("Actions");
	if (pActions) {
		// Loop through all actions.
		for (TiXmlElement* pNode = pActions->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
			TActionInfo actionInfo;
			// Get the name of the action.
			actionInfo.m_strActionName = pNode->Value();

			std::string strActorType;

			// Get the actor_type for the action.
			const char* pcActorType = pNode->Attribute("actor_type");
			if (pcActorType) {
				strActorType = pcActorType;
			}
			else {
				DEBUG_ERROR("Missing \"actor_type\" for action \"" + actionInfo.m_strActionName + "\"");
				return false;
			}

			const char* pcAction = pNode->Attribute("action");
			if (pcAction) {
				std::string strAction = StringToLower(pcAction);
				if (strAction == "seek") {
					actionInfo.m_eAction = AI_SEEK;
				}
				else if (strAction == "flee") {
					actionInfo.m_eAction = AI_FLEE;
				}
				else if (strAction == "evade") {
					actionInfo.m_eAction = AI_EVADE;
				}
				else if (strAction == "pursue") {
					actionInfo.m_eAction = AI_PURSUE;
				}
				else if (strAction == "follow") {
					actionInfo.m_eAction = AI_FOLLOW;
				}
				else {
					DEBUG_ERROR("Unable to regocnise action \"" + strAction + "\"");
				}
			}
			else {
				DEBUG_ERROR("Missing \"action\" for action \"" + actionInfo.m_strActionName + "\"");
				return false;
			}

			double dDistance = 0.0;
			pNode->Attribute("distance", &dDistance);

			actionInfo.m_fDistanceSquared = static_cast<float>(dDistance * dDistance);

			// Insert the action into the map.
			m_actions[strActorType] = actionInfo;
		}
	}

	return true;
}

void CAIComponent::VPostInitialise()
{
	if (!m_strFlockType.empty()) {
		m_pFlock = g_pApp->GetGame()->GetSteeringBehaviours().AddToFlock(m_strFlockType, GetOwner());
		if (m_pFlock) {
			m_eState = AI_FLOCKING;
		}
	}

	if (!m_actions.empty()) {
		CEventManager::GetInstance()->AddListener<CEventData_MoveActor>(MakeDelegate(this, &CAIComponent::MoveActorDelegate));
	}

	CPhysicsComponent* pPhysicsComponent = GetOwner()->GetComponent<CPhysicsComponent>();
	if (!pPhysicsComponent) {
		DEBUG_ERROR("An CAIComponent requires a physics component.");
		return;
	}

	pPhysicsComponent->SetAngularFactor(CVec3(0.0f, 0.0f, 0.0f));
}

void CAIComponent::VUpdate(float _fDeltaTime)
{
	CPhysicsComponent* pPhysicsComponent = GetOwner()->GetComponent<CPhysicsComponent>();
	if (!pPhysicsComponent) {
		DEBUG_ERROR("An CAIComponent requires a physics component.");
		return;
	}

	if (pPhysicsComponent->IsOnGround() == false) {
		// Can't steer if we are not on the ground.
		return;
	}

	CVec3 vec3Position = pPhysicsComponent->GetTransform().GetPosition();
	CVec3 vec3Velocity = pPhysicsComponent->GetVelocity();

	CVec3 vec3Steering;

	switch (m_eState) {
		case AI_SEEK: {
			vec3Steering += DoSeek(vec3Position) * m_fMaxSpeed;
			break;
		}
		case AI_FLEE: {
			vec3Steering += DoFlee(vec3Position) * m_fMaxSpeed;
			break;
		}
		case AI_PURSUE: {
			vec3Steering += DoPursue(vec3Position) * m_fMaxSpeed;
			break;
		}
		case AI_EVADE: {
			vec3Steering += DoEvade(vec3Position) * m_fMaxSpeed;
			break;
		}
		case AI_WANDER: {
			vec3Steering += DoWander(vec3Position, vec3Velocity) * m_fMaxSpeed;
			break;
		}
		case AI_FLOCKING: {
			vec3Steering += DoFlock(vec3Position);
			break;
		}
		case AI_FOLLOW: {
			vec3Steering += DoFollowing(vec3Position);
			break;
		}
		default: {
			break;
		}
	}

	if (m_bIsContained) {
		// Contain the AI within the containment box.
		if (vec3Position.x > m_fContainerWidth / 2.0f) {
			vec3Steering = CVec3(-m_fMaxSpeed, 0.0f, vec3Velocity.z);
		}
		else if (vec3Position.x < -m_fContainerWidth / 2.0f) {
			vec3Steering = CVec3(m_fMaxSpeed, 0.0f, vec3Velocity.z);
		}

		if (vec3Position.z > m_fContainerLength / 2.0f) {
			vec3Steering = CVec3(vec3Velocity.x, 0.0f, -m_fMaxSpeed);
		}
		else if (vec3Position.z < -m_fContainerLength / 2.0f) {
			vec3Steering = CVec3(vec3Velocity.x, 0.0f, m_fMaxSpeed);
		}
	}

	// Limit our velocity to the maximum velocity.
	vec3Steering = LimitVelocity(vec3Steering);

	// Calculate the force required to the steer the actor.
	CVec3 vec3Force = vec3Steering - vec3Velocity;
	pPhysicsComponent->ApplyForce(LimitForce(vec3Force), 1.0f);

	// Rotate to face direction of movement.
	if (vec3Steering.GetSquaredLength() > 0.01f) {
		vec3Steering.y = 0;
		vec3Steering.Normalize();
		pPhysicsComponent->Rotate(vec3Steering);
	}
}

void CAIComponent::Seek(TActorID _targetID)
{
	m_targetActorID = _targetID;
	m_eState = AI_SEEK;
}

void CAIComponent::Flee(TActorID _targetID)
{
	m_targetActorID = _targetID;
	m_eState = AI_FLEE;
}

void CAIComponent::Evade(TActorID _targetID)
{
	m_targetActorID = _targetID;
	m_eState = AI_EVADE;
}

void CAIComponent::Pursue(TActorID _targetID)
{
	m_targetActorID = _targetID;
	m_eState = AI_PURSUE;
}

void CAIComponent::Wander()
{
	m_eState = AI_WANDER;
}

void CAIComponent::Follow(TActorID _leaderID, float _fDistance, float _fSeperation, float _fLeaderSightRadius, float _fEvadeVelocity, float _fNeighbourRadius)
{
	m_pOtherFollowers = g_pApp->GetGame()->GetSteeringBehaviours().Follow(_leaderID, GetOwner());
	if (!m_pOtherFollowers) {
		DEBUG_ERROR("Failed to follow a target - steering behaviours.");
		return;
	}

	m_targetActorID = _leaderID;
	m_fFollowDistance = _fDistance;
	m_fSeparationFollowing = _fSeperation;
	m_fLeaderSightRadiusSquared = _fLeaderSightRadius * _fLeaderSightRadius;
	m_fEvadeVelocity = _fEvadeVelocity;
	m_fNeighbourRadiusSquared = _fNeighbourRadius * _fNeighbourRadius;
	m_eState = AI_FOLLOW;
}

CVec3 CAIComponent::DoSeek(const CVec3& _krvec3OurPosition)
{
	return CSteeringBehaviours::Seek(_krvec3OurPosition, GetTargetPosition(), m_fArrivalDistance);
}

CVec3 CAIComponent::DoFlee(const CVec3& _krvec3OurPosition)
{
	return CSteeringBehaviours::Flee(_krvec3OurPosition, GetTargetPosition());
}

CVec3 CAIComponent::DoEvade(const CVec3& _krvec3OurPosition)
{
	return CSteeringBehaviours::Evade(_krvec3OurPosition, GetTargetPosition(), GetTargetVelocity(), m_fMaxSpeed);
}

CVec3 CAIComponent::DoPursue(const CVec3& _krvec3OurPosition)
{
	return CSteeringBehaviours::Pursue(_krvec3OurPosition, GetTargetPosition(), GetTargetVelocity(), m_fMaxSpeed);
}

CVec3 CAIComponent::DoWander(const CVec3& _krvec3OurPosition, const CVec3& _krvec3Velocity)
{
	DEBUG_ASSERT(_krvec3OurPosition);

	CMatrix4x4 matTransform = GetTransform(GetOwner());
	
	m_vec3WanderTarget += CVec3(g_pApp->GetRandomNumberReal(-1, 1) * m_fWanderJitter,
								0,
								g_pApp->GetRandomNumberReal(-1, 1) * m_fWanderJitter);

	m_vec3WanderTarget.Normalize();
	m_vec3WanderTarget *= m_fWanderRadius;

	CVec3 vec3Forward = _krvec3Velocity;
	vec3Forward.y = 0;
	vec3Forward.Normalize();

	CVec3 vec3Target = m_vec3WanderTarget + (m_fWanderDistance * vec3Forward);
	return vec3Target - matTransform.GetPosition();
}

CVec3 CAIComponent::DoFlock(const CVec3& _krvec3OurPosition)
{
	DEBUG_ASSERT(_krvec3OurPosition);

	CVec3 vec3AlignmentSteering;
	CVec3 vec3SeparationSteering;
	CVec3 vec3CohesionSteering;
	float fNeighbourCount = 0;

	for (unsigned int i = 0; i < m_pFlock->GetSize(); ++i) {
		CPhysicsComponent* pNeighbourPhysics = m_pFlock->Get(i);
		DEBUG_ASSERT(pNeighbourPhysics);

		if (pNeighbourPhysics->GetOwner() == GetOwner()) {
			// Don't evaluate ourselves.
			continue;
		}

		CVec3 vec3NeighbourPosition = pNeighbourPhysics->GetTransform().GetPosition();
		CVec3 vec3Distance = vec3NeighbourPosition - _krvec3OurPosition;

		if (vec3Distance.GetSquaredLength() < m_fNeighbourRadiusSquared) {
			// Is this actor actually a neighbour?
			++fNeighbourCount;
			// Calculate alignment, cohesion and separation.
			vec3AlignmentSteering += pNeighbourPhysics->GetVelocity();
			vec3CohesionSteering += vec3NeighbourPosition;
			vec3SeparationSteering += vec3Distance;
		}
	}

	if (fNeighbourCount == 0) {
		return CVec3();
	}

	vec3AlignmentSteering /= fNeighbourCount;

	vec3CohesionSteering /= fNeighbourCount;
	vec3CohesionSteering = vec3CohesionSteering - _krvec3OurPosition;

	vec3SeparationSteering /= fNeighbourCount;
	vec3SeparationSteering = -vec3SeparationSteering;	// We want to separation, so use negative to repel.

	// Scale alignment to the alignment velocity.
	vec3AlignmentSteering.Normalize();
	vec3AlignmentSteering *= m_fAlignmentVelocity;

	// Scale cohesion to the cohesion velocity.
	vec3CohesionSteering.Normalize();
	vec3CohesionSteering *= m_fCohesionVelocity;

	// Scale separation to the separation velocity.
	vec3SeparationSteering.Normalize();
	vec3SeparationSteering *= m_fSeparationVelocity;

	// Concatenate the alignment cohesion and separation steerings to produce a desired velocity.
	CVec3 vec3DesiredVelocity = vec3AlignmentSteering + vec3CohesionSteering + vec3SeparationSteering;

	return vec3DesiredVelocity;
}

CVec3 CAIComponent::DoFollowing(const CVec3& _krvec3OurPosition)
{
	// Get the target actor and physics component.
	CActor* pLeaderActor = g_pApp->GetGame()->GetActor(m_targetActorID);
	if (!pLeaderActor) {
		return CVec3();
	}

	CPhysicsComponent* pLeaderPhysics = pLeaderActor->GetComponent<CPhysicsComponent>();
	if (!pLeaderPhysics) {
		return CVec3();
	}

	// Get the position behind the leader to follow;
	CVec3 vec3LeaderPosition = pLeaderPhysics->GetTransform().GetPosition();
	CVec3 vec3LeaderVelocity = pLeaderPhysics->GetVelocity();
	vec3LeaderVelocity.Normalize();

	CVec3 vec3BehindPosition = vec3LeaderPosition - (m_fFollowDistance * vec3LeaderVelocity);
	CVec3 vec3AheadPosition = vec3LeaderPosition + (m_fFollowDistance * vec3LeaderVelocity);

	// Seek behind position.
	CVec3 vec3DesiredVelocity = CSteeringBehaviours::Seek(_krvec3OurPosition, vec3BehindPosition, m_fArrivalDistance);
	vec3DesiredVelocity *= m_fMaxSpeed;

	// Calculate separation steering.
	CVec3 vec3SeperationSteering;
	float fNeighbourCount = 0.0f;

	for (unsigned int i = 0; i < m_pOtherFollowers->GetSize(); ++i) {
		CPhysicsComponent* pNeighbourPhysics = m_pOtherFollowers->Get(i);
		DEBUG_ASSERT(pNeighbourPhysics);

		if (pNeighbourPhysics->GetOwner() == GetOwner()) {
			// Don't separate from ourselves.
			continue;
		}

		CVec3 vec3NeighbourPosition = pNeighbourPhysics->GetTransform().GetPosition();
		CVec3 vec3Distance = vec3NeighbourPosition - _krvec3OurPosition;

		if (vec3Distance.GetSquaredLength() < m_fNeighbourRadiusSquared) {
			// Is this actor actually a neighbour?
			++fNeighbourCount;
			// Calculate separation.
			vec3SeperationSteering += vec3Distance;
		}
	}

	if (fNeighbourCount != 0) {
		vec3SeperationSteering /= fNeighbourCount;
		vec3SeperationSteering = vec3SeperationSteering;
		//vec3SeperationSteering.Normalize();
		vec3SeperationSteering *= -m_fSeparationFollowing;
	}

	// Add the separation steering.
	vec3DesiredVelocity += vec3SeperationSteering;

	CVec3 vec3DistanceToAhead = vec3AheadPosition - _krvec3OurPosition;
	CVec3 vec3DistanceToLeader = vec3LeaderPosition - _krvec3OurPosition;
	if (vec3DistanceToAhead.GetSquaredLength() <= m_fLeaderSightRadiusSquared ||
		vec3DistanceToLeader.GetSquaredLength() <= m_fLeaderSightRadiusSquared) {
		// Are we in the way of the leader?
		// Evade the leader.
		CVec3 vec3Evade = CSteeringBehaviours::Evade(_krvec3OurPosition, vec3LeaderPosition, vec3LeaderVelocity, m_fMaxSpeed);
		vec3DesiredVelocity += vec3Evade * m_fEvadeVelocity;
	}

	return vec3DesiredVelocity;
}

CVec3 CAIComponent::GetTargetVelocity()
{
	CActor* pTargetActor = g_pApp->GetGame()->GetActor(m_targetActorID);
	if (!pTargetActor) {
		m_targetActorID = CActor::s_kINVALID_ACTOR_ID;
		Wander();
		return CVec3();
	}

	CPhysicsComponent* pTargetPhysics = pTargetActor->GetComponent<CPhysicsComponent>();
	if (!pTargetPhysics) {
		Wander();
	}

	return pTargetPhysics->GetVelocity();
}

CVec3 CAIComponent::GetTargetPosition()
{
	CActor* pTargetActor = g_pApp->GetGame()->GetActor(m_targetActorID);
	if (!pTargetActor) {
		m_targetActorID = CActor::s_kINVALID_ACTOR_ID;
		Wander();
		return CVec3();
	}

	CPhysicsComponent* pTargetPhysics = pTargetActor->GetComponent<CPhysicsComponent>();
	if (!pTargetPhysics) {
		Wander();
		return CVec3();
	}

	return pTargetPhysics->GetTransform().GetPosition();
}

CVec3 CAIComponent::LimitForce(const CVec3& _krvec3Force)
{
	float fForce = _krvec3Force.GetLength();
	if (fForce > m_fMaxForce && fForce != 0) {
		CVec3 vec3Force = (_krvec3Force / fForce) * m_fMaxForce;
		
		return vec3Force;
	}

	return _krvec3Force;
}

CVec3 CAIComponent::LimitVelocity(const CVec3& _krvec3Velocity)
{
	float fVelocity = _krvec3Velocity.GetLength();
	if (fVelocity > m_fMaxSpeed && fVelocity != 0) {
		CVec3 vec3Velocity = (_krvec3Velocity / fVelocity) * m_fMaxSpeed;

		return vec3Velocity;
	}

	return _krvec3Velocity;
}

void CAIComponent::MoveActorDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_MoveActor> pMoveActorData = static_pointer_cast<CEventData_MoveActor>(_pEventData);

	TActorID actorID = pMoveActorData->GetActorID();
	if (actorID != CActor::s_kINVALID_ACTOR_ID) {
		CActor* pActor = g_pApp->GetGame()->GetActor(actorID);

		if (pActor) {
			auto findIter = m_actions.find(pActor->GetType());
			if (findIter == m_actions.end()) {
			// Is there no action for this actor?
				return;
			}

			CPhysicsComponent* pPhysicsComponent = pActor->GetComponent<CPhysicsComponent>();
			if (!pPhysicsComponent) {
				return;
			}

			CPhysicsComponent* pOurPhysicsComponent = GetOwner()->GetComponent<CPhysicsComponent>();
			if (!pPhysicsComponent) {
				DEBUG_ERROR("An CAIComponent requires a physics component.");
				return;
			}

			TActionInfo actionInfo = findIter->second;
			EAIState eState = actionInfo.m_eAction;

			// Get the distance to the target.
			CVec3 vec3Position = pPhysicsComponent->GetTransform().GetPosition();
			CVec3 vec3Distance = vec3Position - pOurPhysicsComponent->GetTransform().GetPosition();

			if (vec3Distance.GetSquaredLength() <= findIter->second.m_fDistanceSquared) {
			// Are we within range?
				switch (eState) {
					case CAIComponent::AI_SEEK: {
						Seek(actorID);
						break;
					}
					case CAIComponent::AI_FLEE: {
						Flee(actorID);
						break;
					}
					case CAIComponent::AI_PURSUE: {
						Pursue(actorID);
						break;
					}
					case CAIComponent::AI_EVADE: {
						Evade(actorID);
						break;
					}
					case CAIComponent::AI_WANDER: {
						Wander();
						break;
					}
					case CAIComponent::AI_FLOCKING: {
						// TODO: could also have flocking work as a an action.
						break;
					}
					case CAIComponent::AI_FOLLOW: {
						// TODO: these values should probably be something; maybe loaded in with the action?
						Follow(actorID, 0.0f, 0.0f, 0.0f, 0.0f, 10);
						break;
					}
					default: {
						break;
					}
				}
			}
		}
	}
}

bool CAIComponent::VIsActiveOnRemote() const
{
	return false;
}
