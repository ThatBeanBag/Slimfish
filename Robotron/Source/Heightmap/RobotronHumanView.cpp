// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: HeightMapHumanView.cpp
// Description	: CHeightMapHumanView implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include 
#include "RobotronHumanView.h"

// Local Includes
#include "TwinStickController.h"
#include "..\DirectX9Framework\Actor\PhysicsComponent.h"
#include "EventData_PlayerStateChange.h"
#include "EventData_LevelStateChange.h"


CRobotronHumanView::CRobotronHumanView()
	:m_bShowDebugInfo(false)
{
	m_pCamera->SetProjectionTransform(ToRadians(45.0f), 
									  static_cast<float>(g_pApp->GetScreenWidth()),
									  static_cast<float>(g_pApp->GetScreenHeight()),
									  0.001f,
									  1000.0f,
									  PROJECTION_PERSPECTIVE);

	// Add a flying controller.
	m_pFlyingController.reset(new CFlyingController());
	m_pFlyingController->SetObject(m_pCamera);
	m_pFlyingController->SetClickToRotate(true);
	m_pFlyingController->SetInvertYAxis(false);
	m_pFlyingController->SetSpeed(10.0f);

	// Register Listeners.
	CEventManager::GetInstance()->AddListener<CEventData_PlayerLeft>(MakeDelegate(this, &CRobotronHumanView::PlayerLeftDelegate));
	CEventManager::GetInstance()->AddListener<CEventData_PlayerStateChange>(MakeDelegate(this, &CRobotronHumanView::PlayerStateChangeDelegate));
	CEventManager::GetInstance()->AddListener<CEventData_DestroyActor>(MakeDelegate(this, &CRobotronHumanView::DestroyActorDelegate));
	CEventManager::GetInstance()->AddListener<CEventData_LevelStateChange>(MakeDelegate(this, &CRobotronHumanView::LevelStateChangeDelegate));

	// Create Interface.
	auto callback = [this](EGUIEvent _eEvent, AGUIElement* _pElement) {
		GUIEventCallback(_eEvent, _pElement);
	};

	unique_ptr<CBaseGUI> pGUI(new CBaseGUI(callback));
	pGUI->CreateGUIFromXML("UserInterfaces/InGameUI.xml");
	m_pGUI = pGUI.get();

	PushGUI(std::move(pGUI));
}

CRobotronHumanView::~CRobotronHumanView()
{
	CEventManager::GetInstance()->RemoveListener<CEventData_PlayerLeft>(MakeDelegate(this, &CRobotronHumanView::PlayerLeftDelegate));
	CEventManager::GetInstance()->RemoveListener<CEventData_PlayerStateChange>(MakeDelegate(this, &CRobotronHumanView::PlayerStateChangeDelegate));
	CEventManager::GetInstance()->RemoveListener<CEventData_DestroyActor>(MakeDelegate(this, &CRobotronHumanView::DestroyActorDelegate));
	CEventManager::GetInstance()->RemoveListener<CEventData_LevelStateChange>(MakeDelegate(this, &CRobotronHumanView::LevelStateChangeDelegate));
	
	m_pFlyingController = nullptr;
	m_pMouseController = nullptr;
	m_pKeyboardController = nullptr;
}

bool CRobotronHumanView::VOnRestore()
{

	if (m_pFlyingController) {
		if (!m_pFlyingController->OnRestore()) {
			return false;
		}
	}

	if (m_pTwinStickController) {
		if (!m_pTwinStickController->OnRestore()) {
			return false;
		}
	}

	return CHumanView::VOnRestore();
}

void CRobotronHumanView::VOnUpdate(float _fDeltaTime)
{
	CUserInterfaceView::VOnUpdate(_fDeltaTime);

	if (m_fPlayerLeftTimer != 0) {
		m_fPlayerLeftTimer -= _fDeltaTime;
		if (m_fPlayerLeftTimer < 0) {
			m_fPlayerLeftTimer = 0;
			m_pGUI->GetStaticText("PlayerLeftText")->SetText(std::string(" "));
		}
	}

	if (!m_bIsInMenu) {
		// Update the controller.
		if (m_pTwinStickController) {
			m_pTwinStickController->OnUpdate(_fDeltaTime);
		}

		if (m_pFlyingController) {
			m_pFlyingController->OnUpdate(_fDeltaTime);
		}
	}
}

