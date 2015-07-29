// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GameStd.h
// Description	: Pre compiled header file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __GAMESTD_H__
#define __GAMESTD_H__

#define NOMINMAX 1
#define _WINSOCKAPI_

// Library Includes
#include <vector>
#include <map>
#include <list>
#include <memory>
#include <algorithm>

#include <vld.h>	// Leak detection.

// Flood the global namespace here.
using std::unique_ptr;
using std::weak_ptr;
using std::tr1::shared_ptr;
using std::tr1::make_shared;
using std::static_pointer_cast;

// Useful typedefs

// Useful consts
extern const int g_kiDEF_SCREEN_WIDTH;
extern const int g_kiDEF_SCREEN_HEIGHT;
extern const int g_kiMAXIMUM_GAMEPADS_SUPPORTED;
extern const int g_kiMAXIMUM_LIGHTS_SUPPORTED;	// Would be better if this was specific to each renderer.

template<typename T>
void SafeRelease(T* _pObject)
{
	if (_pObject) {
		_pObject->Release();
		_pObject = nullptr;
	}
}

template<typename T>
void SafeDelete(T* _pObject)
{
	if (_pObject) {
		delete _pObject;
		_pObject = nullptr;
	}
}

template<typename T>
void SafeDeleteArray(T* _pObject)
{
	if (_pObject) {
		delete[] _pObject;
		_pObject = nullptr;
	}
}

// Local Includes
#include "Debugging\Logger.h"
#include "Utilities\SimulatedAnnealing.h"
#include "Utilities\Rect.h"
#include "Utilities\Point.h"
#include "Utilities\String.h"
#include "Utilities\Colour.h"
#include "Utilities\IniDocument.h"
#include "Utilities\Math.h"
#include "Events\EventManager.h"
#include "Geometry\Frustum.h"
#include "GameBase\GameApp.h"
#include "3DGraphics\Scene.h"
#include "3DGraphics\Terrain.h"
#include "Controller\FlyingController.h"
#include "View\HumanView.h"
#include "View\UserInterfaceView.h"



#endif // __GAMESTD_H__