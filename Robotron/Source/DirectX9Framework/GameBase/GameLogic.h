// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GameLogic.h
// Description	: CGameLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __GAMELOGIC_H__
#define __GAMELOGIC_H__

// Library Includes

// Local Includes
#include "../GameStd.h"
#include "../View/GameView.h"
#include "../Actor/ActorFactory.h"
#include "../Network/NetworkEventForwarder.h"
#include "../AI/SteeringBehaviours.h"

class IPhysics;

class CGameLogic {
	struct TGameOptions {
		// Multi-player Options
		std::string m_strUsername;
		std::string m_strHostname;
		short m_uiPort;
		int m_iMaxPlayers;
	};

public:
	enum EGameState {
		GS_INVALID,
		GS_INITIALISING,
		GS_MAINMENU,
		GS_LOBBY,
		GS_LOADING,
		GS_WAITING_FOR_PLAYERS_TO_LOAD,
		GS_SPAWNING_PLAYERS,
		GS_RUNNING
	};

	typedef std::map<TActorID, unique_ptr<CActor>> TActorMap;

	// Member Functions
public:
	CGameLogic();
	virtual ~CGameLogic();

	/**
	* Initialises the game logic.
	*
	* Creates the physics engine if there is one to be created.
	* 
	* @author: 	Hayden Asplet
	* @return:  bool - true if initialisation succeeded.
	*/
	bool Initialise();

	/**
	* Restores the game logic.
	* 
	* @author: 	Hayden Asplet
	* @return:  bool - true if restoring succeeded.
	*/
	bool OnRestore();

	/**
	* Loads the initial game resources and settings. 
	* 
	* @author: 	Hayden Asplet
	* @return:  bool - true if load succeeded.
	*/
	bool LoadGame();

	/**
	* Updates the logic.
	* 
	* @author: 	Hayden Asplet
	* @param:	float _fDeltaTime - time since last tick.
	* @return:  void
	*/
	void Update(float _fDeltaTime);

	/**
	* Registers event delegates.
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	void RegisterDelegates();

	/**
	* Unregisters event delegates.
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	void RemoveDelegates();

	/**
	* Changes the state of the game, also calls VOnStateChange() for derived implementation.
	* 
	* @author: 	Hayden Asplet
	* @param:	EGameState _eState - state to change to.
	* @return:  void
	*/
	void ChangeState(EGameState _eNewState);

	/**
	* Calls VOnMsgProc() for all game views currently attached.
	*
	* No derived classes should override this.
	* 
	* @author: 	Hayden Asplet
	* @param:	const TAppMsg & _krMsg - application message
	* @return:  bool - true if the message was handled by a view.
	*/
	bool OnMsgProc(const TAppMsg& _krMsg);

	/**
	* Renders all game views currently attached to the game.
	*
	* No derived classes should override this.
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	void Render();

	/**
	* Adds a view to the game.
	* 
	* @author: 	Hayden Asplet
	* @param:	unique_ptr<IGameView> _pView - game view to add.
	* @return:  void
	*/
	void AddGameView(unique_ptr<IGameView> _pView);

	/**
	* Removes a game view from the game.
	* 
	* @author: 	Hayden Asplet
	* @param:	IGameView * _pView - game view to remove.
	* @return:  void
	*/
	void RemoveGameView(IGameView* _pView);

	/**
	* Creates an actor from an actor resource file (XML).
	* 
	* @author: 	Hayden Asplet
	* @param:	const std::string & _krObjectResource - actor resource file; should be an XML file.
	* @param:	const CMatrix4x4 * _pInitialTransform - an initial transform to override that in the XML file.
	* If NULL no overriding will take place. Also preserves the scale of the XML transform, making the initial
	* transforms scale relative to that of the XML's transform.
	* @param:	TActorID _serverActorID - the actor ID from the server. This is used to sync the IDs of clients
	* with the server actor IDs. If s_kiINVALID_ACTOR_ID the resultant ID will be nondeterministic.
	* @return:  TActorID - ID of created actor. If creation failed ID will be CActor::s_kiINVALID_ACTOR_ID.
	*/
	TActorID CreateActor(const std::string& _krObjectResource, const CMatrix4x4* _pInitialTransform = nullptr, TActorID _serverActorID = CActor::s_kINVALID_ACTOR_ID);

	/**
	* Returns a pointer to an actor assigned to the input actorID.
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to retrieve.
	* @return:  CActor* - pointer to the actor assigned to the input ID. Will be NULL if the actorID isn't found.
	*/
	CActor* GetActor(TActorID _actorID);

