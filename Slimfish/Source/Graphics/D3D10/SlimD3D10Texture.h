// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10Texture.h
// Description	: CSlimD3D10Texture declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10TEXTURE_H__
#define __SLIMD3D10TEXTURE_H__

// Library Includes

// Local Includes
#include "../SlimTexture.h"

namespace Slim {

class CD3D10Texture : public ATexture {
	// Member Functions
public:
	CD3D10Texture();
	~CD3D10Texture();
protected:
private:
	// Member Variables
public:
protected:
private:

};

}

#endif // __SLIMD3D10TEXTURE_H__