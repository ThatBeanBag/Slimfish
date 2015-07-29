// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_Collision.h
// Description	: CEventData_Collision declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_COLLISION_H__
#define __EVENTDATA_COLLISION_H__

// Library Includes

// Local Includes
#include "EventData.h"

class CEventData_Collision : public IEventData {
	// Member Functions
public:
	CEventData_Collision();
	CEventData_Collision(TActorID _actorID1, TActorID _actorID2, 
						 const CVec3& _krvec3SumForceNormal, 
						 const CVec3& _krvec3SumFrictionForce, 
						 const std::vector<CVec3>& _krvecCollisionPoints);
	virtual ~CEventData_Collision();

	TActorID GetActorID1() const;
	TActorID GetActorID2() const;
	const CVec3 GetSumForceNormal() const;
	const CVec3 GetSumFrictionForce() const;
	const std::vector<CVec3> GetCollisionPoints() const;

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
	CVec3 m_vec3SumForceNormal;
	CVec3 m_vec3SumFrictionForce;
	std::vector<CVec3> m_vecCollisionPoints;
};

#endif	// __EVENTDATA_COLLISION_H__