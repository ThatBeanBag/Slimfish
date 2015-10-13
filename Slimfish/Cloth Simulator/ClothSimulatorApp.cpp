// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ClothSimulatorApp.cpp
// Description	: CClothSimulatorApp implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ClothSimulatorStd.h"

// Library Includes

// This Include
#include "ClothSimulatorApp.h"

// Local Includes
#include "ClothSimulatorLogic.h"

CClothSimulatorApp g_ClothSimulatorApp;

CClothSimulatorApp::CClothSimulatorApp()
{

}

CClothSimulatorApp::~CClothSimulatorApp()
{

}

std::string CClothSimulatorApp::VGetTitle()
{
	return std::string("ClothSimulator");
}

HICON CClothSimulatorApp::VGetIcon()
{
	return 0;
}

std::unique_ptr<CGameLogic> CClothSimulatorApp::VCreateGame()
{
	return std::make_unique<CClothSimulatorLogic>();
}
