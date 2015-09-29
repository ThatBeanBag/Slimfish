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

#define NOMINMAX

// Library Includes
#include <Windows.h>

// Local Includes
#include "SlimInput.h"

namespace Slim {

// Forward Declaration.
class CGameLogic;
class ARenderer;

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
	virtual std::unique_ptr<CGameLogic> VCreateGame() = 0;

	virtual bool VInitialise(HINSTANCE hInstance, LPSTR lpCmdLine, HWND hWnd, size_t screenHeight, size_t screenWidth);

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static EKeyCode GetKeyCode(WPARAM wParam, LPARAM lParam);

	HWND GetHWND() { return m_hWnd; }

	void Update();
	void Quit();
	bool OnClose();

	CGameLogic* GetGame();
	ARenderer* GetRenderer();

	bool IsRunning() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;

	std::unique_ptr<CGameLogic> m_pGame;
	std::unique_ptr<ARenderer> m_pRenderer;
	CInput m_Input;

	bool m_bIsRunning;
	bool m_bIsQuitting;
	float m_DeltaTime;
};

extern CGameApp* g_pApp;

}

#endif	// __SLIMGAMEAPP_H__