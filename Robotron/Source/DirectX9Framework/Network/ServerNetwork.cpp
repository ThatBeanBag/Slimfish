// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ServerNetwork.cpp
// Description	: CServerNetwork implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "ServerNetwork.h"

// Local Includes
#include "Packet.h"
#include "..\Events\EventData_ClientJoin.h"

CServerNetwork::CServerNetwork(const std::string& _krstrServerName)
	:m_strServerName(_krstrServerName)
{

}

CServerNetwork::~CServerNetwork()
{

}

void CServerNetwork::VHandleInput()
{
	for (unsigned int i = 0; i < m_packetInList.size(); ++i) {
		// Get the current packet.
		shared_ptr<IPacket> pPacket = m_packetInList[i].m_pPacket;

		if (m_packetInList[i].m_bHasDropped) {
		// Has this player been dropped?
			// Determine who left.
			for (auto iter = m_clients.begin(); iter != m_clients.end();  ++iter) {
				if (iter->second.m_strIPAddress == GetHostByAddress(m_packetInList[i].m_ipAddress &&
					iter->second.m_usPort == m_packetInList[i].m_usPort)) {
				// Send player left event.
					shared_ptr<CEventData_PlayerLeft> pEvent(new CEventData_PlayerLeft(iter->first));
					CEventManager::GetInstance()->QueueEvent(pEvent);

					// Remove the client from the list.
					m_clients.erase(iter);

					break;
				}
			}
			
			continue;
		}

		// Convert the current packet to a input stream for analysis.
		std::istringstream inStream(pPacket->VGetData());

		// Get the message type.
		int iMessageType;
		inStream >> iMessageType;
		ENetworkMessageType eType = static_cast<ENetworkMessageType>(iMessageType);

		switch (eType) {
			case NETMSG_EVENT: {
				CreateAndQueueEvent(inStream);
				break;
			}
			case NETMSG_BROADCAST: {	// A client has sent out a broadcast.
				// Respond immediately.
				std::ostringstream outStream;

				CGameLogic* pGame = g_pApp->GetGame();
				if (pGame) {
					CGameLogic::EGameState eGameState = pGame->GetGameState();

					// Players can only join if the server is waiting for players.
					bool bIsWaiting = eGameState == CGameLogic::GS_LOBBY;

					// This is a broadcast response.
					outStream << static_cast<int>(NETMSG_BROADCAST_RESPONSE) << " ";

					outStream << m_strServerName << " ";
					outStream << g_pApp->GetGame()->GetNumPlayers() << " ";
					outStream << g_pApp->GetGame()->GetMaxPlayers() << " ";
					outStream << bIsWaiting << " ";

					// Send the packet out.
					shared_ptr<IPacket> pPacket(new CTextPacket(outStream.str().c_str()));
					if (pPacket) {
						Send(pPacket);
					}
				}

				break;
			}
			case NETMSG_BROADCAST_RESPONSE: {
				// Servers don't receive broadcast responses.
				break;
			}
			case NETMSG_PLAYER_JOIN_REQUEST: { // A player has request to join the game.
				// Create a response to send back to the client.
				std::ostringstream outStream;
				outStream << static_cast<int>(NETMSG_PLAYER_JOIN_RESPONSE) << " ";

				CGameLogic* pGame = g_pApp->GetGame();
				if (pGame) {
					CGameLogic::EGameState eGameState = pGame->GetGameState();

					if (eGameState != CGameLogic::GS_LOBBY) {
					// Is the game already in session?
						// Players can only join if the server is waiting for players.
						outStream << static_cast<int>(JOINRR_GAME_IN_SESSION) << " ";
					}
					else if (pGame->GetNumPlayers() >= pGame->GetMaxPlayers()) {
					// Is the lobby full?
						outStream << static_cast<int>(JOINRR_FULL_LOBBY) << " ";
					}
					else {
						// Get the username from the request to join.
						std::string strUsername;
						inStream >> strUsername;

						auto findIter = m_clients.find(strUsername);
						if (findIter != m_clients.end() || strUsername == g_pApp->GetGame()->GetUsername()) {
						// Is the user name already in use by another client?
							outStream << static_cast<int>(JOINRR_USERNAME_IN_USE) << " ";
						}
						else {
						// Join is accepted.
							outStream << static_cast<int>(JOINRR_OK);

							TAddress address = m_pSocket->GetRemoteAddress();
							// Add the client to server.
							m_clients[strUsername] = address;

							shared_ptr<CEventData_ClientJoin> pEvent(new CEventData_ClientJoin(address, strUsername));
							CEventManager::GetInstance()->QueueEvent(pEvent);
						}
					}
				}
				else {
					outStream << static_cast<int>(JOINRR_ERROR) << " ";
				}

				// Send the response.
				shared_ptr<CTextPacket> pPacket(new CTextPacket(outStream.str().c_str()));
				if (pPacket) {
					Send(pPacket);
				}
				break;
			}
			case NETMSG_PLAYER_JOIN_RESPONSE: {
				break;
			}
			case NETMSG_PLAYER_LEFT: {
				break;
			}
			case NETMSG_MAX: {
				break;
			}
			default: {
				break;
			}
		}
	}
}
