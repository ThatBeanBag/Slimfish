// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BaseNetwork.cpp
// Description	: CBaseNetwork implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes
#include <ws2tcpip.h>

// This Include
#include "BaseNetwork.h"

// Local Includes
#include "Packet.h"
#include "Socket.h"

ABaseNetwork::ABaseNetwork()
{

}

ABaseNetwork::~ABaseNetwork()
{

}

bool ABaseNetwork::Initialise(unsigned short _usPort)
{
	if (!InitialiseWSA()) {
		return false;
	}

	if (m_pSocket->Initialise(_usPort) != SOCKERR_OK) {
		return false;
	}

	return true;
}

bool ABaseNetwork::InitialiseScan(unsigned short _usPortMin, unsigned short _usPortMax)
{
	if (!InitialiseWSA()) {
		return false;
	}

	if (m_pSocket->InitialiseScan(_usPortMin, _usPortMax) != SOCKERR_OK) {
		return false;
	}

	return true;
}

void ABaseNetwork::DoSelect(unsigned int _uiMicroSecondsPause)
{
	while (true) {
		shared_ptr<CTextPacket> pPacket(new CTextPacket);
		ESocketError eSockErr = m_pSocket->RecievePacket(pPacket, _uiMicroSecondsPause);

		if (eSockErr == SOCKERR_OK || eSockErr == SOCKERR_DROPPED) {
			TPacketAddress packetAddress;
			packetAddress.m_ipAddress = m_pSocket->GetRemoteIPAddress();
			packetAddress.m_usPort = m_pSocket->GetRemotePort();

			if (eSockErr == SOCKERR_DROPPED) {
				// Tell server/client the that an existing connection has been dropped.
				packetAddress.m_bHasDropped = true;
			}
			else {
				packetAddress.m_bHasDropped = false;
				packetAddress.m_pPacket = pPacket;
			}

			m_packetInList.push_back(packetAddress);
		}
		else if (eSockErr == SOCKERR_TIMEOUT) {
			// We have timed out.
			// Stop looping.
			break;
		}
		else if (eSockErr == SOCKERR_UNKNOWN) {
			DEBUG_WARNING(GetWSAErrorInfo(WSAGetLastError()));
			return;
		}
	}

	VHandleInput();

	m_packetInList.clear();
}

void ABaseNetwork::Send(const TAddress& _krAddress, shared_ptr<IPacket> _pPacket)
{
	m_pSocket->SetRemoteIPAddress(GetHostByName(_krAddress.m_strIPAddress));
	m_pSocket->SetRemotePort(_krAddress.m_usPort);
	m_pSocket->SendPacket(_pPacket);
}

void ABaseNetwork::Send(shared_ptr<IPacket> _pPacket)
{
	m_pSocket->SendPacket(_pPacket);
}

bool ABaseNetwork::InitialiseWSA()
{
	// Startup windows sockets.
	WSADATA wsaData;

	if (WSAStartup(0x0202, &wsaData) != 0) {
		DEBUG_ERROR(GetWSAErrorInfo(WSAGetLastError()));
		return false;
	}

	m_pSocket.reset(new CSocket());

	if (!m_pSocket) {
		return false;
	}

	return true;
}

void ABaseNetwork::CreateAndQueueEvent(std::istringstream& _inStream)
{
	TEventTypeID eventTypeID;
	_inStream >> eventTypeID;

	shared_ptr<IEventData> pEvent = CEventManager::GetInstance()->CreateEvent(eventTypeID);
	if (!pEvent) {
		DEBUG_ERROR("Unknown event type sent from remote machine" + ToString(eventTypeID));
	}
	else {
		pEvent->VDeserialise(_inStream);
		CEventManager::GetInstance()->QueueEvent(pEvent);
	}
}

unsigned long GetHostByName(const std::string& _krHostName)
{
	sockaddr_in socketAddress;
	inet_pton(AF_INET, _krHostName.c_str(), &socketAddress.sin_addr);

	return ntohl(socketAddress.sin_addr.s_addr);
}

const std::string GetHostByAddress(unsigned long _ulIP)
{
	static char pcHost[256];
	unsigned long ulNetIP = htonl(_ulIP);

	inet_ntop(AF_INET, &ulNetIP, pcHost, 256);

	return std::string(pcHost);
}

const std::string GetWSAErrorInfo(int _iError)
{
	char* pErrorMessage = nullptr;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				   nullptr,
				   _iError,
				   0,
				   (LPSTR)(&pErrorMessage),
				   0,
				   nullptr);

	std::string strErrorMessage = "WSA Error: ";

	if (pErrorMessage) {
		strErrorMessage += pErrorMessage;
		LocalFree(pErrorMessage);
	}

	return strErrorMessage;
}
