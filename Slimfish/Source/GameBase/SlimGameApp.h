// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimGameApp.h
// Description	: CSlimGameApp declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMGAMEAPP_H__
#define __SLIMGAMEAPP_H__

// Library Includes

// Local Includes

namespace Slim {

// Forward Declaration.


/** Class encapsulating the application layer.
@remarks
	
*/
class CGameApp {
	// Member Functions
public:
	CGameApp();
	virtual ~CGameApp();

	virtual std::string VGetTitle() = 0;
	virtual HICON VGetIcon() = 0;

	virtual bool VInitialise(HINSTANCE hInstance, LPSTR lpCmdLine, HWND hWnd, size_t screenHeight, size_t screenWidth);

	void Update();
protected:
private:
	// Member Variables
public:
protected:
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
};

}

#endif	// __SLIMGAMEAPP_H__