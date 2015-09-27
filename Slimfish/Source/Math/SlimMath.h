// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimMath.h
// Description	: Standard math functions declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMMATH_H__
#define __SLIMMATH_H__

// Library Includes

// Local Includes

namespace Slim {
	const float g_PI = 3.141592653589793238462643383279f;
	const float g_DEGREES_TO_RADIANS = g_PI / 180;
	const float g_RADIANS_TO_DEGREES = 180 / g_PI;

	/** Convert from radians to degrees. @author Hayden Asplet */
	inline float RadiansToDegrees(float radians) 
	{
		return radians * g_RADIANS_TO_DEGREES;
	}

	/** Convert from degrees to radians. @author Hayden Asplet */
	inline float DegreesToRadians(float degrees)
	{
		return degrees * g_DEGREES_TO_RADIANS;
	}

}

#endif	// __SLIMMATH_H__