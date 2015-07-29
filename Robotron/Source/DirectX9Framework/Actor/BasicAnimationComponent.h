// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BasicAnimationComponent.h
// Description	: CBasicAnimationComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __BASICANIMATIONCOMPONENT_H__
#define __BASICANIMATIONCOMPONENT_H__

// Library Includes

// Local Includes
#include "ActorComponent.h"

class CBasicAnimationComponent : public AActorComponent {
	// Member Functions
public:
	CBasicAnimationComponent();
	~CBasicAnimationComponent();

	virtual bool VInitialise(TiXmlElement* _pXmlData) override;
	virtual void VUpdate(float _fDeltaTime) override;

	void Shrink() { m_bIsShrinking = true; }
protected:
private:
	void NewRenderComponentDelegate(TEventDataPtr _pEventData);
	void MoveActorDelegate(TEventDataPtr _pEventData);

	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	float m_fWalkTimer;
	float m_fWalkThreshold;
	float m_fVelocityThreshold;
	float m_fWalkHeight;
	bool m_bIsWalkUp;
	bool m_bIsWalkingAnimation;

	float m_fScaleTimer;
	float m_fScaleThreshold;
	bool m_bIsScale;
	bool m_bIsShrinking;

	float m_fSpeedSquared;
	CVec3 m_vec3LastPosition;

	shared_ptr<CSceneNode> m_pSceneNode;
};

#endif	// __BASICANIMATIONCOMPONENT_H__