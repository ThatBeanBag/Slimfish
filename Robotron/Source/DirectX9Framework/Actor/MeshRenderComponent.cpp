// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MeshRenderComponent.cpp
// Description	: CMeshRenderComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "MeshRenderComponent.h"

// Local Includes
#include "../3rdParty/tinyxml/tinyxml.h"
#include "TransformComponent.h"

const std::string CMeshRenderComponent::s_kstrNAME = "MeshRenderComponent";

CMeshRenderComponent::CMeshRenderComponent()
{

}

CMeshRenderComponent::~CMeshRenderComponent()
{

}

bool CMeshRenderComponent::VInitialiseDelegate(TiXmlElement* _pXmlData)
{
	TiXmlElement* pFilePath = _pXmlData->FirstChildElement("Mesh");

	if (pFilePath) {
		std::string strFilePath = pFilePath->FirstChild()->Value();

		// Create the mesh from the file.
		m_strMeshResource = strFilePath;

		return true;
	}
	else {
		std::string strError = _pXmlData->GetDocument()->Value();
		strError = "Failed to create render component missing \"Mesh\": \"" + strError + "\"";
		DEBUG_ERROR(strError);
		return false;
	}

	return true;
}

shared_ptr<CSceneNode> CMeshRenderComponent::VCreateSceneNode()
{
	shared_ptr<IMesh> pMesh = CResourceManager<IMesh>::GetInstance()->Load(m_strMeshResource);

	if (pMesh) {
		// Did the mesh successfully create from file?
		// Add the mesh to the mesh node.
		shared_ptr<CMeshNode> pMeshNode(new CMeshNode(GetOwner()->GetID(), pMesh));
		CTransformComponent* pTransformComponent = GetOwner()->GetComponent<CTransformComponent>();

		if (!pTransformComponent) {
			DEBUG_ERROR("A render component requires a transform component");
			return nullptr;
		}

		// Set the transform of the mesh node to the component.
		//pMeshNode->SetTransform(pTransformComponent->GetTransform());
		return pMeshNode;
	}
	else {
		DEBUG_ERROR("Failed to create mesh.");
	}

	return nullptr;
}

