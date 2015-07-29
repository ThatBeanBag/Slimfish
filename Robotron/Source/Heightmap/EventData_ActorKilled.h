// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_ActorKilled.h
// Description	: CEventData_ActorKilled declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_ACTORKILLED_H__
#define __EVENTDATA_ACTORKILLED_H__

// Library Includes

// Local Includes

class CEventData_ActorKilled : public IEventData {
	// Member Functions
public:
	CEventData_ActorKilled();
	CEventData_ActorKilled(TActorID _actor, const std::string& _krActorType);
	~CEventData_ActorKilled();

	const TActorID GetActorID() const;
	const std::string GetActorType() const;

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
	std::string m_strActorType;

};

#endif	// __EVENTDATA_ACTORKILLED_H__