	/**
	* Destroys the actor assigned to the input actorID.
	*
	* Will trigger a destroy actor event prior to destruction to let other subsystems know about the destruction
	* of the actor. 
	* 
	* @author: 	Hayden Asplet
	* @param:	TActorID _actorID - ID of the actor to destroy.
	* @return:  void
	*/
	void DestroyActor(TActorID _actorID);

	/**
	* Sets the game logic as a remote logic, making it effectively a dump terminal.
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	void SetAsRemote();

	/**
	* Returns true if the game logic is remote, not a real one.
	* 
	* @author: 	Hayden Asplet
	* @return:  bool - true if the game logic is remote, not a real one.
	*/
	bool IsRemote() const;

	/**
	* Retrieves the physics engine of the game. 
	* 
	* If this returns nullptr the game has no physics
	*
	* @author: 	Hayden Asplet
	* @return:  IPhysics* - Pointer to the physics managed by CGameLogic. This may be NULL.
	*/
	IPhysics* GetGamePhysics();

	/**
	* Gets the actor factory only actor components should be doing this.
	*
	* @author: Hayden Asplet
	* @return: CActorFactory* - Pointer the actor factory, managed by CGameLogic.
	*/
	CActorFactory* GetActorFactory();

	const std::map<TActorID, unique_ptr<CActor>>& GetActorMap();

	int GetNumPlayers() const;
	int GetMaxPlayers() const;
	std::string GetUsername() const;
	EGameState GetGameState() const;
	CSteeringBehaviours& GetSteeringBehaviours();
protected:
	void ClearActors();

private:
	/**
	* Adds the server-side network forwarding event listeners to an input network event forwarder.
	* 
	* @author: 	Hayden Asplet
	* @param:	CNetworkEventForwarder * _pEvents - event forwarder.
	* @return:  void
	*/
	virtual void VAddNetworkEventsServerToClient(CNetworkEventForwarder* _pEvents) {}

	/**
	* Adds the client-side network forwarding event listeners to an input network event forwarder.
	* 
	* @author: 	Hayden Asplet
	* @param:	CNetworkEventForwarder * _pEvents
	* @return:  void
	*/
	virtual void VAddNetworkEventsClientToServer(CNetworkEventForwarder* _pEvents) {}

	// These delegating virtual functions are to be implemented by the derived game logic.
	// They are not required to be implemented, but without implementation the game logic
	// will have no functionality.
	virtual bool VInitialise() { return true; }
	virtual bool VOnRestore() { return true; }
	virtual bool VLoadGame() { return true; }
	virtual void VUpdate(float _fDeltaTime) {}
	virtual void VOnStateChange(EGameState _eNewState) {};
	virtual void VRegisterDelegates() {}
	virtual void VRemoveDelegates() {}

	/**
	* Delegating virtual function to create the physics engine.
	*
	* This can return nullptr, in which case the game will just have no physics.
	* This is entirely valid.
	*
	* Attempting to create a physics component for actor when there is no game physics
	* will result in a error.
	* 
	* @author: 	Hayden Asplet
	* @return:  unique_ptr<IPhysics> - physics engine of the game.
	*/
	virtual unique_ptr<IPhysics> VCreateGamePhysics();

	// Event Delegates
	void StartGameDelegate(shared_ptr<IEventData> _pEventData);
	void JoinGameDelegate(shared_ptr<IEventData> _pEventData);
	void HostGameDelegate(shared_ptr<IEventData> _pEventData);
	void ClientJoinDelegate(shared_ptr<IEventData> _pEventData);
	void RequestNewActorDelegate(shared_ptr<IEventData> _pEventData);
	void RequestDestroyActorDelegate(shared_ptr<IEventData> _pEventData);
	void RequestPlayerReadyDelegate(shared_ptr<IEventData> _pEventData);
	void RequestPlayerLeaveDelegate(shared_ptr<IEventData> _pEventData);
	void LoadedGameDelegate(shared_ptr<IEventData> _pEventData);
	// Member Variables
public:
protected:
	std::vector<unique_ptr<IGameView>> m_gameViews; // The list of views attached to the game.
	std::map<TActorID, unique_ptr<CActor>> m_actorMap;
private:
	float m_fElapsedTime;	// Time since the game has been in session.
	int m_iNumPlayers;
	int m_iNumLoadedPlayers;
	TGameOptions m_options;

	CActorFactory m_actorFactory;	

	unique_ptr<IPhysics> m_pPhysics;

	EGameState m_eState;
	bool m_bIsRemote;	// True if we are a remote logic not the real one, false if we are a server running the logic.
	CSteeringBehaviours m_steeringBehaviours;
};

#endif // __GAMELOGIC_H__
