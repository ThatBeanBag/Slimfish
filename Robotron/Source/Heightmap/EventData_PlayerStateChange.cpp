// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_PlayerStateChange.cpp
// Description	: CEventData_PlayerStateChange implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "EventData_PlayerStateChange.h"


// Local Includes

const TEventTypeID CEventData_PlayerStateChange::s_kEVENT_TYPE_ID = 0xc172fb8c;

CEventData_PlayerStateChange::CEventData_PlayerStateChange()
	:m_iAmmo(0),
	m_iLives(0),
	m_bIsAlive(false),
	m_bHasGoldenPowerup(false)
{

}

CEventData_PlayerStateChange::CEventData_PlayerStateChange(TActorID _actorID, int _iAmmo, int _iLives, bool _bHasGoldenPowerup, bool _bIsAlive)
	:m_actorID(_actorID),
	m_iAmmo(_iAmmo),
	m_iLives(_iLives),
	m_bHasGoldenPowerup(_bHasGoldenPowerup),
	m_bIsAlive(_bIsAlive)
{

}

CEventData_PlayerStateChange::~CEventData_PlayerStateChange()
{

}

const TActorID CEventData_PlayerStateChange::GetActorID() const
{
	return m_actorID;
}

const int CEventData_PlayerStateChange::GetAmmo() const
{
	return m_iAmmo;
}

const int CEventData_PlayerStateChange::GetLives() const
{
	return m_iLives;
}

const bool CEventData_PlayerStateChange::HasGoldenPowerup() const
{
	return m_bHasGoldenPowerup;
}

const bool CEventData_PlayerStateChange::GetIsAlive() const
{
	return m_bIsAlive;
}

void CEventData_PlayerStateChange::VSerialise(std::ostringstream& outStream)
{
	outStream << m_iAmmo << " ";
	outStream << m_iLives << " ";
	outStream << m_bHasGoldenPowerup << " ";
}

void CEventData_PlayerStateChange::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_iAmmo;
	inStream >> m_iLives;
	inStream >> m_bHasGoldenPowerup;
}

TEventTypeID CEventData_PlayerStateChange::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
