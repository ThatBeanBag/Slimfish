// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Actor.h
// Description	: CActor declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __ACTOR_H__
#define __ACTOR_H__


// Library Includes
#include <typeindex>

// Local Includes

class AActorComponent;

typedef unsigned int TActorID;

class CActor {
	// Member Functions
	typedef std::vector<unique_ptr<AActorComponent>> TActorComponents;
	typedef std::map<std::type_index, AActorComponent*> TActorComponentMap;

public:
	CActor(TActorID _ID, const std::string& _krstrType);
	~CActor();

	/**
	* Runs the post initialisation sequence on all components of this actor.
	*
	* @author: Hayden Asplet
	* @return: void
	*/
	void PostInitialise();

	/**
	* Updates all components of this actor.
	*
	* @author: Hayden Asplet
	* @param: float _fDeltaTime - time of last tick.
	* @return: void
	*/
	void Update(float _fDeltaTime);


	/**
	* Clears the components.
	*
	* This MUST be called before destructing the actor.
	*
	* @author: Hayden Asplet
	* @return: void
	*/
	void DestroyComponents();

	/**
	* Gets the ID of the actor.
	*
	* @author: Hayden Asplet
	* @return: const TActorID - ID of the actor.
	*/
	const TActorID GetID() const { return m_ID; };

	/**
	* Gets the type as a string of the actor.
	*
	* @author: Hayden Asplet
	* @return: const std::string - type as a string of the actor.
	*/
	const std::string GetType() const { return m_strType; };

	/**
	* Gets a component of the actor.
	*
	* The component is specified in terms of the template argument.
	*
	* @author: Hayden Asplet
	* @return: ComponentType* - a pointer to the component.
	*/
	template<typename ComponentType>
	ComponentType* GetComponent();

	/**
	* Adds a component to the list of components for the actor.
	*
	* @author: Hayden Asplet
	* @param: unique_ptr<CActorComponent> pActorComponent - component to add.
	* @return: void
	*/
	void AddComponent(unique_ptr<AActorComponent> pActorComponent);
protected:
private:
	// Member Variables
public:
	static const TActorID s_kINVALID_ACTOR_ID;
protected:
private:
	TActorID m_ID;							// This objects unique ID.
	TActorComponentMap m_actorComponentMap;	
	TActorComponents m_actorComponents;		// The components of this actor.
	std::string m_strType;					// The type of this actor as text.
};

template<typename ComponentType>
ComponentType* CActor::GetComponent()
{
	// Get the type index from the typeid of the component's type.
	std::type_index index(typeid(ComponentType));

	// Find the component at the index of the typeid.
	TActorComponentMap::iterator findIter = m_actorComponentMap.find(index);

	if (findIter != m_actorComponentMap.end()) {
	// Does actor has a component of the specified type?
		// Cast to the specified type and return.
		return static_cast<ComponentType*>(findIter->second);
	}
	else {
		// Return a NULL shared_ptr.
		return nullptr;
	}
}

#endif // __ACTOR_H__