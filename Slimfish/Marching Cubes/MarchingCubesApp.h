// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MarchingCubesApp.h
// Description	: CMarchingCubesApp declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MARCHINGCUBESAPP_H__
#define __MARCHINGCUBESAPP_H__

// PCH
#include "MarchingCubesStd.h"

// Library Includes

// Local Includes
#include <GameBase\SlimGameApp.h>

class CMarchingCubesApp : public CGameApp {
	// Member Functions
public:
	CMarchingCubesApp();
	~CMarchingCubesApp();

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

extern CMarchingCubesApp g_MarchingCubesApp;

#endif // __MARCHINGCUBESAPP_H__