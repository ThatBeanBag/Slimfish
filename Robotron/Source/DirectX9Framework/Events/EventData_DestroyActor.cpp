// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_DestroyActor.cpp
// Description	: CEventData_DestroyActor implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_DestroyActor.h"

// Local Includes

const TEventTypeID CEventData_DestroyActor::s_kEVENT_TYPE_ID = 0x55e1e295;

CEventData_DestroyActor::CEventData_DestroyActor()
	:m_actorID(CActor::s_kINVALID_ACTOR_ID)
{

}

CEventData_DestroyActor::CEventData_DestroyActor(TActorID _actorID)
	:m_actorID(_actorID)
{

}

CEventData_DestroyActor::~CEventData_DestroyActor()
{

}

const TActorID CEventData_DestroyActor::GetActorID() const
{
	return m_actorID;
}

void CEventData_DestroyActor::VSerialise(std::ostringstream& outStream)
{
	outStream << m_actorID << " ";
}

void CEventData_DestroyActor::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_actorID;
}

TEventTypeID CEventData_DestroyActor::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
