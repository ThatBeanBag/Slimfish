// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: LobbyView.cpp
// Description	: CLobbyView implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "LobbyView.h"

// Local Includes

CLobbyView::TPlayerReady::TPlayerReady()
	:m_strUsername(""),
	m_bIsOpen(true),
	m_bIsReady(false)
{

}

CLobbyView::CLobbyView(const std::string& _krstrUsername, int iMaxPlayers)
	:m_vecPlayers(iMaxPlayers),
	m_strUsername(_krstrUsername)
{
	CEventManager::GetInstance()->AddListener<CEventData_PlayerReady>(MakeDelegate(this, &CLobbyView::PlayerReadyDelegate));
	CEventManager::GetInstance()->AddListener<CEventData_PlayerLeft>(MakeDelegate(this, &CLobbyView::PlayerLeftDelegate));
	CEventManager::GetInstance()->AddListener<CEventData_ClientJoin>(MakeDelegate(this, &CLobbyView::ClientJoinDelegate));
}

CLobbyView::~CLobbyView()
{
	CEventManager::GetInstance()->RemoveListener<CEventData_PlayerReady>(MakeDelegate(this, &CLobbyView::PlayerReadyDelegate));
	CEventManager::GetInstance()->RemoveListener<CEventData_PlayerLeft>(MakeDelegate(this, &CLobbyView::PlayerLeftDelegate));
	CEventManager::GetInstance()->RemoveListener<CEventData_ClientJoin>(MakeDelegate(this, &CLobbyView::ClientJoinDelegate));
}

bool CLobbyView::Initialise()
{
	auto callback = [this](EGUIEvent _eEvent, AGUIElement* _pElement) {
		GUIEventCallback(_eEvent, _pElement);
	};

	unique_ptr<CBaseGUI> pGUI(new CBaseGUI(callback));
	if (!pGUI->CreateGUIFromXML("UserInterfaces/LobbyMenu.xml")){
		return false;
	}

	if (g_pApp->GetGame()->IsRemote()) {
		// Only the host can start the game.
		pGUI->GetButton("StartGameButton")->SetVisibility(false);
	}

	for (unsigned int i = 0; i < m_vecPlayers.size(); ++i) {
		// Create the GUI elements for the players.
		unique_ptr<CStaticText> pStaticText(new CStaticText(*pGUI->GetStaticText("UsernameTemplate")));
		pStaticText->SetName(ToString(i) + "Username");
		pStaticText->SetText(std::string("-"));
		pStaticText->SetVisibility(false);

		// Add username slot to the menu.
		pGUI->GetMenu("UsernameMenu")->AddGUIElement(pStaticText.get());
		pGUI->AddStaticText(std::move(pStaticText));

		unique_ptr<CStaticImage> pStaticImage(new CStaticImage(*pGUI->GetStaticImage("NotReadyTemplate")));
		pStaticImage->SetName(ToString(i) + "ReadyIcon");
		pStaticImage->SetVisibility(false);

		// Add ready to the menu.
		pGUI->GetMenu("ReadyMenu")->AddGUIElement(pStaticImage.get());
		pGUI->AddStaticImage(std::move(pStaticImage));

	}

	PushGUI(std::move(pGUI));

	if (!g_pApp->GetGame()->IsRemote() && !m_vecPlayers.empty()) {
		m_vecPlayers[0].m_strUsername = m_strUsername;
		m_vecPlayers[0].m_bIsReady = false;
		m_vecPlayers[0].m_bIsOpen = false;
		UpdatePlayer(0);
	}


	return true;
}

void CLobbyView::GUIEventCallback(EGUIEvent _eEvent, AGUIElement* _pElement)
{
	if (_pElement) {
		std::string strElementName = _pElement->GetName();

		if (_eEvent == GUIEVENT_BUTTON_RELEASED) {
			if (strElementName == "ReadyButton") {
				bool bIsReady = GetTopGUI()->GetButton(strElementName)->GetState() == CButton::STATE_PRESSED;

				// Send an event to tell other players that we are ready.
				shared_ptr<CEventData_RequestPlayerReady> pEvent(new CEventData_RequestPlayerReady(m_strUsername, bIsReady));
				CEventManager::GetInstance()->QueueEvent(pEvent);
			}
			else if (strElementName == "StartGameButton") {
				shared_ptr<CEventData_RequestStartGame> pEvent(new CEventData_RequestStartGame(false));
				CEventManager::GetInstance()->QueueEvent(pEvent);
			}
			else if (strElementName == "CancelButton") {
				// Tell the server that we left.
				shared_ptr<CEventData_PlayerLeft> pEvent(new CEventData_PlayerLeft(m_strUsername));
				CEventManager::GetInstance()->QueueEvent(pEvent);

				// Go back to the main menu.
				g_pApp->GetGame()->ChangeState(CGameLogic::GS_MAINMENU);
			}
		}
	}
}

