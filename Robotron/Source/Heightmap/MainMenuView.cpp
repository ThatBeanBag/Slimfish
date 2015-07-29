// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MainMenuView.cpp
// Description	: CMainMenuView implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "MainMenuView.h"

// Local Includes
#include "..\DirectX9Framework\Resource\XmlResource.h"
#include "..\DirectX9Framework\UserInterface\GUIElement.h"
#include "..\DirectX9Framework\Network\ClientNetwork.h"
#include "..\DirectX9Framework\UserInterface\StaticText.h"
#include "..\DirectX9Framework\UserInterface\EditBox.h"
#include "..\DirectX9Framework\UserInterface\Menu.h"


CMainMenuView::CMainMenuView()
{
	g_pApp->GetRenderer()->VSetBackgroundColour(g_kCOLOUR_BLACK);

	auto callback = [this](EGUIEvent _eGUIEvent, AGUIElement* _pGUICallBack) { 
		MainGUIEventCallback(_eGUIEvent, _pGUICallBack); 
	};

	unique_ptr<CBaseGUI> gui(new CBaseGUI(callback));

	CResourceManager<CXmlResource>::GetInstance()->Reload("UserInterfaces/MainMenu.xml");
	gui->CreateGUIFromXML("UserInterfaces/MainMenu.xml");
	PushGUI(std::move(gui));

	/*m_pCamera->SetProjectionTransform(ToRadians(45.0f),
									  static_cast<float>(g_pApp->GetScreenWidth()),
									  static_cast<float>(g_pApp->GetScreenHeight()),
									  0.001f,
									  1000.0f,
									  PROJECTION_PERSPECTIVE);
	TActorID actorID = g_pApp->GetGame()->CreateActor("Actors/Player.xml");
	//g_pApp->GetGame()->CreateActor("Actors/Enemy.xml", CMatrix4x4::s_);
	g_pApp->GetGame()->CreateActor("Actors/SpotLight.xml");
	m_pCamera->SetTarget(m_pScene->FindSceneNodeFromActorID(actorID));
	m_pCamera->SetOffsetFromTarget(0.0f, 1.0f, -5.0f);
	m_pCamera->SetRotationFromTarget(ToRadians(20.0f), ToRadians(0.0f), 0.0f);
	m_pCamera->SetAbsoluteRotation(true);*/
}

CMainMenuView::~CMainMenuView()
{

}

bool CMainMenuView::VOnRestore()
{
	return CUserInterfaceView::VOnRestore();
}

void CMainMenuView::VOnUpdate(float _fDeltaTime)
{
	CUserInterfaceView::VOnUpdate(_fDeltaTime);
}

void CMainMenuView::VRender()
{
	CUserInterfaceView::VRender();
}

bool CMainMenuView::VOnMsgProc(const TAppMsg& _krMsg)
{
	switch (_krMsg.m_eMessageType) {
		case MT_KEYDOWN: {
			break;
		}
		case MT_KEYUP: {
			if (_krMsg.m_uckey == VK_F3) {
				g_pApp->GetGame()->LoadGame();
			}
			if (_krMsg.m_uckey == VK_SNAPSHOT) {
				g_pApp->GetRenderer()->VDumpBackBufferToDisk("Screenshot.jpg");
			}
			if (_krMsg.m_uckey == VK_ESCAPE) {
				if (!g_pApp->IsWindowed()) {
					// Are we in full screen mode?
					// Exit out of full screen.
					g_pApp->ToggleWindowedMode();
				}

				if (IDYES == MessageBox(g_pApp->GetHwnd(), L"Are you sure you want to quit?", L"Quit", MB_YESNO)) {
					g_pApp->Quit();
				}
			}
			else if (_krMsg.m_uckey == VK_F1) {
				g_pApp->ToggleWindowedMode();
			}

			break;
		}
		case MT_MOUSE_MOVE: {
			break;
		}
		case MT_MOUSE_WHEEL: {
			break;
		}
		case MT_LMB_DOWN: {
			break;
		}
		case MT_LMB_UP:	 {
			break;
		}
		case MT_MMB_DOWN: {
			break;
		}
		case MT_MMB_UP:	 {
			break;
		}
		case MT_RMB_DOWN: {
			break;
		}
		case MT_RMB_UP: {
			break;
		}

		default: break;
	}

	return CUserInterfaceView::VOnMsgProc(_krMsg);
}

