// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_RequestNewActor.cpp
// Description	: CEventData_RequestNewActor implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_RequestNewActor.h"

// Local Includes

const TEventTypeID CEventData_RequestNewActor::s_kEVENT_TYPE_ID = 0xe2ad29be;

CEventData_RequestNewActor::CEventData_RequestNewActor()
	:m_actorID(CActor::s_kINVALID_ACTOR_ID),
	m_bHasInitialTransform(false),
	m_bIsPlayerControlled(false)
{

}

CEventData_RequestNewActor::CEventData_RequestNewActor(const std::string& _krActorResource, const CMatrix4x4* _pInitialTransform, const TActorID _actorID)
	:m_strActorResource(_krActorResource),
	m_actorID(_actorID)
{
	if (_pInitialTransform) {
	// Is there an initial transform?
		m_matTransform = (*_pInitialTransform);
		m_bHasInitialTransform = true;
	}
	else {
	// No initial transform.
		m_matTransform = CMatrix4x4::s_kIdentity;
		m_bHasInitialTransform = false;
	}
}


CEventData_RequestNewActor::~CEventData_RequestNewActor()
{

}

const std::string CEventData_RequestNewActor::GetActorResource() const
{
	return m_strActorResource;
}

bool CEventData_RequestNewActor::HasInitialTransform() const
{
	return m_bHasInitialTransform;
}

CMatrix4x4 CEventData_RequestNewActor::GetInitialTransform() const
{
	return m_matTransform;
}

TActorID CEventData_RequestNewActor::GetServerActorID() const
{
	return m_actorID;
}

void CEventData_RequestNewActor::VSerialise(std::ostringstream& outStream)
{
	outStream << m_strActorResource << " ";
	outStream << m_bHasInitialTransform << " ";
	if (m_bHasInitialTransform) {
		outStream << m_matTransform << " ";
	}
	outStream << m_actorID << " ";
	outStream << m_bIsPlayerControlled << " ";
}

void CEventData_RequestNewActor::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_strActorResource;
	inStream >> m_bHasInitialTransform;
	if (m_bHasInitialTransform) {
		inStream >> m_matTransform;
	}
	inStream >> m_actorID;
	inStream >> m_bIsPlayerControlled;
}

TEventTypeID CEventData_RequestNewActor::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

