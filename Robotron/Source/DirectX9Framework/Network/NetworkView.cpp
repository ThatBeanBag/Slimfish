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


CNetworkView::CNetworkView(unique_ptr<CNetworkEventForwarder> pEventForwarder, const std::string& _krstrUsername)
{

}

CNetworkView::~CNetworkView()
{

}

void CNetworkView::VOnUpdate(float m_fDeltaTime)
{

}

void CNetworkView::VSetControlledActor(TActorID _actorID)
{

}

TAddress CNetworkView::GetAddress() const
{
	TAddress address;
	return TAddress;
}

std::string CNetworkView::GetUsername() const
{

}
