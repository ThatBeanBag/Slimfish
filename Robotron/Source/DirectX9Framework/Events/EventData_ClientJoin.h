// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_ClientJoin.h
// Description	: CEventData_ClientJoin declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_CLIENTJOIN_H__
#define __EVENTDATA_CLIENTJOIN_H__

// Library Includes

// Local Includes
#include "..\Network\BaseNetwork.h"

/************************************************************************/
/* CEventData_ClientJoin
/*
/* Sent only on the server-side when a client has been connected 
/* successfully.
/************************************************************************/
class CEventData_ClientJoin : public IEventData {
	// Member Functions
public:
	CEventData_ClientJoin();
	CEventData_ClientJoin(const TAddress& _krAddress, const std::string& krstrUsername);
	~CEventData_ClientJoin();

	/**
	* Returns the address of the client that's joined.
	* 
	* @author: 	Hayden Asplet
	* @return:  TAddress - address of the client.
	*/
	TAddress GetRemoteAddress() const;

	/**
	* Gets the port number of the client.
	* 
	* @author: 	Hayden Asplet
	* @return:  unsigned short - port number of client.
	*/
	unsigned short GetPort() const;

	/**
	* Gets the username of the client that's joining.
	* 
	* @author: 	Hayden Asplet
	* @return:  const std::string - username of the player.
	*/
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
	TAddress m_address;
	std::string m_strUsername;
};

#endif	// __EVENTDATA_CLIENTJOIN_H__