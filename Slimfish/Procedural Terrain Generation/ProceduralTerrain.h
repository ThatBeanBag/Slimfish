// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ProceduralTerrain.h
// Description	: CProceduralTerrain declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __PROCEDURALTERRAIN_H__
#define __PROCEDURALTERRAIN_H__

// Library Includes
#include <queue>

// Local Includes

struct TTerrainVertex {
	CVector3 position;
	CVector3 normal;
	float u;
	float v;
};

class CProceduralTerrain {
	struct TRect {
		size_t left;
		size_t top;
		size_t right;
		size_t bottom;
	};

	struct TRegion {
		TRect rect;
		float baseHeight;
	};

	// Member Functions
public:
	CProceduralTerrain(size_t size, float yMin, float yMax);
	~CProceduralTerrain();

	void Step();
	void SoftSet(size_t x, size_t z, float height);

	void DiamondSquare(const TRect& region, float baseHeight);

	bool IsDone();
	void Reset();
	void Generate();

	/** Set the size. @author Hayden Asplet */
	void SetSize(size_t size);
	/** Get the size. @author Hayden Asplet */
	const size_t GetSize() const;
	/** Set the yMin. @author Hayden Asplet */
	void SetYMin(float yMin);
	/** Get the yMin. @author Hayden Asplet */
	const float GetYMin() const;
	/** Set the yMax. @author Hayden Asplet */
	void SetYMax(float yMax);
	/** Get the yMax. @author Hayden Asplet */
	const float GetYMax() const;
	/** Set the yValues. @author Hayden Asplet */
	void SetHeights(const std::vector<std::vector<float> >& yValues);
	/** Get the yValues. @author Hayden Asplet */
	const std::vector<std::vector<float> >& GetHeights() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	std::vector<std::vector<float> > m_YValues;
	std::vector<std::vector<bool> > m_IsSet;

	std::queue<TRegion> m_RegionQueue;

	size_t m_Size;
	float m_YMin;
	float m_YMax;
};

#endif	// __PROCEDURALTERRAIN_H__
