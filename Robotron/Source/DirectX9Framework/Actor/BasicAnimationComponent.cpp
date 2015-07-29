// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BasicAnimationComponent.cpp
// Description	: CBasicAnimationComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes
#include <tinyxml.h>

// This Include
#include "BasicAnimationComponent.h"
#include "PhysicsComponent.h"

// Local Includes

const std::string CBasicAnimationComponent::s_kstrNAME = "BasicAnimationComponent";

CBasicAnimationComponent::CBasicAnimationComponent()
	:m_fSpeedSquared(0.0f),
	m_fVelocityThreshold(0.0f),
	m_fWalkHeight(0.0f),
	m_fWalkThreshold(0.0f),
	m_fWalkTimer(0.0f),
	m_fScaleTimer(0.0f),
	m_bIsWalkingAnimation(false),
	m_bIsScale(false)
{
	CEventManager::GetInstance()->AddListener<CEventData_NewRenderComponent>(MakeDelegate(this, &CBasicAnimationComponent::NewRenderComponentDelegate));
	CEventManager::GetInstance()->AddListener<CEventData_MoveActor>(MakeDelegate(this, &CBasicAnimationComponent::MoveActorDelegate));
}

CBasicAnimationComponent::~CBasicAnimationComponent()
{
	CEventManager::GetInstance()->RemoveListener<CEventData_NewRenderComponent>(MakeDelegate(this, &CBasicAnimationComponent::NewRenderComponentDelegate));
	CEventManager::GetInstance()->RemoveListener<CEventData_MoveActor>(MakeDelegate(this, &CBasicAnimationComponent::MoveActorDelegate));
}

bool CBasicAnimationComponent::VInitialise(TiXmlElement* _pXmlData)
{
	TiXmlElement* pWalkAnimation = _pXmlData->FirstChildElement("WalkAnimation");
	if (pWalkAnimation) {
		double dWalkTime = 0.0;
		double dWalkHeight = 0.0;
		double dWalkSpeed = 0.0;

		pWalkAnimation->Attribute("time", &dWalkTime);
		pWalkAnimation->Attribute("height", &dWalkHeight);
		pWalkAnimation->Attribute("speed", &dWalkSpeed);	// The speed you must be going at to be considered 'walking'.

		m_fWalkThreshold = static_cast<float>(dWalkTime);
		m_fWalkHeight = static_cast<float>(dWalkHeight);
		m_fVelocityThreshold = static_cast<float>(dWalkSpeed);

		m_bIsWalkingAnimation = true;
	}

	TiXmlElement* pScaleAnimation = _pXmlData->FirstChildElement("ScaleAnimation");
	if (pScaleAnimation) {
		double dScaleTime;

		pScaleAnimation->Attribute("time", &dScaleTime);

		m_fScaleThreshold = static_cast<float>(dScaleTime);

		m_bIsScale = true;
	}

	return true;
}

void CBasicAnimationComponent::VUpdate(float _fDeltaTime)
{
	DEBUG_ASSERT(m_pSceneNode);

	if (m_bIsWalkingAnimation && m_fSpeedSquared > m_fVelocityThreshold * m_fVelocityThreshold) {
		m_fWalkTimer += _fDeltaTime;

		if (m_fWalkTimer >= m_fWalkThreshold) {
			m_fWalkTimer = 0.0f;

			m_bIsWalkUp = !m_bIsWalkUp;
			CMatrix4x4 matRelativeTransform = CMatrix4x4::s_kIdentity;

			if (m_bIsWalkUp) {
				matRelativeTransform.SetPosition(0.0f, m_fWalkHeight, 0.0f);
			}

			m_pSceneNode->SetRelativeTransform(matRelativeTransform);
		}

		m_fSpeedSquared = 0.0f;
	}

	if (m_bIsScale) {
		m_fScaleTimer += _fDeltaTime;
		float fScale = 1.0f;

		if (m_fScaleTimer < m_fScaleThreshold) {
			fScale = (m_fScaleThreshold * m_fScaleTimer * m_fScaleTimer * m_fScaleTimer);
		}

		m_pSceneNode->SetRelativeTransform(BuildScale(fScale, fScale, fScale));
	}
}

void CBasicAnimationComponent::NewRenderComponentDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_NewRenderComponent> pNewRenderData = static_pointer_cast<CEventData_NewRenderComponent>(_pEventData);
	
	if (pNewRenderData->GetActorID() == GetOwnerID()) {
	// Is this a new render component for our owner?
		// Store the node.
		m_pSceneNode = pNewRenderData->GetSceneNode();
	}
}

void CBasicAnimationComponent::MoveActorDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_MoveActor> pMoveActorData = static_pointer_cast<CEventData_MoveActor>(_pEventData);

	if (pMoveActorData->GetActorID() == GetOwnerID()) {
		CVec3 vec3NewPosition = pMoveActorData->GetTransform().GetPosition();
		CVec3 vec3DeltaPosition = vec3NewPosition - m_vec3LastPosition;
		m_fSpeedSquared = vec3DeltaPosition.GetSquaredLength();
	}
}

