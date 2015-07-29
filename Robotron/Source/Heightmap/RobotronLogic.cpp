// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: HeightMapLogic.cpp
// Description	: CHeightMapLogic implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "RobotronLogic.h"

// Local Includes
#include "..\DirectX9Framework\Actor\AIComponent.h"
#include "..\DirectX9Framework\Actor\PhysicsComponent.h"
#include "..\DirectX9Framework\Actor\TransformComponent.h"
#include "..\DirectX9Framework\Events\EventData_RemotePlayerActorAssignment.h"
#include "..\DirectX9Framework\Resource\XmlResource.h"
#include "RobotronHumanView.h"
#include "MainMenuView.h"
#include "EventData_ChangeDirection.h"
#include "EventData_FireBullet.h"
#include "LobbyView.h"
#include "AiView.h"
#include "PlayerComponent.h"
#include "EventData_ActorKilled.h"
#include "EventData_PlayerStateChange.h"
#include "EventData_LevelStateChange.h"

CRobotronLogic::CRobotronLogic()
	:m_iLevelCount(0),
	m_fPowerupTimer(0.0f),
	m_iRemainingEnemies(0),
	m_iActivePowerups(0)
{

}

CRobotronLogic::~CRobotronLogic()
{

}

bool CRobotronLogic::VInitialise()
{
	// Register components to the actor factory.
	GetActorFactory()->RegisterComponent<CPlayerComponent>();

	return true;
}

void CRobotronLogic::VUpdate(float _fDeltaTime)
{
	switch (GetGameState()) {
		case CGameLogic::GS_LOBBY: {
			break;
		}
		case CGameLogic::GS_RUNNING: {
			if (!IsRemote()) {
				m_fPowerupTimer += _fDeltaTime;

				float fRandomNumber = g_pApp->GetRandomNumberReal(g_kfPOWERUP_RANDOM_TIME_RANGE_MIN, g_kfPOWERUP_RANDOM_TIME_RANGE_MAX);

				if (m_fPowerupTimer >= fRandomNumber && m_iActivePowerups < g_kiMAX_POWERUPS) {
					// Is it time to spawn a power-up and have we not reached the limit?
					// Create a power-up.
					if (g_pApp->GetRandomNumberReal(0, 1) < g_kfPOWERUP_CHANCE_TO_SPAWN_CRATE) {
						CreateActor("Actors/CratePowerUp.xml");
					}
					else if (g_pApp->GetRandomNumberReal(0, 1) < g_kfPOWERUP_CHANCE_TO_SPAWN_GOLD) {
					// Did we roll the die high enough to spawn a golden apple?
						CreateActor("Actors/GoldenPowerUp.xml");
					}
					else {
						CreateActor("Actors/StandardPowerUp.xml");
					}

					++m_iActivePowerups;

					// Reset the timer.
					m_fPowerupTimer = 0.0f;
				}

				if (m_iRemainingEnemies <= 0) {
					IncrementLevel();
				}

				if (m_iAlivePlayers <= 0) {
					g_pApp->SetPaused(true);
				}
			}

			break;
		}
		default:
			break;
	}
}

bool CRobotronLogic::VOnRestore()
{
	return true;
}

bool CRobotronLogic::VLoadGame()
{
	return true;
}

