// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_FireBullet.h
// Description	: CEventData_FireBullet declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_FIREBULLET_H__
#define __EVENTDATA_FIREBULLET_H__

// Library Includes

// Local Includes

class CEventData_FireBullet : public IEventData {
	// Member Functions
public:
	CEventData_FireBullet();
	CEventData_FireBullet(TActorID _actorID);

	~CEventData_FireBullet();

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

#endif	// __EVENTDATA_FIREBULLET_H__