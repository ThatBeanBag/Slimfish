// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimMath.cpp
// Description	: CSlimMath implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimMath.h"

// Local Includes

namespace Slim {

std::unique_ptr<Math> Math::m_pSingleton = nullptr;

const float Math::s_PI = 3.1415926535897932384f;
const float Math::s_DEGREES_TO_RADIANS = s_PI / 180;
const float Math::s_RADIANS_TO_DEGREES = 180 / s_PI;

Math::Math()
{
	std::random_device randomDevice;
	m_Seed = randomDevice();
	m_RandomGenerator.seed(m_Seed);
}


}

