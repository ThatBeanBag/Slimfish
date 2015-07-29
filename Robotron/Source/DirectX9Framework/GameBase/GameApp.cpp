// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GameApp.cpp
// Description	: CGameApp implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//


// PCH
#include "GameStd.h"

// Library Includes
#include <Windowsx.h>

// This Include
#include "GameApp.h"

// Local Includes
#include "../Renderer/D3D9Renderer.h"
#include "../Resource/ResourceManager.h"
#include "../Network/BaseNetwork.h"
#include "../Network/ClientNetwork.h"
#include "../Network/ServerNetwork.h"

#pragma comment(lib, "XInput.lib")

CGameApp* g_pApp = NULL;

CGameApp::CGameApp()
	:m_hWnd(NULL),
	m_hInstance(NULL),
	m_bIsRunning(false),
	m_bIsQuitting(false),
	m_fDeltaTime(0.0f),
	m_pGame(nullptr),
	m_pRenderer(nullptr)
{
	g_pApp = this;	// When an app is constructed, any app, it becomes the global app of the game.
}

CGameApp::~CGameApp()
{

}

HWND CGameApp::GetHwnd()
{
	return m_hWnd;
}

HINSTANCE CGameApp::GetInstance()
{
	return m_hInstance;
}

bool CGameApp::VInitialise(HINSTANCE _hInstance, LPSTR _lpCmdLine, HWND _hWnd, bool _bIsWindowedMode, int _iScreenWidth, int _iScreenHeight)
{
	m_hInstance = _hInstance;

	// Define the window class.
	WNDCLASSEX winClass;

	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = MsgProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = m_hInstance;
	winClass.hIcon = VGetIcon();
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = WINDOW_CLASS_NAME;
	winClass.hIconSm = VGetIcon();

	if (!RegisterClassEx(&winClass)) {
	// Failed to register.
		DEBUG_ERROR("Failed to register window class.");
		return false;
	}

	// Create the window.
	m_hWnd = CreateWindowEx(NULL,							 // Extended style.
							WINDOW_CLASS_NAME,				 // Class.
							VGetTitle(),					 // Title.
							WS_OVERLAPPEDWINDOW | WS_VISIBLE,// Window flags.
							CW_USEDEFAULT, CW_USEDEFAULT,	 // Initial x, y.
							_iScreenWidth, _iScreenHeight,	 // Initial width, height.
							NULL,							 // Handle to parent.
							NULL,							 // Handle to menu.
							m_hInstance,					 // Instance of this application.
							NULL);							 // Extra creation parameters.
	if (!m_hWnd) {
	// Window failed to create successfully.
		DEBUG_ERROR("Failed to create the window.");
		return false;
	}

	// Create the renderer.
	m_pRenderer.reset(new CD3D9Renderer);
	if (!m_pRenderer) {
		DEBUG_ERROR("Out of memory.");
		return false;
	}

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	if (!m_pRenderer->VInitialize(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, _bIsWindowedMode)) {
	// Failed to initialize the renderer.
		DEBUG_ERROR("Failed to initialize renderer");
		return false;
	}

//	m_pRenderer->VSetRenderState(RS_FILLMODE, FILL_SOLID);
//	m_pRenderer->VSetRenderState(RS_SHADEMODE, SHADE_GOURAUD);
//	m_pRenderer->VSetRenderState(RS_MULTISAMPLEANTIALIAS, TRUE);
//	m_pRenderer->VSetRenderState(RS_COLOURVERTEX, TRUE);
//	m_pRenderer->VSetRenderState(RS_DIFFUSEMATERIALSOURCE, MCS_COLOUR1);
	m_pRenderer->VSetRenderState(RS_CULLMODE, CULL_CCW);
//	m_pRenderer->VSetRenderState(RS_ZENABLE, ZB_TRUE);

//	m_pRenderer->VSetRenderState(RS_LIGHTING, FALSE);
//	m_pRenderer->VSetRenderState(RS_FILLMODE, FILL_WIREFRAME);
	m_pRenderer->VSetRenderState(RS_SPECULAR, TRUE);
	m_pRenderer->VSetRenderState(RS_AMBIENTMATERIALSOURCE, MCS_COLOUR1);
	m_pRenderer->VSetRenderState(RS_DIFFUSEMATERIALSOURCE, MCS_MATERIAL);
	m_pRenderer->VSetRenderState(RS_NORMALIZENORMALS, TRUE);
	m_pRenderer->VSetBackgroundColour(CreateColourRGB(240, 240, 240));
	m_pRenderer->VSetAmbientColour(CreateColourRGB(0, 0, 0));
	
	// Load the resource manager.
	/*m_pResourceManager.reset(new CResourceManager);
	if (!m_pResourceManager) {
		DEBUG_ERROR("Out of memory.");
		return false;
	}*/

	// Seed the random number generator.
	std::random_device randomDevice;
	m_randomGenerator.seed(randomDevice());
	
	m_pGame = VCreateAndGetGame();

	if (!m_pGame) {
	// Logic failed to create.
		DEBUG_ERROR("Out of memory.");
		return false;
	}

	if (!m_pGame->Initialise()) {
	// Failed to initialize game logic.
		DEBUG_ERROR("Failed to initiliaze game logic.");
		return false;
	}

	timeBeginPeriod(1);

	m_bIsRunning = true;	// The application is now running.

	return true;
}

