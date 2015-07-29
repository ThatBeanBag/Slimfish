// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BaseNetwork.h
// Description	: ABaseNetwork declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __BASENETWORK_H__
#define __BASENETWORK_H__

// Library Includes

// Local Includes
#include "Socket.h"

const int g_kiMAX_PACKET_SIZE = 256;
const int g_kiMAX_USERNAME_LENGTH = 30;
const int g_kiMAX_SERVERNAME_LENGTH = 30;

const char* const g_kpcLOCAL_IP_ADDRESS = "127.0.0.1";
const unsigned short g_kusDEF_SERVER_PORT = 56532;
const unsigned short g_kusDEF_CLIENT_PORT_MIN = 56533;
const unsigned short g_kusDEF_CLIENT_PORT_MAX = 57533;

class IPacket;

struct TAddress {
	std::string m_strIPAddress;
	unsigned short m_usPort;
};

class ABaseNetwork {
	struct TPacketAddress {
		unsigned long m_ipAddress;
		unsigned short m_usPort;
		shared_ptr<IPacket> m_pPacket;
		bool m_bHasDropped;
	};

	typedef std::vector<TPacketAddress> TPackets;
public:
	enum ENetworkMessageType {
		NETMSG_EVENT,
		NETMSG_BROADCAST,
		NETMSG_BROADCAST_RESPONSE,
		NETMSG_PLAYER_JOIN_REQUEST,
		NETMSG_PLAYER_JOIN_RESPONSE,
		NETMSG_PLAYER_LEFT,
		NETMSG_MAX = 255
	};

	enum EJoinRequestResponse {
		JOINRR_OK,
		JOINRR_FULL_LOBBY,
		JOINRR_GAME_IN_SESSION,
		JOINRR_USERNAME_IN_USE,
		JOINRR_ERROR,
		JOINRR_NORESPONSE,
		JOINRR_MAX = 255
	};

	// Member Functions
public:
	ABaseNetwork();
	virtual ~ABaseNetwork();

	bool Initialise(unsigned short _usPort);
	bool InitialiseScan(unsigned short _usPortMin, unsigned short _usPortMax);

	void DoSelect(unsigned int _uiMicroSecondsPause);

	/**
	* Sends a packet to the last receiver and/or sender.
	* 
	* @author: 	Hayden Asplet
	* @param:	shared_ptr<IPacket> _pPacket - packet to send.
	* @return:  void
	*/
	void Send(shared_ptr<IPacket> _pPacket);

	/**
	* Sends a packet to a specific address.
	* 
	* @author: 	Hayden Asplet
	* @param:	const TAddress & _krAddress - address to send to.
	* @param:	shared_ptr<IPacket> _pPacket - packet to send.
	* @return:  void
	*/
	void Send(const TAddress& _krAddress, shared_ptr<IPacket> _pPacket);

	virtual void VHandleInput() = 0;
protected:
	void CreateAndQueueEvent(std::istringstream& _inStream);
private:
	bool InitialiseWSA();

	// Member Variables
public:
protected:
	TPackets m_packetInList;
	TPackets m_packetOutList;
	unique_ptr<CSocket> m_pSocket;
private:
};

unsigned long GetHostByName(const std::string& _krHostName);
const std::string GetHostByAddress(unsigned long _ulIP);
const std::string GetWSAErrorInfo(int _iError);

#endif	// __BASENETWORK_H__