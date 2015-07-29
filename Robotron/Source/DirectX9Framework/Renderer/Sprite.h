// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Sprite.h
// Description	: CSprite declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SPRITE_H__
#define __SPRITE_H__

// Library Includes

// Local Includes

class ISprite {
	// Member Functions
public:
	virtual ~ISprite();

	virtual const CPoint VGetDimensions() const = 0;
	virtual void VRender(const CRect& _krDestRect, const CRect& _krSourceRect = CRect(), unsigned char _ucAlpha = 255) = 0;

	static shared_ptr<ISprite> LoadResource(const std::string& _krstrFileName);
protected:
private:
	// Member Variables
public:
protected:
private:
};

#endif	// __SPRITE_H__