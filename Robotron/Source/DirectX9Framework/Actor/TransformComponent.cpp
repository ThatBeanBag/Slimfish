// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TransformComponent.cpp
// Description	: CTransformComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "TransformComponent.h"
#include "../3rdParty/tinyxml/tinyxml.h"

// Local Includes
#include "PhysicsComponent.h"

const std::string CTransformComponent::s_kstrNAME = "TransformComponent";

CTransformComponent::CTransformComponent()
{

}

CTransformComponent::~CTransformComponent()
{

}

bool CTransformComponent::VInitialise(TiXmlElement* _pXmlData)
{	
	// Create the initial transform from the scale rotation and translation information. 
	m_matTransform = CreateTransformFromXML(_pXmlData);
	m_matInitialTransform = m_matTransform;

	TiXmlElement* pElement = _pXmlData->FirstChildElement("Spawns");
	if (pElement) {
		for (TiXmlElement* pNode = pElement->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
			double dX = 0.0;
			double dY = 0.0;
			double dZ = 0.0;

			pNode->Attribute("x", &dX);
			pNode->Attribute("y", &dY);
			pNode->Attribute("z", &dZ);

			m_vecSpawnPositions.push_back(CVec3(static_cast<float>(dX), static_cast<float>(dY), static_cast<float>(dZ)));
		}

		if (!m_vecSpawnPositions.empty()) {
			// Select and random spawn from the spawn locations.
			int iRandomSpawn = g_pApp->GetRandomNumber(0, m_vecSpawnPositions.size());
			
			// Set the spawn.
			m_matTransform.SetPosition(m_vecSpawnPositions[iRandomSpawn]);
		}
	}

	return true;
}

const CMatrix4x4 CTransformComponent::GetTransform() const
{
	return m_matTransform;
}

void CTransformComponent::SetTransform(const CMatrix4x4& _krmatTransform)
{
	m_matTransform = _krmatTransform;
}

void CTransformComponent::SetPosition(const CVec3& _krVec3)
{
	m_matTransform.SetPosition(_krVec3);
}

const CVec3 CTransformComponent::GetPosition() const
{
	return m_matTransform.GetPosition();
}

void CTransformComponent::RespawnRandom()
{
	if (m_vecSpawnPositions.empty()) {
		return;
	}

	int iRandomSpawn = g_pApp->GetRandomNumber(0, m_vecSpawnPositions.size());

	Respawn(iRandomSpawn);
}

void CTransformComponent::Respawn(int _iSpawnPoint)
{
	CMatrix4x4 matRespawn = m_matInitialTransform;
	matRespawn.SetPosition(m_vecSpawnPositions[_iSpawnPoint]);

	CPhysicsComponent* pPhysicsComponent = GetOwner()->GetComponent<CPhysicsComponent>();
	if (pPhysicsComponent) {
	// If we have physics set the transform on the physics component.
		pPhysicsComponent->SetTransform(matRespawn);
	}

	m_matTransform = matRespawn;
}

