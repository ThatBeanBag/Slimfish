// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimGameApp.cpp
// Description	: CSlimGameApp implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimGameApp.h"

// Local Includes
#include "SlimGameLogic.h"
#include "..\Graphics\D3D10\SlimD3D10Renderer.h"

namespace Slim {

	CGameApp* g_pApp = nullptr;

	CGameApp::CGameApp()
		:m_hWnd(nullptr),
		m_hInstance(nullptr),
		m_bIsRunning(false),
		m_bIsQuitting(false),
		m_DeltaTime(0.0f),
		m_pGame(nullptr),
		m_pRenderer(nullptr)
	{
		g_pApp = this;
	}

	CGameApp::~CGameApp()
	{
		OnClose();
	}

	bool CGameApp::VInitialise(HINSTANCE hInstance, LPSTR lpCmdLine, HWND hWnd, size_t screenHeight, size_t screenWidth)
	{
		CLogger::GetInstance()->CreateLog("SLIM.log");
		SLIM_INFO_IF(false) << "TestMessge";

		m_hInstance = hInstance;

		static LPCWSTR windowClassName = L"WINCLASS1";

		// Define the window class.
		WNDCLASSEX winClass;

		winClass.cbSize = sizeof(WNDCLASSEX);
		winClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		winClass.lpfnWndProc = CGameApp::MsgProc;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hInstance = m_hInstance;
		winClass.hIcon = VGetIcon();
		winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		winClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		winClass.lpszMenuName = NULL;
		winClass.lpszClassName = windowClassName;
		winClass.hIconSm = VGetIcon();

		if (!RegisterClassEx(&winClass)) {
		// Failed to register.
		// TODO: display error.
			return false;
		}

		std::string title = VGetTitle();
		std::wstring wsTitle;

		wsTitle.append(title.begin(), title.end());

		m_hWnd = CreateWindowEx(
			NULL,
			windowClassName,
			wsTitle.c_str(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			screenHeight, screenWidth,
			NULL,
			NULL,
			m_hInstance,
			NULL);

		if (!m_hWnd) {
		// Window failed to create successfully.
		// TODO: display error.
			return false;
		}

		RECT clientRect;
		GetClientRect(m_hWnd, &clientRect);

		m_pRenderer.reset(new CD3D10Renderer(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, true));
		if (!m_pRenderer) {
		// Failed to create the renderer.
		// TODO: display error.
			return false;
		}

		if (!m_pRenderer->VInitialize()) {
		// TODO: display error.
			return false;
		}

		m_pGame = VCreateGame();

		if (!m_pGame) {
		// Failed to create the game.
		// TODO: display error.
			return false;
		}

		if (!m_pGame->Initialise()) {
		// Failed to initialise game logic.
		// TODO: display error.
		}

		timeBeginPeriod(1);

		m_bIsRunning = true;	// Initialisation complete we are now running.

		return true;
	}

	LRESULT CALLBACK CGameApp::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
			case WM_SIZE: {
				g_pApp->GetRenderer()->Resize(0, 0);
				break;
			}
			case WM_CLOSE: {
				if (g_pApp->OnClose()) {
					return 0;
				}

				break;
			}
			default: {
				break;
			}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void CGameApp::Update()
	{
		if (m_bIsQuitting) {
			OnClose();

			return;
		}

		float startTime = static_cast<float>(timeGetTime());

		if (m_pGame && m_pRenderer) {
			// Render.
			m_pRenderer->VPreRender();
			m_pGame->Render();
			m_pRenderer->VPostRender();

			// Update.
			m_pGame->Update(m_DeltaTime);
		}

		m_DeltaTime = static_cast<float>(timeGetTime() - startTime) / 1000.0f;
	}

	void CGameApp::Quit()
	{
		m_bIsQuitting = true;
	}

	bool CGameApp::OnClose()
	{
		DestroyWindow(m_hWnd);
		PostQuitMessage(0);

		return true;
	}

	CGameLogic* CGameApp::GetGame()
	{
		return m_pGame.get();
	}

	ARenderer* CGameApp::GetRenderer()
	{
		return m_pRenderer.get();
	}

	bool CGameApp::IsRunning() const
	{
		return m_bIsRunning;
	}

}
