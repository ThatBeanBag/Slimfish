// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ProceduralTerrainGenerationApp.h
// Description	: CProceduralTerrainGenerationApp declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __PROCEDURALTERRAINGENERATIONAPP_H__
#define __PROCEDURALTERRAINGENERATIONAPP_H__

// PCH
#include "ProceduralTerrainGenerationStd.h"

// Library Includes

// Local Includes
#include <GameBase\SlimGameApp.h>

class CProceduralTerrainGenerationApp : public CGameApp {
	// Member Functions
public:
	CProceduralTerrainGenerationApp();
	~CProceduralTerrainGenerationApp();

	virtual std::string VGetTitle() override;
	virtual HICON VGetIcon() override;
	virtual std::unique_ptr<CGameLogic> VCreateGame() override;

protected:
private:
	// Member Variables
public:
protected:
private:
	
};

extern CProceduralTerrainGenerationApp g_ProceduralTerrainGenerationApp;

#endif // __PROCEDURALTERRAINGENERATIONAPP_H__