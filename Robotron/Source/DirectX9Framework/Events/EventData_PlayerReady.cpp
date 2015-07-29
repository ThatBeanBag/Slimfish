// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_ClientReady.cpp
// Description	: CEventData_ClientReady implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_PlayerReady.h"

// Local Includes

const TEventTypeID CEventData_PlayerReady::s_kEVENT_TYPE_ID = 0x0a5e1ce3;
const TEventTypeID CEventData_RequestPlayerReady::s_kEVENT_TYPE_ID = 0x7f9b72d7;

CEventData_PlayerReady::CEventData_PlayerReady(const std::string& _krstrUsername, bool _bIsReady)
	:m_strUsername(_krstrUsername),
	m_bIsReady(_bIsReady)
{

}

CEventData_PlayerReady::~CEventData_PlayerReady()
{

}

bool CEventData_PlayerReady::IsReady() const
{
	return m_bIsReady;
}

std::string CEventData_PlayerReady::GetUsername() const
{
	return m_strUsername;
}

void CEventData_PlayerReady::VSerialise(std::ostringstream& outStream)
{
	outStream << m_strUsername << " ";
	outStream << m_bIsReady << " ";
}

void CEventData_PlayerReady::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_strUsername;
	inStream >> m_bIsReady;
}

TEventTypeID CEventData_PlayerReady::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}


CEventData_RequestPlayerReady::CEventData_RequestPlayerReady(const std::string& _krstrUsername /*= std::string()*/, bool _bIsReady /*= true*/) 
	:CEventData_PlayerReady(_krstrUsername, _bIsReady)
{

}

CEventData_RequestPlayerReady::~CEventData_RequestPlayerReady()
{

}

TEventTypeID CEventData_RequestPlayerReady::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

