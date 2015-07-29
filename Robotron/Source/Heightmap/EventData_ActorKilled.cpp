// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_ActorKilled.cpp
// Description	: CEventData_ActorKilled implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "EventData_ActorKilled.h"

// Local Includes

const TEventTypeID CEventData_ActorKilled::s_kEVENT_TYPE_ID = 0x738b3230;

CEventData_ActorKilled::CEventData_ActorKilled()
{

}

CEventData_ActorKilled::CEventData_ActorKilled(TActorID _actorID, const std::string& _krActorType)
	:m_actorID(_actorID),
	m_strActorType(_krActorType)
{

}

CEventData_ActorKilled::~CEventData_ActorKilled()
{

}

const TActorID CEventData_ActorKilled::GetActorID() const
{
	return m_actorID;
}

const std::string CEventData_ActorKilled::GetActorType() const
{
	return m_strActorType;
}

void CEventData_ActorKilled::VSerialise(std::ostringstream& outStream)
{
	outStream << m_actorID << " ";
	outStream << m_strActorType << " ";
}

void CEventData_ActorKilled::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_actorID;
	inStream >> m_strActorType;
}

TEventTypeID CEventData_ActorKilled::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
