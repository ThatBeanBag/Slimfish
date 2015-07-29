// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: HeightMapApp.h
// Description	: HeightMapApp declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __HEIGHTMAPAPP_H__
#define __HEIGHTMAPAPP_H__

// Library Inlcudes

// Local Includes

class CHeightMapApp : public CGameApp {
	// Member Functions
public:
	CHeightMapApp();
	virtual ~CHeightMapApp();

	virtual TCHAR* VGetTitle();
	virtual HICON VGetIcon();
	virtual unique_ptr<CGameLogic> VCreateAndGetGame();
protected:
private:

	// Member Variables
public:
protected:
private:
};

extern CHeightMapApp g_heightMapApp;

#endif	// __HEIGHTMAPAPP_H__