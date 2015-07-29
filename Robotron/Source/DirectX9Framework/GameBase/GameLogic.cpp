// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GameLogic.cpp
// Description	: CGameLogic implementation file.
// Author		: Hayden Asplet.
// Mail

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "GameLogic.h"

// Local Includes
#include "EventData_RequestNewActor.h"
#include "..\Actor\Actor.h"
#include "..\Actor\TransformComponent.h"
#include "..\Actor\PhysicsComponent.h"
#include "..\Physics\BulletSDKPhysics.h"
#include "..\Events\EventData_ClientJoin.h"
#include "..\Events\EventData_RequestDestroyActor.h"
#include "..\View\NetworkView.h"
#include "..\AI\SteeringBehaviours.h"

CGameLogic::CGameLogic()
	:m_fElapsedTime(0.0f),
	m_bIsRemote(false),
	m_eState(GS_INVALID),
	m_iNumPlayers(0),
	m_iNumLoadedPlayers(0)
{
	m_options.m_iMaxPlayers = 16;	// TODO: remove this hard code.
}

CGameLogic::~CGameLogic()
{
	for (auto iter = m_actorMap.begin(); iter != m_actorMap.end(); ++iter) {
		iter->second->DestroyComponents();
	}
}

bool CGameLogic::Initialise()
{
	ChangeState(GS_INITIALISING);	// We are now initialising.

	RegisterDelegates(); // Also registers derived delegates.

	// Create the game physics engine.
	m_pPhysics = std::move(VCreateGamePhysics());

	if (m_pPhysics) {
	// Is there physics?
		if (!m_pPhysics->VInitialize()) {
			DEBUG_ERROR("Failed to initialise physics");
			return false;
		}
	}
	else {
	// No physics.
		// It's valid to have no physics.
	}

	// Initialise derived.
	if (!VInitialise()) {
		return false;
	}

	// Done initialisation goto main menu.
	ChangeState(GS_MAINMENU);

	return true;
}

bool CGameLogic::OnRestore()
{
	bool bHasSuceeded = true;

	// Restore game views.
	for (auto iter = m_gameViews.begin(); iter != m_gameViews.end(); ++iter) {
		if (*iter) {
			if (!(*iter)->VOnRestore()) {
				bHasSuceeded = false;
			}
		}
	}

	// Restore derived logic.
	return bHasSuceeded && VOnRestore();
}

bool CGameLogic::LoadGame()
{
	// Load derived game.
	return VLoadGame();
}

void CGameLogic::Update(float _fDeltaTime)
{
	m_fElapsedTime += _fDeltaTime;

	// Update derived logic.
	VUpdate(_fDeltaTime);

	m_steeringBehaviours.Update(_fDeltaTime);

	for (auto iter = m_gameViews.begin(); iter != m_gameViews.end(); ++iter) {
		if (*iter) {
			(*iter)->VOnUpdate(_fDeltaTime);
		}
	}

	for (auto iter = m_actorMap.begin(); iter != m_actorMap.end(); ++iter) {
		iter->second->Update(_fDeltaTime);
	}

	if (m_pPhysics) {
		m_pPhysics->VUpdate(_fDeltaTime);
		m_pPhysics->VSyncScene();
	}
}

void CGameLogic::RegisterDelegates()
{
	// Register base logic delegates.
	CEventManager* pEventManager = CEventManager::GetInstance().get();
	pEventManager->AddListener<CEventData_RequestStartGame>(MakeDelegate(this, &CGameLogic::StartGameDelegate));
	pEventManager->AddListener<CEventData_JoinGame>(MakeDelegate(this, &CGameLogic::JoinGameDelegate));
	pEventManager->AddListener<CEventData_HostGame>(MakeDelegate(this, &CGameLogic::HostGameDelegate));
	pEventManager->AddListener<CEventData_ClientJoin>(MakeDelegate(this, &CGameLogic::ClientJoinDelegate));
	pEventManager->AddListener<CEventData_RequestPlayerReady>(MakeDelegate(this, &CGameLogic::RequestPlayerReadyDelegate));
	pEventManager->AddListener<CEventData_RequestPlayerLeave>(MakeDelegate(this, &CGameLogic::RequestPlayerLeaveDelegate));
	pEventManager->AddListener<CEventData_LoadedGame>(MakeDelegate(this, &CGameLogic::LoadedGameDelegate));

	// Register derived delegates.
	VRegisterDelegates();
}


