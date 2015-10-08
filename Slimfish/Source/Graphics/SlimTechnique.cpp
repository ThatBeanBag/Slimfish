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

namespace Slim {

void CTechnique::Copy(const CTechnique& other)
{
	m_renderPasses.clear();
	m_renderPasses.reserve(other.m_renderPasses.size());

	auto copyPass = [this](const std::unique_ptr<CRenderPass>& renderPass) {
		std::unique_ptr<CRenderPass> copy;
		if (renderPass) {
			copy = std::make_unique<CRenderPass>(*renderPass.get());
		}
		else {
			copy = nullptr;
		}

		m_renderPasses.push_back(std::move(copy));

		return copy;
	};

	std::for_each(other.m_renderPasses.begin(), other.m_renderPasses.end(), copyPass);
}

}
