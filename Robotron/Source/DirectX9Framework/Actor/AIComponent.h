// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AIComponent.h
// Description	: CAIComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __AICOMPONENT_H__
#define __AICOMPONENT_H__

// Library Includes

// Local Includes
#include "ActorComponent.h"

class CPhysicsComponent;

class CAIComponent : public AActorComponent {
	enum EAIState {
		AI_NONE,
		AI_SEEK,
		AI_FLEE,
		AI_PURSUE,
		AI_EVADE,
		AI_WANDER,
		AI_FLOCKING,
		AI_FOLLOW
	};

	struct TActionInfo {
		EAIState m_eAction;
		float m_fDistanceSquared;
		std::string m_strActionName;
	};

	typedef std::map<std::string, TActionInfo> TActionMap;

	// Member Functions
public:
	CAIComponent();
	virtual ~CAIComponent();

	virtual bool VInitialise(TiXmlElement* _pXmlData) override;
	virtual void VPostInitialise() override;
	virtual void VUpdate(float _fDeltaTime) override;

	void Seek(TActorID _targetID);
	void Flee(TActorID _targetID);
	void Evade(TActorID _targetID);
	void Pursue(TActorID _targetID);
	void Wander();
	void Follow(TActorID _leaderID, float _fDistance, float _fSeperation, float _fLeaderSightRadius, float _fEvadeVelocity, float _fNeighbourRadius);
protected:
private:
	CVec3 DoSeek(const CVec3& _krvec3OurPosition);
	CVec3 DoFlee(const CVec3& _krvec3OurPosition);
	CVec3 DoEvade(const CVec3& _krvec3OurPosition);
	CVec3 DoPursue(const CVec3& _krvec3OurPosition);
	CVec3 DoWander(const CVec3& _krvec3OurPosition, const CVec3& _krvec3Velocity);
	CVec3 DoFlock(const CVec3& _krvec3OurPosition);
	CVec3 DoFollowing(const CVec3& _krvec3OurPosition);

	CVec3 GetTargetVelocity();
	CVec3 GetTargetPosition();

	CVec3 LimitForce(const CVec3& _krvec3Force);
	CVec3 LimitVelocity(const CVec3& _krvecVelocity);

	void MoveActorDelegate(TEventDataPtr _pEventData);

	virtual bool VIsActiveOnRemote() const override;

	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	TActorID m_targetActorID;	// The target to seek, flee, evade, follow or pursue.

	EAIState m_eState;
	TActionMap m_actions;

	// General
	float m_fMaxSpeed;
	float m_fMaxForce;

	// Flocking
	TSteeringGroupPtr m_pFlock;
	float m_fAlignmentVelocity;
	float m_fSeparationVelocity;
	float m_fCohesionVelocity;
	float m_fNeighbourRadiusSquared;
	std::string m_strFlockType;

	// Following.
	TSteeringGroupPtr m_pOtherFollowers;
	float m_fFollowDistance;
	float m_fSeparationFollowing;
	float m_fLeaderSightRadiusSquared;
	float m_fEvadeVelocity;

	// Seeking
	float m_fArrivalDistance;
	bool m_bIsFleeing;

	// Wandering.
	CVec3 m_vec3WanderTarget;	// The wander target of last tick.
	float m_fWanderRadius;		// The radius of the wander circle.
	float m_fWanderDistance;	// The distance to the wander circle.
	float m_fWanderJitter;		// The amount angular displacement.

	// Containment.
	float m_fContainerWidth;	// The width of the containment box.
	float m_fContainerLength;	// The length of the containment box.
	bool m_bIsContained;		// True if the actor is contained to the containment box.
};

#endif // __AICOMPONENT_H__