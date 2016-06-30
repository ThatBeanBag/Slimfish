// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: IsoBase.h
// Description	: CIsoBase declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __ISOBASE_H__
#define __ISOBASE_H__

// Library Includes

// Local Includes

// Forward Declaration
class CSprite;

/** Abstract class defining the base of all isometric objects and tiles.
@remarks
	
*/
class CIsoBase {
	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CIsoBase();

	/** Construct an isometric base from a sprite.
		@author Hayden Asplet
	*/
	CIsoBase(std::shared_ptr<CSprite>& pSprite);
	
	/** Destructor
		@author Hayden Asplet
	*/
	virtual ~CIsoBase() = 0;

	void Translate(const CVector3& translation);

	/** Set the position. @author Hayden Asplet */
	void SetPosition(const CVector3& position);
	/** Get the position. @author Hayden Asplet */
	const CVector3& GetPosition() const;

	/** Set the sprite. @author Hayden Asplet */
	void SetSprite(std::shared_ptr<CSprite> sprite);
	/** Get the sprite. @author Hayden Asplet */
	const std::shared_ptr<CSprite> GetSprite() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	CVector3 m_Position;
	std::shared_ptr<CSprite> m_pSprite;
};

#endif	// __ISOBASE_H__