// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRenderPass.cpp
// Description	: CRenderPass implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimRenderPass.h"

// Local Includes

namespace Slim {

	CRenderPass::CRenderPass()
		:m_CullMode(ECullingMode::CLOCKWISE),
		m_FillMode(EFillMode::SOLID)
	{

	}

	CRenderPass::~CRenderPass()
	{

	}

	CTextureLayer& CRenderPass::AddTextureLayer(const std::string& textureName)
	{
		CTextureLayer textureLayer;
		shared_ptr<ATexture> pTexture = g_pApp->GetRenderer()->VLoadTexture(textureName);
		if (pTexture) {
			textureLayer.SetTexture(pTexture);
			m_TextureLayers.push_back(textureLayer);
		}
		// TODO: add error handling here.

		return m_TextureLayers.back();
	}

	void CRenderPass::AddTextureLayer(const CTextureLayer& textureLayer)
	{
		m_TextureLayers.push_back(textureLayer);
	}

	CTextureLayer& CRenderPass::GetTextureLayer(size_t layerIndex)
	{
		return m_TextureLayers[layerIndex];
	}

	void CRenderPass::RemoveTextureLayer(size_t layerIndex)
	{
		m_TextureLayers.erase(m_TextureLayers.begin() + layerIndex);
	}

	void CRenderPass::ClearTextureLayers()
	{
		m_TextureLayers.clear();
	}

	size_t CRenderPass::GetNumTextureLayers() const
	{
		return m_TextureLayers.size();
	}

	void CRenderPass::SetVertexShader(shared_ptr<AShaderProgram> shader)
	{
		m_pVertexShader = shader;
	}

	const shared_ptr<AShaderProgram> CRenderPass::GetVertexShader() const
	{
		return m_pVertexShader;
	}

	void CRenderPass::SetPixelShader(shared_ptr<AShaderProgram> shader)
	{
		m_pPixelShader = shader;
	}

	const shared_ptr<AShaderProgram> CRenderPass::GetPixelShader() const
	{
		return m_pPixelShader;
	}

	void CRenderPass::SetGeometryShader(shared_ptr<AShaderProgram> shader)
	{
		m_pGeometryShader = shader;
	}

	const shared_ptr<AShaderProgram> CRenderPass::GetGeometryShader() const
	{
		return m_pGeometryShader;
	}

	void CRenderPass::SetCullMode(ECullingMode cullingMode)
	{
		m_CullMode = cullingMode;
	}

	const ECullingMode CRenderPass::GetCullMode() const
	{
		return m_CullMode;
	}

	void CRenderPass::SetFillMode(EFillMode fillMode)
	{
		m_FillMode = fillMode;
	}

	const EFillMode CRenderPass::GetFillMode() const
	{
		return m_FillMode;
	}

}

