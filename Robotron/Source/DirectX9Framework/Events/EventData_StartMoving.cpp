// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_StartMoving.cpp
// Description	: CEventData_StartMoving implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_StartMoving.h"

// Local Includes

const TEventTypeID CEventData_StartMoving::s_kEVENT_TYPE_ID = 0xc9cb3b54;

CEventData_StartMoving::CEventData_StartMoving()
	:m_actorID(CActor::s_kINVALID_ACTOR_ID)
{

}

CEventData_StartMoving::CEventData_StartMoving(TActorID _actorID, const CVec3& _krvec3Direction, float _fAcceleration, float _fMaxVelocity)
	:m_actorID(_actorID),
	m_vec3Direction(_krvec3Direction),
	m_fAcceleration(_fAcceleration),
	m_fMaxVelocity(_fMaxVelocity)
{

}

CEventData_StartMoving::~CEventData_StartMoving()
{

}

TActorID CEventData_StartMoving::GetActorID()
{
	return m_actorID;
}

const float CEventData_StartMoving::GetAcceleration() const
{
	return m_fAcceleration;
}

const float CEventData_StartMoving::GetMaxVelocity() const
{
	return m_fMaxVelocity;
}

const CVec3 CEventData_StartMoving::GetDirection() const
{
	return m_vec3Direction;
}

void CEventData_StartMoving::VSerialise(std::ostringstream& outStream)
{
	outStream << m_actorID << " ";
	outStream << m_vec3Direction << " ";
	outStream << m_fAcceleration << " ";
	outStream << m_fMaxVelocity << " ";
}

void CEventData_StartMoving::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_actorID;
	inStream >> m_vec3Direction;
	inStream >> m_fAcceleration;
	inStream >> m_fMaxVelocity;
}

TEventTypeID CEventData_StartMoving::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
