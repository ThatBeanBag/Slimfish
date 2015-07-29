// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_RemotePlayerActorAssignment.cpp
// Description	: CEventData_RemotePlayerActorAssignment implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_RemotePlayerActorAssignment.h"

// Local Includes

const TEventTypeID CEventData_RemotePlayerActorAssignment::s_kEVENT_TYPE_ID = 0x1f27a563;

CEventData_RemotePlayerActorAssignment::CEventData_RemotePlayerActorAssignment(TActorID _actorID /*= CActor::s_kINVALID_ACTOR_ID*/)
	:m_actorID(_actorID)
{

}

CEventData_RemotePlayerActorAssignment::~CEventData_RemotePlayerActorAssignment()
{

}

TActorID CEventData_RemotePlayerActorAssignment::GetActorID() const
{
	return m_actorID;
}

void CEventData_RemotePlayerActorAssignment::VSerialise(std::ostringstream& outStream)
{
	outStream << m_actorID << " ";
}

void CEventData_RemotePlayerActorAssignment::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_actorID;
}

TEventTypeID CEventData_RemotePlayerActorAssignment::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