void CRobotronLogic::VOnStateChange(EGameState _eNewState)
{
	switch (_eNewState) {
		case CGameLogic::GS_INVALID: {
			break;
		}
		case CGameLogic::GS_INITIALISING: {
			break;
		}
		case CGameLogic::GS_MAINMENU: {
			// Close the network if there is one.
			g_pApp->CloseNetwork();
			ClearActors();

			if (m_pHumanView) {
				RemoveGameView(m_pHumanView);
				m_pHumanView = nullptr;
			}

			g_pApp->GetRenderer()->VSetAmbientColour(CreateColourRGB(45, 45, 45));
			g_pApp->GetRenderer()->VSetBackgroundColour(CreateColourRGB(45, 45, 45));

			unique_ptr<CMainMenuView> pMainMenuView(new CMainMenuView());
			m_pHumanView = pMainMenuView.get();

			if (!m_pHumanView) {
				return;
			}

			AddGameView(std::move(pMainMenuView));
			break;
		}
		case CGameLogic::GS_LOBBY: {
			if (m_pHumanView) {
				RemoveGameView(m_pHumanView);
				m_pHumanView = nullptr;
			}

			// Create a lobby view for the player.
			unique_ptr<CLobbyView> pHumanView(new CLobbyView(GetUsername(), GetMaxPlayers()));

			if (!pHumanView || !pHumanView->Initialise()) {
				ChangeState(GS_MAINMENU);
				return;
			}

			m_pHumanView = pHumanView.get();
			AddGameView(std::move(pHumanView));

			break;
		}
		case CGameLogic::GS_LOADING: {
			if (m_pHumanView) {
				RemoveGameView(m_pHumanView);
				m_pHumanView = nullptr;
			}

			ClearActors();

			unique_ptr<CRobotronHumanView> pHumanView(new CRobotronHumanView());

			if (!pHumanView) {
				return;
			}

			m_pHumanView = pHumanView.get();
			AddGameView(std::move(pHumanView));

			// Load the environment.
			CreateActor("Actors/SpotLight.xml");
			CreateActor("Actors/GroundPlane.xml");
			CreateActor("Actors/KillPlane.xml");
			CreateActor("Actors/Tree.xml");

			//m_pTreeNode = m_pHumanView->GetScene()->FindSceneNodeFromActorID(treeID);
			
			// Pre-Load bullets
			CResourceManager<CXmlResource>::GetInstance()->Load("Actors/Bullet.xml");
			CResourceManager<IMesh>::GetInstance()->Load("Meshes/Apple/Applered.x");

			m_iRemainingEnemies = g_kiNUM_OF_ENEMIES_MIN;

			ChangeState(CGameLogic::GS_WAITING_FOR_PLAYERS_TO_LOAD);

			break;
		}
		case CGameLogic::GS_WAITING_FOR_PLAYERS_TO_LOAD: {
			// Tell the server we've loaded.
			shared_ptr<CEventData_LoadedGame> pEvent(new CEventData_LoadedGame());
			CEventManager::GetInstance()->TriggerEvent(pEvent);

			break;
		}
		case CGameLogic::GS_SPAWNING_PLAYERS: {
			if (IsRemote()) {
				ChangeState(GS_RUNNING);
				return;
			}

			for (auto iter = m_gameViews.begin(); iter != m_gameViews.end(); ++iter) {
				// Create a player actor for each attached game view.
				TActorID actorID = CreateActor("Actors/Player.xml");
				(*iter)->VSetControlledActor(actorID);

				CActor* pPlayer = GetActor(actorID);
				if (pPlayer) {
					CPhysicsComponent* pPhysicsComponent = pPlayer->GetComponent<CPhysicsComponent>();
					pPhysicsComponent->SetAngularFactor(CVec3(0.0f, 0.0f, 0.0f));
				}

				m_iAlivePlayers++;
			}

			IncrementLevel();

			ChangeState(CGameLogic::GS_RUNNING);
		}
		case CGameLogic::GS_RUNNING: {
			break;
		}
		default: {
			break;
		}
	}
}

void CRobotronLogic::VRegisterDelegates()
{
	CEventManager* pEventManager = CEventManager::GetInstance().get();
	pEventManager->AddListener<CEventData_StartMoving>(MakeDelegate(this, &CRobotronLogic::StartMovingDelegate));
	pEventManager->AddListener<CEventData_StopMoving>(MakeDelegate(this, &CRobotronLogic::StopMovingDelegate));
	pEventManager->AddListener<CEventData_ChangeDirection>(MakeDelegate(this, &CRobotronLogic::ChangeDirectionDelegate));
	pEventManager->AddListener<CEventData_FireBullet>(MakeDelegate(this, &CRobotronLogic::FireBulletDelegate));
	pEventManager->AddListener<CEventData_Collision>(MakeDelegate(this, &CRobotronLogic::CollisionDelegate));
	pEventManager->AddListener<CEventData_CollisionSeparation>(MakeDelegate(this, &CRobotronLogic::CollisionSeperationDelegate));
	pEventManager->AddListener<CEventData_RemotePlayerActorAssignment>(MakeDelegate(this, &CRobotronLogic::RemotePlayerActorAssigmentDelegate));
	pEventManager->AddListener<CEventData_ActorKilled>(MakeDelegate(this, &CRobotronLogic::ActorKilledDelegate));
}

