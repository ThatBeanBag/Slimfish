// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_ClientReady.h
// Description	: CEventData_ClientReady declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_CLIENTREADY_H__
#define __EVENTDATA_CLIENTREADY_H__

// Library Includes

// Local Includes
#include "EventData.h"

/************************************************************************/
/* CEventData_PlayerReady
/*
/* Sent when a player's ready state changes.
/* Doubles as a message to send to clients when a new player has joined.
/*
/* This is received by both clients and the server and can be sent by
/* either as well.
/*
/* Servers that receive this message should send it out to other clients.
/************************************************************************/
class CEventData_PlayerReady : public IEventData {
	// Member Functions
public:
	CEventData_PlayerReady(const std::string& _krstrUsername = std::string(), bool _bIsReady = true);
	~CEventData_PlayerReady();

	bool IsReady() const;
	std::string GetUsername() const;

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
	bool m_bIsReady;
	std::string m_strUsername;
};

/************************************************************************/
/* CEventData_RequestPlayerReady
/*
/* This is the same as CEventData_PlayerReady, but with a different label
/* to distinguish between events that the server sends and events the
/* the clients send.
/*
/* This event is send by clients to the tell the server to fire a 
/* CEventData_PlayerReady event.
/************************************************************************/
class CEventData_RequestPlayerReady : public CEventData_PlayerReady {
	// Member Functions
public:
	CEventData_RequestPlayerReady(const std::string& _krstrUsername = std::string(), bool _bIsReady = true);
	~CEventData_RequestPlayerReady();

	virtual TEventTypeID VGetEventTypeID() const override;
protected:
private:
	
	// Member Variables
public:
	static const TEventTypeID s_kEVENT_TYPE_ID;
protected:
private:
};

#endif	// __EVENTDATA_CLIENTREADY_H__