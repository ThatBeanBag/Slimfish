// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_ChangeDirection.h
// Description	: CEventData_ChangeDirection declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_CHANGEDIRECTION_H__
#define __EVENTDATA_CHANGEDIRECTION_H__

// Library Includes

// Local Includes

/************************************************************************/
/* CEventData_ChangeDirection
/*
/* Sent from a client when they've changed their characters looking 
/* position.
/************************************************************************/
class CEventData_ChangeDirection : public IEventData {
	// Member Functions
public:
	CEventData_ChangeDirection();
	CEventData_ChangeDirection(TActorID _actorID, CVec3 _vec3Direction);
	~CEventData_ChangeDirection();

	const TActorID GetActorID() const;
	void SetActorID(TActorID _actorID);
	const CVec3 GetDirection() const;
	void SetDirection(CVec3 _vec3Direction);

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
	CVec3 m_vec3Direction;
};

#endif	// __EVENTDATA_CHANGEDIRECTION_H__
