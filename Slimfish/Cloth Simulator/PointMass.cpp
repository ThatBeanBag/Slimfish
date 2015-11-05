// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: PointMass.cpp
// Description	: CPointMass implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ClothSimulatorStd.h"

// Library Includes
#include <algorithm>

// This Include
#include "PointMass.h"

// Local Includes
#include "Link.h"


CPointMass::CPointMass(const CVector3& position, float mass, float damping)
	:m_Position(position),
	m_LastPosition(position),
	m_Mass(mass),
	m_Damping(damping)
{

}

CPointMass::~CPointMass()
{

}

void CPointMass::Update(float timeStep)
{
	// Add the effects of gravity.
	m_Acceleration -= CVector3(0.0f, g_GRAVITY, 0.0f);

	float timeStepSquared = timeStep * timeStep;

	CVector3 velocity = m_Position - m_LastPosition;

	m_LastPosition = m_Position;
	m_Position += velocity * (1.0f - m_Damping) + m_Acceleration * timeStepSquared;

	m_Acceleration = CVector3::s_ZERO;
}

void CPointMass::SolveConstraints()
{
	for (auto iter = m_Links.begin(); iter != m_Links.end();) {
		if (!(*iter).Solve()) {
			iter = m_Links.erase(iter);
		}
		else {
			++iter;
		}
	}

	if (m_IsPinned) {
		m_Position = m_PinPosition;
	}
}

void CPointMass::Attach(CPointMass* pointMass, float restingDistance, float stiffness, float breakForce, bool isVisible /*= true*/)
{
	m_Links.emplace_back(this, pointMass, restingDistance, stiffness, breakForce, isVisible);
}

void CPointMass::ApplyForce(const CVector3& force)
{
	if (m_Mass > 0.0f) {
		m_Acceleration += force * (1.0f / m_Mass);
	}
}

void CPointMass::Pin(const CVector3& pinPosition)
{
	m_IsPinned = true;
	m_PinPosition = pinPosition;
}

void CPointMass::DetachPin()
{
	m_IsPinned = false;
}

void CPointMass::RemoveLink(CLink* pLink)
{
	auto iter = std::remove_if(m_Links.begin(), m_Links.end(), [&](const CLink& link){ return &link == pLink; });
	if (iter != m_Links.end()) {
		m_BrokenLinks.push_back((*iter));
	}

	m_Links.erase(iter);
}

void CPointMass::ClearLinks()
{
	m_Links.clear();
}

void CPointMass::SetPosition(const CVector3& position)
{
	m_Position = position;
}

const CVector3& CPointMass::GetPosition() const
{
	return m_Position;
}

void CPointMass::SetLastPosition(CVector3 lastPosition)
{
	m_LastPosition = lastPosition;
}

const CVector3 CPointMass::GetLastPosition() const
{
	return m_LastPosition;
}

void CPointMass::SetMass(float mass)
{
	m_Mass = mass;
}

const float CPointMass::GetMass() const
{
	return m_Mass;
}

const bool CPointMass::IsPinned() const
{
	return m_IsPinned;
}

void CPointMass::AddToBurntLevel(float burntLevel)
{
	m_BurntLevel += burntLevel;
	for (auto link : m_Links) {
		link.SetRestingDistance(link.GetRestingDistance() + (burntLevel * 5.0f));
	}
	if (m_BurntLevel >= 1.0f) {
		ClearLinks();
	}
}

void CPointMass::SetBurntLevel(float burntLevel)
{
	m_BurntLevel = burntLevel;
}

const float CPointMass::GetBurntLevel() const
{
	return m_BurntLevel;
}

Slim::CVector3 CPointMass::GetVelocity() const
{
	return m_Position - m_LastPosition;
}

const CVector3 CPointMass::GetTotalForce() const
{
	return m_Acceleration * m_Mass;
}

bool CPointMass::HasLinkTo(CPointMass* pPointMass)
{
	auto isLinkedToMass = [&](const CLink& link) { return link.GetPointMassB() == pPointMass; };
	return std::find_if(m_Links.begin(), m_Links.end(), isLinkedToMass) != m_Links.end();
}

const std::vector<CLink>& CPointMass::GetLinks() const
{
	return m_Links;
}

const std::vector<CLink>& CPointMass::GetBrokenLinks() const
{
	return m_BrokenLinks;
}
