// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_LostConnection.cpp
// Description	: CEventData_LostConnection implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_LostConnection.h"

// Local Includes

const TEventTypeID CEventData_LostConnection::s_kEVENT_TYPE_ID = 0x267ad3d5;

CEventData_LostConnection::CEventData_LostConnection()
{

}

CEventData_LostConnection::~CEventData_LostConnection()
{

}

void CEventData_LostConnection::VSerialise(std::ostringstream& outStream)
{
	DEBUG_ERROR("CEventData_LostConnection shouldn't be serialised.");
}

void CEventData_LostConnection::VDeserialise(std::istringstream& inStream)
{
	DEBUG_ERROR("CEventData_LostConnection shouldn't be serialised.");
}

TEventTypeID CEventData_LostConnection::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