LRESULT CALLBACK CGameApp::MsgProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg) {
		case WM_CREATE: {
			// Do initialization here...

			// Return success.
			return 0;
			break;
		}
		case WM_SETCURSOR: {
			if (g_pApp->m_bIsCursorHidden) {
				SetCursor(NULL);

				// We've handled it.
				return 0;
			}
			else {
			// Let windows handle it.
				break;
			}

			break;
		}
		
		case WM_SYSCOMMAND: {
			switch (_wParam) {
				case SC_MAXIMIZE: {
					if (g_pApp->GetRenderer()->VIsWindowed()) {
						g_pApp->ToggleWindowedMode();
						return 0;
					}
					break;
				}
			}
			break;
		}

		case WM_COMMAND: {
			break;
		}

		case WM_KILLFOCUS: {
			g_pApp->m_bIsMinimized = true;
			g_pApp->HideCursor(false);

			// Let windows handle the rest.
			break;
		}
		case WM_SETFOCUS: {
			g_pApp->m_bIsMinimized = false;

			// Restore the application.
			g_pApp->OnRestore();

			// Let windows handle the rest.
			break;
		}

		case WM_ACTIVATEAPP: {
			if (_wParam) {
				// Reset the device.
				g_pApp->m_pRenderer->VReset();

				g_pApp->m_bIsMinimized = false;

				// Restore the application.
				g_pApp->OnRestore();

				// Return success.
				return 0;
			}

			break;
		}

		case WM_MOVE: {
			g_pApp->m_pRenderer->VOnScreenMoved();

			// Return success.
			return 0;

			break;
		}

		case WM_SIZE: {
			int iScreenWidth = LOWORD(_lParam);
			int iScreenHeight = HIWORD(_lParam);

			if (!g_pApp->IsAppMinimized()) {
				g_pApp->m_pRenderer->VUpdateScreenSize(iScreenWidth, iScreenHeight);
			}
			
			break;
		}

		case WM_PAINT: {
			HDC hdc;
			PAINTSTRUCT ps;

			hdc = BeginPaint(_hWnd, &ps);

			// Do painting stuff here...

			EndPaint(_hWnd, &ps);

			// Return success.
			return 0;
			break;
		}

		case WM_CLOSE: {
			return g_pApp->OnClose();

			break;
		}

		// User Input
		case WM_CHAR:			// Fall through.
		case WM_KEYDOWN:		// Fall through.
		case WM_KEYUP:			// Fall through.
		case WM_MOUSEMOVE:		// Fall through.
		case WM_MOUSEWHEEL:		// Fall through.
		case WM_LBUTTONDOWN:	// Fall through.
		case WM_LBUTTONUP:		// Fall through.
		case WM_MBUTTONDOWN:	// Fall through.
		case WM_MBUTTONUP:		// Fall through.
		case WM_RBUTTONDOWN:	// Fall through.
		case WM_RBUTTONUP: {
			TAppMsg appMsg;

			POINT mousePosition;
			GetCursorPos(&mousePosition);
			ScreenToClient(g_pApp->GetHwnd(), &mousePosition);

			appMsg.m_mousePosition = CPoint(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam));
			appMsg.m_uckey = static_cast<unsigned char>(_wParam);
			appMsg.m_sDeltaMouseWheel = GET_WHEEL_DELTA_WPARAM(_wParam);

			switch (_msg) {
				case WM_CHAR: {
					appMsg.m_eMessageType = MT_CHAR;
					break;
				}
				case WM_KEYDOWN: {
					appMsg.m_eMessageType = MT_KEYDOWN;
					break;
				}
				case WM_KEYUP: {
					appMsg.m_eMessageType = MT_KEYUP;
					break;
				}
				case WM_MOUSEMOVE: {
					appMsg.m_eMessageType = MT_MOUSE_MOVE;
					break;
				}
				case WM_MOUSEWHEEL: {
					appMsg.m_eMessageType = MT_MOUSE_WHEEL;
					break;
				}
				case WM_LBUTTONDOWN: {
					appMsg.m_eMessageType = MT_LMB_DOWN;
					break;
				}
				case WM_LBUTTONUP: {
					appMsg.m_eMessageType = MT_LMB_UP;
					break;
				}
				case WM_MBUTTONDOWN: {
					appMsg.m_eMessageType = MT_MMB_DOWN;
					break;
				}
				case WM_MBUTTONUP: {
					appMsg.m_eMessageType = MT_MMB_UP;
					break;
				}
				case WM_RBUTTONDOWN: {
					appMsg.m_eMessageType = MT_RMB_DOWN;
					break;
				}
				case WM_RBUTTONUP: {
					appMsg.m_eMessageType = MT_RMB_UP;
					break;
				}
				default: {
					break;
				}
			}

			if (g_pApp->m_pGame->OnMsgProc(appMsg)) {
				// Return success.
				return 0;
			}

			break;
		}

		default: {
			break;
		}

	}

	// Give control back windows to process messages that we did not take care of.
	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}


