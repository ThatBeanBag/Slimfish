// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_NewRenderComponent.h
// Description	: CEventData_NewRenderComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_NEWRENDERCOMPONENT_H__
#define __EVENTDATA_NEWRENDERCOMPONENT_H__

// Library Includes

// Local Includes
#include "EventData.h"
#include "..\Actor\Actor.h"

// Forward Declaration
class CSceneNode;

class CEventData_NewRenderComponent : public IEventData {
	// Member Functions
public:
	CEventData_NewRenderComponent();
	CEventData_NewRenderComponent(TActorID _actorID, shared_ptr<CSceneNode> _pSceneNode);
	virtual ~CEventData_NewRenderComponent();

	const TActorID GetActorID() const { return m_actorID; }
	shared_ptr<CSceneNode> GetSceneNode() const { return m_pSceneNode; }

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
	shared_ptr<CSceneNode> m_pSceneNode;
};

#endif	// __EVENTDATA_NEWRENDERCOMPONENT_H__