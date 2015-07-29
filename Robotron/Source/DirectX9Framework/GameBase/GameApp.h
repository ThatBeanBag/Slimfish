// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GameApp.h
// Description	: CGameApp declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __GAMEAPP_H__
#define __GAMEAPP_H__

// Library Includes
#include <d3d9.h>
#include <d3dx9.h>
#include <Xinput.h>

// Local Includes
#include "GameLogic.h"
#include "../Renderer/Renderer.h"
#include "../Resource/ResourceManager.h"

#define WINDOW_CLASS_NAME L"WINCLASS1"

class ABaseNetwork;
class CClientNetwork;
class CServerNetwork;

class CGameApp {
	// Member Functions
public:
	CGameApp();
	virtual ~CGameApp();

	// You must define these in an inherited class.
	virtual TCHAR* VGetTitle() = 0;
	virtual HICON VGetIcon() = 0;
	virtual unique_ptr<CGameLogic> VCreateAndGetGame() = 0;

	// Win32 Specific Functions
	HWND GetHwnd();
	HINSTANCE GetInstance();

	virtual bool VInitialise(HINSTANCE _hInstance, 
							 LPSTR _lpCmdLine, 
							 HWND _hWnd = NULL,
							 bool _bIsWindowedMode = true,
							 int _iScreenWidth = g_kiDEF_SCREEN_WIDTH, 
							 int _iScreenHeight = g_kiDEF_SCREEN_HEIGHT);

	static LRESULT CALLBACK MsgProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);	// The message handler.
	void HideCursor(bool _bHideCursor);

	// GameApp Specific Functions
	void Update();
	bool OnRestore();
	void PollGamepads();

	void ToggleWindowedMode();
	void SetPaused(bool _bPaused);

	// Cleans up and frees resources before setting m_bIsRunning to false.
	LRESULT OnClose();

	// Sets m_bIsQuitting to true, which ultimately safely closes the application.
	void Quit();

	CClientNetwork* CreateClientNetwork();
	CServerNetwork* CreateServerNetwork(const std::string& _strServerName);

	void CloseNetwork();

	// Accessors to the various components of the application.
	CGameLogic* GetGame();
	IRenderer* GetRenderer();
	ABaseNetwork* GetNetwork();

	int GetRandomNumber(int _iMinInclusive, int _iMaxExclusive);
	float GetRandomNumberReal(float _fMinInclusive, float _fMaxInclusive);

	float GetAspectRatio() const;
	int GetScreenWidth() const;
	int GetScreenHeight() const;
	CPoint GetScreenCentre() const;

	bool IsWindowed() const;
	bool IsAppMinimized() const;
	bool IsRunning() const;
	bool IsPaused() const;
protected:
private:

	// Member Variables
public:
protected:
private:
	unique_ptr<CGameLogic> m_pGame;

	HWND m_hWnd;			// The handle to the window.
	HINSTANCE m_hInstance;	// The handle to the instance.

	unique_ptr<IRenderer> m_pRenderer;
	unique_ptr<ABaseNetwork> m_pNetwork;

	std::mt19937 m_randomGenerator;

	bool m_bIsRunning;		// True if the application is initialized and running, false otherwise.
	bool m_bIsQuitting;		// True if the application has been requested to quit, false otherwise.
	bool m_bIsPaused;		// True if the application should update with a delta tick of 0.
	bool m_bIsMinimized;	// True if the application is currently minimized.
	bool m_bIsCursorHidden;	// True if the mouse cursor shouldn't be visible, false if it should be shown.
	float m_fDeltaTime;		// The delta time of the previous tick. Used to estimate the current ticks delta time.

	CPoint m_centreOfScreen;

	CGamepadManager m_gamepadManager;
	float m_fSmoothedTime;
};

extern CGameApp* g_pApp;

#endif