void CGameApp::HideCursor(bool _bHideCursor)
{
	m_bIsCursorHidden = _bHideCursor;

	if (!m_bIsCursorHidden) {
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
}

void CGameApp::Update()
{
	if (m_bIsQuitting) {
		g_pApp->OnClose();

		return;
	}

	if (m_pRenderer->VCheckLostDevice() == LDERR_DEVICENOTRESET ||
		m_pRenderer->VCheckLostDevice() == LDERR_DEVICELOST) {
		g_pApp->m_bIsMinimized = true;
	}

	float fStartTime = static_cast<float>(timeGetTime());

	if (m_pGame) {
	// Update and render
		CEventManager::GetInstance()->FireOffEvents(20);

		if (m_pNetwork) {
			// Receive network events.
			m_pNetwork->DoSelect(0);
		}

		if (m_bIsPaused) {
		// Are we paused?
			// Update the logic, but with no delta time.
			m_pGame->Update(0.0f);
		}
		else {
		// Not paused.
			// Update the logic as usual.
			m_pGame->Update(m_fDeltaTime);
			m_gamepadManager.Update(m_fDeltaTime);
		}
	}

	if (!m_bIsMinimized) {
	// Are we not minimized?
		m_pRenderer->VPreRender();
		m_pGame->Render();

#ifdef _DEBUG
		int iFPS = static_cast<int>(1.0f / m_fSmoothedTime);
		std::string fpsText = ToString(iFPS);
		m_pRenderer->VDrawText(fpsText.c_str(), CRect(0, 0, 100, 40), 18, DT_SINGLELINE, g_kCOLOUR_BLUE);
#endif

		m_pRenderer->VPostRender();
	}

	/*while (timeGetTime() - fStartTime < 1000 / 60) {
		Sleep(1);
	}*/

	float fLastFrame = m_fDeltaTime;
	m_fDeltaTime = static_cast<float>(timeGetTime() - fStartTime) / 1000.0f;

	m_fSmoothedTime = m_fDeltaTime * 0.9f + fLastFrame * 0.1f;
}

bool CGameApp::OnRestore()
{
	if (!m_pGame->OnRestore()) {
		return false;
	}

	return true;
}

void CGameApp::ToggleWindowedMode()
{
	m_pRenderer->VToggleWindowedMode();
}

void CGameApp::SetPaused(bool _bPaused)
{
	m_bIsPaused = _bPaused;
}

LRESULT CGameApp::OnClose()
{
	m_bIsRunning = false;

	timeEndPeriod(1);

	// Delete these manually as g_pApp's destructor won't be called for a while yet.
	// This avoids the situation, where g_pApp lingers longer than the singleton CEventManager.
	// It may be a little bit useless storing these as unique pointers if we are just going to delete
	// them manually but it's always nice to have RAII incase OnClose() is never called.
	SafeDelete(m_pGame.release());
	SafeDelete(m_pNetwork.release());
	SafeDelete(m_pRenderer.release());

	// Kill the application
	DestroyWindow(m_hWnd);
	PostQuitMessage(0);

	// Return success.
	return 0;
}

void CGameApp::Quit()
{
	m_bIsQuitting = true;
}

CClientNetwork* CGameApp::CreateClientNetwork()
{
	if (m_pNetwork) {
		DEBUG_WARNING("Overwritting network.");
	}

	m_pNetwork.reset(new CClientNetwork());

	if (m_pNetwork) {
		if (m_pNetwork->InitialiseScan(g_kusDEF_CLIENT_PORT_MIN, g_kusDEF_CLIENT_PORT_MAX)) {
			return static_cast<CClientNetwork*>(m_pNetwork.get());
		}
		else {
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}

CServerNetwork* CGameApp::CreateServerNetwork(const std::string& _krstrServerName)
{
	if (m_pNetwork) {
		DEBUG_WARNING("Overwritting network.");
	}

	m_pNetwork.reset(new CServerNetwork(_krstrServerName));

	if (m_pNetwork) {
		if (m_pNetwork->Initialise(g_kusDEF_SERVER_PORT)) {
			return static_cast<CServerNetwork*>(m_pNetwork.get());
		}
		else {
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}

void CGameApp::CloseNetwork()
{
	if (m_pNetwork) {
		// Delete the network by reseting to null.
		m_pNetwork.reset(nullptr);
	}
}

CGameLogic* CGameApp::GetGame()
{
	return m_pGame.get();
}

IRenderer* CGameApp::GetRenderer()
{
	return m_pRenderer.get();
}

ABaseNetwork* CGameApp::GetNetwork()
{
	return m_pNetwork.get();
}

float CGameApp::GetAspectRatio() const
{
	// Aspect ratio is screen width divided by height.
	return m_pRenderer->VGetAspectRatio();
}

int CGameApp::GetRandomNumber(int _iMinInclusive, int _iMaxExclusive)
{
	if (_iMinInclusive == _iMaxExclusive) {
		return _iMinInclusive;
	}

	std::uniform_int_distribution<int> distribution(_iMinInclusive, _iMaxExclusive - 1);
	return distribution(m_randomGenerator);
}

float CGameApp::GetRandomNumberReal(float _fMinInclusive, float _fMaxInclusive)
{
	std::uniform_real_distribution<float> distibution(_fMinInclusive, _fMaxInclusive);
	return distibution(m_randomGenerator);
}

int CGameApp::GetScreenWidth() const
{
	return m_pRenderer->VGetScreenWidth();
}

int CGameApp::GetScreenHeight() const
{
	return m_pRenderer->VGetScreenHeight();
}

CPoint CGameApp::GetScreenCentre() const
{
	POINT clientPosition;
	clientPosition.x = 0;
	clientPosition.y = 0;
	ClientToScreen(g_pApp->m_hWnd, &clientPosition);

	CPoint centreOfScreen;
	centreOfScreen.SetX(clientPosition.x + static_cast<int>(static_cast<float>(m_pRenderer->VGetScreenWidth()) / 2.0f));
	centreOfScreen.SetY(clientPosition.y + static_cast<int>(static_cast<float>(m_pRenderer->VGetScreenHeight()) / 2.0f));

	return centreOfScreen;
}

bool CGameApp::IsWindowed() const
{
	return m_pRenderer->VIsWindowed();
}

bool CGameApp::IsAppMinimized() const
{
	return m_bIsMinimized;
}

bool CGameApp::IsRunning() const 
{
	return m_bIsRunning;
}

bool CGameApp::IsPaused() const
{
	return m_bIsPaused;
}


void CGameApp::PollGamepads()
{
	m_gamepadManager.PollGamepads();
}
