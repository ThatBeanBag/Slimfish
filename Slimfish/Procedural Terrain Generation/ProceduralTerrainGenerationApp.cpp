// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ProceduralTerrainGenerationApp.cpp
// Description	: CProceduralTerrainGenerationApp implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ProceduralTerrainGenerationStd.h"

// Library Includes

// This Include
#include "ProceduralTerrainGenerationApp.h"

// Local Includes
#include "ProceduralTerrainGenerationLogic.h"

CProceduralTerrainGenerationApp g_ProceduralTerrainGenerationApp;

CProceduralTerrainGenerationApp::CProceduralTerrainGenerationApp()
{

}

CProceduralTerrainGenerationApp::~CProceduralTerrainGenerationApp()
{

}

std::string CProceduralTerrainGenerationApp::VGetTitle()
{
	return std::string("Procedural Terrain Generation");
}

HICON CProceduralTerrainGenerationApp::VGetIcon()
{
	return 0;
}

std::unique_ptr<CGameLogic> CProceduralTerrainGenerationApp::VCreateGame()
{
	return std::make_unique<CProceduralTerrainGenerationLogic>();
}
