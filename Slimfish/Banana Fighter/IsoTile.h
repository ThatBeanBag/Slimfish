// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: IsoTile.h
// Description	: CIsoTile declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __ISOTILE_H__
#define __ISOTILE_H__

// Library Includes

// Local Includes
#include "IsoBase.h"

enum class ETileType {
	GRASS,
	WATER
};

enum class ETileSlopeCase {
	// Standard flat piece (not slopped).
	FLAT,
	// Northern (top) corner.
	NORTH,
	// Southern (bottom) corner.
	SOUTH,
	// Eastern (right) corner.
	EAST,
	// Western (left) corner.
	WEST,
	// North-east edge.
	NORTH_EAST,
	// South-east edge.
	SOUTH_EAST,
	// North-west edge.
	NORTH_WEST,
	// South-west edge.
	SOUTH_WEST
};

/** Class representing a single tile.
@remarks
	
*/
class CIsoTile : public CIsoBase {
	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CIsoTile();
	
	/** Destructor
		@author Hayden Asplet
	*/
	~CIsoTile();

	/** Set the elevation of the northern corner. @author Hayden Asplet */
	void SetElevation(int elevation) { m_Elevation = elevation; }
	/** Get the elevation of the northern corner. @author Hayden Asplet */
	const int GetElevation() const { return m_Elevation; }
protected:
private:
	// Member Variables
public:
protected:
private:
	ETileType m_Type;

	int m_Elevation;	// Elevation of the northern corner.
};

#endif	// __ISOTILE_H__