void CRobotronLogic::VRemoveDelegates()
{
	CEventManager* pEventManager = CEventManager::GetInstance().get();
	pEventManager->RemoveListener<CEventData_StartMoving>(MakeDelegate(this, &CRobotronLogic::StartMovingDelegate));
	pEventManager->RemoveListener<CEventData_StopMoving>(MakeDelegate(this, &CRobotronLogic::StopMovingDelegate));
	pEventManager->RemoveListener<CEventData_ChangeDirection>(MakeDelegate(this, &CRobotronLogic::ChangeDirectionDelegate));
	pEventManager->RemoveListener<CEventData_FireBullet>(MakeDelegate(this, &CRobotronLogic::FireBulletDelegate));
	pEventManager->RemoveListener<CEventData_Collision>(MakeDelegate(this, &CRobotronLogic::CollisionDelegate));
	pEventManager->RemoveListener<CEventData_CollisionSeparation>(MakeDelegate(this, &CRobotronLogic::CollisionSeperationDelegate));
	pEventManager->RemoveListener<CEventData_RemotePlayerActorAssignment>(MakeDelegate(this, &CRobotronLogic::RemotePlayerActorAssigmentDelegate));
	pEventManager->RemoveListener<CEventData_ActorKilled>(MakeDelegate(this, &CRobotronLogic::ActorKilledDelegate));
}

void CRobotronLogic::SpawnEnemies()
{
	for (int i = 0; i < m_currentLevelData.m_iNumEnemyTeir1; ++i) {
		CreateActor("Actors/Enemy.xml");
	}

	for (int i = 0; i < m_currentLevelData.m_iNumEnemyTeir2; ++i) {
		CreateActor("Actors/ChargeRam.xml");
	}

	for (int i = 0; i < m_currentLevelData.m_iNumEnemyTeir3; ++i) {
		CreateActor("Actors/TinyRam.xml");
	}
}

void CRobotronLogic::IncrementLevel()
{
	m_iLevelCount++;
	m_iRemainingEnemies = m_iLevelCount + g_kiNUM_OF_ENEMIES_MIN;

	if (m_iRemainingEnemies > g_kiNUM_OF_ENEMIES_MAX) {
		m_iRemainingEnemies = g_kiNUM_OF_ENEMIES_MAX;
	}

	m_currentLevelData.m_iNumEnemyTeir1 = 0;
	m_currentLevelData.m_iNumEnemyTeir2 = 0;
	m_currentLevelData.m_iNumEnemyTeir3 = 0;

	m_currentLevelData.m_iNumEnemyTeir1 = std::max(0, m_iRemainingEnemies - (m_iLevelCount - 1));

	for (int i = 0; i < m_iRemainingEnemies - m_currentLevelData.m_iNumEnemyTeir1; ++i) {
		if (g_pApp->GetRandomNumberReal(0.0f, 1.0f) > 0.5f) {
			m_currentLevelData.m_iNumEnemyTeir2++;
		}
		else {
			m_currentLevelData.m_iNumEnemyTeir3++;
		}
	}

	SpawnEnemies();

	shared_ptr<CEventData_LevelStateChange> pEvent(new CEventData_LevelStateChange(m_iLevelCount, m_iRemainingEnemies));
	CEventManager::GetInstance()->QueueEvent(pEvent);
}

