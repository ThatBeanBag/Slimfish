// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_ChangeDirection.cpp
// Description	: CEventData_ChangeDirection implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "EventData_ChangeDirection.h"

// Local Includes

const TEventTypeID CEventData_ChangeDirection::s_kEVENT_TYPE_ID = 0xc5ac6663;


CEventData_ChangeDirection::CEventData_ChangeDirection()
	:m_actorID(CActor::s_kINVALID_ACTOR_ID)
{

}

CEventData_ChangeDirection::CEventData_ChangeDirection(TActorID _actorID, CVec3 _vec3Direction)
	:m_actorID(_actorID),
	m_vec3Direction(_vec3Direction)
{

}

CEventData_ChangeDirection::~CEventData_ChangeDirection()
{

}

const TActorID CEventData_ChangeDirection::GetActorID() const
{
	return m_actorID;
}

void CEventData_ChangeDirection::SetActorID(TActorID _actorID)
{
	m_actorID = _actorID;
}

const CVec3 CEventData_ChangeDirection::GetDirection() const
{
	return m_vec3Direction;
}

void CEventData_ChangeDirection::SetDirection(CVec3 _vec3Direction)
{
	m_vec3Direction = _vec3Direction;
}

void CEventData_ChangeDirection::VSerialise(std::ostringstream& outStream)
{
	outStream << m_actorID << " ";
	outStream << m_vec3Direction;
}

void CEventData_ChangeDirection::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_actorID;
	inStream >> m_vec3Direction;
}

TEventTypeID CEventData_ChangeDirection::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
