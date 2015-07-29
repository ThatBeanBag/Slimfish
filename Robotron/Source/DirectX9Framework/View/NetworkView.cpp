// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: NetworkView.cpp
// Description	: CNetworkView implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "NetworkView.h"

// Local Includes
#include "..\Events\EventData_RemotePlayerActorAssignment.h"


CNetworkView::CNetworkView(unique_ptr<CNetworkEventForwarder> pEventForwarder, const std::string& _krstrUsername)
	:m_pEventForwarder(std::move(pEventForwarder)),
	m_strUserName(_krstrUsername)
{
	CEventManager::GetInstance()->AddListener<CEventData_PlayerLeft>(MakeDelegate(this, &CNetworkView::PlayerLeftDelegate));
}

CNetworkView::~CNetworkView()
{
	CEventManager::GetInstance()->RemoveListener<CEventData_PlayerLeft>(MakeDelegate(this, &CNetworkView::PlayerLeftDelegate));
}

bool CNetworkView::VOnRestore()
{
	return true;
}

void CNetworkView::VOnUpdate(float m_fDeltaTime)
{

}

void CNetworkView::VRender()
{

}

bool CNetworkView::VOnMsgProc(const TAppMsg& _krMsg)
{
	return false;
}

bool CNetworkView::VLoadGame(const CIniDocument& _krIniDocument)
{
	return true;
}

void CNetworkView::VSetControlledActor(TActorID _actorID)
{
	m_actorID = _actorID;

	shared_ptr<CEventData_RemotePlayerActorAssignment> pAssignmentData(new CEventData_RemotePlayerActorAssignment(_actorID));

	// Only forward this event through our event forwarder instead of queuing it for the event manager to fire off.
	m_pEventForwarder->ForwardEventDelegate(pAssignmentData);
}

TAddress CNetworkView::GetAddress() const
{
	TAddress address;
	return address;
}

std::string CNetworkView::GetUsername() const
{
	return m_strUserName;
}

void CNetworkView::PlayerLeftDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_PlayerLeft> pPlayerLeftData = static_pointer_cast<CEventData_PlayerLeft>(_pEventData);

	if (pPlayerLeftData->GetUsername() == m_strUserName) {
	// Has the player associated with this view left?
		// Time to remove ourselves.
		// Destroy the actor we are assigned to first.
		g_pApp->GetGame()->DestroyActor(m_actorID);

		// Destroy ourselves.
		g_pApp->GetGame()->RemoveGameView(this);
	}
}
