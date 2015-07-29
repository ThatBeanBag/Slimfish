// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ObjectFactory.h
// Description	: CObjectFactory declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __OBJECTFACTORY_H__
#define __OBJECTFACTORY_H__

// Library Includes

// Local Includes

template<typename TBaseType, typename TIDType>
class CObjectFactory {
	// Member Functions
	typedef TBaseType* (*ObjectCreationFunction)();
	typedef std::map<TIDType, ObjectCreationFunction> TCreationMap;
public:
	CObjectFactory();
	~CObjectFactory();

	template<typename TSubType>
	bool Register(TIDType _ID)
	{
		// Check if a creator has already been registered for the given ID.
		typename TCreationMap::iterator findIter = m_creationFunctions.find(_ID);

		if (findIter != m_creationFunctions.end()) {
		// Is there already a creator registered?
			// Attempting to register a creator function twice.
			return false;
		}
		else {
		// No currently registered creator.
			m_creationFunctions[_ID] = &CreateObjectFunction<TSubType>;
			return true;
		}

		return false;
	}

	TBaseType* Create(TIDType _ID);
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
	TCreationMap m_creationFunctions;
};

template<typename TBaseType, typename TIDType>
CObjectFactory<TBaseType, TIDType>::CObjectFactory()
{

}

template<typename TBaseType, typename TIDType>
CObjectFactory<TBaseType, TIDType>::~CObjectFactory()
{

}

template<typename TBaseType, typename TIDType>
TBaseType* CObjectFactory<TBaseType, TIDType>::Create(TIDType _ID)
{
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

#endif	// __OBJECTFACTORY_H__