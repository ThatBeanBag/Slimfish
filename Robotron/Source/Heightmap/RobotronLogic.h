// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: HeightMapLogic.h
// Description	: CHeightMapLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __HEIGHTMAPLOGIC_H__
#define __HEIGHTMAPLOGIC_H__

// Library Includes

// Local Includes
#include "RobotronHumanView.h"

struct TLevelData {
	int m_iNumEnemyTeir1;
	int m_iNumEnemyTeir2;
	int m_iNumEnemyTeir3;
};

class CRobotronLogic : public CGameLogic {
	// Member Functions
public:
	CRobotronLogic();
	~CRobotronLogic();

protected:
private:
	// Delegating virtual functions
	virtual bool VInitialise() override;
	virtual void VUpdate(float _fDeltaTime) override;
	virtual bool VOnRestore() override;
	virtual bool VLoadGame() override;
	virtual void VOnStateChange(EGameState _eNewState) override;
	virtual void VRegisterDelegates() override;
	virtual void VRemoveDelegates() override;

	void SpawnEnemies();
	void IncrementLevel();

	// Event Delegates
	void ChangeDirectionDelegate(shared_ptr<IEventData> _pEventData);
	void FireBulletDelegate(shared_ptr<IEventData> _pEventData);
	void CollisionDelegate(shared_ptr<IEventData> _pEventData);
	void CollisionSeperationDelegate(shared_ptr<IEventData> _pEventData);
	void RemotePlayerActorAssigmentDelegate(shared_ptr<IEventData> _pEventData);
	void StartMovingDelegate(shared_ptr<IEventData> _pEventData);
	void StopMovingDelegate(shared_ptr<IEventData> _pEventData);
	void ActorKilledDelegate(shared_ptr<IEventData> _pEventData);

	virtual void VAddNetworkEventsServerToClient(CNetworkEventForwarder* _pEventForwarder) override;
	virtual void VAddNetworkEventsClientToServer(CNetworkEventForwarder* _pEventForwarder) override;

	// Member Variables
public:
protected:
private:
	CHumanView* m_pHumanView;

	TActorID m_treeID;
	float m_fTreeRespawnTimer;

	float m_fPowerupTimer;
	int m_iActivePowerups;

	int m_iAlivePlayers;

	int m_iRemainingEnemies;
	int m_iLevelCount;
	TLevelData m_currentLevelData;
};

#endif	// __HEIGHTMAPLOGIC_H__
