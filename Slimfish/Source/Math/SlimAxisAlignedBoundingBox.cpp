// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimAxisAlignedBoundingBox.cpp
// Description	: CSlimAxisAlignedBoundingBox implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimAxisAlignedBoundingBox.h"

// Local Includes

namespace Slim {

	CAxisAlignedBoundingBox::CAxisAlignedBoundingBox()
	{

	}

	CAxisAlignedBoundingBox::CAxisAlignedBoundingBox(const CVector3& min, const CVector3& max)
		:m_Min(min),
		m_Max(max)
	{
	}

	CAxisAlignedBoundingBox::~CAxisAlignedBoundingBox()
	{

	}

	void CAxisAlignedBoundingBox::Encompass(const CAxisAlignedBoundingBox& other)
	{
		m_Min.SetX(std::min(m_Min.GetX(), other.m_Min.GetX()));
		m_Min.SetY(std::min(m_Min.GetY(), other.m_Min.GetY()));
		m_Min.SetZ(std::min(m_Min.GetZ(), other.m_Min.GetZ()));

		m_Max.SetX(std::max(m_Max.GetX(), other.m_Max.GetX()));
		m_Max.SetY(std::max(m_Max.GetY(), other.m_Max.GetY()));
		m_Max.SetZ(std::max(m_Max.GetZ(), other.m_Max.GetZ()));
	}

	void CAxisAlignedBoundingBox::Encompass(const CVector3& point)
	{
		m_Min.SetX(std::min(m_Min.GetX(), point.GetX()));
		m_Min.SetY(std::min(m_Min.GetY(), point.GetY()));
		m_Min.SetZ(std::min(m_Min.GetZ(), point.GetZ()));

		m_Max.SetX(std::max(m_Max.GetX(), point.GetX()));
		m_Max.SetY(std::max(m_Max.GetY(), point.GetY()));
		m_Max.SetZ(std::max(m_Max.GetZ(), point.GetZ()));
	}

	void CAxisAlignedBoundingBox::Transform(const CMatrix4x4& transform)
	{
		auto newMin = transform.GetPosition();
		auto newMax = transform.GetPosition();

		auto fA = 0.0f;
		auto fB = 0.0f;

		for (auto i = 0; i < 3; ++i) {
			fA = transform[i][0] * m_Min.GetX();
			fB = transform[i][0] * m_Max.GetX();

			if (fA < fB) {
				newMin.SetX(newMin.GetX() + fA);
				newMax.SetX(newMax.GetX() + fB);
			}
			else {
				newMin.SetX(newMin.GetX() + fB);
				newMax.SetX(newMax.GetX() + fA);
			}

			fA = transform[i][1] * m_Min.GetY();
			fB = transform[i][1] * m_Max.GetY();

			if (fA < fB) {
				newMin.SetY(newMin.GetY() + fA);
				newMax.SetY(newMax.GetY() + fB);
			}
			else {
				newMin.SetY(newMin.GetY() + fB);
				newMax.SetY(newMax.GetY() + fA);
			}

			fA = transform[i][2] * m_Min.GetZ();
			fB = transform[i][2] * m_Max.GetZ();

			if (fA < fB) {
				newMin.SetZ(newMin.GetZ() + fA);
				newMax.SetZ(newMax.GetZ() + fB);
			}
			else {
				newMin.SetZ(newMin.GetZ() + fB);
				newMax.SetZ(newMax.GetZ() + fA);
			}
		}
	}

	const CVector3 CAxisAlignedBoundingBox::GetCentre() const
	{
		return (m_Min + m_Max) * 0.5f;
	}

	const CVector3 CAxisAlignedBoundingBox::GetExtent() const
	{
		return (m_Max - m_Min) * 0.5f;
	}

	void CAxisAlignedBoundingBox::SetMin(const CVector3& min)
	{
		m_Min = min;
	}

	const CVector3& CAxisAlignedBoundingBox::GetMin() const
	{
		return m_Min;
	}

	void CAxisAlignedBoundingBox::SetMax(const CVector3& max)
	{
		m_Max = max;
	}

	const CVector3& CAxisAlignedBoundingBox::GetMax() const
	{
		return m_Max;
	}

}

