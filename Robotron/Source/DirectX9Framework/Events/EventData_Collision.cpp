// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_Collision.cpp
// Description	: CEventData_Collision implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_Collision.h"

// Local Includes

const TEventTypeID CEventData_Collision::s_kEVENT_TYPE_ID = 0x7344c344;

CEventData_Collision::CEventData_Collision()
	:m_actorID1(CActor::s_kINVALID_ACTOR_ID),
	m_actorID2(CActor::s_kINVALID_ACTOR_ID)
{

}

CEventData_Collision::CEventData_Collision(TActorID _actorID1, 
										   TActorID _actorID2, 
										   const CVec3& _krvec3SumForceNormal, 
										   const CVec3& _krvec3SumFrictionForce, 
										   const std::vector<CVec3>& _krvecCollisionPoints)
	:m_actorID1(_actorID1),
	m_actorID2(_actorID2),
	m_vec3SumForceNormal(_krvec3SumForceNormal),
	m_vec3SumFrictionForce(_krvec3SumFrictionForce),
	m_vecCollisionPoints(_krvecCollisionPoints)
{

}


CEventData_Collision::~CEventData_Collision()
{

}

TActorID CEventData_Collision::GetActorID1() const
{
	return m_actorID1;
}

TActorID CEventData_Collision::GetActorID2() const
{
	return m_actorID2;
}

const CVec3 CEventData_Collision::GetSumForceNormal() const
{
	return m_vec3SumForceNormal;
}

const CVec3 CEventData_Collision::GetSumFrictionForce() const
{
	return m_vec3SumFrictionForce;
}

const std::vector<CVec3> CEventData_Collision::GetCollisionPoints() const
{
	return m_vecCollisionPoints;
}

void CEventData_Collision::VSerialise(std::ostringstream& outStream)
{
	DEBUG_ERROR("CEventData_Collision should not be serialised.");
}

void CEventData_Collision::VDeserialise(std::istringstream& inStream)
{
	DEBUG_ERROR("CEventData_Collision should not be serialised.");
}

TEventTypeID CEventData_Collision::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

