// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: RenderComponent.cpp
// Description	: CRenderComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "RenderComponent.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"
#include "TransformComponent.h"

ARenderComponent::ARenderComponent()
	:m_pSceneNode(nullptr)
{

}

ARenderComponent::~ARenderComponent()
{

}

bool ARenderComponent::VInitialise(TiXmlElement* _pXmlData)
{
	TiXmlElement* pMaterial = _pXmlData->FirstChildElement("Material");
	if (pMaterial) {
		TiXmlElement* pDiffuse = pMaterial->FirstChildElement("Diffuse");
		if (pDiffuse) {
			m_material.SetDiffuse(CreateColourValueFromXML(pDiffuse));
		}

		TiXmlElement* pAmbient = pMaterial->FirstChildElement("Ambient");
		if (pAmbient) {
			m_material.SetAmbient(CreateColourValueFromXML(pAmbient));
		}

		TiXmlElement* pSpecular = pMaterial->FirstChildElement("Specular");
		if (pSpecular) {
			m_material.SetSpecular(CreateColourValueFromXML(pSpecular));
		}
		TiXmlElement* pEmissive = pMaterial->FirstChildElement("Emissive");
		if (pEmissive) {
			m_material.SetEmissive(CreateColourValueFromXML(pEmissive));
		}
	}

	return VInitialiseDelegate(_pXmlData);
}

void ARenderComponent::VPostInitialise()
{
	m_pSceneNode = VCreateSceneNode();

	CTransformComponent* pTransformComponent = GetOwner()->GetComponent<CTransformComponent>();

	if (!pTransformComponent) {
		DEBUG_ERROR("A render component requires a transform component");
		return;
	}

	m_pSceneNode->SetTransform(pTransformComponent->GetTransform());
	m_pSceneNode->SetMaterial(m_material);

	// Trigger an event to let any scenes know about the new node.
	shared_ptr<CEventData_NewRenderComponent> pEvent(new CEventData_NewRenderComponent(GetOwnerID(), m_pSceneNode));
	CEventManager::GetInstance()->TriggerEvent(pEvent);
}