bool CMainMenuView::VLoadGame(const CIniDocument& _krIniDocument)
{
	return true;
}

void CMainMenuView::MainGUIEventCallback(EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement)
{
	auto mainCallback = [this](EGUIEvent _eGUIEvent, AGUIElement* _pGUICallBack) { 
		MainGUIEventCallback(_eGUIEvent, _pGUICallBack); 
	};

	if (_pGUIElement) {
		std::string strName = _pGUIElement->GetName();

		if (_eGUIEvent == GUIEVENT_BUTTON_RELEASED) {
			if (strName == "PlayButton") {
				// Start the game.
				shared_ptr<CEventData_RequestStartGame> pEvent(new CEventData_RequestStartGame(true));
				CEventManager::GetInstance()->QueueEvent(pEvent);
			}
			else if (strName == "HostGameButton") {
				auto callback = [this](EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement) { 
					HostGameGUIEventCallback(_eGUIEvent, _pGUIElement); 
				};

				// Create GUI.
				unique_ptr<CBaseGUI> gui(new CBaseGUI(callback, true));
				gui->CreateGUIFromXML("UserInterfaces/HostGame.xml");

				PushGUI(std::move(gui));
			}
			else if (strName == "JoinGameButton") {
				// Create a client network.
				m_pNetwork = g_pApp->CreateClientNetwork();

				if (m_pNetwork) {
				// Did the network create successfully?
					// Detect servers by broadcasting.
					m_pNetwork->BroadcastToDetectServers(m_serverList);

					auto callback = [this](EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement) { 
						ServerListGUIEventCallback(_eGUIEvent, _pGUIElement); 
					};

					// Create a new user interface.
					unique_ptr<CBaseGUI> gui(new CBaseGUI(callback, true , true));
					gui->CreateGUIFromXML("UserInterfaces/ServerList.xml");

					CRect serverListRect = gui->GetStaticText("ServerNameText")->GetBoundingBox();

					if (m_serverList.empty()) {
						unique_ptr<CStaticText> pText(new CStaticText("NoServersFoundStatic", serverListRect, "No servers found.", 18, g_kCOLOUR_WHITE));
						gui->GetMenu("CurrentPlayersMenu")->AddGUIElement(pText.get());
						gui->AddStaticText(std::move(pText));

						gui->GetButton("JoinServerButton")->SetVisibility(false);
					}
					else {
						gui->GetButton("JoinServerButton")->SetActive(false);

						// Add the servers to the GUI.
						for (unsigned int i = 0; i < m_serverList.size(); ++i) {
							// Add a button for the user to click on to select a server.
							unique_ptr<CButton> pButton(new CButton(*gui->GetButton("ServerListButtonTemplate")));
							pButton->SetName(ToString(i) + "ServerListButton");
							gui->GetMenu("ButtonMenu")->AddGUIElement(pButton.get());
							gui->AddButton(std::move(pButton));

							// Create the server name static text and add it to the menu.
							unique_ptr<CStaticText> pServerNameText(new CStaticText(ToString(i) + "ServerName", serverListRect, m_serverList[i].m_strServerName, 18, g_kCOLOUR_WHITE));
							gui->GetMenu("ServerNameMenu")->AddGUIElement(pServerNameText.get());
							gui->AddStaticText(std::move(pServerNameText));

							// Create the number of player static text and add it to the menu.
							std::string strPlayers = ToString(m_serverList[i].m_iNumPlayers) + '/' + ToString(m_serverList[i].m_iMaxPlayers);
							unique_ptr<CStaticText> pPlayersText(new CStaticText(ToString(i) + "Players", serverListRect, strPlayers, 18, g_kCOLOUR_WHITE));
							gui->GetMenu("CurrentPlayersMenu")->AddGUIElement(pPlayersText.get());
							gui->AddStaticText(std::move(pPlayersText));

							// Create the status static text and add it to the menu.
							std::string strStatus = m_serverList[i].m_bIsWaiting ? "WAITING FOR PLAYERS" : "PLAYING";
							unique_ptr<CStaticText> pStatusText(new CStaticText(ToString(i) + "Status", serverListRect, strStatus, 18, g_kCOLOUR_WHITE));
							gui->GetMenu("StatusMenu")->AddGUIElement(pStatusText.get());
							gui->AddStaticText(std::move(pStatusText));
						}
					}

					// Disable the button template.
					gui->GetButton("ServerListButtonTemplate")->SetVisibility(false);

					PushGUI(std::move(gui));
				}
			}
			else if (strName == "ControlsButton") {
				// Create GUI.
				unique_ptr<CBaseGUI> gui(new CBaseGUI(mainCallback, true, true));
				gui->CreateGUIFromXML("UserInterfaces/Controls.xml");

				PushGUI(std::move(gui));
			}
			else if (strName == "QuitButton") {
				g_pApp->Quit();
			}
			else if (strName == "BackButton") {
				RequestPopGUI();
			}
		}
	}
}