void CGameLogic::RemoveDelegates()
{
	// Remove base logic delegates.
	CEventManager* pEventManager = CEventManager::GetInstance().get();
	pEventManager->RemoveListener<CEventData_RequestStartGame>(MakeDelegate(this, &CGameLogic::StartGameDelegate));
	pEventManager->RemoveListener<CEventData_JoinGame>(MakeDelegate(this, &CGameLogic::JoinGameDelegate));
	pEventManager->RemoveListener<CEventData_HostGame>(MakeDelegate(this, &CGameLogic::HostGameDelegate));
	pEventManager->RemoveListener<CEventData_ClientJoin>(MakeDelegate(this, &CGameLogic::ClientJoinDelegate));
	pEventManager->RemoveListener<CEventData_RequestPlayerReady>(MakeDelegate(this, &CGameLogic::RequestPlayerReadyDelegate));
	pEventManager->RemoveListener<CEventData_RequestPlayerLeave>(MakeDelegate(this, &CGameLogic::RequestPlayerLeaveDelegate));
	pEventManager->RemoveListener<CEventData_LoadedGame>(MakeDelegate(this, &CGameLogic::LoadedGameDelegate));
	
	if (m_bIsRemote) {
		pEventManager->RemoveListener<CEventData_RequestNewActor>(MakeDelegate(this, &CGameLogic::RequestNewActorDelegate));
		pEventManager->RemoveListener<CEventData_RequestDestroyActor>(MakeDelegate(this, &CGameLogic::RequestDestroyActorDelegate));
	}

	// Remove derived delegates.
	VRemoveDelegates();
}

void CGameLogic::ChangeState(EGameState _eNewState)
{
	// Set the internal state to the new state.
	m_eState = _eNewState;

	switch (m_eState) {
		case CGameLogic::GS_INVALID: {
			break;
		}
		case CGameLogic::GS_INITIALISING: {
			break;
		}
		case CGameLogic::GS_MAINMENU: {
			break;
		}
		case CGameLogic::GS_LOBBY: {
			break;
		}
		case CGameLogic::GS_LOADING: {
			break;
		}
		case CGameLogic::GS_RUNNING: {
			break;
		}
		default: {
			break;
		}
	}

	VOnStateChange(_eNewState);
}

void CGameLogic::AddGameView(unique_ptr<IGameView> _pView)
{
	_pView->VOnRestore();
	m_gameViews.push_back(std::move(_pView));
}

void CGameLogic::RemoveGameView(IGameView* _pView)
{
	for (auto iter = m_gameViews.begin(); iter != m_gameViews.end(); ++iter) {
		if (iter->get() == _pView) {
			m_gameViews.erase(iter);
			break;
		}
	}
}


TActorID CGameLogic::CreateActor(const std::string& _krActorResource, const CMatrix4x4* _pInitialTransform, TActorID _serverActorID)
{
	/*if (m_bIsRemote && (m_eState == GS_SPAWNING_PLAYERS || m_eState == GS_RUNNING || m_eState == GS_WAITING_FOR_PLAYERS_TO_LOAD)) {
		DEBUG_ASSERT(_serverActorID != CActor::s_kINVALID_ACTOR_ID);	// If we remote the serverActorID must be valid.
	}
	else {
		DEBUG_ASSERT(_serverActorID == CActor::s_kINVALID_ACTOR_ID);	// If we are running the logic the serverActorID must be invalid.
	}*/

	unique_ptr<CActor> pActor = m_actorFactory.CreateActor(_krActorResource, _pInitialTransform, _serverActorID);

	if (pActor) {
		TActorID actorID = pActor->GetID();

		if (!m_bIsRemote && (m_eState == GS_SPAWNING_PLAYERS || m_eState == GS_RUNNING)) {
			// Request actor creation on client/remote machines.
			shared_ptr<CEventData_RequestNewActor> pNewActor(new CEventData_RequestNewActor(_krActorResource, _pInitialTransform, pActor->GetID()));
			CEventManager::GetInstance()->TriggerEvent(pNewActor);
		}

		m_actorMap[actorID] = std::move(pActor);

		return actorID;
	}
	else {
		return CActor::s_kINVALID_ACTOR_ID;
	}

	return CActor::s_kINVALID_ACTOR_ID;
}


