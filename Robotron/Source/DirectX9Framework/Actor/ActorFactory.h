// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ActorFactory.h
// Description	: CActorFactory declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __ACTORFACTORY_H__
#define __ACTORFACTORY_H__

// Library Includes

// Local Includes
#include "Actor.h"
#include "ActorComponent.h"
#include "..\Utilities\ObjectFactory.h"

class TiXmlElement;

CMatrix4x4 GetTransform(CActor* _pActor);

class CActorFactory {
	// Member Functions
public:
	CActorFactory();
	~CActorFactory();

	/**
	* Adds a component creator the to factory.
	*
	* This function allows for the registering of more specific components; ones that
	* would be in your game.
	*
	* Component should only be registered upon initialisation of the program.
	* If a component creator isn't created before an actor that requires it, the
	* actor creation will fail.
	*
	* A component MUST have a public static const member variable called 's_kstrNAME' of 
	* type std::string to be eligible for registering. This name is used when identifying
	* components for creation.
	* 
	* @author: 	Hayden Asplet
	* @param:	std::string _krstrName - name of the component.
	* @return:  bool - true if the component was successfully registered, false if it was already registered.
	*/
	template<typename TComponentType>
	bool RegisterComponent();

	/**
	* Creates an actor from an actor resource file (XML).
	*
	* @author: 	Hayden Asplet
	* @param:	const std::string & _krObjectResource - actor resource file; should be an XML file.
	* @param:	const CMatrix4x4 * _pInitialTransform - an initial transform to override that in the XML file.
	* If NULL no overriding will take place. Also preserves the scale of the XML transform, making the initial
	* transforms scale relative to that of the XML's transform.
	* @param:	TActorID _serverActorID - the actor ID from the server. This is used to sync the IDs of clients
	* with the server actor IDs. If s_kiINVALID_ACTOR_ID the resultant ID will be nondeterministic.
	* @return: unique_ptr<CActor> - resultant actor pointer - NULL if actor creation failed.
	*/
	unique_ptr<CActor> CreateActor(const std::string& _krResource, const CMatrix4x4* _pInitialTransform = nullptr, TActorID _serverActorID = CActor::s_kINVALID_ACTOR_ID);
	
protected:
private:
	/**
	* Creates a component from an XML element.
	*
	* @author: Hayden Asplet
	* @param: TiXmlElement * _pXmlData
	* @return: unique_ptr<CActorComponent> - the resultant actor component - NULL if creation failed.
	*/
	unique_ptr<AActorComponent> CreateComponent(TiXmlElement* _pXmlData);

	/**
	* Increments the last actor ID and returns it.
	*
	* @author: Hayden Asplet
	* @return: TActorID - the next actor ID to use.
	*/
	TActorID GetNextActorID() { ++m_lastActorID; return m_lastActorID; }

	// Member Variables
public:
protected:
private:
	CObjectFactory<AActorComponent, std::string> m_componentCreators;
	TActorID m_lastActorID;	// A counter of actors to ensure completely unique ID's for actors.
};

template<typename TComponentType>
bool CActorFactory::RegisterComponent()
{
	// Add the component to the component creators.
	return m_componentCreators.Register<TComponentType>(TComponentType::s_kstrNAME);
}

#endif // __ACTORFACTORY_H__