// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ActorComponent.h
// Description	: CActorComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __ACTORCOMPONENT_H__
#define __ACTORCOMPONENT_H__

// Library Includes

// Local Includes

// Forward Declaration
class CActor;
class TiXmlElement;

/**
* Creates a transformation matrix from a xml element.
*
* An example element would be:
*
* <Scale x = "1.0" y = "1.0" z = "1.0 />
* <Rotation yaw = "0.0" pitch = "90.0" roll = "45.0" />
* <Translation x = "0.0" y = "0.0" z = "0.0" />
*
* All rotations will be read as degrees.
*
* @author: 	Hayden Asplet
* @param:	TiXmlElement * _pElement - the XML element.
* @return:  CMatrix4x4 - the generated matrix.
*/
const CMatrix4x4 CreateTransformFromXML(TiXmlElement* _pElement);
const TColourValue CreateColourValueFromXML(TiXmlElement* _pXmlData);

class AActorComponent {
	friend class CActorFactory; // Only actor factory should be setting the owner.

	// Member Functions
public:
	AActorComponent();
	virtual ~AActorComponent();

	/**
	* Intitialises the component.
	*
	* All derived classes should overload this.
	* Returns false if the initialisation was unsuccessful.
	*
	* @author: Hayden Asplet
	* @param: TiXmlElement * _pXmlData - the XML data to initialise from.
	* @return: bool - true if initialisation was successful.
	*/
	virtual bool VInitialise(TiXmlElement* _pXmlData) = 0;

	/**
	* Runs the post initialisation sequence
	*
	* All derived classes should overload this.
	*
	* @author: Hayden Asplet
	* @return: void
	*/
	virtual void VPostInitialise();

	/**
	* Updates the component.
	*
	* All derived classes should overload this.
	*
	* @author: Hayden Asplet
	* @param: float _fDeltaTime - delta time of last tick.
	* @return: void
	*/
	virtual void VUpdate(float _fDeltaTime);

	/**
	* Gets the owner of the component.
	*
	* This is just for access to the actor from derived classes.
	*
	* @author: Hayden Asplet
	* @return: CActor* - handle to the owner.
	*/
	CActor* GetOwner();

	/**
	* Gets the ID of the owner of the component.
	*
	* This is just a helper function instead of having to go GetOwner()->GetID().
	* 
	* @author: 	Hayden Asplet
	* @return:  TActorID - ID of owner.
	*/
	TActorID GetOwnerID();
protected:

private:
	/**
	* Sets the owner of the component.
	*
	* Only the actor factory should be setting the owner.
	*
	* @author: Hayden Asplet
	* @param: CActor * _pOwner - raw pointer to the actor, assumed valid.
	* @return: void
	*/
	void SetOwner(CActor* _pOwner);

	/**
	* Returns if the component is active on remote logics.
	*
	* Only the actor factory should need to worry about this.
	* 
	* @author: 	Hayden Asplet
	* @return:  bool - true if the component is active on remote clients.
	*/
	virtual bool VIsActiveOnRemote() const;

	// Member Variables
public:
protected:
private:
	CActor* m_pOwner;	// Raw pointer to the owner of this component.
						// This is a raw pointer as the lifetime of the
						// the owner matches or exceeds the lifetime of the
						// component. 
};

#endif // __ACTORCOMPONENT_H__