// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Actor.cpp
// Description	: CActor implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Actor.h"

// Local Includes
#include "ActorComponent.h"

const TActorID CActor::s_kINVALID_ACTOR_ID = 0;

CActor::CActor(TActorID _ID, const std::string& _krstrType)
	:m_ID(_ID),
	m_strType(_krstrType)
{

}

CActor::~CActor()
{
	DEBUG_ASSERT(m_actorComponents.empty());
	DEBUG_ASSERT(m_actorComponentMap.empty());
}

void CActor::PostInitialise()
{
	// Call post initialise on all actor components for this actor.
	for (unsigned int i = 0; i < m_actorComponents.size(); ++i) {
		m_actorComponents[i]->VPostInitialise();
	}
}

void CActor::Update(float _fDeltaTime)
{
	// Update all actor components.
	for (unsigned int i = 0; i < m_actorComponents.size(); ++i) {
		m_actorComponents[i]->VUpdate(_fDeltaTime);
	}
}

void CActor::DestroyComponents()
{
	m_actorComponentMap.clear();
	m_actorComponents.clear();
}

void CActor::AddComponent(unique_ptr<AActorComponent> _pActorComponent)
{
	// Insert the component into the map of components.
	m_actorComponentMap[std::type_index(typeid(*_pActorComponent))] = _pActorComponent.get();

	m_actorComponents.push_back(std::move(_pActorComponent));
}