void CRobotronLogic::StartMovingDelegate(shared_ptr<IEventData> _pEventData)
{
	if (IsRemote()) {
		return;
	}

	shared_ptr<CEventData_StartMoving> pStartMovingData = static_pointer_cast<CEventData_StartMoving>(_pEventData);

	// Get the actor from the ID of the event data.
	TActorID actorID = pStartMovingData->GetActorID();
	CActor* pActor = GetActor(actorID);

	if (!pActor) {
		// Is there no actor with this ID?
		DEBUG_ERROR("No actor found with ID: " + ToString(actorID) + " when starting to move actor");
	}
	else {
		// Found an actor with this ID.
		CPhysicsComponent* pPhysicsComponent = pActor->GetComponent<CPhysicsComponent>();

		if (pPhysicsComponent) {
			CVec3 vec3Velocity = pStartMovingData->GetDirection();
			pPhysicsComponent->ApplyAcceleration(vec3Velocity, pStartMovingData->GetAcceleration(), pStartMovingData->GetMaxVelocity());
		}
	}
}

void CRobotronLogic::StopMovingDelegate(shared_ptr<IEventData> _pEventData)
{
	shared_ptr<CEventData_StartMoving> pStopMovingData = static_pointer_cast<CEventData_StartMoving>(_pEventData);

	// Get the actor from the ID of the event data.
	TActorID actorID = pStopMovingData->GetActorID();
	CActor* pActor = GetActor(actorID);

	if (!pActor) {
		// Is there no actor with this ID?
		DEBUG_ERROR("No actor found with ID: " + ToString(actorID) + " when starting to move actor");
	}
	else {
		// Found an actor with this ID.
		CPhysicsComponent* pPhysicsComponent = pActor->GetComponent<CPhysicsComponent>();

		if (pPhysicsComponent) {
			pPhysicsComponent->RemoveAcceleration();
		}
	}
}

void CRobotronLogic::ActorKilledDelegate(shared_ptr<IEventData> _pEventData)
{
	shared_ptr<CEventData_ActorKilled> pActorKilledData = static_pointer_cast<CEventData_ActorKilled>(_pEventData);

	std::string strType = pActorKilledData->GetActorType();

	if (strType == "StandardPowerUp" || strType == "GoldenPowerUp" || strType == "CratePowerUp") {
		// Decrement active power-ups.
		m_iActivePowerups--;
	}
	else if (strType == "Enemy" || strType == "ChargeRam" || strType == "TinyRam") {
		// Decrement remaining enemies;
		m_iRemainingEnemies--;
		shared_ptr<CEventData_LevelStateChange> pEvent(new CEventData_LevelStateChange(m_iLevelCount, m_iRemainingEnemies));
		CEventManager::GetInstance()->QueueEvent(pEvent);

	}
	else if (strType == "Player") {
		CActor* pActor = GetActor(pActorKilledData->GetActorID());
		if (pActor) {
			CPlayerComponent* pPlayerComponent = pActor->GetComponent<CPlayerComponent>();
			if (pPlayerComponent) {
				pPlayerComponent->DecrementLives();

				if (pPlayerComponent->GetLives() <= 0) {
					m_iAlivePlayers--;
					DestroyActor(pActorKilledData->GetActorID());
				}
			}
		}

	}
}

void CRobotronLogic::ChangeDirectionDelegate(shared_ptr<IEventData> _pEventData)
{
	if (IsRemote()) {
		return;
	}

	shared_ptr<CEventData_ChangeDirection> pChangeDirectionData = static_pointer_cast<CEventData_ChangeDirection>(_pEventData);
	
	// Get the actor from the ID of the event data.
	TActorID actorID = pChangeDirectionData->GetActorID();
	CActor* pActor = GetActor(actorID);

	if (!pActor) {
		// Is there no actor with this ID?
		DEBUG_WARNING("No actor found with ID: " + ToString(actorID) + " when changing the direction of the actor.");
	}
	else {
		// Found an actor with this ID.
		CPhysicsComponent* pPhysicsComponent = pActor->GetComponent<CPhysicsComponent>();

		if (pPhysicsComponent) {
		// Is there a physics component for this actor?
			// Set the rotation.
			//pPhysicsComponent->ApplyTorque(pChangeDirectionData->GetDirection(), 1.0f);
			pPhysicsComponent->Rotate(pChangeDirectionData->GetDirection());
		}
	}
}

