// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_RequestDestroyActor.h
// Description	: CEventData_RequestDestroyActor declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_REQUESTDESTROYACTOR_H__
#define __EVENTDATA_REQUESTDESTROYACTOR_H__

// Library Includes

// Local Includes

/************************************************************************/
/* CEventData_RequestDestroyActor
/*
/* Sent by the server to request clients to destroy an actor.
/************************************************************************/
class CEventData_RequestDestroyActor : public IEventData {
	// Member Functions
public:
	CEventData_RequestDestroyActor(TActorID actorID = CActor::s_kINVALID_ACTOR_ID);
	~CEventData_RequestDestroyActor();

	const TActorID GetActorID() const;
	void SetActorID(TActorID _actorID);

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

#endif	// __EVENTDATA_REQUESTDESTROYACTOR_H__