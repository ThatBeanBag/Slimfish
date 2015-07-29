// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: String.h
// Description	: String helper functions declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __STRING_H__
#define __STRING_H__

// Library Includes
#include <string>
#include <sstream>
#include <iomanip>

// Local Includes

extern const int g_kiDEF_PRECISION;

template<typename T>
std::string ToString(const T& _krValue, int _iPrecision = g_kiDEF_PRECISION)
{
	std::ostringstream osOutText;
	osOutText << std::setprecision(_iPrecision) << _krValue;
	return osOutText.str();
}

template<typename T>
std::string ToStringFixed(const T& _krValue, int _iPrecision = g_kiDEF_PRECISION)
{
	std::ostringstream osOutText;
	osOutText << std::fixed << std::setprecision(_iPrecision) << _krValue;
	return osOutText.str();
}

std::string ExtractPathFromResource(const std::string& _krstrFilename);
std::string StringToLower(const std::string& _krstr);

#endif	// __STRING_H__