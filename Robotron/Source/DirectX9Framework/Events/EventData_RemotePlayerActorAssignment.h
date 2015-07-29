// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_RemotePlayerActorAssignment.h
// Description	: CEventData_RemotePlayerActorAssignment declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_REMOTEPLAYERACTORASSIGNMENT_H__
#define __EVENTDATA_REMOTEPLAYERACTORASSIGNMENT_H__

// Library Includes

// Local Includes
#include "EventData.h"

/************************************************************************/
/* CEventData_RemotePlayerActorAssignment
/*
/* Sent from a server to a remote client to inform it of what actor it
/* controls. Clients are responsible for setting the controlled actor of
/* their CHumanView to the actor of the assignment.
/************************************************************************/
class CEventData_RemotePlayerActorAssignment : public IEventData {
	// Member Functions
public:
	CEventData_RemotePlayerActorAssignment(TActorID _actorID = CActor::s_kINVALID_ACTOR_ID);
	~CEventData_RemotePlayerActorAssignment();

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

#endif	// __EVENTDATA_REMOTEPLAYERACTORASSIGNMENT_H__