// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_PlayerLeft.cpp
// Description	: CEventData_PlayerLeft implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_PlayerLeft.h"

// Local Includes

const TEventTypeID CEventData_PlayerLeft::s_kEVENT_TYPE_ID = 0xf528956b;
const TEventTypeID CEventData_RequestPlayerLeave::s_kEVENT_TYPE_ID = 0xe89dc99f;

CEventData_PlayerLeft::CEventData_PlayerLeft(const std::string& _krstrUsername /*= std::string()*/)
	:m_strUsername(_krstrUsername)
{

}

CEventData_PlayerLeft::~CEventData_PlayerLeft()
{

}

const std::string CEventData_PlayerLeft::GetUsername() const
{
	return m_strUsername;
}

void CEventData_PlayerLeft::VSerialise(std::ostringstream& outStream)
{
	outStream << m_strUsername << " ";
}

void CEventData_PlayerLeft::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_strUsername;
}

TEventTypeID CEventData_PlayerLeft::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

CEventData_RequestPlayerLeave::CEventData_RequestPlayerLeave(const std::string& _krstrUsername /*= std::string()*/) 
	:CEventData_PlayerLeft(_krstrUsername)
{

}

CEventData_RequestPlayerLeave::~CEventData_RequestPlayerLeave()
{

}

TEventTypeID CEventData_RequestPlayerLeave::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
