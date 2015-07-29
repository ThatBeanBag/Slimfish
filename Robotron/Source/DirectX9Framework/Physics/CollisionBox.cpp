// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionBox.cpp
// Description	: CCollisionBox implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "CollisionBox.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"

const std::string CCollisionBox::s_kstrNAME = "Box";

CCollisionBox::CCollisionBox()
{

}

CCollisionBox::~CCollisionBox()
{

}

const std::string CCollisionBox::VGetType()
{
	return s_kstrNAME;
}

bool CCollisionBox::VInitializeFromXML(TiXmlElement* _pXmlData)
{
	// Create a box from XML data. <Box width="" height="" length="" />
	double dWidth = 1.0;
	double dHeight = 1.0;
	double dLength = 1.0;

	if (!_pXmlData->Attribute("width", &dWidth)) {
		DEBUG_WARNING("Couldn't find width for box collision shape.");
	}
	if (!_pXmlData->Attribute("height", &dHeight)) {
		DEBUG_WARNING("Couldn't find height for box collision shape.");
	}
	if (!_pXmlData->Attribute("length", &dLength)) {
		DEBUG_WARNING("Couldn't find length for box collision shape.");
	}

	m_vec3Dimensions = CVec3(static_cast<float>(dWidth), static_cast<float>(dHeight), static_cast<float>(dLength));

	return true;
}

const CVec3 CCollisionBox::GetDimensions() const
{
	return m_vec3Dimensions;
}

