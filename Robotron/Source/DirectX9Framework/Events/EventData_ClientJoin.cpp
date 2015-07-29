// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_ClientJoin.cpp
// Description	: CEventData_ClientJoin implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_ClientJoin.h"

// Local Includes

const TEventTypeID CEventData_ClientJoin::s_kEVENT_TYPE_ID = 0x530addbb;

CEventData_ClientJoin::CEventData_ClientJoin()
{

}

CEventData_ClientJoin::CEventData_ClientJoin(const TAddress& _krAddress, const std::string& _krstrUsername)
	:m_address(_krAddress),
	m_strUsername(_krstrUsername)
{

}

CEventData_ClientJoin::~CEventData_ClientJoin()
{

}

TAddress CEventData_ClientJoin::GetRemoteAddress() const
{
	return m_address;
}

const std::string CEventData_ClientJoin::GetUsername() const
{
	return m_strUsername;
}

void CEventData_ClientJoin::VSerialise(std::ostringstream& outStream)
{
	DEBUG_ERROR("CEventData_ClientJoin should not be serialised.");
}

void CEventData_ClientJoin::VDeserialise(std::istringstream& inStream)
{
	DEBUG_ERROR("CEventData_ClientJoin should not be serialised.");
}

TEventTypeID CEventData_ClientJoin::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
