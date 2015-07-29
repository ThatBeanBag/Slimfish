// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionPlane.cpp
// Description	: CCollisionPlane implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "CollisionPlane.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"

const std::string CCollisionPlane::s_kstrNAME = "Plane";

CCollisionPlane::CCollisionPlane()
{

}

CCollisionPlane::~CCollisionPlane()
{

}

const std::string CCollisionPlane::VGetType()
{
	return s_kstrNAME;
}

bool CCollisionPlane::VInitializeFromXML(TiXmlElement* _pXmlData)
{
	DEBUG_ASSERT(_pXmlData);

	// Create a plane from XML data. <Plane a="" b="" c="" d="" />.
	// Uses equation form of a plane.
	double dA = 0.0;
	double dB = 0.0;
	double dC = 0.0;
	double dD = 0.0;

	if (!_pXmlData->Attribute("a", &dA)) {
		DEBUG_WARNING("Couldn't find A for plane collision shape.");
	}
	if (!_pXmlData->Attribute("b", &dB)) {
		DEBUG_WARNING("Couldn't find B for plane collision shape.");
	}
	if (!_pXmlData->Attribute("c", &dC)) {
		DEBUG_WARNING("Couldn't find C for plane collision shape.");
	}
	if (!_pXmlData->Attribute("d", &dD)) {
		DEBUG_WARNING("Couldn't find D for plane collision shape.");
	}

	m_plane = CPlane(static_cast<float>(dA), static_cast<float>(dB),
					 static_cast<float>(dC), static_cast<float>(dD));

	return true;
}

const CPlane CCollisionPlane::GetPlane() const
{
	return m_plane;
}
