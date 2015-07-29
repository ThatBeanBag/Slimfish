// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: IniDocument.cpp
// Description	: CIniDocument implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes
#include <fstream>
#include <sstream>

// This Include
#include "IniDocument.h"

// Local Includes

bool operator<(const TSectionKeyPair& _krA, const TSectionKeyPair& _krB)
{
	if (_krA.m_strSection < _krB.m_strSection) {
		return true;
	}
	else if (_krA.m_strSection == _krB.m_strSection) {
		return _krA.m_strKey < _krB.m_strKey;
	}
	else {
		return false;
	}

	return false;
}

CIniDocument::CIniDocument()
{

}

CIniDocument::~CIniDocument()
{

}

bool CIniDocument::LoadIniFile(const std::string& _krstrFileName)
{
	m_mapValues.clear();

	std::ifstream file;
	file.open(_krstrFileName.c_str());

	if (!file.is_open()) {
	// Did the file fail to open?
		return false;
	}
	else {
		std::string strSection;

		while (!file.eof()) {
			std::string strLine;
			
			// Extract whitespace.
			file >> std::ws;

			if (file.peek() == '[') {
				file.get();
				std::getline(file, strSection, ']');
			}
			else if (file.peek() == ';') {
			// Ignore this line, it is comment.
				std::getline(file, strLine);
			}
			else {
				std::string strKey;
				std::string strValue;

				// Get the line.
				std::getline(file, strLine);

				unsigned int uiOffset = strLine.find_first_of('=');

				for (unsigned int i = 0; i < strLine.size(); ++i) {
					if (isspace(strLine[i])) {
					// Ignore whitespace.
					}
					else if (strLine[i] == ';') {
						break;
					}
					else if (i < uiOffset) {
						strKey.push_back(strLine[i]);
					}
					else if (i > uiOffset) {
						strValue.push_back(strLine[i]);
					}
				}

				AddValue(strSection, strKey, strValue);
			}
		}

		file.close();
	}

	return true;
}

bool CIniDocument::SaveIniFile(const std::string& _krstrFileName) const
{
	return true;
}

bool CIniDocument::AddValue(const std::string& _krstrSection, const std::string& _krstrKey, const std::string& _krstrValue)
{
	TSectionKeyPair sectionKeyPair;
	sectionKeyPair.m_strSection = _krstrSection;
	sectionKeyPair.m_strKey = _krstrKey;

	m_mapValues[sectionKeyPair] = _krstrValue;

	return true;
}

bool CIniDocument::GetValue(const std::string& _krstrSection, const std::string& _krstrKey, std::string& _rstrValue) const
{
	// Pair the section and key together.
	TSectionKeyPair sectionKeyPair;

	sectionKeyPair.m_strSection = _krstrSection;
	sectionKeyPair.m_strKey = _krstrKey;

	// Find the value from the section and key pair.
	auto findIter = m_mapValues.find(sectionKeyPair);

	if (findIter != m_mapValues.end()) {
	// Was the value found?
		_rstrValue = findIter->second;

		return true;
	}
	else {
	// Failed to find the value.
		return false;
	}

	return true;
}

bool CIniDocument::GetValue(const std::string& _krstrSection, const std::string& _krstrKey, int& _riValue) const
{
	std::string strValue;
	if (!GetValue(_krstrSection, _krstrKey, strValue)) {
		return false;
	}

	_riValue = atoi(strValue.c_str());

	return true;
}

bool CIniDocument::GetValue(const std::string& _krstrSection, const std::string& _krstrKey, float& _rfValue) const
{
	std::string strValue;
	if (!GetValue(_krstrSection, _krstrKey, strValue)) {
		return false;
	}

	_rfValue = static_cast<float>(atof(strValue.c_str()));

	return true;
}

