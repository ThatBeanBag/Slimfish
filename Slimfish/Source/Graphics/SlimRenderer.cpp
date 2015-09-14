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

namespace Slim {

ARenderer::ARenderer(int width, int height, bool isWindowed)
	:m_Width(width),
	m_Height(height),
	m_IsWindowed(isWindowed)
{

}

void ARenderer::SetWindowed(bool windowed)
{
	m_IsWindowed = true; 
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

void ARenderer::GetWindowSize(size_t& width, size_t& height)
{
	width = m_Width;
	height = m_Height;
}

void ARenderer::Resize(size_t width, size_t height)
{
	m_Width = width; 
	height = m_Height;
	VOnResize();
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

/*
void ARenderer::SetTextureLayerFiltering(size_t layer, ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter)
{
	SetTextureLayerFiltering(layer, TSAMP_MIN, minFilter);
	SetTextureLayerFiltering(layer, TSAMP_MAG, magFilter);
	SetTextureLayerFiltering(layer, TSAMP_MIP, mipFilter);
}

void ARenderer::SetTextureLayerFiltering(size_t layer, ETextureSamplerType samplerType, ETextureFilterType filterType)
{
	switch (samplerType) {
		case TSAMP_MIN: {
			m_MinFilter = filterType;
			break;
		}
		case TSAMP_MAG: {
			m_MagFilter = filterType;
			break;
		}
		case TSAMP_MIP: {
			m_MipFilter = filterType;
			break;
		}
		default: {
			break;
		}
	}
}
*/

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



