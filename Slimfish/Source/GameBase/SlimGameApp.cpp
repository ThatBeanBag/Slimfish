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
#include <Windowsx.h>

// This Include
#include "SlimGameApp.h"

// Local Includes
#include "SlimGameLogic.h"
#include "..\Graphics\D3D11\SlimD3D11Renderer.h"

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

	m_pRenderer.reset(new CD3D11Renderer(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, true));
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
		
		/************************************************************************/
		/* Input Handling
		/************************************************************************/
		// Keys
		case WM_KEYDOWN: {
			EKeyCode keycode = GetKeyCode(wParam, lParam);
			if (keycode != EKeyCode::MAX) {
				g_pApp->m_Input.SetKeyPress(keycode);
			}

			break;
		}
		case WM_KEYUP: {
			EKeyCode keycode = GetKeyCode(wParam, lParam);
			if (keycode != EKeyCode::MAX) {
				g_pApp->m_Input.SetKeyRelease(keycode);
			}

			break;
		}
		// Mouse Buttons
		case WM_LBUTTONDOWN: {
			g_pApp->m_Input.SetMouseButtonPress(EMouseButton::LEFT);
			break;
		}
		case WM_MBUTTONDOWN: {
			g_pApp->m_Input.SetMouseButtonPress(EMouseButton::MIDDLE);
			break;
		}
		case WM_RBUTTONDOWN: {
			g_pApp->m_Input.SetMouseButtonPress(EMouseButton::MIDDLE);
			break;
		}
		case WM_XBUTTONDOWN: {
			if (HIWORD(wParam) == XBUTTON1) {
				g_pApp->m_Input.SetMouseButtonPress(EMouseButton::EXTRA_1);
			}
			else {
				g_pApp->m_Input.SetMouseButtonPress(EMouseButton::EXTRA_2);
			}

			break;
		}
		case WM_LBUTTONUP: {
			g_pApp->m_Input.SetMouseButtonRelease(EMouseButton::LEFT);
			break;
		}
		case WM_MBUTTONUP: {
			g_pApp->m_Input.SetMouseButtonRelease(EMouseButton::MIDDLE);
			break;
		}
		case WM_RBUTTONUP: {
			g_pApp->m_Input.SetMouseButtonRelease(EMouseButton::MIDDLE);
			break;
		}
		case WM_XBUTTONUP: {
			if (HIWORD(wParam) == XBUTTON1) {
				g_pApp->m_Input.SetMouseButtonRelease(EMouseButton::EXTRA_1);
			}
			else {
				g_pApp->m_Input.SetMouseButtonRelease(EMouseButton::EXTRA_2);
			}

			break;
		}
		case WM_MOUSEMOVE: {
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);

			g_pApp->m_Input.SetMousePosition(xPos, yPos);
			break;
		}
		default: {
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

EKeyCode CGameApp::GetKeyCode(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
		case '0': return EKeyCode::NUM_0;
		case '1': return EKeyCode::NUM_1;
		case '2': return EKeyCode::NUM_2;
		case '3': return EKeyCode::NUM_3;
		case '4': return EKeyCode::NUM_4;
		case '5': return EKeyCode::NUM_5;
		case '6': return EKeyCode::NUM_6;
		case '7': return EKeyCode::NUM_7;
		case '8': return EKeyCode::NUM_8;
		case '9': return EKeyCode::NUM_9;
		case VK_NUMPAD0: return EKeyCode::NUM_PAD_0;
		case VK_NUMPAD1: return EKeyCode::NUM_PAD_1;
		case VK_NUMPAD2: return EKeyCode::NUM_PAD_2;
		case VK_NUMPAD3: return EKeyCode::NUM_PAD_3;
		case VK_NUMPAD4: return EKeyCode::NUM_PAD_4;
		case VK_NUMPAD5: return EKeyCode::NUM_PAD_5;
		case VK_NUMPAD6: return EKeyCode::NUM_PAD_6;
		case VK_NUMPAD7: return EKeyCode::NUM_PAD_7;
		case VK_NUMPAD8: return EKeyCode::NUM_PAD_8;
		case VK_NUMPAD9: return EKeyCode::NUM_PAD_9;
		case 'A': return EKeyCode::A;
		case 'B': return EKeyCode::B;
		case 'C': return EKeyCode::C;
		case 'D': return EKeyCode::D;
		case 'E': return EKeyCode::E;
		case 'F': return EKeyCode::F;
		case 'G': return EKeyCode::G;
		case 'H': return EKeyCode::H;
		case 'I': return EKeyCode::I;
		case 'J': return EKeyCode::J;
		case 'K': return EKeyCode::K;
		case 'L': return EKeyCode::L;
		case 'M': return EKeyCode::M;
		case 'N': return EKeyCode::N;
		case 'O': return EKeyCode::O;
		case 'P': return EKeyCode::P;
		case 'Q': return EKeyCode::Q;
		case 'R': return EKeyCode::R;
		case 'S': return EKeyCode::S;
		case 'T': return EKeyCode::T;
		case 'U': return EKeyCode::U;
		case 'W': return EKeyCode::W;
		case 'V': return EKeyCode::V;
		case 'X': return EKeyCode::X;
		case 'Y': return EKeyCode::Y;
		case 'Z': return EKeyCode::Z;
		case VK_F1: return EKeyCode::F1;
		case VK_F2: return EKeyCode::F2;
		case VK_F3: return EKeyCode::F3;
		case VK_F4: return EKeyCode::F4;
		case VK_F5: return EKeyCode::F5;
		case VK_F6: return EKeyCode::F6;
		case VK_F7: return EKeyCode::F7;
		case VK_F8: return EKeyCode::F8;
		case VK_F9: return EKeyCode::F9;
		case VK_F10: return EKeyCode::F10;
		case VK_F11: return EKeyCode::F11;
		case VK_F12: return EKeyCode::F12;
		case VK_LEFT: return EKeyCode::LEFT_ARROW;
		case VK_RIGHT: return EKeyCode::RIGHT_ARROW;
		case VK_UP: return EKeyCode::UP_ARROW;
		case VK_DOWN: return EKeyCode::DOWN_ARROW;
		case VK_BACK: return EKeyCode::BACK_SPACE;
		case VK_TAB: return EKeyCode::TAB;
		case VK_CLEAR: return EKeyCode::CLEAR;
		case VK_RETURN: return EKeyCode::RETURN;
		case VK_EXECUTE: return EKeyCode::ENTER;
		case VK_CAPITAL: return EKeyCode::CAPS_LOCK;
		case VK_ESCAPE: return EKeyCode::ESCAPE;
		case VK_SPACE: return EKeyCode::SPACE;
		case VK_INSERT: return EKeyCode::INSERT;
		case VK_DELETE: return EKeyCode::DELETE;
		case VK_HOME: return EKeyCode::HOME;
		case VK_END: return EKeyCode::END;
		case VK_PRIOR: return EKeyCode::PAGE_UP;
		case VK_NEXT: return EKeyCode::PAGE_DOWN;
		case VK_SNAPSHOT: return EKeyCode::PRINT_SCREEN;
		case VK_PAUSE: return EKeyCode::PAUSE;
		case VK_OEM_3: return EKeyCode::TELDA;
		case VK_NUMLOCK: return EKeyCode::NUM_LOCK;
		case VK_SCROLL: return EKeyCode::SCROLL_LOCK;
		case VK_OEM_PLUS: return EKeyCode::EQUALS;
		case VK_OEM_MINUS: return EKeyCode::HYPHEN;
		case VK_SUBTRACT: return EKeyCode::NUM_PAD_MINUS;
		case VK_ADD: return EKeyCode::NUM_PAD_PLUS;
		case VK_DIVIDE: return EKeyCode::NUM_PAD_DIVIDE;
		case VK_MULTIPLY: return EKeyCode::NUM_PAD_MULTIPLY;
		case VK_OEM_COMMA: return EKeyCode::COMMA;
		case VK_OEM_PERIOD: return EKeyCode::PERIOD;
		case VK_OEM_2: return EKeyCode::FORWARD_SLASH;
		case VK_OEM_5: return EKeyCode::BACK_SLASH;
		case VK_OEM_1: return EKeyCode::SEMI_COLON;
		case VK_OEM_7: return EKeyCode::QUOTE;
		case VK_OEM_4: return EKeyCode::LEFT_BRACKET;
		case VK_OEM_6: return EKeyCode::RIGHT_BRACKET;
		case VK_SHIFT:		// Fall through.
		case VK_CONTROL:	// Fall through.
		case VK_MENU: {
			UINT scancode = (lParam & 0x00ff0000) >> 16;
			WPARAM mappedKey = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
			switch (mappedKey) {
				case VK_LSHIFT: return EKeyCode::LEFT_SHIFT;
				case VK_RSHIFT: return EKeyCode::RIGHT_SHIFT;
				case VK_LCONTROL: return EKeyCode::LEFT_CONTROL;
				case VK_RCONTROL: return EKeyCode::RIGHT_CONTROL;
				case VK_LMENU: return EKeyCode::LEFT_ALT;
				case VK_RMENU: return EKeyCode::RIGHT_ALT;
				default: break;
			}
		}

		default: break;
	}

	return EKeyCode::MAX;
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
		m_pGame->HandleInput(m_Input, m_DeltaTime);
		m_pGame->Update(m_DeltaTime);
	}

	m_Input.FlushPerFrameStates();

	m_DeltaTime = static_cast<float>(timeGetTime() - startTime) / 1000.0f;
}

void CGameApp::Quit()
{
	m_bIsQuitting = true;
}

bool CGameApp::OnClose()
{
	m_pGame.reset(nullptr);
	m_pRenderer.reset(nullptr);

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
