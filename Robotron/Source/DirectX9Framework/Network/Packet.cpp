// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Packet.cpp
// Description	: IPacket implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes
#include <cstring>
#include <assert.h>

// This Include
#include "Packet.h"

// Local Includes

CTextPacket::CTextPacket()
	:m_iLength(0),
	m_pcData(nullptr)
{

}

CTextPacket::CTextPacket(char const* const _pcData)
	:CTextPacket()
{
	VSetData(_pcData, strlen(_pcData) + 1);
}

CTextPacket::~CTextPacket()
{
	if (m_pcData) {
		delete m_pcData;
		m_pcData = nullptr;
	}
}

void CTextPacket::VSetData(const char* _pcData, unsigned long _ulLength)
{
	// De-allocate any previously set data.
	if (m_pcData) {
		delete m_pcData;
		m_pcData = nullptr;
	}

	m_iLength = strlen(_pcData) + 1;

	m_pcData = new char[m_iLength];

	assert(m_pcData);
	strcpy_s(m_pcData, m_iLength, _pcData);
}

const char* CTextPacket::VGetData() const
{
	return m_pcData;
}

int CTextPacket::VGetLength() const
{
	return m_iLength;
}
