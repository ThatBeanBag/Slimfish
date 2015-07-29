// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_StopMoving.h
// Description	: CEventData_StopMoving declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_STOPMOVING_H__
#define __EVENTDATA_STOPMOVING_H__

// Library Includes

// Local Includes
#include "EventData.h"

class CEventData_StopMoving : public IEventData {
	// Member Functions
public:
	CEventData_StopMoving();
	CEventData_StopMoving(TActorID _actorID);
	virtual ~CEventData_StopMoving();

	TActorID GetActorID() const;

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
};

#endif	// __EVENTDATA_STOPMOVING_H__