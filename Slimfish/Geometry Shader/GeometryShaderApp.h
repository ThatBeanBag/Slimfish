// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GeometryShaderApp.h
// Description	: CGeometryShaderApp declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __GEOMETRYSHADERAPP_H__
#define __GEOMETRYSHADERAPP_H__

// PCH
#include "GeometryShaderStd.h"

// Library Includes

// Local Includes
#include <GameBase\SlimGameApp.h>

class CGeometryShaderApp : public CGameApp {
	// Member Functions
public:
	CGeometryShaderApp();
	~CGeometryShaderApp();

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

extern CGeometryShaderApp g_GeometryShaderApp;

#endif // __GEOMETRYSHADERAPP_H__