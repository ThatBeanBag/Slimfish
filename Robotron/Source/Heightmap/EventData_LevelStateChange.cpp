// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_LevelStateChange.cpp
// Description	: CEventData_LevelStateChange implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "EventData_LevelStateChange.h"

// Local Includes

const TEventTypeID CEventData_LevelStateChange::s_kEVENT_TYPE_ID = 0x9854395f;

CEventData_LevelStateChange::CEventData_LevelStateChange()
{

}

CEventData_LevelStateChange::CEventData_LevelStateChange(int _iLevelsCompleted, int _iEnemiesRemaining)
	:m_iLevelsCompleted(_iLevelsCompleted),
	m_iEnemiesRemaining(_iEnemiesRemaining)
{

}

const int CEventData_LevelStateChange::GetLevelsCompleted() const
{
	return m_iLevelsCompleted;
}

const int CEventData_LevelStateChange::GetEnemiesRemaining() const
{
	return m_iEnemiesRemaining;
}

void CEventData_LevelStateChange::VSerialise(std::ostringstream& outStream)
{
	outStream << m_iLevelsCompleted << " "
			  << m_iEnemiesRemaining << " ";
}

void CEventData_LevelStateChange::VDeserialise(std::istringstream& inStream)
{
	inStream >> m_iLevelsCompleted >> m_iEnemiesRemaining;
}

TEventTypeID CEventData_LevelStateChange::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}

