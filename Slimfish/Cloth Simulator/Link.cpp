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


CLink::CLink(CPointMass* pPointMassA, CPointMass* pPointMassB, float restingDistance, float stiffness, float breakForce, bool isVisible)
	:m_pPointMassA(pPointMassA),
	m_pPointMassB(pPointMassB),
	m_RestingDistance(restingDistance),
	m_Stiffness(stiffness),
	m_TearDistance(breakForce),
	m_bIsVisible(isVisible)
{

}

CLink::~CLink()
{

}

bool CLink::Solve()
{
	const auto& positionA = m_pPointMassA->GetPosition();
	const auto& positionB = m_pPointMassB->GetPosition();
	auto delta = positionA - positionB;
	auto distance = delta.GetLength();

	if (distance >= m_TearDistance) {
		return false;
	}

	auto difference = 0.0f;

	if (distance != 0.0f) {
		difference = (m_RestingDistance - distance) / distance;
	}

	auto inverseMassA = 1.0f / m_pPointMassA->GetMass();
	auto inverseMassB = 1.0f / m_pPointMassB->GetMass();

	auto newPositionA = positionA + delta * (inverseMassA / (inverseMassA + inverseMassB)) * m_Stiffness * difference;
	auto newPositionB = positionB - delta * (inverseMassB / (inverseMassA + inverseMassB)) * m_Stiffness * difference;

	// Update the positions.
	m_pPointMassA->SetPosition(newPositionA);
	m_pPointMassB->SetPosition(newPositionB);

	return true;
}

void CLink::SetPointMassA(CPointMass* pointMassA)
{
	m_pPointMassA = pointMassA;
}

const CPointMass* CLink::GetPointMassA() const
{
	return m_pPointMassA;
}

void CLink::SetPointMassB(CPointMass* pointMassB)
{
	m_pPointMassB = pointMassB;
}

const CPointMass* CLink::GetPointMassB() const
{
	return m_pPointMassB;
}

void CLink::SetIsVisible(bool isVisible)
{
	m_bIsVisible = isVisible;
}

const bool CLink::GetIsVisible() const
{
	return m_bIsVisible;
}

void CLink::SetStiffness(float stiffness)
{
	m_Stiffness = stiffness;
}

const float CLink::GetStiffness() const
{
	return m_Stiffness;
}

void CLink::SetRestingDistance(float restingDistance)
{
	m_RestingDistance = restingDistance;
}

const float CLink::GetRestingDistance() const
{
	return m_RestingDistance;
}

void CLink::SetTearDistance(float tearDistance)
{
	m_TearDistance = tearDistance;
}

const float CLink::GetTearDistance() const
{
	return m_TearDistance;
}
