// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: NetworkEventForwarder.h
// Description	: CNetworkEventForwarder declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __NETWORKEVENTFORWARDER_H__
#define __NETWORKEVENTFORWARDER_H__

// Library Includes

// Local Includes
#include "BaseNetwork.h"

struct TAddress;

class CNetworkEventForwarder {
	// Member Functions
public:
	CNetworkEventForwarder(TAddress _address);
	~CNetworkEventForwarder();

	/**
	* Adds a listener to events of the template parameter type that will 
	* forward the event to that address of the CNetworkEventForwarder.
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	template<typename TEventType>
	void AddEventForwarder();

	/**
	* Gets the address of the network event forwarder.
	* 
	* @author: 	Hayden Asplet
	* @return:  const TAddress
	*/
	const TAddress GetAddress() const { return m_address;  }

	/**
	* Event delegate for all events that are forwarded through the network.
	* 
	* @author: 	Hayden Asplet
	* @param:	shared_ptr<IEventData> _pEventData - event data to be sent through the network.
	* @return:  void
	*/
	void ForwardEventDelegate(shared_ptr<IEventData> _pEventData);
protected:
private:
	// Member Variables
public:
protected:
private:
	TAddress m_address;	// The remote address of where events are to be forwarded to.
	std::vector<TEventTypeID> m_vecEventTypeIDs; // The type ids of the events the forwarder is listening to, this is use for automatic removal of listeners.
};

template<typename TEventType>
void CNetworkEventForwarder::AddEventForwarder()
{
	CEventManager::GetInstance()->AddListener<TEventType>(MakeDelegate(this, &CNetworkEventForwarder::ForwardEventDelegate));
	m_vecEventTypeIDs.push_back(TEventType::s_kEVENT_TYPE_ID);
}

#endif	// __NETWORKEVENTFORWARDER_H__