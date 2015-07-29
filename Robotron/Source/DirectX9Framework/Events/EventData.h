// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EventData.h
// Description	: IEventData declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EVENTDATA_H__
#define __EVENTDATA_H__

// Library Includes

// Local Includes

typedef unsigned int TEventTypeID;

/************************************************************************/
/* IEventData
/*
/* An interface that must be derived from in order to send event data to
/* listeners.
/************************************************************************/
class IEventData {
	// Member Functions
public:
	virtual ~IEventData() {};

	/**
	* Serialises the event data so that it can be sent as a packet.
	* 
	* @author: 	Hayden Asplet
	* @param:	std::ostringstream & outStream - serialisation stream.
	* @return:  void
	*/
	virtual void VSerialise(std::ostringstream& outStream) = 0;

	/**
	* Deserialises event data that has been sent from a remote machine.
	* 
	* @author: 	Hayden Asplet
	* @param:	std::istringstream & inStream - deserialisation stream.
	* @return:  void
	*/
	virtual void VDeserialise(std::istringstream& inStream) = 0;

	/**
	* Gets the event type ID of the event data at run-time.
	* 
	* @author: 	Hayden Asplet
	* @return:  TEventTypeID - event type ID.
	*/
	virtual TEventTypeID VGetEventTypeID() const = 0;
protected:
private:

	// Member Variables
public:
protected:
private:
};

#endif