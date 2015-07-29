// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Math.cpp
// Description	: Math functions implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Math.h"

// Local Includes

const float g_kfPI = 3.14159265359f;

float ToRadians(float m_fDegrees)
{
	return m_fDegrees * g_kfPI / 180;
}

float ToDegrees(float m_fRadians)
{
	return m_fRadians / g_kfPI * 180;
}
