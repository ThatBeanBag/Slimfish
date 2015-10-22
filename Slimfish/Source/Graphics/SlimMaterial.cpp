// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimMaterial.cpp
// Description	: CSlimMaterial implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimMaterial.h"

// Local Includes
#include "SlimTechnique.h"

namespace Slim {

	CMaterial::CMaterial()
	{

	}

	CMaterial::~CMaterial()
	{

	}

	CTechnique* CMaterial::CreateTechnique()
	{
		return AddTechnique(CTechnique());
	}

	CTechnique* CMaterial::AddTechnique(const CTechnique& technique)
	{
		m_Techniques.push_back(std::make_unique<CTechnique>(technique));
		return m_Techniques.back().get();
	}

	CTechnique* CMaterial::GetBestTechnique()
	{
		return m_Techniques[m_CurrentLOD].get();
	}

	CTechnique* CMaterial::GetTechnique(size_t lod)
	{
		return m_Techniques[lod].get();
	}

	void CMaterial::RemoveTechnique(size_t lod)
	{
		m_Techniques.erase(m_Techniques.begin() + lod);
	}

	void CMaterial::ClearTechniques()
	{
		m_Techniques.clear();
	}

	const size_t CMaterial::SetLevelOfDetail() const
	{
		return m_CurrentLOD;
	}

	void CMaterial::SetLevelOfDetail(size_t lod)
	{
		m_CurrentLOD = lod;
	}

	size_t CMaterial::GetNumTechniques() const
	{
		return m_Techniques.size();
	}

}



