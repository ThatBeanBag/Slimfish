// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: XmlResource.cpp
// Description	: CXmlResource implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "XmlResource.h"

// Local Includes


CXmlResource::CXmlResource()
{

}

CXmlResource::~CXmlResource()
{

}

bool CXmlResource::LoadFile(const std::string& _krstrFileName)
{
	return m_xmlDocument.LoadFile(_krstrFileName.c_str());
}

TiXmlElement* CXmlResource::GetRoot()
{
	return m_xmlDocument.RootElement();
}

shared_ptr<CXmlResource> CXmlResource::LoadResource(const std::string& _krstrFileName)
{
	shared_ptr<CXmlResource> pXmlResource(new CXmlResource());

	if (pXmlResource) {
		if (pXmlResource->LoadFile(_krstrFileName)) {
			return pXmlResource;
		}
		else {
			return nullptr;
		}
	}

	return nullptr;
}
