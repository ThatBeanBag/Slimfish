// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: IsoTileGrid.cpp
// Description	: CIsoTileGrid implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "BananaFighterStd.h"

// Library Includes

// This Include
#include "IsoTileGrid.h"

// Local Includes

CIsoTileGrid::CIsoTileGrid()
{

}

CIsoTileGrid::CIsoTileGrid(size_t width, size_t height)
{
	Resize(width, height);
}

CIsoTileGrid::~CIsoTileGrid()
{

}

const ETileSlopeCase CIsoTileGrid::GetSlopeCase(size_t x, size_t y) const
{
	return ETileSlopeCase::FLAT;
}

void CIsoTileGrid::Resize(size_t width, size_t height)
{
	for (auto& tileLine : m_Grid) {
		// Resize a line.
		tileLine.resize(width);
	}

	m_Grid.resize(height, std::vector<CIsoTile>(width));

	// Set the tile positions.
	for (auto y = 0U; y < m_Grid.size(); ++y) {
		for (auto x = 0U; x < m_Grid[y].size(); ++x) {
			m_Grid[y][x].SetPosition(CVector3(static_cast<float>(x), static_cast<float>(y), 0.0f));
		}
	}
}
