// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimStd.h
// Description	: SlimStd declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMSTD_H__
#define __SLIMSTD_H__

// Library Includes
#include <memory>
#include <assert.h>
#include <algorithm>
#include <string>
#include <vector>

namespace Slim {

// Commonly used constants
const size_t g_MAX_TEXTURE_LAYERS = 8;
const size_t g_MAX_ACTIVE_LIGHTS = 8;

}

// Local Includes
#include "..\Utilities\SlimSingleton.h"
#include "..\Math\SlimMatrix4x4.h"
#include "..\Math\SlimQuaternion.h"
#include "..\Math\SlimVector.h"

using std::shared_ptr;


#endif	// __SLIMSTD_H__
