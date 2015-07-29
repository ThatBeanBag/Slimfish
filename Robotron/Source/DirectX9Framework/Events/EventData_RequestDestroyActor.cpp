// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_RequestDestroyActor.cpp
// Description	: CEventData_RequestDestroyActor implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_RequestDestroyActor.h"

// Local Includes

const TEventTypeID CEventData_RequestDestroyActor::s_kEVENT_TYPE_ID = 0x2370eaad;

CEventData_RequestDestroyActor::CEventData_RequestDestroyActor(TActorID _actorID /*= CActor::s_kINVALID_ACTOR_ID*/)
	:m_actorID(_actorID)
{

}

CEventData_RequestDestroyActor::~CEventData_RequestDestroyActor()
{

}

const TActorID CEventData_RequestDestroyActor::GetActorID() const
{
	return m_actorID;
}

void CEventData_RequestDestroyActor::SetActorID(TActorID _actorID)
{
	m_actorID = _actorID;
}

void CEventData_RequestDestroyActor::VSerialise(std::ostringstream& outStream)
{
	outStream << m_actorID << " ";
}

void CEventData_RequestDestroyActor::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_actorID;
}

TEventTypeID CEventData_RequestDestroyActor::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