bool CIniDocument::GetValue(const std::string& _krstrSection, const std::string& _krstrKey, double& _rdValue) const
{
	std::string strValue;
	if (!GetValue(_krstrSection, _krstrKey, strValue)) {
		return false;
	}

	_rdValue = atof(strValue.c_str());

	return true;
}

bool CIniDocument::GetValue(const std::string& _krstrSection, const std::string& _krstrKey, bool& _rbValue) const
{
	std::string strValue;
	if (!GetValue(_krstrSection, _krstrKey, strValue)) {
		return false;
	}

	for (unsigned int i = 0; i < strValue.size(); ++i) {
		strValue[i] = tolower(strValue[i]);
	}

	if (strValue == "true" || strValue == "1") {
		_rbValue = true;
	}
	else if (strValue == "false"  || strValue == "0") {
		_rbValue = false;
	}
	else {
		return false;
	}

	return true;
}

bool CIniDocument::GetValue(const std::string& _krstrSection, const std::string& _krstrKey, TColour& _rValue) const
{
	std::string strValue;
	if (!GetValue(_krstrSection, _krstrKey, strValue)) {
		return false;
	}

	std::stringstream ss(strValue);
	std::string strRed;
	std::string strGreen;
	std::string strBlue;
	std::string strAlpha;

	getline(ss, strRed, ',');
	getline(ss, strGreen, ',');
	getline(ss, strBlue, ',');
	getline(ss, strAlpha);

	if (strRed == "" || strGreen == "" || strBlue == "") {
		return false;
	}

	TColour colour;
	colour.m_r = atoi(strRed.c_str());
	colour.m_g = atoi(strGreen.c_str());
	colour.m_b = atoi(strBlue.c_str());

	if (strAlpha == "") {
	// Did the user not input an alpha value?
		// Use full alpha.
		colour.m_a = 255;
	}
	else {
		colour.m_a = atoi(strAlpha.c_str());
	}

	_rValue = colour;

	return true;
}

bool CIniDocument::GetValue(const std::string& _krstrSection, const std::string& _krstrKey, TColourValue& _rValue) const
{
	std::string strValue;
	if (!GetValue(_krstrSection, _krstrKey, strValue)) {
		return false;
	}

	std::stringstream ss(strValue);
	std::string strRed;
	std::string strGreen;
	std::string strBlue;
	std::string strAlpha;

	getline(ss, strRed, ',');
	getline(ss, strGreen, ',');
	getline(ss, strBlue, ',');
	getline(ss, strAlpha);

	if (strRed == "" || strGreen == "" || strBlue == "") {
		return false;
	}

	TColourValue colourValue;
	colourValue.m_fR = static_cast<float>(atof(strRed.c_str()));
	colourValue.m_fG = static_cast<float>(atof(strGreen.c_str()));
	colourValue.m_fB = static_cast<float>(atof(strBlue.c_str()));

	if (strAlpha == "") {
		// Did the user not input an alpha value?
		// Use full alpha.
		colourValue.m_fA = 1.0f;
	}
	else {
		colourValue.m_fA = static_cast<float>(atof(strAlpha.c_str()));
	}

	_rValue = colourValue;

	return true;
}

bool CIniDocument::GetValue(const std::string& _krstrSection, const std::string& _krstrKey, CVec3& _rValue) const
{
	std::string strValue;
	if (!GetValue(_krstrSection, _krstrKey, strValue)) {
		return false;
	}

	std::stringstream ss(strValue);
	std::string strX;
	std::string strY;
	std::string strZ;

	getline(ss, strX, ',');
	getline(ss, strY, ',');
	getline(ss, strZ);

	if (strX == "" || strY == "" || strZ == "") {
		return false;
	}

	CVec3 vec3;
	vec3.x = static_cast<float>(atof(strX.c_str()));
	vec3.y = static_cast<float>(atof(strY.c_str()));
	vec3.z = static_cast<float>(atof(strZ.c_str()));

	_rValue = vec3;

	return true;
}