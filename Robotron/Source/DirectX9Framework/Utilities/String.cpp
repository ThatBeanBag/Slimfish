// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: String.cpp
// Description	: String helper functions implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "String.h"

// Local Includes

const int g_kiDEF_PRECISION = 6;

std::string ExtractPathFromResource(const std::string& _krstrFilename)
{
	unsigned int uiOffsetForwardSlash = _krstrFilename.find_last_of('/');
	unsigned int uiOffsetBackSlash = _krstrFilename.find_last_of('\\');

	std::string strFilePath;

	if (uiOffsetBackSlash != std::string::npos &&
		uiOffsetForwardSlash != std::string::npos) {
		strFilePath = _krstrFilename.substr(0, std::max(uiOffsetBackSlash, uiOffsetForwardSlash) + 1);
	}
	else if (uiOffsetBackSlash != std::string::npos) {
		strFilePath = _krstrFilename.substr(0, uiOffsetBackSlash + 1);
	}
	else if (uiOffsetForwardSlash != std::string::npos) {
		strFilePath = _krstrFilename.substr(0, uiOffsetForwardSlash + 1);
	}
	else {
	// There is no path to this file.
		strFilePath = _krstrFilename;
	}

	return strFilePath;
}

std::string StringToLower(const std::string& _krstr)
{
	std::string strLower = _krstr;

	for (unsigned int i = 0; i < strLower.size(); ++i) {
		strLower[i] = tolower(strLower[i]);
	}

	return strLower;
}
