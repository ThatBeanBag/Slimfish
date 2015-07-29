// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: PlayerComponent.h
// Description	: CPlayerComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __PLAYERCOMPONENT_H__
#define __PLAYERCOMPONENT_H__

// Library Includes

// Local Includes

/************************************************************************/
/* CPlayerComponent
/*
/* A Robotron specific component that manages stuff specific to players
/* such as ammo, lives, score etc.
/************************************************************************/
class CPlayerComponent : public AActorComponent {
	// Member Functions
public:
	CPlayerComponent();
	~CPlayerComponent();

	virtual bool VInitialise(TiXmlElement* _pXmlData) override;
	virtual void VPostInitialise() override;
	virtual void VUpdate(float _fDeltaTime) override;

	void DecrementLives();
	void IncrementLives();
	int GetLives() const;

	void IncrementAmmo();
	void DecrementAmmo();
	void SetAmmo(int _iAmount);
	int GetAmmo() const;

	void ApplyGoldenPowerup(float _fTimeSpan);
	bool HasGoldenPowerup() const;
protected:
private:
	void UpdateState();


	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	int m_iLives;
	int m_iAmmo;

	bool m_bHasGoldenPowerup;
	float m_fGoldenPowerupTimer;
	float m_fGolderPowerupTimeSpan;
};

#endif	// __PLAYERCOMPONENT_H__