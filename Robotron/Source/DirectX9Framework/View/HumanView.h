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

#pragma once

#ifndef __HUMANVIEW_H__
#define __HUMANVIEW_H__

// Library Includes
#include "../UserInterface/BaseGUI.h"

// Local Includes
#include "GameView.h"
#include "../GameStd.h"	// REMOVE: this.

class CHumanView : public IGameView {
	// Member Functions
public:
	CHumanView();
	virtual ~CHumanView();

	virtual bool VOnRestore();
	virtual void VOnUpdate(float _fDeltaTime);
	virtual void VRender();
	virtual bool VOnMsgProc(const TAppMsg& _krMsg);
	virtual bool VLoadGame(const CIniDocument& _krIniDocument);
	virtual void VSetControlledActor(TActorID _actorID);

	virtual shared_ptr<CScene> GetScene();
protected:
private:

	// Member Variables
public:
protected:
	shared_ptr<CScene> m_pScene;
	shared_ptr<CCameraNode> m_pCamera;

	IKeyboardController* m_pKeyboardController;
	IMouseController* m_pMouseController;
	IGamepadController* m_pGamepadController;

	TActorID m_controlledActor;
private:
};

#endif	// __HUMANVIEW_H__