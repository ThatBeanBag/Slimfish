// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_StopMoving.cpp
// Description	: CEventData_StopMoving implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_StopMoving.h"

// Local Includes

const TEventTypeID CEventData_StopMoving::s_kEVENT_TYPE_ID = 0xc0d963da;

CEventData_StopMoving::CEventData_StopMoving()
	:m_actorID(CActor::s_kINVALID_ACTOR_ID)
{

}

CEventData_StopMoving::CEventData_StopMoving(TActorID _actorID)
	:m_actorID(_actorID)
{

}

CEventData_StopMoving::~CEventData_StopMoving()
{

}

TActorID CEventData_StopMoving::GetActorID() const
{
	return m_actorID;
}

void CEventData_StopMoving::VSerialise(std::ostringstream& outStream)
{
	outStream << m_actorID << " ";
}

void CEventData_StopMoving::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_actorID;
}

TEventTypeID CEventData_StopMoving::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

