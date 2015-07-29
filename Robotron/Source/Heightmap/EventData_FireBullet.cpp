// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_FireBullet.cpp
// Description	: CEventData_FireBullet implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "EventData_FireBullet.h"

// Local Includes

const TEventTypeID CEventData_FireBullet::s_kEVENT_TYPE_ID = 0x46ffae7d;

CEventData_FireBullet::CEventData_FireBullet(TActorID _actorID) 
	:m_actorID(_actorID)
{

}

CEventData_FireBullet::CEventData_FireBullet() 
	:m_actorID(CActor::s_kINVALID_ACTOR_ID)
{

}

CEventData_FireBullet::~CEventData_FireBullet()
{

}

const TActorID CEventData_FireBullet::GetActorID() const
{
	return m_actorID;
}

void CEventData_FireBullet::SetActorID(TActorID _actorID)
{
	m_actorID = _actorID;
}

void CEventData_FireBullet::VSerialise(std::ostringstream& outStream)
{
	outStream << m_actorID;
}

void CEventData_FireBullet::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_actorID;
}

TEventTypeID CEventData_FireBullet::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

