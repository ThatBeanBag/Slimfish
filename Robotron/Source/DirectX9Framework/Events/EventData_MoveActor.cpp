// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_MoveActor.cpp
// Description	: CEventData_MoveActor implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_MoveActor.h"

// Local Includes

const TEventTypeID CEventData_MoveActor::s_kEVENT_TYPE_ID = 0xc8fbc9cf;

CEventData_MoveActor::CEventData_MoveActor()
	:m_actorID(CActor::s_kINVALID_ACTOR_ID),
	m_matTransform(CMatrix4x4::s_kIdentity)
{

}

CEventData_MoveActor::CEventData_MoveActor(TActorID _actorID, const CMatrix4x4& _krmatTransform)
	:m_actorID(_actorID),
	m_matTransform(_krmatTransform)
{

}

CEventData_MoveActor::~CEventData_MoveActor()
{

}

void CEventData_MoveActor::VSerialise(std::ostringstream& outStream)
{
	outStream << m_actorID << " ";
	outStream << m_matTransform << " ";
}

void CEventData_MoveActor::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_actorID;
	inStream >> m_matTransform;
}

TEventTypeID CEventData_MoveActor::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
