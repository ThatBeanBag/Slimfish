// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: HumanView.h
// Description	: CHumanView declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "HumanView.h"

// Local Includes

CHumanView::CHumanView()
	:m_pKeyboardController(nullptr),
	m_pMouseController(nullptr),
	m_pGamepadController(nullptr),
	m_pScene(shared_ptr<CScene>()),
	m_pCamera(shared_ptr<CCameraNode>())
{
	m_pScene.reset(new CScene());
	m_pCamera.reset(new CCameraNode());

	m_pScene->AddChild(m_pCamera);
	m_pScene->SetCamera(m_pCamera);
}

CHumanView::~CHumanView()
{

}

bool CHumanView::VOnRestore()
{
	if (m_pScene) {
		return m_pScene->OnRestore();
	}

	return true;
}


void CHumanView::VOnUpdate(float _fDeltaTime)
{
	if (m_pScene) {
		m_pScene->OnUpdate(_fDeltaTime);
	}
}

void CHumanView::VRender()
{
	if (m_pScene) {
		m_pScene->OnRender();
	}
}


bool CHumanView::VOnMsgProc(const TAppMsg& _krMsg)
{
	switch (_krMsg.m_eMessageType) {
		case MT_GAMEPAD_BUTTON_DOWN: {
			if (m_pGamepadController) {
				return m_pGamepadController->VOnButtonDown(_krMsg.m_eGamepadButton);
			}
			break;
		}
		case MT_GAMEPAD_BUTTON_UP: {
			if (m_pGamepadController) {
				return m_pGamepadController->VOnButtonUp(_krMsg.m_eGamepadButton);
			}
			break;
		}
		case MT_GAMEPAD_L_THUMB_MOVE: {
			if (m_pGamepadController) {
				return m_pGamepadController->VOnThumbstick(GTHUMB_LEFT, 
													       _krMsg.m_gamepadState.m_sThumbLeftX,
													       _krMsg.m_gamepadState.m_sThumbLeftY);
			}
		}
		case MT_GAMEPAD_R_THUMB_MOVE: {
			if (m_pGamepadController) {
				return m_pGamepadController->VOnThumbstick(GTHUMB_RIGHT,
														   _krMsg.m_gamepadState.m_sThumbRightX,
														   _krMsg.m_gamepadState.m_sThumbRightY);
			}
			break;
		}
		case MT_GAMEPAD_L_TRIGGER: {
			if (m_pGamepadController) {
				return m_pGamepadController->VOnTrigger(GTRIGGER_LEFT,
														_krMsg.m_gamepadState.m_ucLeftTrigger);
			}
			break;
		}
		case MT_GAMEPAD_R_TRIGGER: {
			if (m_pGamepadController) {
				return m_pGamepadController->VOnTrigger(GTRIGGER_RIGHT,
														_krMsg.m_gamepadState.m_ucRightTrigger);
			}
			break;
		}
		case MT_KEYDOWN: {
			if (m_pKeyboardController) {
				return m_pKeyboardController->VOnKeyDown(_krMsg.m_uckey);
			}

			break;
		}
		case MT_KEYUP: {
			if (m_pKeyboardController) {
				return m_pKeyboardController->VOnKeyUp(_krMsg.m_uckey);
			}

			break;
		}
		case MT_MOUSE_MOVE: {
			if (m_pMouseController) {
				return m_pMouseController->VOnMouseMove(_krMsg.m_mousePosition);
			}

			break;
		}
		case MT_MOUSE_WHEEL: {
			if (m_pMouseController) {
				return m_pMouseController->VOnMouseWheel(_krMsg.m_sDeltaMouseWheel);
			}

			break;
		}
		case MT_LMB_DOWN: {
			if (m_pMouseController) {
				return m_pMouseController->VOnMouseButtonDown(_krMsg.m_mousePosition, MBUTTON_LEFT);
			}

			break;
		}
		case MT_LMB_UP:	 {
			if (m_pMouseController) {
				return m_pMouseController->VOnMouseButtonUp(_krMsg.m_mousePosition, MBUTTON_LEFT);
			}

			break;
		}
		case MT_MMB_DOWN: {
			if (m_pMouseController) {
				return m_pMouseController->VOnMouseButtonDown(_krMsg.m_mousePosition, MBUTTON_MIDDLE);
			}

			break;
		}
		case MT_MMB_UP:	 {
			if (m_pMouseController) {
				return m_pMouseController->VOnMouseButtonUp(_krMsg.m_mousePosition, MBUTTON_MIDDLE);
			}

			break;
		}
		case MT_RMB_DOWN: {
			if (m_pMouseController) {
				return m_pMouseController->VOnMouseButtonDown(_krMsg.m_mousePosition, MBUTTON_RIGHT);
			}

			break;
		}
		case MT_RMB_UP: {
			if (m_pMouseController) {
				return m_pMouseController->VOnMouseButtonUp(_krMsg.m_mousePosition, MBUTTON_RIGHT);
			}

			break;
		}

		default: break;
	}

	return false;
}

bool CHumanView::VLoadGame(const CIniDocument& _krIniDocument)
{
	return true;
}

void CHumanView::VSetControlledActor(TActorID _actorID)
{
	m_controlledActor = _actorID;
}

shared_ptr<CScene> CHumanView::GetScene()
{
	return m_pScene;
}

