// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: XmlResource.h
// Description	: CXmlResource declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __XMLRESOURCE_H__
#define __XMLRESOURCE_H__

// Library Includes

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"

class CXmlResource {
	// Member Functions
public:
	CXmlResource();
	~CXmlResource();

	bool LoadFile(const std::string& _krstrFileName);
	TiXmlElement* GetRoot();

	static shared_ptr<CXmlResource> LoadResource(const std::string& _krstrFileName);
protected:
private:
	// Member Variables
public:
protected:
private:
	TiXmlDocument m_xmlDocument;
};

#endif	// __XMLRESOURCE_H__