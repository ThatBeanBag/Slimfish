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

	void ARenderer::SetTextureLayer(size_t layer, CTextureLayer& textureLayer)
{
	// Set the texture to the layer.
	VSetTexture(layer, textureLayer.GetTexture(), false);

	// Set texture filtering.
	SetTextureLayerFiltering(layer, 
							 textureLayer.GetTextureFilter(TSAMP_MIN),
							 textureLayer.GetTextureFilter(TSAMP_MAG),
							 textureLayer.GetTextureFilter(TSAMP_MIP));

	// Set the blending options.
	VSetTextureLayerBlendState(layer, textureLayer.GetBlendState());

	const TTextureUVWAddressModes& addressModes = textureLayer.GetTextureAddressModes();
	if (addressModes.m_u == TADDRESS_BORDER || addressModes.m_v == TADDRESS_BORDER || addressModes.m_w == TADDRESS_BORDER) {
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

void ARenderer::SetTextureLayerFiltering(size_t layer, ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter)
{
	VSetTextureLayerFiltering(layer, TSAMP_MIN, minFilter);
	VSetTextureLayerFiltering(layer, TSAMP_MAG, magFilter);
	VSetTextureLayerFiltering(layer, TSAMP_MIP, mipFilter);
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



