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
#include "..\GameBase\SlimGameApp.h"
#include "SlimRenderer.h"

namespace Slim {

	CRenderPass::CRenderPass()
		:m_CullingMode(ECullingMode::CLOCKWISE),
		m_FillMode(EFillMode::SOLID),
		m_DepthCheckEnabled(true),
		m_DepthWriteEnabled(true),
		m_DepthCompareFunction(EComparisonFunction::LESS),
		m_RenderQueueGroupCategory(ERenderQueueGroupCategory::RQ_GROUP_GEOMETRY)
	{

	}

	CRenderPass::~CRenderPass()
	{

	}

	CTextureLayer* CRenderPass::AddTextureLayer(const std::string& textureName)
	{
		CTextureLayer* pTextureLayer = AddTextureLayer();
		shared_ptr<ATexture> pTexture = g_pApp->GetRenderer()->VLoadTexture(textureName);
		if (pTexture && pTextureLayer) {
			pTextureLayer->SetTexture(pTexture);
		}
		else {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to add texture layer with texture " << textureName;
		}

		return m_TextureLayers.back().get();
	}

	CTextureLayer* CRenderPass::AddTextureLayer(const CTextureLayer& textureLayer)
	{
		std::unique_ptr<CTextureLayer> pTextureLayer = std::make_unique<CTextureLayer>(textureLayer);
		m_TextureLayers.push_back(std::move(pTextureLayer));
		return m_TextureLayers.back().get();
	}

	CTextureLayer* CRenderPass::AddTextureLayer()
	{
		return AddTextureLayer(CTextureLayer());
	}

	CTextureLayer* CRenderPass::AddTextureLayer(std::shared_ptr<ATexture> pTexture)
	{
		auto pTextureLayer = AddTextureLayer();
		if (pTextureLayer) {
			pTextureLayer->SetTexture(pTexture);
		}

		return pTextureLayer;
	}

	CTextureLayer* CRenderPass::GetTextureLayer(size_t layerIndex)
	{
		return m_TextureLayers[layerIndex].get();
	}

	const std::vector<std::unique_ptr<CTextureLayer>>& CRenderPass::GetTextureLayers() const
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

	void CRenderPass::SetStreamOutputTargets(const CRenderPass::TSOTargetList& streamOutputTargets)
	{
		m_StreamOutputTargets = streamOutputTargets;
	}

	void CRenderPass::AddStreamOutputTarget(const std::shared_ptr<AGpuBuffer>& pTargetBuffer)
	{
		m_StreamOutputTargets.push_back(pTargetBuffer);
	}

	void CRenderPass::ClearStreamOutputTargets()
	{
		m_StreamOutputTargets.clear();
	}

	const CRenderPass::TSOTargetList& CRenderPass::GetStreamOutputTargets() const
	{
		return m_StreamOutputTargets;
	}

	void CRenderPass::SetBlendingMode(TBlendingMode blendingMode)
	{
		m_BlendingMode = blendingMode;
	}

	const TBlendingMode& CRenderPass::GetBlendingMode() const
	{
		return m_BlendingMode;
	}

	void CRenderPass::SetSourceColourBlendFactor(EBlendFactor sourceColourBlendFactor)
	{
		m_BlendingMode.sourceColourBlendFactor = sourceColourBlendFactor;
	}

	const EBlendFactor CRenderPass::GetSourceColourBlendFactor() const
	{
		return m_BlendingMode.sourceColourBlendFactor;
	}

	void CRenderPass::SetDestColourBlendFactor(EBlendFactor destColourBlendFactor)
	{
		m_BlendingMode.destColourBlendFactor = destColourBlendFactor;
	}

	const EBlendFactor CRenderPass::GetDestColourBlendFactor() const
	{
		return m_BlendingMode.destColourBlendFactor;
	}

	void CRenderPass::SetColourBlendOperation(EBlendOperation colourBlendOperation)
	{
		m_BlendingMode.colourBlendOperation = colourBlendOperation;
	}

	const EBlendOperation CRenderPass::GetColourBlendOperation() const
	{
		return m_BlendingMode.colourBlendOperation;
	}

	void CRenderPass::SetSourceAlphaBlendFactor(EBlendFactor sourceAlphaBlendFactor)
	{
		m_BlendingMode.sourceAlphaBlendFactor = sourceAlphaBlendFactor;
	}

	const EBlendFactor CRenderPass::GetSourceAlphaBlendFactor() const
	{
		return m_BlendingMode.sourceAlphaBlendFactor;
	}

	void CRenderPass::SetDestAlphaBlendFactor(EBlendFactor destAlphaBlendFactor)
	{
		m_BlendingMode.destAlphaBlendFactor = destAlphaBlendFactor;
	}

	const EBlendFactor CRenderPass::GetDestAlphaBlendFactor() const
	{
		return m_BlendingMode.destAlphaBlendFactor;
	}

	void CRenderPass::SetAlphaBlendOperation(EBlendOperation alphaBlendOperation)
	{
		m_BlendingMode.alphaBlendOperation = alphaBlendOperation;
	}

	const EBlendOperation CRenderPass::GetAlphaBlendOperation() const
	{
		return m_BlendingMode.alphaBlendOperation;
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


	void CRenderPass::SetRenderQueueGroupCategory(ERenderQueueGroupCategory renderQueueGroupCategory)
	{
		m_RenderQueueGroupCategory = renderQueueGroupCategory;
	}

	const ERenderQueueGroupCategory CRenderPass::GetRenderQueueGroupCategory() const
	{
		return m_RenderQueueGroupCategory;
	}

}

