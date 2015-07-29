// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AIView.cpp
// Description	: CAIView implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "AIView.h"
#include "..\DirectX9Framework\Actor\AIComponent.h"

CAIView::CAIView()
{
	CEventManager::GetInstance()->AddListener<CEventData_MoveActor>(MakeDelegate(this, &CAIView::MoveActorDelegate));
}

CAIView::~CAIView()
{
	CEventManager::GetInstance()->RemoveListener<CEventData_MoveActor>(MakeDelegate(this, &CAIView::MoveActorDelegate));
}

bool CAIView::VOnRestore()
{
	return true;
}

void CAIView::VOnUpdate(float m_fDeltaTime)
{
	/*CActor* pAIActor = g_pApp->GetGame()->GetActor(m_actorID);
	if (!pAIActor) {
		return;
	}
		
	CMatrix4x4 matTransform = GetTransform(pAIActor);

	const CGameLogic::TActorMap& actorMap = g_pApp->GetGame()->GetActorMap();
	for (auto iter = actorMap.begin(); iter != actorMap.end(); ++iter) {
		if (iter->second->GetType() == "Player") {
			CMatrix4x4 matPlayerTransform = GetTransform(iter->second.get());

			CVec3 vec3Distance = matPlayerTransform.GetPosition() - matTransform.GetPosition();
			float fDistance = vec3Distance.GetSquaredLength();
			if (fDistance <= 1.0f) {
				CAIComponent* pAIComponent = pAIActor->GetComponent<CAIComponent>();
				if (pAIComponent) {
					pAIComponent->Seek(iter->first);
				}
			}
		}
	}*/
}

void CAIView::VRender()
{
	return;
}

bool CAIView::VOnMsgProc(const TAppMsg& _krMsg)
{
	return false;
}

bool CAIView::VLoadGame(const CIniDocument& _krIniDocument)
{
	return true;
}

void CAIView::VSetControlledActor(TActorID _actorID)
{
	m_actorID = _actorID;
}

void CAIView::MoveActorDelegate(TEventDataPtr _pEventData)
{
	shared_ptr<CEventData_MoveActor> pMoveActorData = static_pointer_cast<CEventData_MoveActor>(_pEventData);

	CActor* pActor = g_pApp->GetGame()->GetActor(pMoveActorData->GetActorID());
	if (pActor && pActor->GetType() == "Player") {
		CMatrix4x4 matPlayerTransform = GetTransform(pActor);

		CActor* pAIActor = g_pApp->GetGame()->GetActor(m_actorID);
		if (!pAIActor) {
			return;
		}

		CMatrix4x4 matTransform = GetTransform(pAIActor);

		CVec3 vec3Distance = matPlayerTransform.GetPosition() - matTransform.GetPosition();
		float fDistance = vec3Distance.GetSquaredLength();
		if (fDistance <= 3.0f * 3.0f) {
			CAIComponent* pAIComponent = pAIActor->GetComponent<CAIComponent>();
			if (pAIComponent) {
				pAIComponent->Seek(pMoveActorData->GetActorID());
			}
		}
	}
}

// Local Includes
