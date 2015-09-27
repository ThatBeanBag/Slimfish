// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRenderTexture.h
// Description	: CRenderTexture declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMRENDERTEXTURE_H__
#define __SLIMRENDERTEXTURE_H__

// Library Includes

// Local Includes

namespace Slim {

// Forward Declaration.
class ATexture;

/** Abstract class representing a render target as a texture.
@remarks
		
*/
class ARenderTexture {
	// Member Functions
public:
	ARenderTexture(shared_ptr<ATexture> pTexture);
	virtual ~ARenderTexture();

	shared_ptr<ATexture> GetTexture();
protected:
private:
	// Member Variables
public:
protected:
private:
	shared_ptr<ATexture> m_pTexture;
};

}

#endif	// __SLIMRENDERTEXTURE_H__