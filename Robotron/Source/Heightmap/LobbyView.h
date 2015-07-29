// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: LobbyView.h
// Description	: CLobbyView declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __LOBBYVIEW_H__
#define __LOBBYVIEW_H__

// Library Includes

// Local Includes

class CLobbyView : public CUserInterfaceView {
	// Member Functions
	struct TPlayerReady {	// Group players with a ready state.
		std::string m_strUsername;
		bool m_bIsReady;
		bool m_bIsOpen;	// True if the slot is open for a new player.

		TPlayerReady();
	};
	
public:
	CLobbyView(const std::string& _krstrUsername, int iMaxPlayers);
	~CLobbyView();

	bool Initialise();
	void GUIEventCallback(EGUIEvent _eEvent, AGUIElement* _pElement);
protected:
private:
	void PlayerReadyDelegate(TEventDataPtr _pEventData);
	void PlayerLeftDelegate(TEventDataPtr _pEventData);
	void ClientJoinDelegate(TEventDataPtr _pEventData);

	void UpdatePlayer(int _iPlayerIndex);

	// Member Variables
public:
protected:
private:
	std::vector<TPlayerReady> m_vecPlayers;
	std::string m_strUsername;
};

#endif	// __LOBBYVIEW_H__