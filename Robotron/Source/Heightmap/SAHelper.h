// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SAHepler.h
// Description	: CSAHepler declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SAHELPER_H__
#define	__SAHELPER_H__

// Library Includes
#include <mutex>
#include <thread>

// Local Includes

// Simulated Annealing for a height map.
// State type - CPoint. (point on the height map image).
// Energy function - Gets the height of the CPoint from the image.
// Schedule function - Decrement temparature linearly.
// Perturb - randomly select a point adjecent to us.

class CSAHelper {
	// Member Functions
public:
	CSAHelper();
	~CSAHelper();

	CPoint Anneal(CImage _heightMap, float _fInitialTemp, float _fFinalTemp, int _iIterMax);

	float GetHeight(const CPoint& _krImagePoint) const;	// Energy function - gets the height of the CPoint from the image.
	float ScheduleTemperature(float _fTemperature); // Temperaure function - reduces the temperature.
	CPoint GetNext(const CPoint& _krPoint);			// Perturb function - randomly selects a point adjecent to us.

	// Gets the unit position (between -0.5, -0.5, -0.5 and 0.5, 0.5, 0.5) of a point on the heightmap.
	CVec3 GetUnitPosition(const CPoint& _krImagePoint) const;

	// Gets the current state as a unit position.
	CVec3 GetCurrentPosition() const;
protected:
private:
	// Member Variables
public:
	static const float s_kfDEF_INITIAL_TEMP;
protected:
private:
	float m_fInitialTemperature;
	CImage m_heightMap;
	CPoint m_currentState;
	std::thread m_thread;

	bool m_bStopWaiting;

	mutable std::recursive_mutex m_mutex;
};

#endif	// __SIMULATEDANNEALINGHELPER_H__