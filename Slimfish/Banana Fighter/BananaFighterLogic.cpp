// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BananaFighterLogic.cpp
// Description	: CBananaFighterLogic implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "BananaFighterStd.h"

// Library Includes

// This Include
#include "BananaFighterLogic.h"

// Local Includes
#include <Graphics/SlimRenderer.h>
#include <Graphics/SlimGpuBuffer.h>
#include <Math/SlimAxisAlignedBoundingBox.h>
#include "Sprite.h"


CBananaFighterLogic::CBananaFighterLogic()
	:m_Tiles(30, 30),
	m_Renderer(128, 64),
	m_Camera(nullptr)
{

}

CBananaFighterLogic::~CBananaFighterLogic()
{

}

bool CBananaFighterLogic::Initialise()
{
	auto pSprite = std::make_shared<CSprite>(g_pApp->GetRenderer()->VLoadTexture("Assets/TileBlock.png"));
	pSprite->AddMask(CRect<>(0, 0, pSprite->GetTexture()->GetWidth(), pSprite->GetTexture()->GetHeight()));

	for (auto y = 0U; y < m_Tiles.GetHeight(); ++y) {
		for (auto x = 0U; x < m_Tiles.GetWidth(); ++x) {
			m_Tiles[y][x].SetSprite(pSprite);
		}
	}

	auto windowSize = g_pApp->GetRenderer()->GetWindowSize();
	m_Camera.SetPosition({ 
		-windowSize.GetX() / 2.0f,
		-windowSize.GetY() / 2.0f,
		0.0f 
	});

	if (!m_Renderer.Initialise()) {
		return false;
	}

	for (auto y = 0U; y < m_Tiles.GetHeight(); ++y) {
		for (auto x = 0U; x < m_Tiles.GetWidth(); ++x) {
			m_Renderer.Queue(&m_Tiles[y][x], EIsoLayer::TERRAIN);
		}
	}

	// Create the player.
	pSprite = std::make_shared<CSprite>(g_pApp->GetRenderer()->VLoadTexture("Assets/Player.png"));
	pSprite->AddMask(CRect<>(0, 0, pSprite->GetTexture()->GetWidth(), pSprite->GetTexture()->GetHeight()));

	m_Player.SetSprite(pSprite);

	m_Renderer.Queue(&m_Player, EIsoLayer::OBJECTS);

	return true;
}

void CBananaFighterLogic::Update(float deltaTime)
{

}

void CBananaFighterLogic::Render()
{
	m_Renderer.Render(m_Camera);
}

void CBananaFighterLogic::HandleInput(const CInput& input, float deltaTime)
{
	float cameraSpeed = 1.0f * deltaTime;

	if (input.IsKeyDown(EKeyCode::W)) {
		m_Player.Translate(CVector3(-cameraSpeed, -cameraSpeed, 0.0f));
	}

	if (input.IsKeyDown(EKeyCode::S)) {
		m_Player.Translate(CVector3(cameraSpeed, cameraSpeed, 0.0f));
	}

	if (input.IsKeyDown(EKeyCode::A)) {
		m_Player.Translate(CVector3(-cameraSpeed, cameraSpeed, 0.0f));
	}

	if (input.IsKeyDown(EKeyCode::D)) {
		m_Player.Translate(CVector3(cameraSpeed, -cameraSpeed, 0.0f));
	}

	auto windowSize = g_pApp->GetRenderer()->GetWindowSize();

	m_Camera.SetPosition(m_Renderer.ToScreenCoordinate(m_Player.GetPosition()));
	m_Camera.Translate(CVector3(-windowSize.GetX() / 2.0f, -windowSize.GetY() / 2.0f, 0.0f));
}