void CMainMenuView::HostGameGUIEventCallback(EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement)
{
	if (_pGUIElement) {
		std::string strName = _pGUIElement->GetName();

		if (_eGUIEvent == GUIEVENT_BUTTON_PRESSED) {
			if (strName == "BackButton") {
				// Close the network we've created.
				g_pApp->CloseNetwork();

				RequestPopGUI();
			}
			else if (strName == "OkButton") {
				CEditBox* pEditBox = GetTopGUI()->GetEditBox("ServerNameEditBox");
				DEBUG_ASSERT(pEditBox);	// PP: Assert

				std::string strServerName = pEditBox->GetUserText();

				if (!strServerName.empty()) {
					shared_ptr<CEventData_HostGame> pEvent(new CEventData_HostGame(pEditBox->GetUserText(), pEditBox->GetUserText()));
					CEventManager::GetInstance()->QueueEvent(pEvent);
					return;
				}
			}
		}
	}
}

void CMainMenuView::ServerListGUIEventCallback(EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement)
{
	auto mainCallback = [this](EGUIEvent _eGUIEvent, AGUIElement* _pGUICallBack) {
		MainGUIEventCallback(_eGUIEvent, _pGUICallBack);
	};

	if (!_pGUIElement) {
		return;
	}

	std::string strName = _pGUIElement->GetName();
	std::string strType = strName.substr(1);

	if (_eGUIEvent == GUIEVENT_BUTTON_RELEASED) {
		if (strName == "CancelButton") {
			RequestPopGUI();

			// Clear the server list.
			m_serverList.clear();

			// Close the network we've created.
			g_pApp->CloseNetwork();
		}
		else if (strName == "JoinServerButton") {
			auto callback = [this](EGUIEvent _eGUIEvent, AGUIElement* _pGUICallBack) {
				UserNameGUIEVentCallback(_eGUIEvent, _pGUICallBack);
			};

			unique_ptr<CBaseGUI> pGUI(new CBaseGUI(callback, true));
			pGUI->CreateGUIFromXML("UserInterfaces/Username.xml");
			PushGUI(std::move(pGUI));

			/*DEBUG_ASSERT(m_iServerSelection != -1);
			DEBUG_ASSERT(m_pNetwork);

			TServerDetails serverDetails = m_serverList[m_iServerSelection];

			ABaseNetwork::EJoinRequestResponse eResponse = m_pNetwork->JoinServer(serverDetails.m_serverAddress, "MR.TODO");

			switch (eResponse) {
				case ABaseNetwork::JOINRR_OK: {
					// Send a join game event.
					shared_ptr<CEventData_JoinGame> pEvent(new CEventData_JoinGame(serverDetails.m_serverAddress, "MR.TODO", serverDetails.m_iMaxPlayers));
					CEventManager::GetInstance()->TriggerEvent(pEvent);
					return;
				}
				case ABaseNetwork::JOINRR_FULL_LOBBY: {
					// TODO: Display Error.
					break;
				}
				case ABaseNetwork::JOINRR_GAME_IN_SESSION: {
					// TODO: Display Error.
					break;
				}
				case ABaseNetwork::JOINRR_USERNAME_IN_USE: {
					// TODO: Display Error.
					break;
				}
				case ABaseNetwork::JOINRR_ERROR: {
					// TODO: Display Error.
					break;
				}
				case ABaseNetwork::JOINRR_NORESPONSE: {
					// TODO: Display Error.
					break;
				}
				default: {
					break;
				}
			}*/
		}
		else if (strType == "ServerListButton") {
			CBaseGUI* pGUI = GetTopGUI();
			DEBUG_ASSERT(pGUI);

			CButton* pButton = pGUI->GetButton(strName);
			if (pButton->GetState() == CButton::STATE_PRESSED) {
				m_iServerSelection = atoi(strName.substr(0, 1).c_str());
				pGUI->GetButton("JoinServerButton")->SetActive(true);

				for (unsigned int i = 0; i < m_serverList.size(); ++i) {
					// Add a button for the user to click on to select a server.
					CButton* pServerListButton = GetTopGUI()->GetButton(ToString(i) + strType);
					DEBUG_ASSERT(pServerListButton);

					if (i != m_iServerSelection) {
						pServerListButton->SetState(CButton::STATE_OUT);
					}
				}
			}
			else {
				m_iServerSelection = -1;
				pGUI->GetButton("JoinServerButton")->SetActive(false);
			}
		}
	}
}

