// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData_NewRenderComponent.cpp
// Description	: CEventData_NewRenderComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "EventData_NewRenderComponent.h"

// Local Includes

const TEventTypeID CEventData_NewRenderComponent::s_kEVENT_TYPE_ID = 0x7c27f069;

CEventData_NewRenderComponent::CEventData_NewRenderComponent()
	:m_actorID(CActor::s_kINVALID_ACTOR_ID),
	m_pSceneNode(nullptr)
{

}

CEventData_NewRenderComponent::CEventData_NewRenderComponent(TActorID _actorID, shared_ptr<CSceneNode> _pSceneNode)
	:m_actorID(_actorID),
	m_pSceneNode(_pSceneNode)
{

}

CEventData_NewRenderComponent::~CEventData_NewRenderComponent()
{

}

void CEventData_NewRenderComponent::VSerialise(std::ostringstream& outStream)
{
	DEBUG_ERROR("CEventData_NewRenderComponent should not be serialised.");
}

void CEventData_NewRenderComponent::VDeserialise(std::istringstream& inStream)
{
	DEBUG_ERROR("CEventData_NewRenderComponent should not be serialised.");
}

TEventTypeID CEventData_NewRenderComponent::VGetEventTypeID() const
{
	return s_kEVENT_TYPE_ID;
}
