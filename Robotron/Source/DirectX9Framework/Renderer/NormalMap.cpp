// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: NormalMap.cpp
// Description	: CNormalMap implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "NormalMap.h"

// Local Includes

shared_ptr<INormalMap> INormalMap::LoadResource(const std::string& _krstrFileName)
{
	return g_pApp->GetRenderer()->VCreateNormalMapFromFile(_krstrFileName);
}
