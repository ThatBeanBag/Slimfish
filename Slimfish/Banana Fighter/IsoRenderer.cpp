// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: 2DDrawer.cpp
// Description	: C2DDrawer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "BananaFighterStd.h"

// Library Includes

// This Include
#include "IsoRenderer.h"

// Local Includes
#include "IsoBase.h"
#include "Sprite.h"

CIsoRenderer::CIsoRenderer(size_t tileWidth, size_t tileHeight)
	:m_TileWidth(tileWidth),
	m_TileHeight(tileHeight)
{
	ComputeTileHeightOverTwoWidth();
}

CIsoRenderer::~CIsoRenderer()
{

}

bool CIsoRenderer::Initialise()
{
	return this->C2DRenderer::Initialise();
}

void CIsoRenderer::Queue(const CIsoBase* pIsoRenderable, EIsoLayer layer)
{
	m_RenderQueueList[static_cast<int>(layer)].push_back(pIsoRenderable);
}

void CIsoRenderer::Render(const CCamera& camera)
{
	static auto further = [](const CIsoBase* pLhs, const CIsoBase* pRhs) {
		return (pLhs->GetPosition().GetX() + pLhs->GetPosition().GetY()) <
			(pRhs->GetPosition().GetX() + pRhs->GetPosition().GetY());
	};

	// Sort rendering queues.
	for (int i = 0; i < static_cast<int>(EIsoLayer::MAX); ++i) {
		if (g_LAYER_REQUIRES_SORTING[i]) {
			std::sort(m_RenderQueueList[i].begin(), m_RenderQueueList[i].end(), further);
		}
	}

	// Prepare for rendering.
	this->C2DRenderer::Prepare();

	auto i = 0;

	for (const auto& renderQueue : m_RenderQueueList) {
		for (const auto& renderable : renderQueue) {
			const auto& position = renderable->GetPosition();

			auto screenPosition = this->ToScreenCoordinate(position);
			screenPosition -= camera.GetPosition();

			auto mask = renderable->GetSprite()->GetMask(0);

			CRect<> destRect = { 
				static_cast<int>(screenPosition.GetX() - (mask.GetWidth() / 2.0f)),
				static_cast<int>(screenPosition.GetY() - (mask.GetHeight() / 2.0f)),
				mask.GetWidth(), 
				mask.GetHeight() 
			};

			if (i == static_cast<int>(EIsoLayer::OBJECTS)) {
				destRect.SetY(screenPosition.GetY() - (destRect.GetHeight() - destRect.GetWidth() * m_TileHeightOverTwoWidth));
			}

			C2DRenderer::Render(mask, destRect, renderable->GetSprite()->GetTexture());
		}

		++i;
	}

	//this->FlushQueues();
}

void CIsoRenderer::FlushQueues()
{
	for (auto& renderQueue : m_RenderQueueList) {
		renderQueue.clear();
	}
}

CVector3 CIsoRenderer::ToScreenCoordinate(const CVector3& isoPosition)
{
	return CVector3(
		(isoPosition.GetX() - isoPosition.GetY()) * m_TileWidth / 2.0f,
		(isoPosition.GetX() + isoPosition.GetY()) * m_TileHeight / 2.0f,
		isoPosition.GetZ());
}

void CIsoRenderer::ComputeTileHeightOverTwoWidth()
{
	m_TileHeightOverTwoWidth = static_cast<int>(m_TileHeight / (2.0f * m_TileWidth));
}
