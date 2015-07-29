// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_PlayerStateChange.h
// Description	: CEventData_PlayerStateChange declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_PLAYERSTATECHANGE_H__
#define __EVENTDATA_PLAYERSTATECHANGE_H__

// Library Includes

// Local Includes

class CEventData_PlayerStateChange : public IEventData {
	// Member Functions
public:
	CEventData_PlayerStateChange();
	CEventData_PlayerStateChange(TActorID _actorID, int _iAmmo, int _iLives, bool _bHasGoldenPowerup, bool _bIsAlive);
	~CEventData_PlayerStateChange();

	const TActorID GetActorID() const;
	const int GetAmmo() const;
	const int GetLives() const;
	const bool HasGoldenPowerup() const;
	const bool GetIsAlive() const;

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
	TActorID m_actorID;
	int m_iAmmo;
	int m_iLives;
	bool m_bHasGoldenPowerup;
	bool m_bIsAlive;
};

#endif	// __EVENTDATA_PLAYERSTATECHANGE_H__