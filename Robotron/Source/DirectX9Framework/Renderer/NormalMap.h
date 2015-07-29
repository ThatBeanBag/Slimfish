// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: NormalMap.h
// Description	: CNormalMap declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __NORMALMAP_H__
#define __NORMALMAP_H__

// Library Includes

// Local Includes
#include "Texture.h"

class INormalMap : public ITexture {
	// Member Functions
public:
	virtual ~INormalMap() {}

	virtual void SetLight(const TLight& _krLightProperties) = 0;

	static shared_ptr<INormalMap> LoadResource(const std::string& _krstrFileName);
protected:
private:
	// Member Variables
public:
protected:
private:
};

#endif	// __NORMALMAP_H__