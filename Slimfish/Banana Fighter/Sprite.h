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
#include <Graphics/SlimTexture.h>

/** Class encapsulating a sprite sheet.
@remarks
*/
class CSprite {
	using TMaskList = std::vector<CRect<>>;

	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CSprite();

	/** Construct a sprite from a texture.
		@author Hayden Asplet
	*/
	CSprite(std::shared_ptr<ATexture>& pTexture);
	
	/** Destructor
		@author Hayden Asplet
	*/
	~CSprite();

	/** Add a mask to the back of the list of masks. @author Hayden Asplet */
	void AddMask(const CRect<>& mask);

	/** Get a mask.
	 	@author Hayden Asplet
	 	@param index The index of the mask in the list of masks.
	*/
	const CRect<> GetMask(size_t index) const;

	/** Get the number of masks. @author Hayden Asplet */
	const size_t GetNumMasks() const;

	/** Create a grid of masks 
		@remarks 
			Appends the masks to the back of the list in the order going from 
			left to right, layer by layer. Like so:
				----->
				>---->
	 	@author Hayden Asplet
	 	@param maskWidth The width of a single mask.
	 	@param maskHeight The height of a single mask.
	 	@param numAcross The number of horizontal masks.
	 	@param numDown The number of vertical masks.
		@param 
			startX The X component of the initial coordinate to start 
			from. Default is 0 (top left corner).
		@param 
			startY The Y component of the initial coordinate to start
			from. Default is 0 (top left corner).
	*/
	void CreateMasks(int maskWidth, int maskHeight, size_t numAcross, size_t numDown, int startX = 0, int startY = 0);

	/** Set the texture. @author Hayden Asplet */
	void SetTexture(std::shared_ptr<ATexture> texture);
	/** Get the texture. @author Hayden Asplet */
	const std::shared_ptr<ATexture>& GetTexture() const;

	/** Set all the masks. @author Hayden Asplet */
	void SetMasks(const TMaskList& masks);
	/** Get all the masks. @author Hayden Asplet */
	const TMaskList& GetMasks() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	TMaskList m_Masks;
	std::shared_ptr<ATexture> m_pTexture;
};

#endif	// __SPRITE_H__