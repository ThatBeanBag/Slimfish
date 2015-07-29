// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2014 Media Design School
//
// File Name	: EventManager.h
// Description	: CEventManager declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

// Library Includes
#include <functional>
#include <typeindex>

// Local Includes
#include "../3rdParty/fastdelegate/FastDelegate.h"
#include "../Utilities/ObjectFactory.h"
#include "EventData.h"
#include "EventData_NewRenderComponent.h"
#include "EventData_MoveActor.h"
#include "EventData_DestroyActor.h"
#include "EventData_Collision.h"
#include "EventData_CollisionSeparation.h"
#include "EventData_StartMoving.h"
#include "EventData_StopMoving.h"
#include "EventData_RequestNewActor.h"
#include "EventData_RequestStartGame.h"
#include "EventData_LostConnection.h"
#include "EventData_PlayerReady.h"
#include "EventData_PlayerLeft.h"
#include "EventData_ClientJoin.h"
#include "EventData_JoinGame.h"
#include "EventData_HostGame.h"
#include "EventData_LoadedGame.h"

using fastdelegate::MakeDelegate;

typedef shared_ptr<IEventData> TEventDataPtr;

//=========================================================================
// CEventManager is a singleton class that handles the events of the game.
//
// The singleton instance is managed as a unique_ptr which is an auto 
// pointer that deallocates itself whenever it goes out of scope. This
// ensures that the event manager does not leak and the instance is always
// de-allocated upon program termination.
//
// Adding listeners allows the registered function/delegated to be called
// whenever an event of the event type is fired off.
//
// Events do not fire off immediately unless TriggerEvent() is used.
// Instead they put in a queue to be fired off on the next update tick.
//
// Immediately triggering events using TriggerEvent() is not recommended
// unless it's absolutely necessary. 
//=========================================================================
class CEventManager {
	// Member Functions
	typedef fastdelegate::FastDelegate1<TEventDataPtr> TEventListener;
	typedef std::map<TEventTypeID, std::list<TEventListener> > TEventListenerMap;
public:
	/**
	* Destructor - does nothing as the singleton instance is stored as a unique_ptr which handles its own
	* destruction.
	* 
	* @author: 	Hayden Asplet
	* @return:  
	*/
	~CEventManager();

	/**
	* Gets the singleton instance of the event manager.
	* 
	* @author: 	Hayden Asplet
	* @return:  std::unique_ptr<CEventManager>& - pointer to the instance.
	*/
	static std::unique_ptr<CEventManager>& GetInstance();

	/**
	* Adds a listeners that is called whenever an event of the listener's event type is fired off.
	*
	* Implicitly registers the event type so that CreateEvent() can be used to create the event from
	* an ID.
	* 
	* @author: 	Hayden Asplet
	* @param:	const TEventListener & _krEventListener - listener delegate to add.
	* @return:  void
	*/
	template<typename EventType>
	void AddListener(const TEventListener& _krEventListener);

	/**
	* Removes a listener from the list of listeners
	* 
	* @author: 	Hayden Asplet
	* @param:	const TEventListener & _krEventListener - listener
	* @return:  void
	*/
	template<typename EventType>
	void RemoveListener(const TEventListener& _krEventListener);

	void RemoveListener(const TEventListener& _krEventListener, TEventTypeID _eventTypeID);

	/**
	* Immediately fires off an event.
	*
	* Perfer QueueEvent() over this.
	* 
	* @author: 	Hayden Asplet
	* @param:	const TEventDataPtr & _pEvent - event to trigger
	* @return:  void
	*/
	void TriggerEvent(TEventDataPtr _pEvent);

	/**
	* Places an event into the queue to be fired off next tick.
	*
	* This is the default and preferable way of firing off events
	* 
	* @author: 	Hayden Asplet
	* @param:	const TEventDataPtr & _pEvent - event to queue
	* @return:  void
	*/
	void QueueEvent(TEventDataPtr _pEvent);
	
	/**
	* Fires off all events in the current queue, this should happen every tick.
	* 
	* @author: 	Hayden Asplet
	* @param:	unsigned long _uiMaxMilliseconds - maximum time to process events for.
	* @return:  void
	*/
	void FireOffEvents(unsigned long _uiMaxMilliseconds);

	/**
	* Creates an event from an event type id that has been registered in the event factory.
	*
	* The registration of an event type happens implicitly when a listener of that event type is first added.
	* You should only ever need to create an event of a type that is being listened to.
	*
	* @author: 	Hayden Asplet
	* @param:	TEventTypeID _eventTypeID - type id of the type of event to create.
	* @return:  shared_ptr<IEventData> - created event. NULL if no registered event for _eventTypeID.
	*/
	shared_ptr<IEventData> CreateEvent(TEventTypeID _eventTypeID);
protected:
private:
	/**
	* Constructor
	*
	* Made private to disable instancing of CEventManagers.
	* 
	* @author: 	Hayden Asplet 
	*/
	CEventManager();

	// Disable copying.
	CEventManager(const CEventManager& _krEventManager) = delete;
	CEventManager& operator=(const CEventManager& _krEventManager) = delete;

	// Member Variables
public:
protected:
private:
	static std::unique_ptr<CEventManager> s_pInstance;	// The singleton instance, stored as a unique ptr
														// to ensure its de-allocation.
	TEventListenerMap m_eventListeners;	// The registered delegates, to be called if an event of the
										// corresponding type is fired off.
	std::list<TEventDataPtr> m_eventQueue;	// The queue of events that are to be fired off next 
											// update tick.

	CObjectFactory<IEventData, TEventTypeID> m_eventFactory; // Event creator factory, for creating events when you don't know their type, but know their type id.
};

template<typename EventType>
void CEventManager::AddListener(const TEventListener& _krEventListener)
{
	// Register the event creator here. There only ever needs to be a 
	// creator for an event if we have a listener for it.
	// Note: type_index cannot be used to identify event types here as events will need to be identified remotely.
	// typeid() does not guarantee the type id of our event will be the same as a remote machine's.
	m_eventFactory.Register<EventType>(EventType::s_kEVENT_TYPE_ID);

	// Find the list of listeners at the type index.
	std::list<TEventListener>& listenerList = m_eventListeners[EventType::s_kEVENT_TYPE_ID];

	std::list<TEventListener>::iterator listIter;

	for(listIter = listenerList.begin(); listIter != listenerList.end(); ++listIter) {
		if(_krEventListener == (*listIter)) {
		// This listener is already registered.
			DEBUG_WARNING("Attempting to register a listener twice.");

			// Don't register the listener twice.
			return;
		}
	}

	// Register listener.
	listenerList.push_back(_krEventListener);
}

template<typename EventType>
void CEventManager::RemoveListener(const TEventListener& _krEventListener)
{
	RemoveListener(_krEventListener, EventType::s_kEVENT_TYPE_ID);
}

#endif