// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: WinMain.cpp
// Description	: WinMain implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes
#include <new>

// Local Includes
#include "GameApp.h"

//==========================================================
// WinMain() is the entry point to the application.
//
// It triggers the initialization sequence and enters a 
// message processing loop. Giving control to the CGameApp.
//==========================================================

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	try {
		// Initialise the logging system before doing anything else.
		Logger::InitialiseLogger("");	// TODO: Initialise logger from file.

		MSG msg;

		ZeroMemory(&msg, sizeof(msg));

		// Perform the initialization sequence.
		if (!g_pApp->VInitialise(_hInstance, _lpCmdLine, NULL, true)) {
			// Failed to initialize.
			g_pApp->OnClose();
			return 0;
		}

		while (msg.message != WM_QUIT && g_pApp->IsRunning()) {
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				// Translate any accelerator keys.
				TranslateMessage(&msg);

				// Send the message to the window proc.
				DispatchMessage(&msg);
			}
			else {
				// Update the game.
				g_pApp->PollGamepads();
				g_pApp->Update();
			}
		}


		// Return to windows.
		return static_cast<int>(msg.wParam);
	}
	catch (std::bad_alloc& _badAlloc) {
		std::string strMessage = "Out of memory";
		strMessage += _badAlloc.what();
		MessageBoxA(nullptr, strMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		abort();
	}
}