CActor* CGameLogic::GetActor(TActorID _actorID)
{
	auto findIter = m_actorMap.find(_actorID);

	if (findIter != m_actorMap.end()) {
	// Did we find an actor with the specified ID?
		return findIter->second.get();
	}
	
	return nullptr;
}


void CGameLogic::DestroyActor(TActorID _actorID)
{
	auto findIter = m_actorMap.find(_actorID);

	if (findIter != m_actorMap.end()) {
	// Did we find an actor with the specified ID?
		// First let other sub-systems know about the destroying of the actor.
		shared_ptr<CEventData_DestroyActor> pEvent(new CEventData_DestroyActor(_actorID));
		CEventManager::GetInstance()->TriggerEvent(pEvent);

		if (!m_bIsRemote) {
			shared_ptr<CEventData_RequestDestroyActor> pEvent(new CEventData_RequestDestroyActor(_actorID));
			CEventManager::GetInstance()->QueueEvent(pEvent);
		}

		// Destroy the actors components, this must be done before destroying the actor.
		findIter->second->DestroyComponents();

		// Remove the actor.
		m_actorMap.erase(findIter);
	}
	else {
	// No actor found.
		DEBUG_WARNING("Failed to find actor with id: " + ToString(_actorID) + " to destroy");
	}
}


void CGameLogic::SetAsRemote()
{
	m_bIsRemote = true;

	CEventManager::GetInstance()->AddListener<CEventData_RequestNewActor>(MakeDelegate(this, &CGameLogic::RequestNewActorDelegate));
	CEventManager::GetInstance()->AddListener<CEventData_RequestDestroyActor>(MakeDelegate(this, &CGameLogic::RequestDestroyActorDelegate));

	// A remote logic does not need to run physics.
	m_pPhysics.reset(nullptr);
}

bool CGameLogic::IsRemote() const
{
	return m_bIsRemote;
}

bool CGameLogic::OnMsgProc(const TAppMsg& _krMsg)
{
	for (auto iter = m_gameViews.begin(); iter != m_gameViews.end(); ++iter) {
		if (*iter) {
			if ((*iter)->VOnMsgProc(_krMsg)) {
			// Did this view handle the message?
				return true;	// Breaks from the loop, so no more game views can handle the input.
			}
		}
	}

	// Failed to be handled.
	return false;
}

void CGameLogic::Render()
{
	// Render the game views.
	for (auto iter = m_gameViews.begin(); iter != m_gameViews.end(); ++iter) {
		if (*iter) {
			(*iter)->VRender();
		}
	}

	if (m_pPhysics) {
		g_pApp->GetRenderer()->VSetWorldTransform(&CMatrix4x4::s_kIdentity);
		//m_pPhysics->VDrawDebugDiagnostics();
	}
}

unique_ptr<IPhysics> CGameLogic::VCreateGamePhysics()
{
	unique_ptr<CBulletSDKPhysics> pPhysics(new CBulletSDKPhysics());
	return std::move(pPhysics);
}

void CGameLogic::StartGameDelegate(shared_ptr<IEventData> _pEventData)
{
	shared_ptr<CEventData_RequestStartGame> pStartGameData = static_pointer_cast<CEventData_RequestStartGame>(_pEventData);

	if (m_eState == GS_LOBBY || m_eState == GS_MAINMENU) {
		ChangeState(GS_LOADING);
	}
	else if (m_eState == GS_WAITING_FOR_PLAYERS_TO_LOAD) {
		ChangeState(GS_SPAWNING_PLAYERS);
	}	
}

