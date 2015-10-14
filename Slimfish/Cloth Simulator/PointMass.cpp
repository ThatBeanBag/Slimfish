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

// This Include
#include "PointMass.h"

// Local Includes
#include "Link.h"


CPointMass::CPointMass(const CVector3& position)
	:m_Position(position)
{

}

CPointMass::~CPointMass()
{

}

void CPointMass::Update(float timeStep)
{
	// Add the effects of gravity.
	m_Acceleration += CVector3(0.0f, g_GRAVITY, 0.0f);

	// Calculate the velocity to apply.
	CVector3 velocity = m_Position - m_LastPosition;
	// Dampen velocity.
	velocity *= 0.99f;

	float timeStepSquared = timeStep * timeStep;

	CVector3 nextPosition = m_Position + velocity + 0.5f + m_Acceleration * timeStepSquared;

	// Update positions.
	m_LastPosition = m_Position;
	m_Position = nextPosition;

	m_Acceleration = CVector3::s_ZERO;
}

void CPointMass::SolveConstraints()
{
	for (auto& link : m_Links) {
		link.Solve();
	}

	if (m_IsPinned) {
		m_Position = m_PinPosition;
	}
}

void CPointMass::Attach(CPointMass* pointMass, float restingDistance, float stiffness, float breakForce)
{
	m_Links.emplace_back(this, pointMass, restingDistance, stiffness, breakForce);
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

void CPointMass::SetPosition(const CVector3& position)
{
	m_Position = position;
}

const CVector3& CPointMass::GetPosition() const
{
	return m_Position;
}

void CPointMass::SetMass(float mass)
{
	m_Mass = mass;
}

const float CPointMass::GetMass() const
{
	return m_Mass;
}

const std::vector<CLink>& CPointMass::GetLinks() const
{
	return m_Links;
}
