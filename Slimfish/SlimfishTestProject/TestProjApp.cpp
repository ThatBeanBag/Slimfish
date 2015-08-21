// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TestProjApp.cpp
// Description	: CTestProjApp implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimTestProjectStd.h"

// Library Includes

// This Include
#include "TestProjApp.h"

// Local Includes
#include "TestProjLogic.h"

CTestProjApp g_testProj;

CTestProjApp::CTestProjApp()
{

}

CTestProjApp::~CTestProjApp()
{

}

std::string CTestProjApp::VGetTitle()
{
	return "Test Project";
}

HICON CTestProjApp::VGetIcon()
{
	return NULL;
}

std::unique_ptr<CGameLogic> CTestProjApp::VCreateGame()
{
	std::unique_ptr<CGameLogic> pGame(new CTestProjLogic());

	return pGame;
}
