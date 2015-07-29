// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SteeringBehaviours.h
// Description	: CSteeringBehaviours declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __STEERINGBEHAVIOURS_H__
#define __STEERINGBEHAVIOURS_H__

// Library Includes

// Local Includes

class CPhysicsComponent;

/************************************************************************/
/* CSteeringGroup 
/*
/* Maintains a group of actors.
/*
/* This class primarily wraps a list of pointers to physics components
/* which makes up the steering group.
/* It only provides the bare essentials for manipulating and using 
/* Steering groups.
/************************************************************************/
class CSteeringGroup {
	// Member Functions
public:
	CSteeringGroup();
	~CSteeringGroup();

	void Add(CActor* _pActor);
	void Remove(TActorID _actorID);

	CPhysicsComponent* Get(unsigned int _uiIndex);
	const CPhysicsComponent* Get(unsigned int _uiIndex) const;

	unsigned int GetSize() const;
protected:
private:
	
	// Member Variables
public:
protected:
private:
	std::vector<CPhysicsComponent*> m_vecGroup;
};

typedef shared_ptr<CSteeringGroup> TSteeringGroupPtr;

/************************************************************************/
/* CSteeringBehaviours
/*
/* This class just maintains the various steering groups of the game.
/* Having the groups in a single class like this helps AI to be added to
/* groups and removed from groups without specifically having the tell
/* the other AI of the group about the change.
/************************************************************************/
class CSteeringBehaviours {
	// Member Functions
	struct TFollower {
		CPhysicsComponent* m_pPhysicsComponent;
		float m_fFollowDistance;
	};

	typedef std::map<std::string, TSteeringGroupPtr> TFlockMap;
	typedef std::map<TActorID, TSteeringGroupPtr> TFollowingMap;
public:
	CSteeringBehaviours();
	~CSteeringBehaviours();

	void Update(float _fDeltaTime);

/*
	void Follow(TActorID _actorID, TActorID _actorIDTarget, float _fFollowDistance);
	void AddToFlock(TActorID _actorID, const std::string& strFlockType);*/
	/**
	* Adds an actor to a flock specified with a type.
	*
	* If the type of flock doesn't exist it is created.
	* The function then returns the steering group of the flock or nullptr, if the actor failed to be added.
	* 
	* @author: 	Hayden Asplet
	* @param:	std::string & _krstrFlockType - The type of flock to add the actor to.
	* @param:	CActor * _pActor - the actor to add to the flock.
	* @return:  TSteeringGroupPtr - The steering group for the flock. nullptr if the actor failed to be added.
	*/
	TSteeringGroupPtr AddToFlock(std::string& _krstrFlockType, CActor* _pActor);

	/**
	* Adds an actor to a steering group of followers.
	*
	* The function returns a pointer to the steering group following the target.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorTarget - the target to follow.
	* @param:	CActor * _pActor - actor to add to the group of followers.
	* @return:  TSteeringGroupPtr - the steering group for the followers. nullptr if the actor failed to be added.
	*/
	TSteeringGroupPtr Follow(TActorID _actorTarget, CActor* _pActor);

	/**
	* Returns a desired direction to seek a target.
	* 
	* @author: 	Hayden Asplet
	* @param:	const CVec3 & _krPosition - current position.
	* @param:	const CVec3 _krTargetPosition - target position.
	* @param:	float _fArrivalDistance - distance to being arrival
	* @return:  CVec3 - steering vector 
	*/
	static CVec3 Seek(const CVec3& _krPosition, const CVec3& _krTargetPosition, float _fArrivalDistance = 0.0f);

	/**
	* Returns a steering vector flee from a target.
	* 
	* @author: 	Hayden Asplet
	* @param:	const CVec3 & _krPosition - current position.
	* @param:	const CVec3 _krTargetPosition - target position.
	* @return:  CVec3 - steering vector.
	*/
	static CVec3 Flee(const CVec3& _krPosition, const CVec3& _krTargetPosition) { return Seek(_krTargetPosition, _krPosition); }
	/**
	* Returs steering vector for evading a target.
	* 
	* @author: 	Hayden Asplet
	* @param:	const CVec3& _krPosition - current position.
	* @param:	const CVec3& _krTargetPosition - target's position
	* @param:	const CVec3& _krTargetVelocity - target's velocity.
	* @param:	float _fMaxVelocity - maximum velocity.
	* @return:  CVec3 - steering vector.
	*/
	static CVec3 Evade(const CVec3& _krPosition, const CVec3& _krTargetPosition, const CVec3& _krTargetVelocity, float _fMaxVelocity);
	/**
	* Returns steering vector for pursueing a target.
	* 
	* @author: 	Hayden Asplet
	* @param:	const CVec3 & _krPosition
	* @param:	const CVec3 & _krTargetPosition
	* @param:	const CVec3 & _krTargetVelocity
	* @param:	float _fMaxVelocity
	* @return:  CVec3
	*/
	static CVec3 Pursue(const CVec3& _krPosition, const CVec3& _krTargetPosition, const CVec3& _krTargetVelocity, float _fMaxVelocity);
protected:
private:
	/*void DoFlock(const TFlock& _krFlock);
	void DoFollow(const TActorID _actorID, const TFollowers& _krFollowers);*/

	void DestroyActorDelegate(TEventDataPtr _pEventData);

	// Member Variables
public:
protected:
private:
	TFlockMap m_flockMap;
	TFollowingMap m_followingMap;
};

#endif	// __STEERINGBEHAVIOURS_H__