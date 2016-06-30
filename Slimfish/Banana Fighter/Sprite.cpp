// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Sprite.cpp
// Description	: CSprite implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "BananaFighterStd.h"

// Library Includes

// This Include
#include "Sprite.h"

// Local Includes

CSprite::CSprite()
{

}

CSprite::CSprite(std::shared_ptr<ATexture>& pTexture)
	:m_pTexture(pTexture)
{

}

CSprite::~CSprite()
{

}

void CSprite::AddMask(const CRect<>& mask)
{
	m_Masks.push_back(mask);
}

const CRect<> CSprite::GetMask(size_t index) const
{
	return m_Masks[index];
}

const size_t CSprite::GetNumMasks() const
{
	return m_Masks.size();
}

void CSprite::CreateMasks(int maskWidth, int maskHeight, size_t numAcross, size_t numDown, int startX, int startY)
{
	for (size_t y = 0; y < numDown; ++y) {
		auto maskY = static_cast<int>(y) * maskHeight + startY;
		for (size_t x = 0; x < numAcross; ++x) {
			auto maskX = static_cast<int>(x) * maskWidth + startX;

			// Add the mask to the list.
			m_Masks.emplace_back(maskX, maskY, maskWidth, maskHeight);
		}
	}
}

void CSprite::SetTexture(std::shared_ptr<ATexture> texture)
{
	m_pTexture = texture;
}

const std::shared_ptr<ATexture>& CSprite::GetTexture() const
{
	return m_pTexture;
}

void CSprite::SetMasks(const TMaskList& masks)
{
	m_Masks = masks;
}

const CSprite::TMaskList& CSprite::GetMasks() const
{
	return m_Masks;
}
