// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Cloth.cpp
// Description	: CCloth implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ClothSimulatorStd.h"

// Library Includes

// This Include
#include "Cloth.h"

// Local Includes
#include "Link.h"



CCloth::CCloth()
{

}

CCloth::~CCloth()
{

}

void CCloth::Initalise()
{
	m_BurningPoints.clear();
	m_PinnedPoints.clear();
	m_PointMasses.clear();
	m_PointMasses.reserve(m_NumMassesX * m_NumMassesY);
	float midWidth = (m_NumMassesX * m_RestingDistance) / 2.0f;
	float midHeight = (m_NumMassesY * m_RestingDistance) / 2.0f;

	for (size_t y = 0; y < m_NumMassesY; ++y) {
		float yPosition = m_HungFromHeight - (y * m_RestingDistance);

		for (size_t x = 0; x < m_NumMassesX; ++x) {
			CVector3 position(m_RestingDistance * x - midWidth, yPosition, 0.0f);

			// Add a very small movement in the Z for one of the points to avoid no movement in the z axis.
			if (x % 2 == 0) {
				position.SetZ(0.001f);
			}

			auto pPointMass = std::make_unique<CPointMass>(position, 1.0f, 0.01f);
			float diagonalRestingDistance = sqrt(2 * (m_RestingDistance * m_RestingDistance));
			auto diagonalBreakingDistnace = sqrt(2 * (m_LinkBreakingDistance * m_LinkBreakingDistance));

			// Create links.
			if (x != 0) {	// Horizontal link.
				pPointMass->Attach(GetPointMass(x - 1, y), m_RestingDistance, m_Stiffness, m_LinkBreakingDistance);

				// Create tightly bound link.
				if (y != 0) {
					pPointMass->Attach(GetPointMass(x - 1, y - 1), diagonalRestingDistance, m_Stiffness / 6.0f, diagonalBreakingDistnace, false);
				}

				if (x > 1) {
					pPointMass->Attach(GetPointMass(x - 2, y), m_RestingDistance * 2.0f, m_Stiffness / 3.0f, m_LinkBreakingDistance * 2.0f, false);
				}
			}

			if (y != 0) {	// Vertical link.
				pPointMass->Attach( GetPointMass(x, y - 1), m_RestingDistance, m_Stiffness, m_LinkBreakingDistance);

				// Create tightly bound link.
				if (x < m_NumMassesX - 1) {
					pPointMass->Attach(GetPointMass(x + 1, y - 1), diagonalRestingDistance, m_Stiffness / 6.0f, diagonalBreakingDistnace, false);
				}

				if (y > 1) {
					pPointMass->Attach(GetPointMass(x, y - 2), m_RestingDistance * 2.0f, m_Stiffness / 3.0f, m_LinkBreakingDistance * 2.0f, false);
				}
			}

			m_PointMasses.push_back(std::move(pPointMass));
		}
	}

	PinCloth();
}

