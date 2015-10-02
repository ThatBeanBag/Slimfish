// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRenderer.cpp
// Description	: ARenderer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimRenderer.h"

// Local Includes
#include "SlimRenderPass.h"

namespace Slim {

ARenderer::ARenderer(int width, int height, bool isWindowed)
	:m_Width(width),
	m_Height(height),
	m_IsWindowed(isWindowed)
{

}

void ARenderer::SetWindowed(bool windowed)
{
	m_IsWindowed = windowed; 
	VSetWindowed(windowed);
}

void ARenderer::ToggleWindowed()
{
	SetWindowed(!m_IsWindowed);
}

bool ARenderer::IsWindowed() const
{
	return m_IsWindowed;
}

CPoint ARenderer::GetWindowSize()
{
	CPoint windowSize(m_Width, m_Height);
	return windowSize;
}

void ARenderer::Resize(size_t width, size_t height)
{
	m_Width = width; 
	height = m_Height;
	VOnResize();
}

void ARenderer::SetRenderPass(CRenderPass* pPass)
{
	assert(pPass);

	// Set the texture layers from the pass.
	for (unsigned int i = 0; i < pPass->GetNumTextureLayers(); ++i) {
		SetTextureLayer(i, *pPass->GetTextureLayer(i));
	}

	// Disable the unused texture layers.
	DisableTextureLayer(pPass->GetNumTextureLayers());

	// Set rasterizer settings.
	VSetCullingMode(pPass->GetCullingMode());
	VSetFillMode(pPass->GetFillMode());

	// Set blending settings
	VSetBlendingMode(pPass->GetBlendingMode());
	VSetColourWritesEnabled(pPass->GetColourWritesEnabled());

	// Set depth-stencil settings.
	VSetDepthBufferSettings(
		pPass->GetDepthCheckEnabled(), 
		pPass->GetDepthWriteEnabled(), 
		pPass->GetDepthCompareFunction());
	VSetStencilBufferSettings(pPass->GetStencilBufferSettings());

	// Set the shader programs.
	if (pPass->HasVertexShader()) {
		VSetShaderProgram(pPass->GetVertexShader());
	}
	else {
		VDisableShaderProgram(EShaderProgramType::VERTEX);
	}

	if (pPass->HasPixelShader()) {
		VSetShaderProgram(pPass->GetPixelShader());
	}
	else {
		VDisableShaderProgram(EShaderProgramType::PIXEL);
	}

	if (pPass->HasGeometryShader()) {
		VSetShaderProgram(pPass->GetGeometryShader());
	}
	else {
		VDisableShaderProgram(EShaderProgramType::GEOMETRY);
	}
}

shared_ptr<AIndexGpuBuffer> ARenderer::CreateIndexBuffer(const std::vector<int>& indices, EGpuBufferUsage usage /*= EGpuBufferUsage::STATIC*/, bool isInSystemMemory /*= false*/)
{
	return VCreateIndexBuffer(indices.size(), AIndexGpuBuffer::INDEX_TYPE_32, reinterpret_cast<const void*>(&indices[0]), usage, isInSystemMemory);
}

shared_ptr<AIndexGpuBuffer> ARenderer::CreateIndexBuffer(const std::vector<short>& indices, EGpuBufferUsage usage /*= EGpuBufferUsage::STATIC*/, bool isInSystemMemory /*= false*/)
{
	return VCreateIndexBuffer(indices.size(), AIndexGpuBuffer::INDEX_TYPE_16, reinterpret_cast<const void*>(&indices[0]), usage, isInSystemMemory);
}

void ARenderer::SetTextureLayer(size_t layer, CTextureLayer& textureLayer)
{
	// Set the texture to the layer.
	VSetTexture(layer, textureLayer.GetTexture(), false);

	// Set texture filtering.
	VSetTextureLayerFiltering(
		layer, 
		textureLayer.GetTextureFilter(ETextureSamplerType::MIN),
		textureLayer.GetTextureFilter(ETextureSamplerType::MAG),
		textureLayer.GetTextureFilter(ETextureSamplerType::MIP));

	// Set the blending options.
	VSetTextureLayerBlendState(layer, textureLayer.GetBlendState());

	const TTextureUVWAddressModes& addressModes = textureLayer.GetTextureAddressModes();
	if (addressModes.m_u == ETextureAddressMode::BORDER || addressModes.m_v == ETextureAddressMode::BORDER || addressModes.m_w == ETextureAddressMode::BORDER) {
	// Does one of the addressing modes use TADDRESS_BORDER?
		// Set the border colour.
		VSetTextureBorderColour(layer, textureLayer.GetTextureBorderColour());
	}

	VSetTextureAddressModes(layer, addressModes);

	m_disableTextureLayerFrom = layer + 1;
}

void ARenderer::DisableTextureLayer(size_t layer)
{
	VSetTexture(layer, nullptr, true);
}

void ARenderer::DisableTextureLayerFrom(size_t layer)
{
	size_t disableMax = g_MAX_TEXTURE_LAYERS;
	if (disableMax > m_disableTextureLayerFrom) {
		disableMax = m_disableTextureLayerFrom;
	}

	m_disableTextureLayerFrom = layer;

	for (size_t i = layer; i < disableMax; ++i) {
		DisableTextureLayerFrom(layer);
	}
}

}



