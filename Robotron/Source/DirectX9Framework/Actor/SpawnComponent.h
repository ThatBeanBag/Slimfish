// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SpawnComponent.h
// Description	: CSpawnComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SPAWNCOMPONENT_H__
#define __SPAWNCOMPONENT_H__

// Library Includes

// Local Includes

class CSpawnComponent {
	// Member Functions
public:
	CSpawnComponent();
	~CSpawnComponent();

	
protected:
private:
	// Member Variables
public:
protected:
private:
	std::vector<CMatrix4x4> m_vecSpawns;
};

#endif	// __SPAWNCOMPONENT_H__