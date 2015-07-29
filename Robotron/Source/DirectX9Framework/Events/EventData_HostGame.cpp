// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_HostGame.cpp
// Description	: CEventData_HostGame implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_HostGame.h"

// Local Includes

const TEventTypeID CEventData_HostGame::s_kEVENT_TYPE_ID = 0x4d68efda;

CEventData_HostGame::CEventData_HostGame(const std::string& _krstrUsername, const std::string& _krstrServerName) 
	:m_strUsername(_krstrUsername),
	m_strServerName(_krstrServerName)
{

}

CEventData_HostGame::CEventData_HostGame()
{

}

CEventData_HostGame::~CEventData_HostGame()
{

}

const std::string CEventData_HostGame::GetUsername() const
{
	return m_strUsername;
}

const std::string CEventData_HostGame::GetServerName() const
{
	return m_strServerName;
}

void CEventData_HostGame::VSerialise(std::ostringstream& outStream)
{
	DEBUG_ERROR("CEventData_HostGame shouldn't be serialized");
}

void CEventData_HostGame::VDeserialise(std::istringstream& inStream)
{
	DEBUG_ERROR("CEventData_HostGame shouldn't be serialized");
}


TEventTypeID CEventData_HostGame::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
