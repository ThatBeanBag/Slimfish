// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimObjectFactory.h
// Description	: CObjectFactory declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMOBJECTFACTORY_H__
#define __SLIMOBJECTFACTORY_H__

// Library Includes

// Local Includes

namespace Slim {

/************************************************************************/
/* CObjectFactory
/*
/* A generic class for creating sub types of a base type through unique 
/* identifiers.
/*
/* The factory basically provides a mapping between identifiers and 
/* subclasses.
/************************************************************************/
template<typename TBaseType, typename TIDType>
class CObjectFactory {
	// Member Functions
	typedef TBaseType* (*ObjectCreationFunction)();
	typedef std::map<TIDType, ObjectCreationFunction> TCreationMap;

public:
	CObjectFactory();
	~CObjectFactory();

	template<typename TSubType>
	bool Register(TIDType id) {
		// Check if a creator has already been registered for the given ID.
		typename TCreationMap::iterator findIter = m_creationFunctions.find(_ID);

		if (findIter != m_creationFunctions.end()) {
			// Is there already a creator registered?
			// Attempting to register a creator function twice.
			return false;
		}
		else {
			// No currently registered creator.
			m_creationFunctions[id] = &CreateObjectFunction<TSubType>;
			return true;
		}

		return false;
	}

	TBaseType* Create(TIDType id) {
		typename TCreationMap::iterator findIter = m_creationFunctions.find(_ID);

		if (findIter != m_creationFunctions.end()) {
			// Is there a creator registered?
			// Call the creator function.
			return findIter->second();
		}
		else {
			// No currently registered creator.
			return nullptr;
		}

		return nullptr;
	}
protected:
private:
	template<typename TSubType>
	static TBaseType* CreateObjectFunction()
	{
		return new TSubType;
	}

	// Member Variables
public:
protected:
private:
};

template<typename TBaseType, typename TIDType>
template<typename TSubType>
bool CObjectFactory<TBaseType, TIDType>::Register(TIDType id) {
	// Check if a creator has already been registered for the given ID.
	typename TCreationMap::iterator findIter = m_creationFunctions.find(id);

	if (findIter != m_creationFunctions.end()) {
		// Is there already a creator registered?
		// Attempting to register a creator function twice.
		return false;
	}
	else {
		// No currently registered creator.
		m_creationFunctions[id] = &CreateObjectFunction<TSubType>;
		return true;
	}

	return false;
}

template<typename TBaseType, typename TIDType>
TBaseType* CObjectFactory<TBaseType, TIDType>::Create(TIDType id) {
	typename TCreationMap::iterator findIter = m_creationFunctions.find(id);

	if (findIter != m_creationFunctions.end()) {
		// Is there a creator registered?
		// Call the creator function.
		return findIter->second();
	}
	else {
		// No currently registered creator.
		return nullptr;
	}

	return nullptr;
}

template<typename TBaseType, typename TIDType>
template<typename TSubType>
static TBaseType* CObjectFactory<TBaseType, TIDType>::CreateObjectFunction()
{
	return new TSubType;
}

}

#endif	// __SLIMOBJECTFACTORY_H__