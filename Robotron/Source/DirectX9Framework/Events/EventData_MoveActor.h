// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_MoveActor.h
// Description	: CEventData_MoveActor declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_MOVEACTOR_H__
#define __EVENTDATA_MOVEACTOR_H__

// Library Includes

// Local Includes
#include "EventData.h"

class CEventData_MoveActor : public IEventData {
	// Member Functions
public:
	CEventData_MoveActor();
	CEventData_MoveActor(TActorID _actorID, const CMatrix4x4& _krmatTransform);
	virtual ~CEventData_MoveActor();

	const TActorID GetActorID() const { return m_actorID; }
	const CMatrix4x4 GetTransform() const { return m_matTransform; }

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
	CMatrix4x4 m_matTransform;
};

#endif	// __EVENTDATA_MOVEACTOR_H__