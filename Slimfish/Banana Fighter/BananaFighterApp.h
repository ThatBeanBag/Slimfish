// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BananaFighterApp.h
// Description	: CBananaFighterApp declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MARCHINGCUBESAPP_H__
#define __MARCHINGCUBESAPP_H__

// PCH
#include "BananaFighterStd.h"

// Library Includes

// Local Includes
#include <GameBase\SlimGameApp.h>

class CBananaFighterApp : public CGameApp {
	// Member Functions
public:
	CBananaFighterApp();
	~CBananaFighterApp();

	virtual std::string VGetTitle() override;
	virtual HICON VGetIcon() override;
	virtual std::unique_ptr<CGameLogic> VCreateGame() override;

protected:
private:
	// Member Variables
public:
protected:
private:
	
};

extern CBananaFighterApp g_BananaFighterApp;

#endif // __MARCHINGCUBESAPP_H__