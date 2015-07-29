// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ActorFactory.cpp
// Description	: CActorFactory implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "ActorFactory.h"

// Local Includes
#include "../Resource/XmlResource.h"
#include "MeshRenderComponent.h"
#include "LightRenderComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "AIComponent.h"
#include "PlaneRenderComponent.h"
#include "CubeRenderComponent.h"
#include "BasicAnimationComponent.h"

CActorFactory::CActorFactory()
	:m_lastActorID(CActor::s_kINVALID_ACTOR_ID)
{
	// This is where base actor components should be registered.
	// Registering components here ensures they are registered before any actor can be created.
	// All components require a static name called s_kstrNAME to be registered. 
	RegisterComponent<CTransformComponent>();
	RegisterComponent<CMeshRenderComponent>();
	RegisterComponent<CLightRenderComponent>();
	RegisterComponent<CPhysicsComponent>();
	RegisterComponent<CAIComponent>();
	RegisterComponent<CPlaneRenderComponent>();
	RegisterComponent<CCubeRenderComponent>();
	RegisterComponent<CBasicAnimationComponent>();
}

CActorFactory::~CActorFactory()
{

}

unique_ptr<CActor> CActorFactory::CreateActor(const std::string& _krResource, const CMatrix4x4* _pInitialTransform, TActorID _serverActorID)
{
	// Load the document.
	shared_ptr<CXmlResource> pXmlDoc = CResourceManager<CXmlResource>::GetInstance()->Load(_krResource);
	if (!pXmlDoc) {
		return nullptr;
	}

	TiXmlElement* pRoot = pXmlDoc->GetRoot();

	if (!pRoot) {
		DEBUG_ERROR("Failed to create actor from resource \"" + _krResource + "\" - no root element");
		return nullptr;
	}

	const char* pcType = pRoot->Attribute("type");

	if (!pcType) {
		DEBUG_ERROR("Failed to create actor from resource \"" + _krResource + "\" - no type attribute");
		return nullptr;
	}

	// If _serverActorID is valid use that, otherwise generate one.
	TActorID nextActorID = _serverActorID;
	if (_serverActorID == CActor::s_kINVALID_ACTOR_ID) {
		nextActorID = GetNextActorID();
	}

	// Create a new actor.
	unique_ptr<CActor> pActor(new CActor(nextActorID, pcType));

	if (!pActor) {
		DEBUG_ERROR("Failed to create actor from resource \"" + _krResource + "\" - out of memory");
		return nullptr;
	}

	// Loop through all child elements of the XML's root for components.
	for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
		unique_ptr<AActorComponent> pComponent(CreateComponent(pNode));

		if (g_pApp->GetGame()->IsRemote() && !pComponent->VIsActiveOnRemote()) {
		// Are we a remote logic and is this component not active on remote logics?
			// Don't add it to the actor.
			continue;
		}

		if (pComponent) {
			pComponent->SetOwner(pActor.get());
			pActor->AddComponent(std::move(pComponent));
		}
		else {
			DEBUG_ERROR("Failed to create a component when creating actor from resource \"" + _krResource + "\"");
		}
	}

	CTransformComponent* pTrasformComponent = pActor->GetComponent<CTransformComponent>();
	if (pTrasformComponent && _pInitialTransform) {
	// Does the actor have a transform component and do we want to override it with an initial transform?
		// Preserve the scale of the original transform.
		// The scale of the initial transform will be relative to the transform of the transform component.
		CMatrix4x4 matTransform = pTrasformComponent->GetTransform();
		CVec3 vec3Scale = matTransform.GetScale();

		matTransform = BuildScale(vec3Scale) * (*_pInitialTransform);

		pTrasformComponent->SetTransform(matTransform);
	}

	// Run the post initialisation sequence.
	pActor->PostInitialise();

	return std::move(pActor);
}

unique_ptr<AActorComponent> CActorFactory::CreateComponent(TiXmlElement* _pXmlData)
{
	DEBUG_ASSERT(_pXmlData);

	// Get the name of the element, essentially the name of the component.
	std::string strName(_pXmlData->Value());

	unique_ptr<AActorComponent> pComponent(m_componentCreators.Create(strName));

	if (pComponent) {
	// Was a component of the given name found in the list of component creators?
		if (!pComponent->VInitialise(_pXmlData)) {
		// Did the initialisation of the component fail?
			DEBUG_ERROR("Component failed to initialize.");

			return nullptr;
		}	
	}
	else {
	// The component creator wasn't found.
		DEBUG_WARNING("Couldn't find component creator for \"" + strName + "\"");
		return nullptr;
	}

	return std::move(pComponent);
}

CMatrix4x4 GetTransform(CActor* _pActor)
{
	DEBUG_ASSERT(_pActor);

	CTransformComponent* pTransformComponent = _pActor->GetComponent<CTransformComponent>();
	if (!pTransformComponent) {
		DEBUG_ERROR("No transform component.");
	}
	else {
		return pTransformComponent->GetTransform();
	}

	return CMatrix4x4::s_kIdentity;
}
