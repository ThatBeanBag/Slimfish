// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: 2DRenderer.cpp
// Description	: C2DRenderer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "BananaFighterStd.h"

// Library Includes

// This Include
#include "2DRenderer.h"

// Local Includes
#include <Graphics\SlimRenderer.h>

C2DRenderer::C2DRenderer()
	:m_pTextureLayer(nullptr)
{
	// Create the vertex declaration.
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);
	m_VertexDeclaration.AddElement("COLOUR", CInputElement::FORMAT_COLOUR_RGBA);
}

C2DRenderer::~C2DRenderer()
{

}

bool C2DRenderer::Initialise()
{
	auto pRenderer = g_pApp->GetRenderer();

	auto pVertexShader = pRenderer->VCreateShaderProgram("QuadTex_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0");
	if (!pVertexShader) {
		return false;
	}

	auto pPixelShader = pRenderer->VCreateShaderProgram("QuadTex_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0");
	if (!pPixelShader) {
		return false;
	}

	// Create shaders.
	m_RenderPass.SetVertexShader(pVertexShader);
	m_RenderPass.SetPixelShader(pPixelShader);

	// Create the single texture layer that is going to be used to render 2D images.
	m_pTextureLayer = m_RenderPass.AddTextureLayer();
	if (!m_pTextureLayer) {
		return false;
	}

	// Disable depth checks.
	m_RenderPass.SetDepthCheckEnabled(false);
	m_RenderPass.SetCullingMode(ECullingMode::NONE);
	// Add alpha blending.
	m_RenderPass.SetSourceColourBlendFactor(EBlendFactor::SOURCE_ALPHA);
	m_RenderPass.SetDestColourBlendFactor(EBlendFactor::INVERSE_SOURCE_ALPHA);
	m_RenderPass.SetColourBlendOperation(EBlendOperation::ADD);

	// Create the vertex buffer for drawing quads.
	m_pQuadBuffer = g_pApp->GetRenderer()->VCreateVertexBuffer(4, sizeof(TQuadVertex), nullptr, EGpuBufferUsage::WRITE_ONLY);
	if (!m_pQuadBuffer) {
		return false;
	}

	return true;
}

void C2DRenderer::Prepare()
{
	m_pTextureLayer->SetTexture(g_pApp->GetRenderer()->VLoadTexture("Assets/TileBlock.png"));
	g_pApp->GetRenderer()->SetRenderPass(&m_RenderPass);
}

void C2DRenderer::Render(const CRect<>& srcRect, const CRect<>& destRect,
	std::shared_ptr<ATexture> pImage, const CColourValue& colour)
{
	auto pRenderer = g_pApp->GetRenderer();
	auto screenRect = CRect<>(CPoint<>(), pRenderer->GetWindowSize());

	if (!destRect.IsIntersecting(screenRect)) {
		// Outside of view.
		return;
	}

	assert(m_pTextureLayer);

	{	// Lock the buffer for writing to.
		CGpuBufferLock lock(m_pQuadBuffer, 0, m_pQuadBuffer->GetSize(), EGpuBufferLockType::DISCARD);
		TQuadVertex* pVertices = reinterpret_cast<TQuadVertex*>(lock.GetLockedContents());

		float invScreenWidth = 1.0f / static_cast<float>(screenRect.GetWidth());
		float invScreenHeight = 1.0f / static_cast<float>(screenRect.GetHeight());

		// Transform destination rectangle to normalised device coordinate space [-1, 1].
		float left = ((static_cast<float>(destRect.GetX()) * invScreenWidth * 2.0f) - 1);
		float right = left + (static_cast<float>(destRect.GetWidth()) * invScreenWidth * 2.0f);
		float top = -((static_cast<float>(destRect.GetY()) * invScreenHeight * 2.0f) - 1);
		float bottom = top - (static_cast<float>(destRect.GetHeight()) * invScreenHeight * 2.0f);

		auto invImageWidth =  1.0f / static_cast<float>(pImage->GetWidth());
		auto invImageHeight = 1.0f / static_cast<float>(pImage->GetHeight());

		// Top left.
		pVertices[0].position = { left, top, 0.0f };
		pVertices[0].uv = {
			srcRect.GetLeft() * invImageWidth,
			srcRect.GetTop() * invImageHeight };
		pVertices[0].colour = colour;

		// Top right.
		pVertices[1].position = { right, top, 0.0f };
		pVertices[1].uv = {
			srcRect.GetRight() * invImageWidth,
			srcRect.GetTop() * invImageHeight };
		pVertices[1].colour = colour;

		// Bottom left.
		pVertices[2].position = { left, bottom, 0.0f };
		pVertices[2].uv = {
			srcRect.GetLeft() * invImageWidth,
			srcRect.GetBottom() * invImageHeight };
		pVertices[2].colour = colour;

		// Bottom right.
		pVertices[3].position = { right, bottom, 0.0f };
		pVertices[3].uv = {
			srcRect.GetRight() * invImageWidth,
			srcRect.GetBottom() * invImageHeight };
		pVertices[3].colour = colour;

	} // Unlocks the buffer.
	//m_pTextureLayer->SetTexture(pImage);

	// Render the image.
	pRenderer->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pQuadBuffer);
}
