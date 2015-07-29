// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2014 Media Design School
//
// File Name	: EventManager.cpp
// Description	: CEventManager implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventManager.h"

// Local Includes

std::unique_ptr<CEventManager> CEventManager::s_pInstance = nullptr;

CEventManager::~CEventManager()
{
	// No de-allocation required; unique_ptr will de-allocate itself automatically.
}

std::unique_ptr<CEventManager>& CEventManager::GetInstance()
{
	if(!s_pInstance)  {
		// Create the instance.
		s_pInstance = std::unique_ptr<CEventManager>(new CEventManager());
	}

	return s_pInstance;
}

void CEventManager::QueueEvent(TEventDataPtr _pEvent)
{
	if (!_pEvent) {
		DEBUG_ERROR("Failed to queue event.");
		return;
	}

	// Find the list of listeners at the type index.
	TEventListenerMap::iterator findIter = m_eventListeners.find(_pEvent->VGetEventTypeID());

	if (findIter != m_eventListeners.end()) {
		// Are there any listeners listening to this event type?
		// Push event into queue.
		m_eventQueue.push_back(_pEvent);
	}
}

void CEventManager::RemoveListener(const TEventListener& _krEventListener, TEventTypeID _eventTypeID)
{
	// Find the list of listeners at the type index.
	TEventListenerMap::iterator findIter = m_eventListeners.find(_eventTypeID);

	if (findIter != m_eventListeners.end()) {
		std::list<TEventListener>::iterator listIter;

		for (listIter = findIter->second.begin(); listIter != findIter->second.end(); ++listIter) {
			if (_krEventListener == (*listIter)) {
				// Listener found.
				findIter->second.erase(listIter);
				return;
			}
		}
	}
}

void CEventManager::TriggerEvent(TEventDataPtr _pEvent)
{
	if (!_pEvent) {
		DEBUG_ERROR("Failed to trigger event");
		return;
	}

	// Find the list of listeners at the type index.
	TEventListenerMap::iterator findIter = m_eventListeners.find(_pEvent->VGetEventTypeID());

	if (findIter != m_eventListeners.end()) {
		// Found the listeners of the _pEvent type.
		std::list<TEventListener>::iterator iter;

		// Iterate through all listeners of the type of the _pEvent.
		for (iter = findIter->second.begin(); iter != findIter->second.end(); ++iter) {
			TEventListener listener = *iter;

			// Call the listener.
			listener(_pEvent);
		}
	}
}

void CEventManager::FireOffEvents(unsigned long _uiMaxMilliseconds)
{
	unsigned long uiCurrentMilliseconds = GetTickCount();
	unsigned long uiMaxTicks = uiCurrentMilliseconds + _uiMaxMilliseconds;

	while(!m_eventQueue.empty()) {
	// Are we done firing off all events in the queue?
		TEventDataPtr pEvent = m_eventQueue.front();

		// Pop the event from the queue.
		m_eventQueue.pop_front();

		// Find the list of listeners at the type index.
		TEventListenerMap::iterator findIter = m_eventListeners.find(pEvent->VGetEventTypeID());

		if(findIter != m_eventListeners.end()) {
		// Found the listeners of the _pEvent type.
			std::list<TEventListener>::iterator iter;
		
			// Iterate through all listeners of the type of the _pEvent.
			for(iter = findIter->second.begin(); iter != findIter->second.end(); ++iter) {
				TEventListener listener = *iter;

				// Call the listener.
				listener(pEvent);
			}
		}

		uiCurrentMilliseconds = GetTickCount();
		if (uiCurrentMilliseconds >= uiMaxTicks) {
			DEBUG_LOG("EVENTS", "Time ran out when proccessing events. Max milliseconds: " + ToString(_uiMaxMilliseconds));
			//break;
		}
	}
}

shared_ptr<IEventData> CEventManager::CreateEvent(TEventTypeID _eventTypeID)
{
	return shared_ptr<IEventData>(m_eventFactory.Create(_eventTypeID));
}

CEventManager::CEventManager()
{

}