void CGameLogic::JoinGameDelegate(shared_ptr<IEventData> _pEventData)
{
	shared_ptr<CEventData_JoinGame> pJoinGameData = static_pointer_cast<CEventData_JoinGame>(_pEventData);

	SetAsRemote();

	m_options.m_iMaxPlayers = pJoinGameData->GetMaxPlayers();
	m_options.m_strUsername = pJoinGameData->GetUsername();

	TAddress address = pJoinGameData->GetAddress();
	if (address.m_strIPAddress != "" && address.m_usPort != 0) {
		// Create a new event forwarder to forward events to this client.
		unique_ptr<CNetworkEventForwarder> pEventForwarder(new CNetworkEventForwarder(address));
		if (!pEventForwarder) {
			std::string strError = "Failed to create network event forwarder for address: ";
			strError += address.m_strIPAddress + ":" + ToString(address.m_usPort);
			DEBUG_ERROR(strError);
			return;
		}

		// Add the events to forward to the server.
		pEventForwarder->AddEventForwarder<CEventData_RequestPlayerReady>();
		pEventForwarder->AddEventForwarder<CEventData_LoadedGame>();
		pEventForwarder->AddEventForwarder<CEventData_RequestPlayerLeave>();
		VAddNetworkEventsClientToServer(pEventForwarder.get());

		// Add the server as a view to the game.
		unique_ptr<CNetworkView> pNetworkView(new CNetworkView(std::move(pEventForwarder), "Server"));
		if (pNetworkView) {
			AddGameView(std::move(pNetworkView));
		}
	}
	else {
		DEBUG_ERROR("Join game event is invalid");
	}

	ChangeState(GS_LOBBY);
}

void CGameLogic::HostGameDelegate(shared_ptr<IEventData> _pEventData)
{
	shared_ptr<CEventData_HostGame> pHostGameData = static_pointer_cast<CEventData_HostGame>(_pEventData);

	CServerNetwork* pNetwork = g_pApp->CreateServerNetwork(pHostGameData->GetServerName());
	m_options.m_strUsername = pHostGameData->GetUsername();

	// We've joined the game.
	m_iNumPlayers = 1;

	if (pNetwork) {
		ChangeState(GS_LOBBY);
	}
	else {
		// TODO: Display a nice error message.
		ChangeState(GS_MAINMENU);
	}
}

void CGameLogic::RequestNewActorDelegate(shared_ptr<IEventData> _pEventData)
{
	DEBUG_ASSERT(m_bIsRemote);	// Only remote logic's should be listening to this event.

	shared_ptr<CEventData_RequestNewActor> pRequestNewActorData = static_pointer_cast<CEventData_RequestNewActor>(_pEventData);

	CMatrix4x4 initialTransform = pRequestNewActorData->GetInitialTransform();
	TActorID serverActorID = pRequestNewActorData->GetServerActorID();
	std::string strActorResource = pRequestNewActorData->GetActorResource();
	TActorID actorID = CActor::s_kINVALID_ACTOR_ID;

	if (pRequestNewActorData->HasInitialTransform()) {
		actorID = CreateActor(strActorResource, &initialTransform, serverActorID);
	}
	else {
		actorID = CreateActor(strActorResource, nullptr, serverActorID);
	}

	if (actorID != serverActorID) {
		DEBUG_WARNING("Failed to create actor request by server from resource \"" + strActorResource + "\"");
	}
}

void CGameLogic::RequestDestroyActorDelegate(shared_ptr<IEventData> _pEventData)
{
	DEBUG_ASSERT(m_bIsRemote);	// Only remote logic's should be listening to this event.

	shared_ptr<CEventData_RequestDestroyActor> pRequestNewActorData = static_pointer_cast<CEventData_RequestDestroyActor>(_pEventData);

	TActorID actorID = pRequestNewActorData->GetActorID();
	DestroyActor(actorID);
}

void CGameLogic::RequestPlayerReadyDelegate(shared_ptr<IEventData> _pEventData)
{
	// This event is to be received by the server to fire off a player ready event.
	// This avoids entering an infinite loop.
	if (m_bIsRemote) {
		return;
	}

	shared_ptr<CEventData_RequestPlayerReady> pPlayerReadyData = static_pointer_cast<CEventData_RequestPlayerReady>(_pEventData);

	shared_ptr<CEventData_PlayerReady> pEvent(new CEventData_PlayerReady(pPlayerReadyData->GetUsername(), pPlayerReadyData->IsReady()));
	CEventManager::GetInstance()->QueueEvent(pEvent);
}

