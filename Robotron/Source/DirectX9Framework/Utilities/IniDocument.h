// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: IniDocument.h
// Description	: CIniDocument declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __INIDOCUMENT_H__
#define __INIDOCUMENT_H__

// Library Includes
#include <map>
#include <string>

// Local Includes
#include "../Geometry/Matrix4x4.h"

// ================================================
// TSectionKeyPair couples the section with the key 
// ================================================
struct TSectionKeyPair {
	std::string m_strSection;
	std::string m_strKey;
};

bool operator<(const TSectionKeyPair& _krA, const TSectionKeyPair& _krB);

class CIniDocument {
	// Member Functions
public:
	CIniDocument();
	~CIniDocument();

	// Loads and parsers an INI file.
	// Populates the map with key, value pairs.
	// Returns true if the file loaded successfully, false otherwise
	bool LoadIniFile(const std::string& _krstrFileName);

	// Saves the map of section key and value pairs out to a file.
	// Returns true if the file saved successfully, false otherwise
	bool SaveIniFile(const std::string& _krstrFileName) const;

	// Adds a value to the map.
	// Combines _krstrSection and _krstrKey into a TSectionKeyPair to create a key for the map.
	bool AddValue(const std::string& _krstrSection, const std::string& _krstrKey, const std::string& _krstrValue);

	// Gets a value as a string from the map.
	// Returns true if the value was found, false otherwise.
	// _rstrValue will be populated with the correct value if the key is found in the map.
	bool GetValue(const std::string& _krstrSection, const std::string& _krstrKey, std::string& _rstrValue) const;

	// Gets a value as an int from the map.
	// Returns true if the value was found, false otherwise.
	// _riValue will be populated with the correct value if the key is found in the map.
	bool GetValue(const std::string& _krstrSection, const std::string& _krstrKey, int& _riValue) const;

	// Gets a value as a float from the map.
	// Returns true if the value was found, false otherwise.
	// _rfValue will be populated with the correct value if the key is found in the map.
	bool GetValue(const std::string& _krstrSection, const std::string& _krstrKey, float& _rfValue) const;

	// Gets a value as a float from the map.
	// Returns true if the value was found, false otherwise.
	// _rfValue will be populated with the correct value if the key is found in the map.
	bool GetValue(const std::string& _krstrSection, const std::string& _krstrKey, double& _rdValue) const;

	// Gets a value as a bool from the map.
	// Returns true if the value was found, false otherwise.
	// _rbValue will be populated with the correct value if the key is found in the map.
	bool GetValue(const std::string& _krstrSection, const std::string& _krstrKey, bool& _rbValue) const;

	// Gets a value as a TColour from the map.
	// Returns true if the value was found, false otherwise.
	// _rValue will be populated with the correct value if the key is found in the map.
	bool GetValue(const std::string& _krstrSection, const std::string& _krstrKey, TColour& _rValue) const;
	bool GetValue(const std::string& _krstrSection, const std::string& _krstrKey, TColourValue& _rValue) const;

	bool GetValue(const std::string& _krstrSection, const std::string& _krstrKey, CVec3& _rValue) const;
protected:
private:

	// Member Variables
public:
protected:
private:
	std::map<TSectionKeyPair, std::string> m_mapValues;
};

#endif	// __INIDOCUMENT_H__