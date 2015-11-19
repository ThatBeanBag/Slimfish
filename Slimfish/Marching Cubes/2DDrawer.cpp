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
#include "MarchingCubesStd.h"

// Library Includes

// This Include
#include "2DDrawer.h"

// Local Includes
#include <Graphics\SlimRenderer.h>

C2DDrawer::C2DDrawer()
	:m_pTextureLayer(nullptr)
{
	// Create the vertex declaration.
	m_VertexDeclaration.AddElement("POSITION", CInputElement::FORMAT_FLOAT3);
	m_VertexDeclaration.AddElement("TEXCOORD", CInputElement::FORMAT_FLOAT2);
}

C2DDrawer::~C2DDrawer()
{

}

bool C2DDrawer::Initialise()
{
	auto pRenderer = g_pApp->GetRenderer();

	// Create shaders.
	m_2DRenderPass.SetVertexShader(pRenderer->VCreateShaderProgram("QuadTex_VS.hlsl", EShaderProgramType::VERTEX, "main", "vs_4_0"));
	m_2DRenderPass.SetPixelShader(pRenderer->VCreateShaderProgram("QuadTex_PS.hlsl", EShaderProgramType::PIXEL, "main", "ps_4_0"));

	// Create the single texture layer that is going to be used to render 2D images.
	m_pTextureLayer = m_2DRenderPass.AddTextureLayer();

	// Disable depth checks.
	m_2DRenderPass.SetDepthCheckEnabled(false);
	m_2DRenderPass.SetCullingMode(ECullingMode::NONE);
	m_2DRenderPass.SetSourceColourBlendFactor(EBlendFactor::SOURCE_ALPHA);
	m_2DRenderPass.SetDestColourBlendFactor(EBlendFactor::INVERSE_SOURCE_ALPHA);
	m_2DRenderPass.SetColourBlendOperation(EBlendOperation::ADD);

	// Create the vertex buffer for drawing quads.
	m_pQuadBuffer = g_pApp->GetRenderer()->VCreateVertexBuffer(4, sizeof(TQuadVertex), nullptr, EGpuBufferUsage::WRITE_ONLY);

	return true;
}

void C2DDrawer::Render(const CRect& destRect, std::shared_ptr<ATexture> pImage)
{
	auto pRenderer = g_pApp->GetRenderer();

	assert(m_pTextureLayer);

	{	// Lock the buffer for writing to.
		CGpuBufferLock lock(m_pQuadBuffer, 0, m_pQuadBuffer->GetSize(), EGpuBufferLockType::DISCARD);
		TQuadVertex* pVertices = reinterpret_cast<TQuadVertex*>(lock.GetLockedContents());

		CPoint screenSize = pRenderer->GetWindowSize();
		float invScreenWidth = 1.0f / static_cast<float>(screenSize.GetX());
		float invScreenHeight = 1.0f / static_cast<float>(screenSize.GetY());

		float left = ((static_cast<float>(destRect.GetX()) * invScreenWidth * 2.0f) - 1);
		float right = left + (static_cast<float>(destRect.GetWidth()) * invScreenWidth * 2.0f);
		float top = -((static_cast<float>(destRect.GetY()) * invScreenHeight * 2.0f) - 1);
		float bottom = top - (static_cast<float>(destRect.GetHeight()) * invScreenHeight * 2.0f);

		pVertices[0].position = CVector3(left, top, 0.0f);
		pVertices[0].uv = CVector2(0.0f, 0.0f);

		pVertices[1].position = CVector3(right, top, 0.0f);
		pVertices[1].uv = CVector2(1.0f, 0.0f);

		pVertices[2].position = CVector3(left, bottom, 0.0f);
		pVertices[2].uv = CVector2(0.0f, 1.0f);

		pVertices[3].position = CVector3(right, bottom, 0.0f);
		pVertices[3].uv = CVector2(1.0f, 1.0f);
	} // Unlocks the buffer.
	m_pTextureLayer->SetTexture(pImage);

	// Render the image.
	pRenderer->SetRenderPass(&m_2DRenderPass);
	pRenderer->VRender(m_VertexDeclaration, EPrimitiveType::TRIANGLESTRIP, m_pQuadBuffer);
}
