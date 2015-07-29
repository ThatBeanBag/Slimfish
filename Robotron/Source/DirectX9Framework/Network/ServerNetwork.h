// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ServerNetwork.h
// Description	: CServerNetwork declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SERVERNETWORK_H__
#define __SERVERNETWORK_H__

// Library Includes

// Local Includes

class CServerNetwork : public ABaseNetwork {
	// Member Functions
	typedef std::map<std::string, TAddress> TClientMap;
public:
	CServerNetwork(const std::string& _krstrServerName);
	~CServerNetwork();

	virtual void VHandleInput() override;
protected:
private:
	// Member Variables
public:
protected:
private:
	std::string m_strServerName;
	TClientMap m_clients;
	std::vector<TAddress> m_broadcastedTo;
};

#endif	// __SERVERNETWORK_H__