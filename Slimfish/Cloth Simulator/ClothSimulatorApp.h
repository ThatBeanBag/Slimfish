// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ClothSimulatorApp.h
// Description	: CClothSimulatorApp declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __CLOTHSIMULATORAPP_H__
#define __CLOTHSIMULATORAPP_H__

// PCH
#include "ClothSimulatorStd.h"

// Library Includes

// Local Includes
#include <GameBase\SlimGameApp.h>

class CClothSimulatorApp : public CGameApp {
	// Member Functions
public:
	CClothSimulatorApp();
	~CClothSimulatorApp();

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

extern CClothSimulatorApp g_ClothSimulatorApp;

#endif // __CLOTHSIMULATORAPP_H__