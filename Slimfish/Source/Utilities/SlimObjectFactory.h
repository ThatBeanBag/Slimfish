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

/** Generic class for creating sub types of a base type through unique identifiers.
@remarks
	Provides a mapping between identifiers and subclasses for easy creation of sub types.
*/
template<typename TBaseType, typename TIDType>
class CObjectFactory {
	// Member Functions
	typedef TBaseType* (*ObjectCreationFunction)();
	typedef std::map<TIDType, ObjectCreationFunction> TCreationMap;

public:
	/** Default constructor.
	 	@author
	 		Hayden Asplet
	*/
	CObjectFactory();

	/** Destructor.
	 	@author
	 		Hayden Asplet	
	*/
	~CObjectFactory();

	/** Register a creator for a sub type.
	 	@author
	 		Hayden Asplet
	 	@param	
			id Identifier to use when creating objects of the sub type.
	 	@return   
	 		bool True if the sub type was registered successfully, false if a creator
			was already registered for the input id and thus failed to register.
	*/
	template<typename TSubType>
	bool Register(TIDType id);

	/** Create a object of a sub type that has been mapped to an id.
	 	@author
	 		Hayden Asplet
	 	@param	
			id Identifier of the sub type to create, which has been registered using 
			Register().
	 	@return   
	 		Pointer to the created object. Will be NULL if there is
			no sub type creator registered for the input id.
	*/
	TBaseType* Create(TIDType id);
protected:
private:
	/** Internal helper method for creating objects of a sub type.
	 	@author
	 		Hayden Asplet
	 	@return   
	 		Pointer to the created object.
	*/
	template<typename TSubType>
	static TBaseType* CreateObjectFunction();

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