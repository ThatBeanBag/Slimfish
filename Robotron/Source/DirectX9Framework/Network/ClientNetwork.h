// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ClientNetwork.h
// Description	: CClientNetwork declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __CLIENTNETWORK_H__
#define __CLIENTNETWORK_H__

// Library Includes

// Local Includes
#include "BaseNetwork.h"

struct TServerDetails {
	TAddress m_serverAddress;
	std::string m_strServerName;
	int m_iNumPlayers;
	int m_iMaxPlayers;
	bool m_bIsWaiting;
};

typedef std::vector<TServerDetails> TServerList;

class CClientNetwork : public ABaseNetwork {
	// Member Functions
public:
	CClientNetwork();
	~CClientNetwork();

	void BroadcastToDetectServers(TServerList& _rServerList);
	EJoinRequestResponse JoinServer(const TAddress& _krServerAddress, const std::string& _krstrUsername);

	virtual void VHandleInput() override;
protected:
private:
	// Member Variables
public:
protected:
private:
};

#endif	// __CLIENTNETWORK_H__