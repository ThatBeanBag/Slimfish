// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: HeightMapStd.cpp
// Description	: Creates the pre compiled header file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// Library Includes

// This Include
#include "RobotronStd.h"	// Creates the PCH file.

// Local Includes

const float g_kfPLAYER_ACCELERATION = 550.0f;
const float g_kfPLAYER_MAX_VELOCITY = 7.0f;

const float g_kfENEMY_PUSH_FORCE = 4.5f;
const float g_kfENEMY_PUSH_Y_ADJUST = 0.2f;
const float g_kfBULLET_VELOCITY = -30.0f;

const int g_kiMAX_POWERUPS = 10;
const float g_kfPOWERUP_RANDOM_TIME_RANGE_MIN = 2.0f;
const float g_kfPOWERUP_RANDOM_TIME_RANGE_MAX = 10.0f;
const float g_kfPOWERUP_CHANCE_TO_SPAWN_GOLD = 0.05f;
const float g_kfPOWERUP_CHANCE_TO_SPAWN_CRATE = 0.05f;
const float g_kfPOWERUP_GOLDEN_APPLE_TIME_SPAN = 5.0f;
const int g_kiPOWERUP_CRATE_AMMO_AMOUNT = 3;

const float g_kfTREE_RESPAWN_TIMER_RANGE_MIN = 50.0f;
const float g_kfTREE_RESPAWN_TIMER_RANGE_MAX = 120.0f;

const int g_kiNUM_OF_ENEMIES_MIN = 5;
const int g_kiNUM_OF_ENEMIES_MAX = 20;