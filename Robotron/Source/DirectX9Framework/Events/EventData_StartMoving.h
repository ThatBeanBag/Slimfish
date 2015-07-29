// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_StartMoving.h
// Description	: CEventData_StartMoving declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_STARTMOVING_H__
#define __EVENTDATA_STARTMOVING_H__

// Library Includes

// Local Includes
#include "EventData.h"

class CEventData_StartMoving : public IEventData {
	// Member Functions
public:
	CEventData_StartMoving();
	CEventData_StartMoving(TActorID _actorID, const CVec3& _krvec3Direction, float _fAcceleration, float _fMaxVelocity);
	virtual ~CEventData_StartMoving();

	TActorID GetActorID();
	const float GetAcceleration() const;
	const float GetMaxVelocity() const;
	const CVec3 GetDirection() const;

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
	float m_fAcceleration;
	float m_fMaxVelocity;
};

#endif	// __EVENTDATA_STARTMOVING_H__