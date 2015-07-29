// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Packet.h
// Description	: IPacket declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __PACKET_H__
#define __PACKET_H__

// Library Includes

// Local Includes

class IPacket {
	// Member Functions
public:
	virtual ~IPacket() {};

	virtual void VSetData(const char* _pcData, unsigned long _ulSize) = 0;
	virtual const char* VGetData() const = 0;
	virtual int VGetLength() const = 0;
protected:
private:
	// Member Variables
public:
protected:
private:
};

class CTextPacket : public IPacket {
	// Member Functions
public:
	CTextPacket();
	CTextPacket(char const* const _pcData);
	virtual ~CTextPacket();

	virtual void VSetData(const char* _pcData, unsigned long _ulLength);
	virtual const char* VGetData() const;
	virtual int VGetLength() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	int m_iLength;
	char* m_pcData;
};

#endif	// __PACKET_H__


