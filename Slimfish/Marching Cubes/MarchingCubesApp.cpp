// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MarchingCubesApp.cpp
// Description	: CMarchingCubesApp implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "MarchingCubesStd.h"

// Library Includes

// This Include
#include "MarchingCubesApp.h"

// Local Includes
#include "MarchingCubesLogic.h"

CMarchingCubesApp g_MarchingCubesApp;

CMarchingCubesApp::CMarchingCubesApp()
{

}

CMarchingCubesApp::~CMarchingCubesApp()
{

}

std::string CMarchingCubesApp::VGetTitle()
{
	return std::string("Marching Cubes");
}

HICON CMarchingCubesApp::VGetIcon()
{
	return 0;
}

std::unique_ptr<CGameLogic> CMarchingCubesApp::VCreateGame()
{
	return std::make_unique<CMarchingCubesLogic>();
}
