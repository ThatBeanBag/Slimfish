// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: HeightMapApp.cpp
// Description	: CHeightMapApp implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Inlcudes

// This Include
#include "RobotronApp.h"

// Local Includes
#include "Msvc\resource.h"
#include "RobotronLogic.h"

CHeightMapApp g_heightMapApp;

CHeightMapApp::CHeightMapApp()
{

}

CHeightMapApp::~CHeightMapApp()
{

}

TCHAR* CHeightMapApp::VGetTitle()
{
	return L"Slimfish";
}

HICON CHeightMapApp::VGetIcon()
{
	return LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_APPLICATION));
}

unique_ptr<CGameLogic>CHeightMapApp::VCreateAndGetGame()
{
	unique_ptr<CGameLogic> pGame(new CRobotronLogic());
	return std::move(pGame);
}