// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimTechnique.cpp
// Description	: CSlimTechnique implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimTechnique.h"

// Local Includes
#include "SlimRenderPass.h"

namespace Slim {

	CTechnique::CTechnique()
	{

	}

	CTechnique::CTechnique(const CTechnique& other)
	{
		Copy(other);
	}

	CTechnique::~CTechnique()
	{

	}

	CTechnique& CTechnique::operator=(const CTechnique& other)
	{
		Copy(other);
		return *this;
	}

	CRenderPass* CTechnique::CreateRenderPass()
	{
		return AddRenderPass(CRenderPass());
	}


	CRenderPass* CTechnique::AddRenderPass(const CRenderPass& pass)
	{
		m_RenderPasses.push_back(std::make_unique<CRenderPass>(pass));
	}

	CRenderPass* CTechnique::GetRenderPass(size_t index)
	{
		return m_RenderPasses[index].get();
	}

	void CTechnique::RemoveRenderPass(size_t index)
	{
		m_RenderPasses.erase(m_RenderPasses.begin() + index);
	}

	void CTechnique::ClearRenderPasses()
	{
		m_RenderPasses.clear();
	}

	const size_t CTechnique::GetNumRenderPasses() const
	{
		return m_RenderPasses.size();
	}

	void CTechnique::Copy(const CTechnique& other)
	{
		m_RenderPasses = TRenderPasses(other.m_RenderPasses.size(), nullptr);

		auto copyPass = [](const std::unique_ptr<CRenderPass>& renderPass) {
			std::unique_ptr<CRenderPass> copy;
			if (renderPass) {
				copy = std::make_unique<CRenderPass>(*renderPass.get());
			}
			else {
				copy = nullptr;
			}

			return copy;
		};

		std::transform(other.m_RenderPasses.begin(), other.m_RenderPasses.end(), 
			m_RenderPasses.begin(), copyPass);
	}

}
