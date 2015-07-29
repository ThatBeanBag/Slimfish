// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_CollisionSeparation.cpp
// Description	: CEventData_CollisionSeparation implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_CollisionSeparation.h"

// Local Includes

const TEventTypeID CEventData_CollisionSeparation::s_kEVENT_TYPE_ID = 0xad25d64b;

CEventData_CollisionSeparation::CEventData_CollisionSeparation()
	:m_actorID1(CActor::s_kINVALID_ACTOR_ID),
	m_actorID2(CActor::s_kINVALID_ACTOR_ID)
{

}

CEventData_CollisionSeparation::CEventData_CollisionSeparation(TActorID _actorID1, TActorID _actorID2)
	:m_actorID1(_actorID1),
	m_actorID2(_actorID2)
{

}

CEventData_CollisionSeparation::~CEventData_CollisionSeparation()
{

}

TActorID CEventData_CollisionSeparation::GetActorID1() const
{
	return m_actorID1;
}

TActorID CEventData_CollisionSeparation::GetActorID2() const
{
	return m_actorID2;
}

void CEventData_CollisionSeparation::VSerialise(std::ostringstream& outStream)
{
	DEBUG_ERROR("CEventData_CollisionSeparation should not be serialised.");
}

void CEventData_CollisionSeparation::VDeserialise(std::istringstream& inStream)
{
	DEBUG_ERROR("CEventData_CollisionSeparation should not be serialised.");
}

TEventTypeID CEventData_CollisionSeparation::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

