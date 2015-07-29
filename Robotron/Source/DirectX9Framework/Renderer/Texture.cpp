// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Texture.cpp
// Description	: ITexture implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Texture.h"

// Local Includes

ITexture::~ITexture()
{

}

shared_ptr<ITexture> ITexture::LoadResource(const std::string& _krstrFileName)
{
	return g_pApp->GetRenderer()->VCreateTextureFromFile(_krstrFileName);
}
