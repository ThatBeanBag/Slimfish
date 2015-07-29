// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: RobotronStd.h
// Description	: CRobotronStd declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __ROBOTRONSTD_H__
#define __ROBOTRONSTD_H__

// Library Includes

// Local Includes
#include "../DirectX9Framework/GameStd.h"	// Gives us access to the framework.

extern const float g_kfPLAYER_ACCELERATION;
extern const float g_kfPLAYER_MAX_VELOCITY;
extern const float g_kfENEMY_PUSH_FORCE;
extern const float g_kfENEMY_PUSH_Y_ADJUST;
extern const float g_kfBULLET_VELOCITY;

extern const int g_kiMAX_POWERUPS;
extern const float g_kfPOWERUP_RANDOM_TIME_RANGE_MIN;
extern const float g_kfPOWERUP_RANDOM_TIME_RANGE_MAX;
extern const float g_kfPOWERUP_CHANCE_TO_SPAWN_GOLD;	// The chance to spawn a golden apple each time an apple drops.
extern const float g_kfPOWERUP_CHANCE_TO_SPAWN_CRATE;
extern const float g_kfPOWERUP_GOLDEN_APPLE_TIME_SPAN;
extern const int g_kiPOWERUP_CRATE_AMMO_AMOUNT;

extern const float g_kfTREE_RESPAWN_TIMER_RANGE_MIN;
extern const float g_kfTREE_RESPAWN_TIMER_RANGE_MAX;

extern const int g_kiNUM_OF_ENEMIES_MIN;
extern const int g_kiNUM_OF_ENEMIES_MAX;

#endif // __ROBOTRONSTD_H__