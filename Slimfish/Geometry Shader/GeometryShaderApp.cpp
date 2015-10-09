// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GeometryShaderApp.cpp
// Description	: CGeometryShaderApp implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GeometryShaderStd.h"

// Library Includes

// This Include
#include "GeometryShaderApp.h"

// Local Includes
#include "GeometryShaderLogic.h"

CGeometryShaderApp g_GeometryShaderApp;

CGeometryShaderApp::CGeometryShaderApp()
{

}

CGeometryShaderApp::~CGeometryShaderApp()
{

}

std::string CGeometryShaderApp::VGetTitle()
{
	return std::string("GeometryShader");
}

HICON CGeometryShaderApp::VGetIcon()
{
	return 0;
}

std::unique_ptr<CGameLogic> CGeometryShaderApp::VCreateGame()
{
	return std::make_unique<CGeometryShaderLogic>();
}
