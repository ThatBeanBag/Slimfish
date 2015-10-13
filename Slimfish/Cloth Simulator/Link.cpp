// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Link.cpp
// Description	: CLink implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ClothSimulatorStd.h"

// Library Includes

// This Include
#include "Link.h"

// Local Includes
#include "PointMass.h"


CLink::CLink(CPointMass* pPointMassA, CPointMass* pPointMassB, float restingDistance, float stiffness, float breakForce)
	:m_pPointMassA(pPointMassA),
	m_pPointMassB(pPointMassB),
	m_RestingDistance(restingDistance),
	m_Stiffness(stiffness),
	m_BreakForce(breakForce)
{

}

CLink::~CLink()
{

}

void CLink::Solve()
{
	const auto& positionA = m_pPointMassA->GetPosition();
	const auto& positionB = m_pPointMassB->GetPosition();
	auto delta = positionA - positionB;
	auto distance = delta.GetLength();

	auto difference = 0.0f;

	if (distance != 0.0f) {
		difference = (m_RestingDistance - distance) / distance;
	}

	auto impulseA = 1 / m_pPointMassA->GetMass();
	auto impulseB = 1 / m_pPointMassB->GetMass();

	auto newPositionA = positionA + delta * (impulseA / (impulseA + impulseB)) * m_Stiffness * difference;
	auto newPositionB = positionB - delta * (impulseB / (impulseA + impulseB)) * m_Stiffness * difference;

	// Update the positions.
	m_pPointMassA->SetPosition(newPositionA);
	m_pPointMassB->SetPosition(newPositionB);
}
