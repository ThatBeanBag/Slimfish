// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ClientNetwork.cpp
// Description	: CClientNetwork implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "ClientNetwork.h"

// Local Includes
#include "Packet.h"

CClientNetwork::CClientNetwork()
{

}

CClientNetwork::~CClientNetwork()
{

}

void CClientNetwork::BroadcastToDetectServers(TServerList& _rServerList)
{
	std::ostringstream outStream;
	outStream << static_cast<int>(NETMSG_BROADCAST) << "\r\n";

	shared_ptr<CTextPacket> pPacket(new CTextPacket(outStream.str().c_str()));

	if (!pPacket) {
		return;
	}

	// Send out a broadcast message.
	m_pSocket->SetRemoteIPAddress(INADDR_BROADCAST);
	m_pSocket->SetRemotePort(g_kusDEF_SERVER_PORT);
	Send(pPacket);

	while (true) {
		shared_ptr<CTextPacket> pPacket(new CTextPacket());

		ESocketError eSockErr = m_pSocket->RecievePacket(pPacket, 500000);
		if (eSockErr == SOCKERR_OK) {
			std::istringstream inStream(pPacket->VGetData());
			int iType;

			inStream >> iType;
			ENetworkMessageType eType = static_cast<ENetworkMessageType>(iType);

			if (iType == NETMSG_BROADCAST_RESPONSE) {
				// Get the details of the server from the response.
				TServerDetails serverDetails;
				inStream >> serverDetails.m_strServerName;
				inStream >> serverDetails.m_iNumPlayers;
				inStream >> serverDetails.m_iMaxPlayers;
				inStream >> serverDetails.m_bIsWaiting;
				serverDetails.m_serverAddress.m_strIPAddress = GetHostByAddress(m_pSocket->GetRemoteIPAddress());
				serverDetails.m_serverAddress.m_usPort = m_pSocket->GetRemotePort();

				// Add the server to the list.
				_rServerList.push_back(serverDetails);
			}
			else {
				// Not a broadcast response; not interested.
			}
		}
		else if (eSockErr == SOCKERR_TIMEOUT) {
			break;
		}
		else {
			DEBUG_ERROR(GetWSAErrorInfo(WSAGetLastError()));
			break;
		}
	}

	/*// Receive packets for 30 micro seconds.
	DoSelect(30000);

	for (unsigned int i = 0; i < m_packetInList.size(); ++i) {
		shared_ptr<IPacket> pPacket = m_packetInList[i].m_pPacket;
		std::istringstream inStream(pPacket->VGetData());
		unsigned char ucType;

		inStream >> ucType;
		ENetworkMessageType eType = static_cast<ENetworkMessageType>(ucType);

		if (ucType == NETMSG_BROADCAST_RESPONSE) {
			// Get the details of the server from the response.
			TServerDetails serverDetails;
			inStream >> serverDetails.m_strServerName;
			inStream >> serverDetails.m_iNumPlayers;
			inStream >> serverDetails.m_iMaxPlayers;
			inStream >> serverDetails.m_bIsWaiting;
			serverDetails.m_serverAddress.m_strIPAddress = GetHostByAddress(m_pSocket->GetRemoteIPAddress());
			serverDetails.m_serverAddress.m_usPort = m_pSocket->GetRemotePort();

			// Add the server to the list.
			_rServerList.push_back(serverDetails);
		}
		else {
			// Not a broadcast response; not interested.
		}
	}*/
}

ABaseNetwork::EJoinRequestResponse CClientNetwork::JoinServer(const TAddress& _krServerAddress, const std::string& _krstrUsername)
{
	// Send a request to join to the server.
	std::ostringstream outStream;
	outStream << static_cast<int>(NETMSG_PLAYER_JOIN_REQUEST) << " ";
	outStream << _krstrUsername << "\r\n";

	shared_ptr<CTextPacket> pPacket(new CTextPacket(outStream.str().c_str()));
	Send(_krServerAddress, pPacket);

	EJoinRequestResponse eResponse = ABaseNetwork::JOINRR_NORESPONSE;

	shared_ptr<CTextPacket> pRecievePacket(new CTextPacket());

	ESocketError eSockErr = m_pSocket->RecievePacket(pRecievePacket, 500000);
	if (eSockErr == SOCKERR_OK) {
		std::istringstream inStream(pRecievePacket->VGetData());
		int iType;

		inStream >> iType;
		ENetworkMessageType eType = static_cast<ENetworkMessageType>(iType);

		if (iType == NETMSG_PLAYER_JOIN_RESPONSE) {
			int iResponse;
			inStream >> iResponse;
			eResponse = static_cast<EJoinRequestResponse>(iResponse);
		}
		else {
			// Not a broadcast response; not interested.
		}
	}
	else if (SOCKERR_UNKNOWN) {
		DEBUG_WARNING(GetWSAErrorInfo(WSAGetLastError()));
	}

	return eResponse;
}

void CClientNetwork::VHandleInput()
{
	for (unsigned int i = 0; i < m_packetInList.size(); ++i) {
		shared_ptr<IPacket> pPacket = m_packetInList[i].m_pPacket;

		if (m_packetInList[i].m_bHasDropped) {
			shared_ptr<CEventData_LostConnection> pEvent(new CEventData_LostConnection());
			CEventManager::GetInstance()->QueueEvent(pEvent);
			continue;
		}

		std::istringstream inStream(pPacket->VGetData());
		int iType;
		inStream >> iType;
		ENetworkMessageType eType = static_cast<ENetworkMessageType>(iType);

		switch (eType) {
			case ABaseNetwork::NETMSG_EVENT: {
				CreateAndQueueEvent(inStream);
				break;
			}
			case ABaseNetwork::NETMSG_BROADCAST: {
				break;
			}
			case ABaseNetwork::NETMSG_BROADCAST_RESPONSE: {
				break;
			}
			case ABaseNetwork::NETMSG_PLAYER_JOIN_REQUEST: {
				break;
			}
			case ABaseNetwork::NETMSG_PLAYER_JOIN_RESPONSE: {
				break;
			}
			case ABaseNetwork::NETMSG_PLAYER_LEFT: {
				break;
			}
			case ABaseNetwork::NETMSG_MAX: {
				break;
			}
			default: {
				break;
			}
		}
	}
}
