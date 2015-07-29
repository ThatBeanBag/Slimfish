// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Sprite.cpp
// Description	: ISprite implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Sprite.h"

// Local Includes

ISprite::~ISprite()
{

}

shared_ptr<ISprite> ISprite::LoadResource(const std::string& _krstrFileName)
{
	return g_pApp->GetRenderer()->VCreateSpriteFromFile(_krstrFileName);
}