void CRobotronLogic::FireBulletDelegate(shared_ptr<IEventData> _pEventData)
{
	if (IsRemote()) {
		return;
	}

	shared_ptr<CEventData_FireBullet> pChangeDirectionData = static_pointer_cast<CEventData_FireBullet>(_pEventData);

	// Get the actor from the ID of the event data.
	TActorID actorID = pChangeDirectionData->GetActorID();
	CActor* pActor = GetActor(actorID);

	if (!pActor) {
		// Is there no actor with this ID?
		DEBUG_WARNING("No actor found with ID: " + ToString(actorID) + " when changing the direction of the actor.");
	}
	else {
		// Found an actor with this ID.
		CTransformComponent* pTransformComponent = pActor->GetComponent<CTransformComponent>();
		CPlayerComponent* pPlayerComponent = pActor->GetComponent<CPlayerComponent>();

		if (pPlayerComponent) {
			if (pPlayerComponent->GetAmmo() <= 0) {
			// Does this player have enough ammo to fire a bullet?
				return;
			}
			else {
				pPlayerComponent->DecrementAmmo();
			}
		}

		if (pTransformComponent) {
		// Is there a transform component for this actor?
			// Create a bullet at this transform.
			CMatrix4x4 matTransform = pTransformComponent->GetTransform();
			CVec3 vec3Direction = matTransform.GetForward();
			CVec3 vec3Position = matTransform.GetPosition();
			vec3Direction.Normalize();
			vec3Position -= vec3Direction;
			matTransform = BuildRotationDirection(vec3Direction);
			matTransform.SetPosition(vec3Position);

			bool bUseGolden = false;

			CPlayerComponent* pPlayerComponent = pActor->GetComponent<CPlayerComponent>();
			if (pPlayerComponent) {
				bUseGolden = pPlayerComponent->HasGoldenPowerup();
			}

			CActor* pBullet = nullptr;

			if (bUseGolden) {
				pBullet = GetActor(CreateActor("Actors/GoldenBullet.xml", &matTransform));
			}
			else {
				pBullet = GetActor(CreateActor("Actors/Bullet.xml", &matTransform));
			}

			if (pBullet) {
				CPhysicsComponent* pPhysicsComponent = pBullet->GetComponent<CPhysicsComponent>();
				if (pPhysicsComponent) {
					pPhysicsComponent->SetVelocity(vec3Direction * g_kfBULLET_VELOCITY);
				}
			}
		}
	}
}