void CCloth::Update(float deltaTime)
{
	static float fixedDeltaTime = 16.0f / 1000.0f;
	static int constraintAccuracy = 2;

	/*m_AccumulatedTime += deltaTime;
	if (m_AccumulatedTime <= fixedDeltaTime) {
	return;
	}

	m_AccumulatedTime = 0.0f;*/

	static const float minClothDistance = 0.7f;
	static const float minClothDistanceSqr = minClothDistance * minClothDistance;

	for (int j = 0; j < constraintAccuracy; ++j) {
		for (auto& pointMass : m_PointMasses) {
			pointMass->SolveConstraints();
		}
	}

	/*while (m_AccumulatedTime >= fixedDeltaTime) {
	for (auto& pointMass : m_PointMasses) {
	pointMass->Update(fixedDeltaTime);
	}

	m_AccumulatedTime -= fixedDeltaTime;
	}*/

	for (auto& pointMass : m_PointMasses) {
		pointMass->Update(deltaTime);
	}

	// Handle collision with self.
	for (unsigned int i = 0; i < m_PointMasses.size(); ++i) {
		auto iMass = m_PointMasses[i].get();
		auto iPosition = iMass->GetPosition();

		for (unsigned int j = i + 1; j < m_PointMasses.size(); ++j) {
			auto jMass = m_PointMasses[j].get();
			auto jPosition = jMass->GetPosition();
			auto toPointMass = jPosition - iPosition;
			auto sqrDistance = toPointMass.GetLengthSquared();

			if (sqrDistance < minClothDistanceSqr) {
				//iMass->SetPosition(iMass->GetLastPosition());
				//jMass->SetPosition(jMass->GetLastPosition());

				toPointMass /= sqrt(sqrDistance);
				iMass->SetPosition(jPosition - (toPointMass * (minClothDistance + 0.0001f)));
				jMass->SetPosition(iPosition + (toPointMass * (minClothDistance + 0.0001f)));

				//m_PointMasses[i]->ApplyForce(-toPointMass * (200.0f));
				//m_PointMasses[j]->ApplyForce(toPointMass * (200.0f));
			}

			if (m_IsBurning) {
				// Update burning if the two masses are too close.
				if (iMass->GetBurntLevel() - std::floor(iMass->GetBurntLevel()) > m_PropagateBurningThreshold ||
					jMass->GetBurntLevel() - std::floor(jMass->GetBurntLevel()) > m_PropagateBurningThreshold) {
					// Get a semi-random distance to compare with the point to burn.
					auto randDistance = Math::Random(m_MinBurningDistance, m_MaxBurningDistance);

					if (sqrDistance < randDistance * randDistance && Math::Random() < m_ChanceToPropagate) {
						// Burn the new point if we are not already.
						jMass->SetIsBurning(true);
						iMass->SetIsBurning(true);
					}
				}
			}
		}
	}

	//if (m_IsBurning) {
	//	UpdateBurning(deltaTime);
	//}
}

void CCloth::PinCloth()
{
	// Unpin the cloth if we are already pinned.
	UnPin();

	float midWidth = (m_NumMassesX * m_RestingDistance) / 2.0f;
	auto pointsPerHook = static_cast<int>(m_NumMassesX / static_cast<float>(m_NumHooks));
	pointsPerHook = std::max(pointsPerHook, 1); // Avoids division by 0.

	for (size_t x = 0; x < m_NumMassesX && x < m_PointMasses.size(); ++x) {
		if (x % m_NumHooks == 0 || x == m_NumMassesX - 1) {
			CVector3 position(m_RestingDistance * x - midWidth, m_HungFromHeight, 0.0f);
			auto pPoint = GetPointMass(x, 0);
			pPoint->Pin(position);
			m_PinnedPoints.push_back(pPoint);
		}
	}
}

void CCloth::UnPin()
{
	for (auto& pinnedPoint : m_PinnedPoints) {
		pinnedPoint->DetachPin();
	}

	m_PinnedPoints.clear();
}

void CCloth::BurnCloth(size_t x, size_t y)
{
	m_IsBurning = true;
	m_PointMasses[x + y * m_NumMassesX]->SetIsBurning(true);
	//m_BurningPoints.emplace_back(x, y);
	//m_BurningPoints.push_back(m_PointMasses[x + y * m_NumMassesX].get());
}

CPointMass* CCloth::GetPointMass(size_t x, size_t y)
{
	return m_PointMasses[x + y * m_NumMassesX].get();
}

const std::vector<CPointMass*>& CCloth::GetPinnedPoints()
{
	return m_PinnedPoints;
}

void CCloth::SetNumPointMassesX(size_t numPointMassesX)
{
	m_NumMassesX = numPointMassesX;
}

const size_t CCloth::GetNumPointMassesX() const
{
	return m_NumMassesX;
}

void CCloth::SetNumPointMassesY(size_t numPointMassesY)
{
	m_NumMassesY = numPointMassesY;
}

const size_t CCloth::GetNumPointMassesY() const
{
	return m_NumMassesY;
}

void CCloth::SetNumHooks(size_t numHooks)
{
	m_NumHooks = numHooks;
}

const size_t CCloth::GetNumHooks() const
{
	return m_NumHooks;
}

void CCloth::SetLinkBreakingDistance(float linkBreakingDistance)
{
	m_LinkBreakingDistance = linkBreakingDistance;
	for (auto& pointMass : m_PointMasses) {
		for (auto& link : pointMass->GetLinks()) {
			link.SetTearDistance(linkBreakingDistance);
		}
	}
}

