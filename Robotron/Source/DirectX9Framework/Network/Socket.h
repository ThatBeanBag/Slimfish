// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Socket.h
// Description	: CSocket declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SOCKET_H__
#define __SOCKET_H__

// Library Includes
#include <WinSock2.h>

// Local Includes

class IPacket;
struct TAddress;

const unsigned int g_kuiMAX_PACKET_LENGTH = 256;

// Exceptions
class CSocketError {
public:
	CSocketError(int _iWSAError)
		:m_iWSAError(_iWSAError)
	{}

	int GetError() const { return m_iWSAError; }
private:
	int m_iWSAError;
};

class CTimeOut : public CSocketError {
public:
	CTimeOut(int _iWSAError)
		:CSocketError(_iWSAError)
	{
	}
};

class CDropped : public CSocketError {
public:
	CDropped(int _iWSAError)
		:CSocketError(_iWSAError)
	{
	}
};

enum ESocketError {
	SOCKERR_OK,
	SOCKERR_TIMEOUT,
	SOCKERR_DROPPED,
	SOCKERR_CLOSED,
	SOCKERR_UNKNOWN
};

/************************************************************************/
/* CSocket
/*
/* The low level implementation sending and receiving packets from remote
/* machines through UDP sockets. 
/************************************************************************/
class CSocket {
	// Member Functions
public:
	CSocket();
	~CSocket();

	/**
	* Creates the socket and binds it to the local port number.
	*
	* Doesn't prepare the socket for sending packets, please set the remote address first.
	* 
	* @author: 	Hayden Asplet
	* @param:	unsigned short _usLocalPort - local port to bind to.
	* @return:  bool - true if binding and creation was successful.
	*/
	ESocketError Initialise(unsigned short _usLocalPort);

	/**
	* Creates a socket and binds to a port by checking a range of ports until a unused port is found.
	* 
	* @author: 	Hayden Asplet
	* @param:	unsigned short _usPortMin - minimum port number to bind to.
	* @param:	unsigned short _usPortMax - maximum port number to bind to.
	* @return:  bool - true if binding and creation was successful.
	*/
	ESocketError InitialiseScan(unsigned short _usPortMin, unsigned short _usPortMax);

	/**
	* Sends a packet to the remote application, specified by the remote IP address and port number of the socket.
	*
	* @author: 	Hayden Asplet
	* @param:	shared_ptr<IPacket> pPacketToSend - packet to send.
	* @return:  bool - true if the packet was sent.
	*/
	ESocketError SendPacket(shared_ptr<IPacket> pPacketToSend);

	/**
	* Receives a packet from a remote application, the remote IP address and
	* port number of this socket will then be specified in terms of the sender.
	* 
	* @author: 	Hayden Asplet
	* @param:	shared_ptr<IPacket> _pPacket - packet to be received. Will be filled with received information if function succeeded.
	* @param:	unsigned int _uiTimeout - the amount of time to wait for in micro seconds. If negative the call will block until a packet is received.
	* @return:  ESocketError
	*/
	ESocketError RecievePacket(shared_ptr<IPacket> _pPacket, int _iTimeout = -1);

	/**
	* Gets the remote IP address of the last machine the socket received a packet from.
	*
	* Using SetRemoteIPAddress() will override this, until another packet is received.
	* 
	* @author: 	Hayden Asplet
	* @return:  unsigned long - IP address.
	*/
	unsigned long GetRemoteIPAddress() { return m_ulRemoteIPAddress; }

	/**
	* Gets the remote port number of the last machine the socket received a packet from.
	*
	* Using SetRemotePort() will override this, until another packet is received.
	*
	* @author: 	Hayden Asplet
	* @return:  unsigned short - port number.
	*/
	unsigned short GetRemotePort() { return m_usRemotePort; }

	/**
	* Sets the IP address to send packets to.
	* 
	* @author: 	Hayden Asplet
	* @param:	unsigned long _ulIPAddress - IP Address to set for outgoing packets to go to.
	* @return:  void
	*/
	void SetRemoteIPAddress(unsigned long _ulIPAddress) { m_ulRemoteIPAddress = _ulIPAddress; }

	/**
	* Sets the remote port to send packets to.
	* 
	* @author: 	Hayden Asplet
	* @param:	unsigned short _usPort - port number to set for outgoing packets to go to.
	* @return:  void
	*/
	void SetRemotePort(unsigned short _usPort) { m_usRemotePort = _usPort; }

	/**
	* Gets the full remote address of the last machine the socket received a packet from. 
	* 
	* @author: 	Hayden Asplet
	* @return:  const TAddress - full address; IP and port.
	*/
	const TAddress GetRemoteAddress() const;
protected:
private:

	// Member Variables
public:
protected:
private:
	SOCKET m_hSocket;
	char m_pcBufferRecieveData[g_kuiMAX_PACKET_LENGTH];

	unsigned long m_ulRemoteIPAddress;	// The IP address of whoever we are currently sending/receiving data to/from.
	unsigned short m_usRemotePort;	// The port number of whoever we are currently sending/receiving data to/from.
};

#endif	// __SOCKET_H__