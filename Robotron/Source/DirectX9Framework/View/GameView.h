// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GameView.h
// Description	: IGameView declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __GAMEVIEW_H__
#define __GAMEVIEW_H__

// Library Includes

// Local Includes
#include "AppMsg.h"

class IGameView {
	// Member Functions
public:
	virtual ~IGameView() {}

	virtual bool VOnRestore() = 0;
	virtual void VOnUpdate(float m_fDeltaTime) = 0;
	virtual void VRender() = 0;
	virtual bool VOnMsgProc(const TAppMsg& _krMsg) = 0;
	virtual bool VLoadGame(const CIniDocument& _krIniDocument) = 0;
	virtual void VSetControlledActor(TActorID _actorID) = 0;

protected:
private:

	// Member Variables
public:
protected:
private:
};

#endif	// __GAMEVIEW_H__