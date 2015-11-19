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
	/** Default constructor. 
	 	@author Hayden Asplet
	*/
	ARenderTexture(shared_ptr<ATexture> pTexture);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	virtual ~ARenderTexture();

	/** Get the underlying texture resource. @author Hayden Asplet */
	shared_ptr<ATexture> GetTexture();

	/** Set the background colour. @author Hayden Asplet */
	void SetBackgroundColour(const CColourValue& backgroundColour);
	/** Get the background colour. @author Hayden Asplet */
	const CColourValue& GetBackgroundColour() const;
protected:
private:
	// Member Variables
public:
protected:
	shared_ptr<ATexture> m_pTexture;
private:
	CColourValue m_BackgroundColour;
};

}

#endif	// __SLIMRENDERTEXTURE_H__