void CRobotronLogic::CollisionDelegate(shared_ptr<IEventData> _pEventData)
{
	shared_ptr<CEventData_Collision> pCollisionData = static_pointer_cast<CEventData_Collision>(_pEventData);

	CActor* pActor1 = GetActor(pCollisionData->GetActorID1());
	CActor* pActor2 = GetActor(pCollisionData->GetActorID2());

	if (!pActor1 || !pActor2) {
		//DEBUG_ERROR("Collision event was triggered on actor(s) that are non existent.");
		return;
	}

	std::string strActor1Type = pActor1->GetType();
	std::string strActor2Type = pActor2->GetType();

	if (IsRemote()) {
		return;
	}

	if (((strActor1Type == "StandardPowerUp" || strActor1Type == "GoldenPowerUp" || strActor1Type == "CratePowerUp") &&
		strActor2Type == "Player") || 
		((strActor2Type == "StandardPowerUp" || strActor2Type == "GoldenPowerUp" || strActor2Type == "CratePowerUp") &&
		strActor1Type == "Player")) {
	// Has a player collided with a power-up?
		CActor* pPlayerActor = nullptr;
		CActor* pPowerUpActor = nullptr;

		// Determine which actor is the player and which is the power-up.
		if (strActor1Type == "Player") {
			pPlayerActor = pActor1;
			pPowerUpActor = pActor2;
		}
		else {
			pPlayerActor = pActor2;
			pPowerUpActor = pActor1;
		}

		DEBUG_ASSERT(pPlayerActor);
		DEBUG_ASSERT(pPowerUpActor);

		CPlayerComponent* pPlayerComponent = pPlayerActor->GetComponent<CPlayerComponent>();

		// Apply the power-up.
		if (pPlayerComponent) {
			if (pPowerUpActor->GetType() == "StandardPowerUp") {
				pPlayerComponent->IncrementAmmo();
			}
			else if (pPowerUpActor->GetType() == "GoldenPowerUp") {
				pPlayerComponent->ApplyGoldenPowerup(g_kfPOWERUP_GOLDEN_APPLE_TIME_SPAN);
			}
			else if (pPowerUpActor->GetType() == "CratePowerUp") {
				pPlayerComponent->SetAmmo(pPlayerComponent->GetAmmo() + g_kiPOWERUP_CRATE_AMMO_AMOUNT);
			}
		}

		// Tell subsystems we are killing this actor.
		shared_ptr<CEventData_ActorKilled> pEvent(new CEventData_ActorKilled(pPowerUpActor->GetID(), pPowerUpActor->GetType()));
		CEventManager::GetInstance()->QueueEvent(pEvent);

		// Destroy the power up.
		DestroyActor(pPowerUpActor->GetID());
	}

	if (strActor1Type == "GroundPlane" || strActor2Type == "GroundPlane") {
		CPhysicsComponent* pPhysicsComponent = nullptr;

		if (strActor1Type == "GroundPlane") {
			pPhysicsComponent = pActor2->GetComponent<CPhysicsComponent>();
		}
		else {
			pPhysicsComponent = pActor1->GetComponent<CPhysicsComponent>();
		}

		if (pPhysicsComponent) {
			pPhysicsComponent->SetOnGround(true);
		}

		return;
	}

	if (strActor1Type == "TinyRam" && strActor2Type == "Bullet") {
		DestroyActor(pActor1->GetID());

		shared_ptr<CEventData_ActorKilled> pEvent(new CEventData_ActorKilled(pActor1->GetID(), strActor1Type));
		CEventManager::GetInstance()->QueueEvent(pEvent);

		return;
	}

	if (strActor2Type == "TinyRam" && strActor1Type == "Bullet") {
		DestroyActor(pActor2->GetID());

		shared_ptr<CEventData_ActorKilled> pEvent(new CEventData_ActorKilled(pActor2->GetID(), strActor2Type));
		CEventManager::GetInstance()->QueueEvent(pEvent);

		return;
	}

	if ((strActor1Type == "Player" && strActor2Type == "TinyRam") ||
		(strActor2Type == "Player" && strActor1Type == "TinyRam")) {
		CPlayerComponent* pPlayer = nullptr;

		if (strActor1Type == "Player") {
			pPlayer = pActor1->GetComponent<CPlayerComponent>();
		}
		else {
			pPlayer = pActor2->GetComponent<CPlayerComponent>();
		}

		if (pPlayer) {
			pPlayer->SetAmmo(0);
		}

		return;
	}

	if ((strActor1Type == "Player" && strActor2Type == "Enemy") ||
		(strActor1Type == "Enemy" && strActor2Type == "Player")) {

		CPhysicsComponent* pPhysicsComponentEnemy = nullptr;
		CPhysicsComponent* pPhysicsComponentPlayer = nullptr;

		if (strActor1Type == "Enemy") {
			pPhysicsComponentEnemy = pActor1->GetComponent<CPhysicsComponent>();
			pPhysicsComponentPlayer = pActor2->GetComponent<CPhysicsComponent>();
		}
		else {
			pPhysicsComponentEnemy = pActor2->GetComponent<CPhysicsComponent>();
			pPhysicsComponentPlayer = pActor1->GetComponent<CPhysicsComponent>();
		}

		if (pPhysicsComponentEnemy && pPhysicsComponentPlayer) {
			CVec3 vec3Velocity = pPhysicsComponentEnemy->GetVelocity();
			vec3Velocity.y = g_kfENEMY_PUSH_Y_ADJUST * vec3Velocity.GetSquaredLength();
			pPhysicsComponentPlayer->ApplyForce(vec3Velocity, g_kfENEMY_PUSH_FORCE);
			return;
		}
	}

	if (strActor1Type == "KillPlane" || strActor2Type == "KillPlane") {
		CActor* pActorToKill = nullptr;
		std::string strActorToKillType;

		if (strActor1Type == "KillPlane") {
			pActorToKill = pActor2;
			strActorToKillType = strActor2Type;
		}
		else {
			pActorToKill = pActor1;
			strActorToKillType = strActor1Type;
		}

		DEBUG_ASSERT(pActorToKill);

		// Tell subsystems we are killing this actor.
		shared_ptr<CEventData_ActorKilled> pEvent(new CEventData_ActorKilled(pActorToKill->GetID(), strActorToKillType));
		CEventManager::GetInstance()->QueueEvent(pEvent);

		if (strActorToKillType == "Player") {
			CPhysicsComponent* pPhysicsComponent = pActorToKill->GetComponent<CPhysicsComponent>();
			CTransformComponent* pTransformComponent = pActorToKill->GetComponent<CTransformComponent>();

			if (pPhysicsComponent) {
				pPhysicsComponent->Stop();
			}

			if (pTransformComponent) {
				pTransformComponent->RespawnRandom();
			}

			return;
		}

		// Destroy the actor.
		DestroyActor(pActorToKill->GetID());
	}
}

