// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BananaFighterApp.cpp
// Description	: CBananaFighterApp implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "BananaFighterStd.h"

// Library Includes

// This Include
#include "BananaFighterApp.h"

// Local Includes
#include "BananaFighterLogic.h"

CBananaFighterApp g_BananaFighterApp;

CBananaFighterApp::CBananaFighterApp()
{

}

CBananaFighterApp::~CBananaFighterApp()
{

}

std::string CBananaFighterApp::VGetTitle()
{
	return std::string("Banana Fighter 2");
}

HICON CBananaFighterApp::VGetIcon()
{
	return 0;
}

std::unique_ptr<CGameLogic> CBananaFighterApp::VCreateGame()
{
	return std::make_unique<CBananaFighterLogic>();
}
