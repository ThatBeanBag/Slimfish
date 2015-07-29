// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_PlayerLeft.h
// Description	: CEventData_PlayerLeft declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_PLAYERLEFT_H__
#define __EVENTDATA_PLAYERLEFT_H__

// Library Includes

// Local Includes
#include "EventData.h"

/************************************************************************/
/* CEventData_PlayerLeft
/*
/* Sent from the server when a player has left the game.
/************************************************************************/
class CEventData_PlayerLeft : public IEventData {
	// Member Functions
public:
	CEventData_PlayerLeft(const std::string& _krstrUsername = std::string());
	~CEventData_PlayerLeft();

	const std::string GetUsername() const;

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
	std::string m_strUsername;
};

/************************************************************************/
/* CEventData_RequestPlayerLeave
/*
/* Event sent by the client to request to leave the server.
/************************************************************************/
class CEventData_RequestPlayerLeave : public CEventData_PlayerLeft {
	// Member Functions
public:
	CEventData_RequestPlayerLeave(const std::string& _krstrUsername = std::string());
	~CEventData_RequestPlayerLeave();

	virtual TEventTypeID VGetEventTypeID() const override;
protected:
private:
	
	// Member Variables
public:
	static const TEventTypeID s_kEVENT_TYPE_ID;
protected:
private:
};

#endif	// __EVENTDATA_PLAYERLEFT_H__