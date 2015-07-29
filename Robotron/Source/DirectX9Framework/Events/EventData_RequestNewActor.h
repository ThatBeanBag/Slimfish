// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_RequestNewActor.h
// Description	: CEventData_RequestNewActor declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_REQUESTNEWACTOR_H__
#define __EVENTDATA_REQUESTNEWACTOR_H__

// Library Includes

// Local Includes

/************************************************************************/
/* CEventData_RequestNewActor
/*
/* Sent to clients from the server when an actor is to be created from 
/* a resource. 
/************************************************************************/
class CEventData_RequestNewActor : public IEventData {
	// Member Functions
public:
	CEventData_RequestNewActor();
	CEventData_RequestNewActor(const std::string& _krActorResource, const CMatrix4x4* _pInitialTransform, const TActorID _actorID);
	~CEventData_RequestNewActor();

	const std::string GetActorResource() const;
	bool HasInitialTransform() const;
	CMatrix4x4 GetInitialTransform() const;
	TActorID GetServerActorID() const;

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
	std::string m_strActorResource;
	bool m_bHasInitialTransform;
	bool m_bIsPlayerControlled;
	CMatrix4x4 m_matTransform;
	TActorID m_actorID;
};

#endif	// __EVENTDATA_REQUESTNEWACTOR_H__