// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionSphere.cpp
// Description	: CCollisionSphere implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "CollisionSphere.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"

const std::string CCollisionSphere::s_kstrNAME = "Sphere";

CCollisionSphere::CCollisionSphere()
{

}

CCollisionSphere::~CCollisionSphere()
{

}

const std::string CCollisionSphere::VGetType()
{
	return s_kstrNAME;
}

bool CCollisionSphere::VInitializeFromXML(TiXmlElement* _pXmlData)
{
	DEBUG_ASSERT(_pXmlData);

	double dRadius = 1.0;

	if (!_pXmlData->Attribute("radius", &dRadius)) {
		DEBUG_WARNING("Couldn't find radius for sphere collision shape.");
	}

	m_fRadius = static_cast<float>(dRadius);

	return true;
}

float CCollisionSphere::GetRaidus() const
{
	return m_fRadius;
}

