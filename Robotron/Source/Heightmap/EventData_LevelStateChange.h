// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_LevelStateChange.h
// Description	: CEventData_LevelStateChange declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_LEVELSTATECHANGE_H__
#define __EVENTDATA_LEVELSTATECHANGE_H__

// Library Includes

// Local Includes

class CEventData_LevelStateChange : public IEventData {
	// Member Functions
public:
	CEventData_LevelStateChange();
	CEventData_LevelStateChange(int _iLevelsCompleted, int _iEnemiesRemaining);
	~CEventData_LevelStateChange() {} 

	const int GetLevelsCompleted() const;
	const int GetEnemiesRemaining() const;

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
	int m_iLevelsCompleted;
	int m_iEnemiesRemaining;
};

#endif	// __EVENTDATA_LEVELSTATECHANGE_H__