void CRobotronLogic::CollisionSeperationDelegate(shared_ptr<IEventData> _pEventData)
{
	if (IsRemote()) {
		return;
	}

	shared_ptr<CEventData_CollisionSeparation> pCollisionSeparationData = static_pointer_cast<CEventData_CollisionSeparation>(_pEventData);

	CActor* pActor1 = GetActor(pCollisionSeparationData->GetActorID1());
	CActor* pActor2 = GetActor(pCollisionSeparationData->GetActorID2());

	if (!pActor1 || !pActor2) {
		//DEBUG_ERROR("Collision event was triggered on actor(s) that are non existent.");
		return;
	}

	std::string strActor1Type = pActor1->GetType();
	std::string strActor2Type = pActor2->GetType();

	if (strActor1Type == "GroundPlane" || strActor2Type == "GroundPlane") {
		CPhysicsComponent* pPhysicsComponent = nullptr;

		if (strActor1Type == "GroundPlane") {
			pPhysicsComponent = pActor2->GetComponent<CPhysicsComponent>();
		}
		else {
			pPhysicsComponent = pActor1->GetComponent<CPhysicsComponent>();
		}

		if (pPhysicsComponent) {
			pPhysicsComponent->SetOnGround(false);
		}

		return;
	}
}

void CRobotronLogic::RemotePlayerActorAssigmentDelegate(shared_ptr<IEventData> _pEventData)
{
	if (!IsRemote()) {
		return;
	}

	shared_ptr<CEventData_RemotePlayerActorAssignment> pAssignmentData = static_pointer_cast<CEventData_RemotePlayerActorAssignment>(_pEventData);

	if (m_pHumanView) {
		m_pHumanView->VSetControlledActor(pAssignmentData->GetActorID());
	}
	else {
		DEBUG_WARNING("No human view to assign player actor to.");
	}
}

void CRobotronLogic::VAddNetworkEventsServerToClient(CNetworkEventForwarder* _pEventForwarder)
{
	DEBUG_ASSERT(_pEventForwarder);
	// Register events sent from the server to the clients here.
	_pEventForwarder->AddEventForwarder<CEventData_ActorKilled>();
	_pEventForwarder->AddEventForwarder<CEventData_PlayerStateChange>();
	_pEventForwarder->AddEventForwarder<CEventData_LevelStateChange>();
}

void CRobotronLogic::VAddNetworkEventsClientToServer(CNetworkEventForwarder* _pEventForwarder)
{
	DEBUG_ASSERT(_pEventForwarder);

	// Register events sent from the client to the server here.
	_pEventForwarder->AddEventForwarder<CEventData_FireBullet>();
	_pEventForwarder->AddEventForwarder<CEventData_ChangeDirection>();
	_pEventForwarder->AddEventForwarder<CEventData_StartMoving>();
	_pEventForwarder->AddEventForwarder<CEventData_StopMoving>();
}

