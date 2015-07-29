// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_RequestStartGame.cpp
// Description	: CEventData_RequestStartGame implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_RequestStartGame.h"

// Local Includes

const TEventTypeID CEventData_RequestStartGame::s_kEVENT_TYPE_ID = 0xb151a433;

CEventData_RequestStartGame::CEventData_RequestStartGame(bool _bIsSinglePlayer, bool _bIsRemote, std::string& _krstrHostName, unsigned short _usPort)
	:m_bIsRemote(_bIsRemote),
	m_strHostName(_krstrHostName),
	m_usPort(_usPort)
{

}

CEventData_RequestStartGame::~CEventData_RequestStartGame()
{

}

bool CEventData_RequestStartGame::IsRemote() const
{
	return m_bIsRemote;
}

std::string CEventData_RequestStartGame::GetHostName() const
{
	return m_strHostName;
}

unsigned short CEventData_RequestStartGame::GetPort() const
{
	return m_usPort;
}

void CEventData_RequestStartGame::VSerialise(std::ostringstream& outStream)
{

}

void CEventData_RequestStartGame::VDeserialise(std::istringstream& inStream)
{

}

TEventTypeID CEventData_RequestStartGame::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
