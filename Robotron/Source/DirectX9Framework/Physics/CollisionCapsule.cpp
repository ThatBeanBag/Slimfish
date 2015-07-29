// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionCapsule.cpp
// Description	: CCollisionCapsule implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "CollisionCapsule.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"

const std::string CCollisionCapsule::s_kstrNAME = "Capsule";

CCollisionCapsule::CCollisionCapsule()
{

}

CCollisionCapsule::~CCollisionCapsule()
{

}

const std::string CCollisionCapsule::VGetType()
{
	return s_kstrNAME;
}

bool CCollisionCapsule::VInitializeFromXML(TiXmlElement* _pXmlData)
{
	DEBUG_ASSERT(_pXmlData);

	double dRadius;
	double dHeight;

	if (!_pXmlData->Attribute("radius", &dRadius)) {
		DEBUG_WARNING("Couldn't find radius for capsule collision shape.");
	}
	if (!_pXmlData->Attribute("height", &dHeight)) {
		DEBUG_WARNING("Couldn't find height for capsule collision shape.");
	}

	m_fRadius = static_cast<float>(dRadius);
	m_fHeight = static_cast<float>(dHeight);

	return true;
}

float CCollisionCapsule::GetRadius() const
{
	return m_fRadius;
}

float CCollisionCapsule::GetHeight() const
{
	return m_fHeight;
}

