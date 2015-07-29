// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_JoinGame.cpp
// Description	: CEventData_JoinGame implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_JoinGame.h"

// Local Includes

const TEventTypeID CEventData_JoinGame::s_kEVENT_TYPE_ID = 0x978bfbcf;

CEventData_JoinGame::CEventData_JoinGame()
	:m_iMaxPlayers(0)
{
	m_address.m_strIPAddress = "";
	m_address.m_usPort = 0;
}

CEventData_JoinGame::CEventData_JoinGame(const TAddress& _krServerAddress, const std::string& _krstrUsername, int _iMaxPlayers)
	: m_address(_krServerAddress),
	m_strUsername(_krstrUsername),
	m_iMaxPlayers(_iMaxPlayers)
{

}

CEventData_JoinGame::~CEventData_JoinGame()
{

}

const TAddress CEventData_JoinGame::GetAddress() const
{
	return m_address;
}

const std::string CEventData_JoinGame::GetUsername() const
{
	return m_strUsername;
}

const int CEventData_JoinGame::GetMaxPlayers() const
{
	return m_iMaxPlayers;
}

void CEventData_JoinGame::VSerialise(std::ostringstream& outStream)
{
	DEBUG_ERROR("CEventData_JoinGame shouldn't be serialised");
}

void CEventData_JoinGame::VDeserialise(std::istringstream& inStream)
{
	DEBUG_ERROR("CEventData_JoinGame shouldn't be serialised");
}

TEventTypeID CEventData_JoinGame::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

