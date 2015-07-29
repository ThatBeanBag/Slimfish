// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Texture.h
// Description	: ITexture declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

// Library Includes

// Local Includes

enum ETextureStage {
	TEXSTAGE_0 = 0,
	TEXSTAGE_1 = 1,
	TEXSTAGE_2 = 2,
	TEXSTAGE_3 = 3,
	TEXSTAGE_4 = 4,
	TEXSTAGE_5 = 5,
	TEXSTAGE_6 = 6,
	TEXSTAGE_7 = 7
};

class ITexture {
	// Member Functions
public:
	virtual ~ITexture();

	virtual void VSetToRenderer(ETextureStage eTextureStage) = 0;
	virtual void VSetTranslation(float _fX, float _fY) = 0;

	static shared_ptr<ITexture> LoadResource(const std::string& _krstrFileName);
protected:
private:
	// Member Variables
public:
protected:
private:
};

#endif	// __TEXTURE_H__