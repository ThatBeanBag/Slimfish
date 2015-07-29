// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: NetworkView.h
// Description	: CNetworkView declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __NETWORKVIEW_H__
#define __NETWORKVIEW_H__

// Library Includes

// Local Includes
#include "GameView.h"

/************************************************************************/
/* CNetworkView
/*
/* For every remote player a network view is assigned to them. This view
/* acts as a passageway between the server and the client.
/*
/* At the very basic level it groups an event forwarder with an actorID.
/************************************************************************/
class CNetworkView : public IGameView {
	// Member Functions
public:
	CNetworkView(unique_ptr<CNetworkEventForwarder> pEventForwarder, const std::string& _krstrUsername);
	~CNetworkView();

	virtual bool VOnRestore() override;
	virtual void VOnUpdate(float m_fDeltaTime) override;
	virtual void VRender() override;
	virtual bool VOnMsgProc(const TAppMsg& _krMsg) override;
	virtual bool VLoadGame(const CIniDocument& _krIniDocument) override;
	virtual void VSetControlledActor(TActorID _actorID);

	TAddress GetAddress() const;
	std::string GetUsername() const;
protected:
private:
	void PlayerLeftDelegate(TEventDataPtr _pEventData);

	// Member Variables
public:
protected:
private:
	unique_ptr<CNetworkEventForwarder> m_pEventForwarder;
	TActorID m_actorID;
	std::string m_strUserName;
};

#endif	// __NETWORKVIEW_H__