void CGameLogic::RequestPlayerLeaveDelegate(shared_ptr<IEventData> _pEventData)
{
	// This event is to be received by the server to fire off a player left event.
	// This avoids entering an infinite loop.
	if (m_bIsRemote) {
		return;
	}

	shared_ptr<CEventData_RequestPlayerLeave> pRequestLeaveData = static_pointer_cast<CEventData_RequestPlayerLeave>(_pEventData);

	shared_ptr<CEventData_PlayerLeft> pEvent(new CEventData_PlayerLeft(pRequestLeaveData->GetUsername()));
	CEventManager::GetInstance()->QueueEvent(pEvent);
}

void CGameLogic::LoadedGameDelegate(shared_ptr<IEventData> _pEventData)
{
	if (!m_bIsRemote) {
		m_iNumLoadedPlayers++;

		if (m_iNumLoadedPlayers >= m_iNumPlayers) {
		// Have all players loaded?
			// Start the game.
			shared_ptr<CEventData_RequestStartGame> pEvent(new CEventData_RequestStartGame(false));
			CEventManager::GetInstance()->TriggerEvent(pEvent);
		}
	}
}

CSteeringBehaviours& CGameLogic::GetSteeringBehaviours()
{
	return m_steeringBehaviours;
}

void CGameLogic::ClearActors()
{
	while (!m_actorMap.empty()) {
		auto iter = m_actorMap.begin();
		DestroyActor(iter->first);
	}
}

void CGameLogic::ClientJoinDelegate(shared_ptr<IEventData> _pEventData)
{
	if (m_bIsRemote) {	// Only servers actually add clients.
		return;
	}

	shared_ptr<CEventData_ClientJoin> pClientJoinData = static_pointer_cast<CEventData_ClientJoin>(_pEventData);

	// Get the remote address from the client join data.
	TAddress remoteAddress = pClientJoinData->GetRemoteAddress();

	// Create a new event forwarder to forward events to this client.
	unique_ptr<CNetworkEventForwarder> pEventForwarder(new CNetworkEventForwarder(remoteAddress));
	if (!pEventForwarder) {
		std::string strError = "Failed to create network event forwarder for address: ";
		strError += remoteAddress.m_strIPAddress + ":" + ToString(remoteAddress.m_usPort);
		strError += " username: \"" + pClientJoinData->GetUsername() + "\"";
		DEBUG_ERROR(strError);
		return;
	}
	
	// Add the events to forward to the client.
	pEventForwarder->AddEventForwarder<CEventData_MoveActor>();
	pEventForwarder->AddEventForwarder<CEventData_RequestNewActor>();
	pEventForwarder->AddEventForwarder<CEventData_RequestDestroyActor>();
	pEventForwarder->AddEventForwarder<CEventData_RequestStartGame>();
	pEventForwarder->AddEventForwarder<CEventData_PlayerReady>();
	pEventForwarder->AddEventForwarder<CEventData_PlayerLeft>();
	VAddNetworkEventsServerToClient(pEventForwarder.get());

	unique_ptr<CNetworkView> pNetworkView(new CNetworkView(std::move(pEventForwarder), pClientJoinData->GetUsername()));
	if (pNetworkView) {
		AddGameView(std::move(pNetworkView));
		m_iNumPlayers++;
	}
}

IPhysics* CGameLogic::GetGamePhysics()
{
	return m_pPhysics.get();
}

CActorFactory* CGameLogic::GetActorFactory()
{
	return &m_actorFactory;
}

const CGameLogic::TActorMap& CGameLogic::GetActorMap()
{
	return m_actorMap;
}

int CGameLogic::GetNumPlayers() const
{
	return m_iNumPlayers;
}

int CGameLogic::GetMaxPlayers() const
{
	return m_options.m_iMaxPlayers;
}

std::string CGameLogic::GetUsername() const
{
	return m_options.m_strUsername;
}

CGameLogic::EGameState CGameLogic::GetGameState() const
{
	return m_eState;
}

