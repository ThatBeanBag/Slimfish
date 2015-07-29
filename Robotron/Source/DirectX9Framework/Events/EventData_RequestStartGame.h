// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_RequestStartGame.h
// Description	: CEventData_RequestStartGame declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_REQUESTSTARTGAME_H__
#define __EVENTDATA_REQUESTSTARTGAME_H__

// Library Includes

// Local Includes
#include "EventData.h"

class CEventData_RequestStartGame : public IEventData {
	// Member Functions
public:
	CEventData_RequestStartGame(bool _bIsSinglePlayer = true, 
								bool _bIsRemote = false, 
								std::string& _krstrHostName = std::string(), 
								unsigned short _usPort = 0);
	~CEventData_RequestStartGame();

	bool IsSinglePlayer() const { return m_bIsSinglePlayer; }
	bool IsRemote() const;
	std::string GetHostName() const;
	std::string GetUsername() const { return m_strUsername; }
	unsigned short GetPort() const;

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
	bool m_bIsRemote;
	bool m_bIsSinglePlayer;
	std::string m_strUsername;
	std::string m_strHostName;
	unsigned short m_usPort;
};

#endif	// __EVENTDATA_REQUESTSTARTGAME_H__