void CMainMenuView::UserNameGUIEVentCallback(EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement)
{
	auto mainCallback = [this](EGUIEvent _eGUIEvent, AGUIElement* _pGUICallBack) {
		MainGUIEventCallback(_eGUIEvent, _pGUICallBack);
	};

	if (!_pGUIElement) {
		return;
	}

	std::string strName = _pGUIElement->GetName();

	if (_eGUIEvent == GUIEVENT_BUTTON_RELEASED) {
		if (strName == "BackButton") {
			RequestPopGUI();
		}
		else if (strName == "OkButton") {
			DEBUG_ASSERT(m_iServerSelection != -1);
			DEBUG_ASSERT(m_pNetwork);

			CEditBox* pEditBox = GetTopGUI()->GetEditBox("UserNameEditBox");
			DEBUG_ASSERT(pEditBox);

			std::string strUsername = pEditBox->GetUserText();
			if (strUsername.empty()) {
			// Can't have an empty username.
				return;
			}

			TServerDetails serverDetails = m_serverList[m_iServerSelection];

			// Attempt to join the server.
			ABaseNetwork::EJoinRequestResponse eResponse = m_pNetwork->JoinServer(serverDetails.m_serverAddress, strUsername);

			unique_ptr<CBaseGUI> pResponseGUI(new CBaseGUI(mainCallback, true));
			pResponseGUI->CreateGUIFromXML("UserInterfaces/JoinGameResponseMessage.xml");

			switch (eResponse) {
				case ABaseNetwork::JOINRR_OK: {
					// Send a join game event.
					shared_ptr<CEventData_JoinGame> pEvent(new CEventData_JoinGame(serverDetails.m_serverAddress, strUsername, serverDetails.m_iMaxPlayers));
					CEventManager::GetInstance()->TriggerEvent(pEvent);
					return;
				}
				case ABaseNetwork::JOINRR_FULL_LOBBY: {
					pResponseGUI->GetStaticImage("LobbyFullStatic")->SetVisibility(true);

					break;
				}
				case ABaseNetwork::JOINRR_GAME_IN_SESSION: {
					pResponseGUI->GetStaticImage("GameInSessionStatic")->SetVisibility(true);

					break;
				}
				case ABaseNetwork::JOINRR_USERNAME_IN_USE: {
					pResponseGUI->GetStaticImage("UserNameInUseStatic")->SetVisibility(true);

					break;
				}
				case ABaseNetwork::JOINRR_ERROR: // Fall through
				case ABaseNetwork::JOINRR_NORESPONSE: {
					pResponseGUI->GetStaticImage("ConnectionLostStatic")->SetVisibility(true);
					break;
				}
				default: {
					break;
				}
			}

			PopGUI();
			PushGUI(std::move(pResponseGUI));
		}
	}
}
