// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_HostGame.h
// Description	: CEventData_HostGame declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_HOSTGAME_H__
#define __EVENTDATA_HOSTGAME_H__

// Library Includes

// Local Includes

class CEventData_HostGame : public IEventData {
	// Member Functions
public:
	CEventData_HostGame();
	CEventData_HostGame(const std::string& _krstrUsername, const std::string& _krstrServerName);

	~CEventData_HostGame();

	const std::string GetUsername() const;
	const std::string GetServerName() const;

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
	std::string m_strServerName;
	std::string m_strUsername;
};

#endif	// __EVENTDATA_HOSTGAME_H__