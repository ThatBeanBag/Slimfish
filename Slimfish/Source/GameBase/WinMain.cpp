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
#include "SlimStd.h"

// Library Includes
#include <new>	// For bad alloc.

// Local Includes
#include "SlimGameApp.h"

using namespace Slim;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try {
		// Initialise the logging system before doing anything else.
		//Logger::InitialiseLogger("");	// TODO: Initialise logger from file.

		MSG msg;

		ZeroMemory(&msg, sizeof(msg));

		// Perform the initialization sequence.
		if (!g_pApp->VInitialise(hInstance, lpCmdLine, NULL, 860, 640)) {
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
				g_pApp->Update();
			}
		}


		// Return to windows.
		return static_cast<int>(msg.wParam);
	}
	catch (std::bad_alloc& badAlloc) {
		std::string strMessage = "Out of memory ";
		strMessage += badAlloc.what();
		MessageBoxA(nullptr, strMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		abort();
	}
	catch (...) {
		MessageBoxA(nullptr, "Unknown error occurred.", "Error", MB_OK | MB_ICONERROR);
	}
}