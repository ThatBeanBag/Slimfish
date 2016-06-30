// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: IsoTileGrid.h
// Description	: CIsoTileGrid declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __ISOTILEGRID_H__
#define __ISOTILEGRID_H__

// Library Includes

// Local Includes
#include "IsoTile.h"

class CIsoTileGrid {
	using TTileGrid = std::vector<std::vector<CIsoTile> >;
	using TTileIndex = size_t;

	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CIsoTileGrid();

	/** Construct an isometric tile grid by specifying the width and height.
		@author Hayden Asplet
	*/
	CIsoTileGrid(size_t width, size_t height);
	
	/** Destructor
		@author Hayden Asplet
	*/
	~CIsoTileGrid();

	/** Get the slope case of a tile in the grid.
	 	@author Hayden Asplet
		@param x Column number of the tile in the grid.
		@param y Row number of the tile in the grid.
	*/
	const ETileSlopeCase GetSlopeCase(size_t x, size_t y) const;

	/** Retrieve a tile from the grid.
	 	@author Hayden Asplet
		@param x Column number of the tile in the grid.
		@param y Row number of the tile in the grid.
	*/
	const CIsoTile& GetTile(size_t x, size_t y) const { return m_Grid[y][x]; }

	/** Retrieve a row of tiles from the grid.
	 	@author Hayden Asplet
	 	@param y Row number.
	*/
	const TTileGrid::value_type& operator[](size_t y) const { return m_Grid[y]; }

	/** Retrieve a row of tiles from the grid.
		@author Hayden Asplet
		@param y Row number.
	*/
	TTileGrid::value_type& operator[](size_t y) 
	{ 
		return const_cast<TTileGrid::value_type&>(static_cast<const CIsoTileGrid&>(*this)[y]); 
	}

	/** Resize the tile grid. @author Hayden Asplet */
	void Resize(size_t width, size_t height);

	/** Get the width of the grid. @author Hayden Asplet */
	const size_t GetWidth() const 
	{
		if (!m_Grid.empty()) {
			return m_Grid[0].size();
		}
		else {
			return 0;
		}
	}
	/** Get the height of the grid. @author Hayden Asplet */
	const size_t GetHeight() const { return m_Grid.size(); }

	/** Get the dimensions (width and height) of the grid. @author Hayden Asplet */
	const CPoint<> GetDimensions() { return CPoint<>(GetWidth(), GetHeight()); }

	/** Get the size of the grid (number of tiles). @author Hayden Asplet */
	const size_t GetSize() const { return GetWidth() * GetHeight(); }
protected:
private:
	// Member Variables
public:
protected:
private:
	TTileGrid m_Grid;
};

#endif	// __ISOTILEGRID_H__