const float CCloth::GetLinkBreakingDistance() const
{
	return m_LinkBreakingDistance;
}

void CCloth::SetPointMassDamping(float pointMassDamping)
{
	m_PointMassDamping = pointMassDamping;
}

const float CCloth::GetPointMassDamping() const
{
	return m_PointMassDamping;
}

void CCloth::SetPointMassMass(float pointMassMass)
{
	m_PointMassMass = pointMassMass;
}

const float CCloth::GetPointMassMass() const
{
	return m_PointMassMass;
}

void CCloth::SetHungFromHeight(float hungFromHeight)
{
	m_HungFromHeight = hungFromHeight;
}

const float CCloth::GetHungFromHeight() const
{
	return m_HungFromHeight;
}

void CCloth::SetRestingDistance(float restingDistance)
{
	m_RestingDistance = restingDistance;
}

const float CCloth::GetRestingDistance() const
{
	return m_RestingDistance;
}

void CCloth::SetStiffness(float stiffness)
{
	m_Stiffness = stiffness;
}

const float CCloth::GetStiffness() const
{
	return m_Stiffness;
}

const std::vector<std::unique_ptr<CPointMass> >& CCloth::GetPointMasses() const
{
	return m_PointMasses;
}

/*void CCloth::UpdateBurning(float deltaTime)
{
	std::vector<CPoint> newBurningPoints;

	for (auto& burningPoint : m_BurningPoints) {
		auto* pPoint = GetPointMass(burningPoint.GetX(), burningPoint.GetY());
		assert(pPoint);

		pPoint->AddToBurntLevel(m_BurningRate * deltaTime);
		if (pPoint->GetBurntLevel() - std::floor(pPoint->GetBurntLevel()) > m_PropagateBurningThreshold) {
			CPoint neighbourPoint;
			neighbourPoint.SetX(Math::Random(-2, 3) + burningPoint.GetX());
			neighbourPoint.SetY(Math::Random(-2, 3) + burningPoint.GetY());

			if (neighbourPoint.GetX() < m_NumMassesX && neighbourPoint.GetX() >= 0 &&
				neighbourPoint.GetY() < m_NumMassesY && neighbourPoint.GetY() >= 0) {
				// Check to see if the point is not already burning.
				auto findIter = std::find(m_BurningPoints.begin(), m_BurningPoints.end(), neighbourPoint);
				if (findIter == m_BurningPoints.end()) {
					newBurningPoints.push_back(neighbourPoint);
				}
			}
		}

		pPoint->ApplyForce(CVector3::s_UP * 20.0f);
	}

	std::copy(newBurningPoints.begin(), newBurningPoints.end(), 
		std::back_inserter(m_BurningPoints));
}*/

void CCloth::UpdateBurning(float deltaTime)
{
	std::vector<CPointMass*> newBurningPoints;

	for (auto& pBurningPoint : m_BurningPoints) {
		assert(pBurningPoint);

		pBurningPoint->AddToBurntLevel(m_BurningRate * deltaTime);

		/*// Check to see if it's time to propagate the burning.
		if (pBurningPoint->GetBurntLevel() - std::floor(pBurningPoint->GetBurntLevel()) > m_PropagateBurningThreshold) {
			for (auto& pPoint : m_PointMasses) {
				// Get a semi-random distance to compare with the point to burn.
				auto randDistance = Math::Random(m_MinBurningDistance, m_MaxBurningDistance);
				auto toBurningPoint = pBurningPoint->GetPosition() - pPoint->GetPosition();

				if (toBurningPoint.GetLengthSquared() < randDistance * randDistance) {
					// Burn the new point if we are not already.
					auto findIter = std::find_if(m_BurningPoints.begin(), m_BurningPoints.end(), 
						[&pPoint](const CPointMass* pMass) { return pMass == pPoint.get(); });

					if (findIter == m_BurningPoints.end()) {
						//pPoint->DetachPin();
						newBurningPoints.push_back(pPoint.get());
						break;
					}
				}
			}
		}*/

		// Looks cool, like paper burning allows burning points to rise up.
		//pBurningPoint->ApplyForce(CVector3::s_UP * 10.0f);
	}

	//std::copy(newBurningPoints.begin(), newBurningPoints.end(),
	//	std::back_inserter(m_BurningPoints));
}
