// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_GameLoaded.cpp
// Description	: CEventData_GameLoaded implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_LoadedGame.h"

// Local Includes

const TEventTypeID CEventData_LoadedGame::s_kEVENT_TYPE_ID = 0xd827cd03;

CEventData_LoadedGame::CEventData_LoadedGame()
{

}

CEventData_LoadedGame::~CEventData_LoadedGame()
{

}

void CEventData_LoadedGame::VSerialise(std::ostringstream& outStream)
{

}

void CEventData_LoadedGame::VDeserialise(std::istringstream& inStream)
{

}

TEventTypeID CEventData_LoadedGame::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
