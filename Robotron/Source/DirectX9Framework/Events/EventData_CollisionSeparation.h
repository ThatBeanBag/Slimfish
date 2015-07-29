// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_CollisionSeparation.cpp
// Description	: CEventData_CollisionSeparation declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_COLLISIONSEPARATION_H__
#define __EVENTDATA_COLLISIONSEPARATION_H__

// Library Includes

// Local Includes
#include "EventData.h"

class CEventData_CollisionSeparation : public IEventData {
	// Member Functions
public:
	CEventData_CollisionSeparation();
	CEventData_CollisionSeparation(TActorID _actorID1, TActorID _actorID2);
	~CEventData_CollisionSeparation();

	TActorID GetActorID1() const;
	TActorID GetActorID2() const;

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
	TActorID m_actorID1;
	TActorID m_actorID2;
};

#endif	// __EVENTDATA_COLLISIONSEPARATION_H__