// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Socket.cpp
// Description	: CSocket implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Socket.h"

// Local Includes
#include "Packet.h"

/**
* Default Constructor
*
* @author: 	Hayden Asplet
*/
CSocket::CSocket()
	:m_hSocket(INVALID_SOCKET),
	m_ulRemoteIPAddress(0),
	m_usRemotePort(0)
{

}

/**
* Destructor
*
* Closes and invalidates the socket.
*
* @author: 	Hayden Asplet
*/
CSocket::~CSocket()
{
	if (m_hSocket != INVALID_SOCKET) {
	// Is the socket valid?
		// Close and invalidate it.
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

/**
* Creates a socket and binds it to the local port number.
*
* Doesn't prepare the socket for sending packets.
* To send packets you must first set the remote address, or have received a packet from a remote machine.
*
* @author: 	Hayden Asplet
* @param:	unsigned short _usLocalPort - port number to bind the socket to.
* @return:  bool
*/
ESocketError CSocket::Initialise(unsigned short _usLocalPort)
{
	// Create the UDP socket.
	m_hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (m_hSocket == INVALID_SOCKET) {
		// Did the socket fail to be created?
		return SOCKERR_UNKNOWN;
	}

	sockaddr_in socketAddress;
	ZeroMemory(&socketAddress, sizeof(socketAddress));

	// Fill the socket address struct for the server address.
	socketAddress.sin_addr.s_addr = ADDR_ANY;
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(_usLocalPort);

	// Bind the socket to the port number.
	if (bind(m_hSocket, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) == SOCKET_ERROR) {
		// Did the socket fail to bind to the port?

		// Close and invalidate the socket.
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;

		return SOCKERR_UNKNOWN;
	}

	int iBroadcast = 1;

	if (setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&iBroadcast), sizeof(iBroadcast)) == SOCKET_ERROR) {
		// Did the socket fail to set broadcast?
		throw CSocketError(WSAGetLastError());

		// Close and invalidate the socket.
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;

		return SOCKERR_UNKNOWN;
	}

	return SOCKERR_OK;
}

ESocketError CSocket::InitialiseScan(unsigned short _usPortMin, unsigned short _usPortMax)
{
	// Create the UDP socket.
	m_hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (m_hSocket == INVALID_SOCKET) {
		// Did the socket fail to be created?
		return SOCKERR_UNKNOWN;
	}

	sockaddr_in socketAddress;
	ZeroMemory(&socketAddress, sizeof(socketAddress));

	// Fill the socket address struct for the server address.
	socketAddress.sin_addr.s_addr = ADDR_ANY;
	socketAddress.sin_family = AF_INET;

	bool bBound = false;

	for (unsigned short usPort = _usPortMin; usPort <= _usPortMax; ++usPort) {
		socketAddress.sin_port = htons(usPort);

		// Bind the socket to the port number.
		if (bind(m_hSocket, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) != SOCKET_ERROR) {
			// Did the binding succeed?
			bBound = true;
			break;
		}
	}

	if (!bBound) {
		// Did the socket fail to bind all ports in the range?
		// Close and invalidate the socket.
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;

		return SOCKERR_UNKNOWN;
	}

	int iBroadcast = 1;

	if (setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&iBroadcast), sizeof(iBroadcast)) == SOCKET_ERROR) {
		// Did the socket fail to set broadcast?
		throw CSocketError(WSAGetLastError());

		// Close and invalidate the socket.
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;

		return SOCKERR_UNKNOWN;
	}

	return SOCKERR_OK;
}

/**
* Sends a packet to the remote IP address and port.
*
* @author: 	Hayden Asplet
* @param:	shared_ptr<IPacket> pPacketToSend - Pointer to the packet to be sent.
* @return:  bool - true if the packet was sent successfully.
*/
ESocketError CSocket::SendPacket(shared_ptr<IPacket> pPacketToSend)
{
	const char* const pcPacketData = pPacketToSend->VGetData();
	int iPacketLength = pPacketToSend->VGetLength();

	sockaddr_in sockAddress;
	ZeroMemory(&sockAddress, sizeof(sockAddress));

	// Fill the socket address struct for the server address.
	sockAddress.sin_addr.s_addr = htonl(m_ulRemoteIPAddress);
	sockAddress.sin_family = AF_INET;
	sockAddress.sin_port = htons(m_usRemotePort);

	// Send the packet data to the connected socket.
	int iReturnCode = sendto(m_hSocket,
							 pcPacketData,
							 iPacketLength,
							 0,
							 reinterpret_cast<sockaddr*>(&sockAddress),
							 sizeof(sockAddress));

	if (iReturnCode <= SOCKET_ERROR) {
		return SOCKERR_UNKNOWN;
	}

	return SOCKERR_OK;
}


/**
* Receives a packet from anywhere.
*
* m_ulRemoteIPAddress and m_usRemotePort will be replaced with
* the IP address and port number of the sender upon succession.
*
* Timeout is in micro seconds which is 0.000001 seconds or 0.001 milliseconds
* if timeout is 0 the receive call will block instead.
*
* @author: 	Hayden Asplet
* @param:	shared_ptr<IPacket> _pPacket - packet to be received.
* @param:	unsigned int _uiMicroTimeout - timeout in microseconds
* @return:  ESocketError
*/
ESocketError CSocket::RecievePacket(shared_ptr<IPacket> _pPacket, int _iMicroTimeout)
{
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_hSocket, &fds);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = _iMicroTimeout;

	int iNumBytesToRecieve = 0;

	if (_iMicroTimeout < 0) {
		// If the timeout is negative, we block until there's data to receive.
		// Block until we have bytes to receive.
		iNumBytesToRecieve = select(m_hSocket, &fds, NULL, NULL, NULL);
	}
	else {
		iNumBytesToRecieve = select(m_hSocket, &fds, NULL, NULL, &tv);
	}

	if (iNumBytesToRecieve == 0) {
	// Did the select() call timeout?
		return SOCKERR_TIMEOUT;
	}
	else if (iNumBytesToRecieve == SOCKET_ERROR) {
		return SOCKERR_UNKNOWN;
	}

	sockaddr_in sockAddress;
	int iSizeOfAdd = sizeof(sockAddress);

	int iNumBytesRecieved = recvfrom(m_hSocket,
									 m_pcBufferRecieveData,
									 g_kuiMAX_PACKET_LENGTH,
									 0,
									 reinterpret_cast<sockaddr*>(&sockAddress),
									 &iSizeOfAdd);

	if (iNumBytesRecieved == 0) {
		// Did the connection close?
		return SOCKERR_CLOSED;
	}
	else if (iNumBytesRecieved <= SOCKET_ERROR) {
		if (WSAGetLastError() == WSAECONNRESET) {
			return SOCKERR_DROPPED;
		}
		else if (WSAGetLastError() == WSAETIMEDOUT) {
			return SOCKERR_TIMEOUT;
		}

		return SOCKERR_UNKNOWN;
	}
	else {
	// Successfully received packet data.
		// Set the data of the packet pointer.
		_pPacket->VSetData(m_pcBufferRecieveData, strlen(m_pcBufferRecieveData) + 1);

		// Store the IP address and port number of the sender.
		m_ulRemoteIPAddress = ntohl(sockAddress.sin_addr.s_addr);
		m_usRemotePort = ntohs(sockAddress.sin_port);
	}

	return SOCKERR_OK;
}

const TAddress CSocket::GetRemoteAddress() const
{
	TAddress address;
	address.m_strIPAddress = GetHostByAddress(m_ulRemoteIPAddress);
	address.m_usPort = m_usRemotePort;
	return address;
}

