// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_GameLoaded.h
// Description	: CEventData_GameLoaded declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_GAMELOADED_H__
#define __EVENTDATA_GAMELOADED_H__

// Library Includes

// Local Includes

class CEventData_LoadedGame : public IEventData {
	// Member Functions
public:
	CEventData_LoadedGame();
	~CEventData_LoadedGame();

	virtual void VSerialise(std::ostringstream& outStream) override;
	virtual void VDeserialise(std::istringstream& inStream) override;
	virtual TEventTypeID VGetEventTypeID() const override;
	
protected:
private:
	// Member Variables
public:
	static const TEventTypeID s_kEVENT_TYPE_ID;
protected:
private:
};

#endif	// __EVENTDATA_GAMELOADED_H__