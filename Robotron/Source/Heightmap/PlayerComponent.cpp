// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: PlayerComponent.cpp
// Description	: CPlayerComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes
#include <tinyxml.h>

// This Include
#include "PlayerComponent.h"
#include "EventData_PlayerStateChange.h"

// Local Includes

const std::string CPlayerComponent::s_kstrNAME = "PlayerComponent";

CPlayerComponent::CPlayerComponent() 
	:m_iLives(1),
	m_iAmmo(0)
{

}

CPlayerComponent::~CPlayerComponent()
{

}

bool CPlayerComponent::VInitialise(TiXmlElement* _pXmlData)
{
	DEBUG_ASSERT(_pXmlData);

	TiXmlElement* pStartingLives = _pXmlData->FirstChildElement("StartingLives");
	if (pStartingLives && pStartingLives->FirstChild()) {
		const char* pcStartingLives = pStartingLives->FirstChild()->Value();
		if (pcStartingLives) {
			m_iLives = std::atoi(pcStartingLives);
		}
	}

	TiXmlElement* pStartingAmmo = _pXmlData->FirstChildElement("StartingAmmo");
	if (pStartingLives && pStartingLives->FirstChild()) {
		const char* pcStartingAmmo = pStartingAmmo->FirstChild()->Value();
		if (pcStartingAmmo) {
			m_iAmmo= std::atoi(pcStartingAmmo);
		}
	}

	return true;
}

void CPlayerComponent::VPostInitialise()
{
	UpdateState();
}

void CPlayerComponent::VUpdate(float _fDeltaTime)
{
	if (m_bHasGoldenPowerup) {
		m_fGoldenPowerupTimer += _fDeltaTime;
		if (m_fGoldenPowerupTimer > m_fGolderPowerupTimeSpan) {
		// Has the golden power up run out?
			m_bHasGoldenPowerup = false;
			m_fGoldenPowerupTimer = 0.0f;
			m_fGolderPowerupTimeSpan = 0.0f;

			UpdateState();
		}
	}
}

void CPlayerComponent::DecrementLives()
{
	--m_iLives; 

	if (m_iLives < 0) { 
		m_iLives = 0; 
	}

	UpdateState();
}

void CPlayerComponent::IncrementLives()
{
	++m_iLives;

	UpdateState();
}

int CPlayerComponent::GetLives() const
{
	return m_iLives;
}

void CPlayerComponent::IncrementAmmo()
{
	++m_iAmmo; 
	if (m_iAmmo < 0) { 
		m_iAmmo = 0; 
	}

	UpdateState();
}

void CPlayerComponent::DecrementAmmo()
{
	--m_iAmmo;

	UpdateState();
}

void CPlayerComponent::SetAmmo(int _iAmount)
{
	m_iAmmo = _iAmount;

	UpdateState();
}

int CPlayerComponent::GetAmmo() const
{
	return m_iAmmo;
}

void CPlayerComponent::ApplyGoldenPowerup(float _fTimeSpan)
{
	m_fGoldenPowerupTimer = 0.0f;
	m_fGolderPowerupTimeSpan = _fTimeSpan;
	m_bHasGoldenPowerup = true;
	UpdateState();
}

bool CPlayerComponent::HasGoldenPowerup() const
{
	return m_bHasGoldenPowerup;
}

void CPlayerComponent::UpdateState()
{
	shared_ptr<CEventData_PlayerStateChange> pEvent(new CEventData_PlayerStateChange(GetOwnerID(), m_iAmmo, m_iLives, m_bHasGoldenPowerup, true));
	CEventManager::GetInstance()->QueueEvent(pEvent);
}

