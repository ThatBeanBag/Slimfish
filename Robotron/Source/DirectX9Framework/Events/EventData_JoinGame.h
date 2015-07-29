// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_JoinGame.h
// Description	: CEventData_JoinGame declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_JOINGAME_H__
#define __EVENTDATA_JOINGAME_H__

// Library Includes

// Local Includes
#include "..\Network\BaseNetwork.h"

/************************************************************************/
/* CEventData_JoinGame
/*
/* This is sent when the user has elected to join a game and has been 
/* added the server appropriately. 
/*
/* Firing off this event will put the game into the GS_WAITING or lobby
/* state.
/*
/* This event is entirely local and should not be be sent as a request to
/* join a server. Use CClientNetwork::JoinServer() to perform that step.
/************************************************************************/
class CEventData_JoinGame : public IEventData {
	// Member Functions
public:
	CEventData_JoinGame();
	CEventData_JoinGame(const TAddress& _krServerAddress, const std::string& _krstrUsername, int _iMaxPlayers);
	~CEventData_JoinGame();

	const TAddress GetAddress() const;
	const std::string GetUsername() const;
	const int GetMaxPlayers() const;

	virtual void VSerialise(std::ostringstream& outStream) override;
	virtual void VDeserialise(std::istringstream& inStream) override;
	virtual TEventTypeID VGetEventTypeID() const override;	
protected:
private:
	// Member Variables
public:
	static const TEventTypeID s_kEVENT_TYPE_ID;
protected:
private:
	TAddress m_address;
	std::string m_strUsername;
	int m_iMaxPlayers;
};

#endif	// __EVENTDATA_JOINGAME_H__