void CRobotronHumanView::VRender()
{
	CUserInterfaceView::VRender();

	m_pTwinStickController->Render();

	if (m_bShowDebugInfo) {
		ShowDebugInfo();
	}
}

bool CRobotronHumanView::VOnMsgProc(const TAppMsg& _krMsg)
{
	if (CUserInterfaceView::VOnMsgProc(_krMsg) == true) {
		return true;
	}

	switch (_krMsg.m_eMessageType) {
		case MT_GAMEPAD_BUTTON_UP: {
			if (_krMsg.m_eGamepadButton == GBUTTON_START) {
				if (m_bIsInMenu) {
					PopGUI();
					m_bIsInMenu = false;
					return true;
				}
				else {
					auto callback = [this](EGUIEvent _eEvent, AGUIElement* _pElement) {
						GUIEventCallback(_eEvent, _pElement);
					};

					unique_ptr<CBaseGUI> pGUI(new CBaseGUI(callback, true));
					if (pGUI) {
						pGUI->CreateGUIFromXML("UserInterfaces/InGameMenu.xml");
						PushGUI(std::move(pGUI));

						m_bIsInMenu = true;
					}
				}
			}

			break;
		}
		case MT_KEYDOWN: {
			break;
		}
		case MT_KEYUP: {
			if (_krMsg.m_uckey == VK_F1) {
#ifndef NDEBUG
				if(g_pApp->GetRenderer()->VGetRenderState(RS_FILLMODE) != FILL_WIREFRAME) {
					g_pApp->GetRenderer()->VSetRenderState(RS_FILLMODE, FILL_WIREFRAME);
				}
				else {
					g_pApp->GetRenderer()->VSetRenderState(RS_FILLMODE, FILL_SOLID);
				}
#endif // NDEBUG
			}
			else if (_krMsg.m_uckey == VK_F2) {
#ifndef NDEBUG
				m_bShowDebugInfo = !m_bShowDebugInfo;
#endif // NDEBUG
			}
			else if (_krMsg.m_uckey == VK_F3) {
				g_pApp->ToggleWindowedMode();
			}
			else if (_krMsg.m_uckey == VK_F4) {
#ifndef NDEBUG
				// Toggle controller setting.
				if (m_pKeyboardController == m_pFlyingController.get()) { // CS: Toggle Debug Cam
					SetTwinStickController();
				}
				else {
					SetFlyingController();
				}
#endif // NDEBUG
			}
			else if (_krMsg.m_uckey == VK_SNAPSHOT) {
				g_pApp->GetRenderer()->VDumpBackBufferToDisk("Screenshot.jpg");
			}
			else if (_krMsg.m_uckey == VK_ESCAPE) {
				if (m_bIsInMenu) {
					PopGUI();
					m_bIsInMenu = false;
					return true;
				}
				else {
					auto callback = [this](EGUIEvent _eEvent, AGUIElement* _pElement) {
						GUIEventCallback(_eEvent, _pElement);
					};

					unique_ptr<CBaseGUI> pGUI(new CBaseGUI(callback, true));
					if (pGUI) {
						pGUI->CreateGUIFromXML("UserInterfaces/InGameMenu.xml");
						PushGUI(std::move(pGUI));

						m_bIsInMenu = true;
					}
				}
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

	return false;
}

bool CRobotronHumanView::VLoadGame(const CIniDocument& _krIniDocument)
{
	return true;
}

void CRobotronHumanView::VSetControlledActor(TActorID _actorID)
{
	shared_ptr<CSceneNode> pActorNode = m_pScene->FindSceneNodeFromActorID(_actorID);

	if (!pActorNode) {
		DEBUG_ERROR("Couldnd't find actor with ID: " + ToString(_actorID) + " to control.");
		return;
	}

	m_actorID = _actorID;

	m_pTwinStickController.reset(new CTwinStickController(pActorNode, m_pCamera));

	m_pMouseController = m_pTwinStickController.get();
	m_pKeyboardController = m_pTwinStickController.get();
	m_pGamepadController = m_pTwinStickController.get();
	m_pFlyingController->SetClickToRotate(true);
}

void CRobotronHumanView::ShowDebugInfo() const
{
	std::string strDebugText = "Debug info (F2 to hide):\n\n";

	if (m_pCamera) {
		const CMatrix4x4& matView = m_pCamera->GetViewMatrix();
		const CMatrix4x4& matProjection = m_pCamera->GetProjectionMatrix();

		strDebugText += "View Matrix:\n" + matView.ToText(4) + '\n';
		strDebugText +="Projection Matrix:\n" + matProjection.ToText(4) + '\n';
	}

	if (m_pCube) {
		const CMatrix4x4& matCubeTransform = m_pCube->GetTransformMatrix();
		strDebugText += "Cube Matrix:\n" + matCubeTransform.ToText(4) + '\n';
	}

	strDebugText += "Camera Mode: ";

	switch (m_eControlMode) {
		case CONTROL_FIRSTPERSON: {
			strDebugText += "First person\n\n";
			break;
		}
		case CONTROL_THIRDPERSON: {
			strDebugText += "Third person\n\n";
			break;
		}
		case CONTROL_FREE: {
			strDebugText += "Free camera\n\n";
			break;
		}
	}

	if (m_pScene) {
		for (unsigned int i = 0; i < m_pScene->GetLights().size(); ++i) {
			TLight current = m_pScene->GetLights()[i];

			strDebugText += "Light " + ToString(i);

			switch (current.m_eType) {
				case LIGHT_POINT: {
					strDebugText += " - Point";
					break;
				}
				case LIGHT_SPOT: {
					strDebugText += " - Spot";
					break;
				}
				case LIGHT_DIRECTIONAL: {
					strDebugText += " - Directional";
					break;
				}
			}

			if (current.m_bIsEnabled) {
				strDebugText += " - Enabled\n";
			}
			else {
				strDebugText += " - Disabled\n";
			}

			strDebugText += "Diffuse: " + ColourToText(current.m_diffuse) + '\n';
			strDebugText += "Ambient: " + ColourToText(current.m_ambient) + '\n';
			strDebugText += "Specular: " + ColourToText(current.m_specular) + '\n';

			switch (current.m_eType) {
				case LIGHT_POINT: {
					strDebugText += "Position: " + current.m_position.ToText() + '\n';
					strDebugText += "Range: " + ToString(current.m_fRange) + '\n';
					strDebugText += "Attenuation: " + ToString(current.m_fAttenuation0) + ' ';
					strDebugText += "- " + ToString(current.m_fAttenuation1) + ' ';
					strDebugText += "- " + ToString(current.m_fAttenuation2) + '\n';
					break;
				}
				case LIGHT_SPOT: {
					strDebugText += "Position: " + current.m_position.ToText() + '\n';
					strDebugText += "Direction: " + current.m_direction.ToText() + '\n';
					strDebugText += "Range: " + ToString(current.m_fRange) + '\n';
					strDebugText += "Attenuation: " + ToString(current.m_fAttenuation0) + ' ';
					strDebugText += "- " + ToString(current.m_fAttenuation1) + ' ';
					strDebugText += "- " + ToString(current.m_fAttenuation2) + '\n';
					strDebugText += "Falloff: " + ToString(current.m_fFalloff) + '\n';
					strDebugText += "Thetha: " + ToString(current.m_fTheta) + ' ';
					strDebugText += "Phi: " + ToString(current.m_fPhi) + '\n';
					break;
				}
				case LIGHT_DIRECTIONAL: {
					strDebugText += "Direction: " + current.m_direction.ToText() + '\n';
					break;
				}
			}

			strDebugText += "\n";
		}
	}

	CRect textRect(10, 10, g_pApp->GetScreenWidth() - 20, g_pApp->GetScreenHeight() - 20);
	g_pApp->GetRenderer()->VDrawText(strDebugText.c_str(), 
									 textRect, 
									 14,
									 DT_TOP | DT_LEFT, 
									 CreateColourRGB(255, 230, 0));
}

void CRobotronHumanView::GUIEventCallback(EGUIEvent _eEvent, AGUIElement* _pElement)
{
	if (!_pElement) {
		return;
	}

	if (_eEvent == GUIEVENT_BUTTON_RELEASED) {
		std::string strName = _pElement->GetName();

		if (strName == "ResumeButton") {
			RequestPopGUI();
			m_bIsInMenu = false;
		}
		else if (strName == "MainMenuButton") {
			g_pApp->GetGame()->ChangeState(CGameLogic::GS_MAINMENU);
		}
		else if (strName == "OkButton") {
			RequestPopGUI();
		}
	}
}

void CRobotronHumanView::PlayerLeftDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_PlayerLeft> pPlayerLeftData = static_pointer_cast<CEventData_PlayerLeft>(_pEventData);

	std::string strUsername = pPlayerLeftData->GetUsername();

	if (m_pGUI) {
		m_pGUI->GetStaticText("PlayerLeftText")->SetText(strUsername + " Left.");
		m_pGUI->GetStaticText("PlayerLeftText")->SetVisibility(false);
		m_fPlayerLeftTimer = 10.0f;
	}
}

void CRobotronHumanView::PlayerStateChangeDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_PlayerStateChange> pPlayerStateData = static_pointer_cast<CEventData_PlayerStateChange>(_pEventData);

	if (pPlayerStateData->GetActorID() == m_actorID) {
		m_iAmmo = pPlayerStateData->GetAmmo();
		m_iLives = pPlayerStateData->GetLives();

		m_pGUI->GetStaticText("AmmoText")->SetText(ToString(m_iAmmo));
		m_pGUI->GetStaticText("LivesText")->SetText(ToString(m_iLives));
	}
}

void CRobotronHumanView::DestroyActorDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_DestroyActor> pDestroyActorData = static_pointer_cast<CEventData_DestroyActor>(_pEventData);

	if (pDestroyActorData->GetActorID() == m_actorID && m_actorID != CActor::s_kINVALID_ACTOR_ID) {
		m_actorID = CActor::s_kINVALID_ACTOR_ID;

		auto callback = [this](EGUIEvent _eEvent, AGUIElement* _pElement) {
			GUIEventCallback(_eEvent, _pElement);
		};

		unique_ptr<CBaseGUI> pGUI(new CBaseGUI(callback, true, true));

		pGUI->CreateGUIFromXML("UserInterfaces/GameOverMessage.xml");
		PushGUI(std::move(pGUI));

		SetFlyingController();
	}
}

void CRobotronHumanView::LevelStateChangeDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_LevelStateChange> pLevelStateChangeData = static_pointer_cast<CEventData_LevelStateChange>(_pEventData);

	m_iEnemiesRemaining = pLevelStateChangeData->GetEnemiesRemaining();
	m_iLevelsCompleted = pLevelStateChangeData->GetLevelsCompleted();

	m_pGUI->GetStaticText("EnemiesText")->SetText(ToString(m_iEnemiesRemaining));
	m_pGUI->GetStaticText("LevelText")->SetText(ToString(m_iLevelsCompleted));
}

void CRobotronHumanView::SetFlyingController()
{
	m_pMouseController = m_pFlyingController.get();
	m_pKeyboardController = m_pFlyingController.get();
	m_pGamepadController = m_pFlyingController.get();
	m_pFlyingController->SetClickToRotate(true);
	m_pCamera->SetTarget(nullptr);
}

void CRobotronHumanView::SetTwinStickController()
{
	VSetControlledActor(m_actorID);
}
