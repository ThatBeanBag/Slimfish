// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ProceduralTerrain.cpp
// Description	: CProceduralTerrain implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ProceduralTerrainGenerationStd.h"

// Library Includes

// This Include
#include "ProceduralTerrain.h"

// Local Includes



CProceduralTerrain::CProceduralTerrain(size_t size, float yMin, float yMax)
	:m_Size(size),
	m_YMin(yMin),
	m_YMax(yMax),
	m_YValues(size, std::vector<float>(size, yMin)),
	m_IsSet(size, std::vector<bool>(size, false))
{
	float midHeight = (yMin + yMax) / 2.0f;

	// Set corners. 
	SoftSet(0, 0, midHeight);
	SoftSet(0, size - 1, midHeight);
	SoftSet(size - 1, 0, midHeight);
	SoftSet(size - 1, size - 1, midHeight);

	TRegion region = { { 0, 0, size - 1, size - 1 }, midHeight };

	m_RegionQueue.push(region);
}

CProceduralTerrain::~CProceduralTerrain()
{

}

void CProceduralTerrain::Step()
{
	if (!m_RegionQueue.empty()) {
		// Get the next region in the queue.
		const auto& region = m_RegionQueue.front();
		// Perform diamond square on region.
		DiamondSquare(region.rect, region.baseHeight);
		// Pop off the region from the queue.
		m_RegionQueue.pop();
	}
}

void CProceduralTerrain::SoftSet(size_t x, size_t z, float height)
{
	if (m_IsSet[z][x] == false) {
		m_IsSet[z][x] = true;
		m_YValues[z][x] = height;
	}
}

void CProceduralTerrain::DiamondSquare(const TRect& region, float baseHeight)
{
	auto centreIndexX = static_cast<size_t>((region.left + region.right) / 2.0f);
	auto centreIndexZ = static_cast<size_t>((region.top + region.bottom) / 2.0f);

	// Diamond.
	// Average the corners to get the centre height.
	auto centreY = m_YValues[region.top][region.left];
	centreY += m_YValues[region.top][region.right];
	centreY += m_YValues[region.bottom][region.left];
	centreY += m_YValues[region.bottom][region.right];
	centreY /= 4.0f;
	centreY -= Math::Random(-0.5f, 0.5f) * baseHeight * 2.0f;

	SoftSet(centreIndexX, centreIndexZ, centreY);

	// Square.
	SoftSet(centreIndexX, region.top,   (m_YValues[region.top][region.left]		+ m_YValues[region.top][region.right])		/ 2.0f + (Math::Random(-0.5f, 0.5f) * baseHeight));
	SoftSet(centreIndexX, region.bottom,(m_YValues[region.bottom][region.left]	+ m_YValues[region.bottom][region.right])	/ 2.0f + (Math::Random(-0.5f, 0.5f) * baseHeight));
	SoftSet(region.left,  centreIndexZ, (m_YValues[region.top][region.left]		+ m_YValues[region.bottom][region.left])	/ 2.0f + (Math::Random(-0.5f, 0.5f) * baseHeight));
	SoftSet(region.right, centreIndexZ, (m_YValues[region.top][region.right]	+ m_YValues[region.bottom][region.right])	/ 2.0f + (Math::Random(-0.5f, 0.5f) * baseHeight));

	if (region.right - region.left > 2) {
		baseHeight = baseHeight * std::pow(2.0f, -0.75f);

		m_RegionQueue.emplace(TRegion{ TRect{ region.left, region.top, centreIndexX, centreIndexZ }, baseHeight });
		m_RegionQueue.emplace(TRegion{ TRect{ centreIndexX, region.top, region.right, centreIndexZ }, baseHeight });
		m_RegionQueue.emplace(TRegion{ TRect{ region.left, centreIndexZ, centreIndexX, region.bottom }, baseHeight });
		m_RegionQueue.emplace(TRegion{ TRect{ centreIndexX, centreIndexZ, region.right, region.bottom }, baseHeight });
	}

}

bool CProceduralTerrain::IsDone()
{
	return m_RegionQueue.empty();
}

void CProceduralTerrain::Reset()
{
	m_YValues = std::vector<std::vector<float> >(m_Size, std::vector<float>(m_Size, m_YMin));
	m_IsSet = std::vector<std::vector<bool> >(m_Size, std::vector<bool>(m_Size, false));

	float midHeight = (m_YMin + m_YMax) / 2.0f;

	// Set corners. 
	SoftSet(0, 0, midHeight);
	SoftSet(0, m_Size - 1, midHeight);
	SoftSet(m_Size - 1, 0, midHeight);
	SoftSet(m_Size - 1, m_Size - 1, midHeight);

	TRegion region = { { 0, 0, m_Size - 1, m_Size - 1 }, midHeight };

	m_RegionQueue.push(region);
}

void CProceduralTerrain::Generate()
{
	while (!m_RegionQueue.empty()) {
		Step();
	}
}

void CProceduralTerrain::SetSize(size_t size)
{
	m_Size = size;
}

const size_t CProceduralTerrain::GetSize() const
{
	return m_Size;
}

void CProceduralTerrain::SetYMin(float yMin)
{
	m_YMin = yMin;
}

const float CProceduralTerrain::GetYMin() const
{
	return m_YMin;
}

void CProceduralTerrain::SetYMax(float yMax)
{
	m_YMax = yMax;
}

const float CProceduralTerrain::GetYMax() const
{
	return m_YMax;
}

void CProceduralTerrain::SetHeights(const std::vector<std::vector<float> >& yValues)
{
	m_YValues = yValues;
}

const std::vector<std::vector<float> >& CProceduralTerrain::GetHeights() const
{
	return m_YValues;
}
