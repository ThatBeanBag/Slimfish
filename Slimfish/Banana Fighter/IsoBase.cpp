// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: IsoBase.cpp
// Description	: CIsoBase implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "BananaFighterStd.h"

// Library Includes

// This Include
#include "IsoBase.h"

// Local Includes
#include "Sprite.h"

CIsoBase::CIsoBase()
{

}

CIsoBase::CIsoBase(std::shared_ptr<CSprite>& pSprite)
	:m_pSprite(pSprite)
{

}

CIsoBase::~CIsoBase()
{

}

void CIsoBase::Translate(const CVector3& translation)
{
	m_Position += translation;
}

void CIsoBase::SetPosition(const CVector3& position)
{
	m_Position = position;
}

const CVector3& CIsoBase::GetPosition() const
{
	return m_Position;
}

void CIsoBase::SetSprite(std::shared_ptr<CSprite> sprite)
{
	m_pSprite = sprite;
}

const std::shared_ptr<CSprite> CIsoBase::GetSprite() const
{
	return m_pSprite;
}