void CLobbyView::PlayerReadyDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_PlayerReady> pPlayerReadyData = static_pointer_cast<CEventData_PlayerReady>(_pEventData);

	std::string strUsername = pPlayerReadyData->GetUsername();
	bool bIsReady = pPlayerReadyData->IsReady();

	for (unsigned int i = 0; i < m_vecPlayers.size(); ++i) {
		if (m_vecPlayers[i].m_strUsername == strUsername && !m_vecPlayers[i].m_bIsOpen) {
		// Is this the player that has readied up and hasn't left?
			if (m_vecPlayers[i].m_bIsReady != bIsReady) {
				m_vecPlayers[i].m_bIsReady = bIsReady;
				UpdatePlayer(i);
			}

			return;
		}
	}

	// If we get here, we've joined the server and are receiving the states of existing players.

	// Create a new player.
	TPlayerReady player;
	player.m_strUsername = strUsername;
	player.m_bIsReady = bIsReady;
	player.m_bIsOpen = false;

	// Find an open slot.
	for (unsigned int i = 0; i < m_vecPlayers.size(); ++i) {
		if (m_vecPlayers[i].m_bIsOpen) {
			m_vecPlayers[i] = player;
			UpdatePlayer(i);
			break;
		}
	}
}

void CLobbyView::PlayerLeftDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_PlayerLeft> pPlayerReadyData = static_pointer_cast<CEventData_PlayerLeft>(_pEventData);

	std::string strUsername = pPlayerReadyData->GetUsername();

	for (unsigned int i = 0; i < m_vecPlayers.size(); ++i) {
		if (m_vecPlayers[i].m_strUsername == strUsername && !m_vecPlayers[i].m_bIsOpen) {
		// Is this the player that has left?
			m_vecPlayers[i].m_bIsOpen = true;
			UpdatePlayer(i);

			return;
		}
	}
}

void CLobbyView::ClientJoinDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_ClientJoin> pClientJoinData = static_pointer_cast<CEventData_ClientJoin>(_pEventData);

	/*// Create a new player.
	TPlayerReady player;
	player.m_strUsername = pClientJoinData->GetUsername();
	player.m_bIsReady = false;
	player.m_bIsOpen = false;

	// Find an open slot.
	for (unsigned int i = 0; i < m_vecPlayers.size(); ++i) {
		if (m_vecPlayers[i].m_bIsOpen) {
			m_vecPlayers[i] = player;
			UpdatePlayer(i);
		}
	}*/

	DEBUG_ASSERT(!g_pApp->GetGame()->IsRemote());

	for (unsigned int i = 0; i < m_vecPlayers.size(); ++i) {
		if (!m_vecPlayers[i].m_bIsOpen) {
			shared_ptr<CEventData_PlayerReady> pPlayerReadyData(new CEventData_PlayerReady(m_vecPlayers[i].m_strUsername, m_vecPlayers[i].m_bIsReady));
			CEventManager::GetInstance()->QueueEvent(pPlayerReadyData);
		}
	}

	// Only servers receive this event, tell the clients about it by sending a player ready event.
	shared_ptr<CEventData_PlayerReady> pPlayerReadyData(new CEventData_PlayerReady(pClientJoinData->GetUsername(), false));
	CEventManager::GetInstance()->QueueEvent(pPlayerReadyData);

}

void CLobbyView::UpdatePlayer(int _iPlayerIndex)
{
	DEBUG_ASSERT(_iPlayerIndex >= 0 && _iPlayerIndex < static_cast<int>(m_vecPlayers.size()));

	TPlayerReady player = m_vecPlayers[_iPlayerIndex];

	CBaseGUI* pGUI = GetTopGUI();
	CStaticText* pUsernameText = pGUI->GetStaticText(ToString(_iPlayerIndex) + "Username");
	CStaticImage* pReadyImage = pGUI->GetStaticImage(ToString(_iPlayerIndex) + "ReadyIcon");

	DEBUG_ASSERT(pUsernameText&& pReadyImage);

	pUsernameText->SetVisibility(!player.m_bIsOpen);
	pReadyImage->SetVisibility(!player.m_bIsOpen);

	// Set the text to the username.
	pUsernameText->SetText(player.m_strUsername);

	// Set the ready sprite.
	shared_ptr<ISprite> pSprite;
	if (player.m_bIsReady) {
		pSprite = GetTopGUI()->GetStaticImage("ReadyTemplate")->GetImage();
	}
	else {
		pSprite = GetTopGUI()->GetStaticImage("NotReadyTemplate")->GetImage();
	}

	if (pSprite) {
		pReadyImage->SetImage(pSprite);
	}

	if (!g_pApp->GetGame()->IsRemote()) {
		bool bIsEveryoneReady = true;

		for (unsigned int i = 0; i < m_vecPlayers.size(); ++i) {
			if (!m_vecPlayers[i].m_bIsOpen && 
				!m_vecPlayers[i].m_bIsReady && 
				m_strUsername != m_vecPlayers[i].m_strUsername) {
			// Is a player not ready and are they not us.
				bIsEveryoneReady = false;
			}
		}

		if (bIsEveryoneReady) {
			GetTopGUI()->GetButton("StartGameButton")->SetActive(true);
		}
		else {
			GetTopGUI()->GetButton("StartGameButton")->SetActive(false);
		}
	}
}
