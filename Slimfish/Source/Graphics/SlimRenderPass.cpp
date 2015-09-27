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
		:m_CullingMode(ECullingMode::CLOCKWISE),
		m_FillMode(EFillMode::SOLID),
		m_DepthCheckEnabled(true),
		m_DepthWriteEnabled(true),
		m_DepthCompareFunction(EComparisonFunction::LESS)
	{

	}

	CRenderPass::~CRenderPass()
	{

	}

	CTextureLayer& CRenderPass::AddTextureLayer(const std::string& textureName)
	{
		//CTextureLayer textureLayer;
		//shared_ptr<ATexture> pTexture = g_pApp->GetRenderer()->VLoadTexture(textureName);
		//if (pTexture) {
		//	textureLayer.SetTexture(pTexture);
		//	m_TextureLayers.push_back(textureLayer);
		//}
		//// TODO: add error handling here.

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

	const std::vector<CTextureLayer>& CRenderPass::GetTextureLayers() const
	{
		return m_TextureLayers;
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

	const bool CRenderPass::HasVertexShader() const
	{
		return m_pVertexShader != nullptr;
	}

	const bool CRenderPass::HasPixelShader() const
	{
		return m_pPixelShader != nullptr;
	}

	const bool CRenderPass::HasGeometryShader() const
	{
		return m_pGeometryShader != nullptr;
	}

	void CRenderPass::SetBlendingMode(TBlendingMode blendingMode)
	{
		m_BlendingMode = blendingMode;
	}

	const TBlendingMode& CRenderPass::GetBlendingMode() const
	{
		return m_BlendingMode;
	}

	void CRenderPass::SetColourWritesEnabled(bool all)
	{
		SetColourWritesEnabled(all, all, all, all);
	}

	void CRenderPass::SetColourWritesEnabled(bool red, bool blue, bool green, bool alpha)
	{
		m_ColourWrites.red = red;
		m_ColourWrites.blue = blue;
		m_ColourWrites.green = green;
		m_ColourWrites.alpha = alpha;
	}

	const TColourWritesEnabled CRenderPass::GetColourWritesEnabled()
	{
		return m_ColourWrites;
	}

	void CRenderPass::SetDepthCheckEnabled(bool depthCheckEnabled)
	{
		m_DepthCheckEnabled = depthCheckEnabled;
	}

	const bool CRenderPass::GetDepthCheckEnabled() const
	{
		return m_DepthCheckEnabled;
	}

	void CRenderPass::SetDepthWriteEnabled(bool depthWriteEnabled)
	{
		m_DepthWriteEnabled = depthWriteEnabled;
	}

	const bool CRenderPass::GetDepthWriteEnabled() const
	{
		return m_DepthWriteEnabled;
	}

	void CRenderPass::SetDepthCompareFunction(EComparisonFunction compareFunction)
	{
		m_DepthCompareFunction = compareFunction;
	}

	const EComparisonFunction CRenderPass::GetDepthCompareFunction() const
	{
		return m_DepthCompareFunction;
	}

	void CRenderPass::SetStencilBufferSettings(const TStencilBufferSettings& settings)
	{
		m_StencilBufferSettings = settings;
	}

	const TStencilBufferSettings& CRenderPass::GetStencilBufferSettings() const
	{
		return m_StencilBufferSettings;
	}

	void CRenderPass::SetStencilCheckEnabled(bool enabled)
	{
		m_StencilBufferSettings.stencilCheckEnabled = enabled;
	}

	const bool CRenderPass::GetStencilCheckEnabled() const
	{
		return m_StencilBufferSettings.stencilCheckEnabled;
	}

	void CRenderPass::SetStencilCompareFunction(EComparisonFunction compareFunction)
	{
		m_StencilBufferSettings.stencilCompareFunction = compareFunction;
	}

	const EComparisonFunction CRenderPass::GetStencilCompareFunction() const
	{
		return m_StencilBufferSettings.stencilCompareFunction;
	}

	void CRenderPass::SetStencilReferenceValue(size_t refValue)
	{
		m_StencilBufferSettings.stencilReferenceValue = refValue;
	}

	const size_t CRenderPass::GetStencilReferenceValue() const
	{
		return m_StencilBufferSettings.stencilReferenceValue;
	}

	void CRenderPass::SetStencilWriteMask(size_t mask)
	{
		m_StencilBufferSettings.stencilWriteMask = mask;
	}

	const size_t CRenderPass::GetStencilWriteMask() const
	{
		return m_StencilBufferSettings.stencilWriteMask;
	}

	void CRenderPass::SetStencilFailOperation(EStencilOperation operation)
	{
		m_StencilBufferSettings.stencilFailOperation = operation;
	}

	const EStencilOperation CRenderPass::GetStencilFailOperation() const
	{
		return m_StencilBufferSettings.stencilFailOperation;
	}

	void CRenderPass::SetStencilDepthFailOperation(EStencilOperation operation)
	{
		m_StencilBufferSettings.stencilDepthFailOperation = operation;
	}

	const EStencilOperation CRenderPass::GetStencilDepthFailOperation() const
	{
		return m_StencilBufferSettings.stencilDepthFailOperation;
	}

	void CRenderPass::SetStencilPassOperation(EStencilOperation operation)
	{
		m_StencilBufferSettings.stencilPassOperation = operation;
	}

	const EStencilOperation CRenderPass::GetStencilPassOperation() const
	{
		return m_StencilBufferSettings.stencilPassOperation;
	}

	void CRenderPass::SetCullingMode(ECullingMode cullingMode)
	{
		m_CullingMode = cullingMode;
	}

	const ECullingMode CRenderPass::GetCullingMode() const
	{
		return m_CullingMode;
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

