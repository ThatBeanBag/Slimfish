// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimTextureLayer.cpp
// Description	: CTextureLayer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimTextureLayer.h"

// Local Includes

namespace Slim {

CTextureLayer::CTextureLayer()
	:m_pTexture(nullptr),
	m_minFilter(TFILTER_LINEAR),
	m_magFilter(TFILTER_LINEAR),
	m_mipFilter(TFILTER_POINT),
	m_textureTransform(CMatrix4x4::s_IDENTITY),
	m_uTranslation(0),
	m_vTranslation(0),
	m_uScale(1),
	m_vScale(1),
	m_radAngle(0),
	m_isTransformDirty(false)
{
	SetColourOperation(TLAYEROP_MODULATE);
	SetAlphaOperation(TLAYEROP_MODULATE);
	SetTextureAddressModes(TADDRESS_WRAP);
}

CTextureLayer::~CTextureLayer()
{

}

void CTextureLayer::SetTexture(const shared_ptr<ATexture>& pTexture)
{
	m_pTexture = pTexture;
}

const shared_ptr<ATexture>& CTextureLayer::GetTexture()
{
	return m_pTexture;
}

void CTextureLayer::SetTextureFilter(ETextureFilterTypeBroad filterType)
{
	switch (filterType) {
		case TFILTERB_NONE: {
			m_minFilter = TFILTER_POINT;
			m_magFilter = TFILTER_POINT;
			m_mipFilter = TFILTER_NONE;
			break;
		}
		case TFILTERB_BILINEAR: {
			m_minFilter = TFILTER_LINEAR;
			m_magFilter = TFILTER_LINEAR;
			m_mipFilter = TFILTER_POINT;
			break;
		}
		case TFILTERB_TRILINEAR: {
			m_minFilter = TFILTER_LINEAR;
			m_magFilter = TFILTER_LINEAR;
			m_mipFilter = TFILTER_LINEAR;
			break;
		}
		case TFILTERB_ANISOTROPIC: {
			m_minFilter = TFILTER_ANISOTROPIC;
			m_magFilter = TFILTER_ANISOTROPIC;
			m_mipFilter = TFILTER_LINEAR;
			break;
		}
	}
}

void CTextureLayer::SetTextureFilter(ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter)
{
	m_minFilter = minFilter;
	m_magFilter = magFilter;
	m_mipFilter = mipFilter;
}

ETextureFilterType CTextureLayer::GetTextureFilter(ETextureSamplerType samplerType)
{
	switch (samplerType) {
		case TSAMP_MIN: {
			return m_minFilter;
		}
		case TSAMP_MAG: {
			return m_magFilter;
		}
		case TSAMP_MIP: {
			return m_mipFilter;
		}
		default: {
			break;
		}
	}

	// Should never get here logically, but VS throws up a warning if this line isn't here.
	return m_minFilter;
}

void CTextureLayer::SetColourOperation(ETextureLayerBlendOperation operation, 
									   ETextureLayerBlendSource source1 /*= TLAYERSRC_TEXTURE*/, 
									   ETextureLayerBlendSource source2 /*= TLAYERSRC_CURRENT*/, 
									   const TColourValue& argument1 /*= TColourValue::s_WHITE*/, 
									   const TColourValue& argument2 /*= TColourValue::s_WHITE*/, 
									   float manualBlend /*= 0.0f*/)
{
	m_blendState.m_colourBlendOperation = operation;
	m_blendState.m_colourSource1 = source1;
	m_blendState.m_colourSource2 = source2;
	m_blendState.m_colourArg1 = argument1;
	m_blendState.m_colourArg2 = argument2;
	m_blendState.m_colourManualBlend = manualBlend;
}

void CTextureLayer::SetAlphaOperation(ETextureLayerBlendOperation operation, 
									  ETextureLayerBlendSource source1 /*= TLAYERSRC_TEXTURE*/,
									  ETextureLayerBlendSource source2 /*= TLAYERSRC_CURRENT*/, 
									  float argument1 /*= TColourValue::s_ALPHA_OPAQUE*/, 
									  float argument2 /*= TColourValue::s_ALPHA_OPAQUE*/, 
									  float manualBlend /*= 0.0f*/)
{
	m_blendState.m_alphaBlendOperation = operation;
	m_blendState.m_alphaSource1 = source1;
	m_blendState.m_alphaSource2 = source2;
	m_blendState.m_alphaArg1 = argument1;
	m_blendState.m_alphaArg2 = argument2;
	m_blendState.m_alphaManualBlend = manualBlend;
}

const TTextureLayerBlendState& CTextureLayer::GetBlendState() const
{
	return m_blendState;
}

void CTextureLayer::SetTextureTranslation(float uTranslation, float vTranslation)
{
	m_uTranslation = uTranslation;
	m_vTranslation = vTranslation;
	m_isTransformDirty = true;
}

void CTextureLayer::SetTextureScale(float uScale, float vScale)
{
	m_uScale = uScale;
	m_vScale = vScale;
	m_isTransformDirty = true;
}

void CTextureLayer::SetTextureRotation(float radAngle)
{
	m_radAngle = radAngle;
	m_isTransformDirty = true;
}

void CTextureLayer::SetTextureTransform(const CMatrix4x4& textureTransform)
{
	m_textureTransform = textureTransform;
	m_isTransformDirty = false;
}

const CMatrix4x4& CTextureLayer::GetTextureTransform()
{
	if (m_isTransformDirty) {
		// Re-calculate the matrix.
		CMatrix4x4 scale(CMatrix4x4::s_IDENTITY);

		if (m_uScale == 0 || m_vScale == 0) {
			scale[0][0] = 0;
			scale[1][1] = 0;
		}
		else {
			scale[0][0] = 1 / m_uScale;
			scale[1][1] = 1 / m_vScale;
		}

		CMatrix4x4 rotation = CMatrix4x4::BuildRotationZ(m_radAngle);
		CMatrix4x4 translation = CMatrix4x4::BuildTranslation(m_uTranslation, m_vTranslation, 0);

		m_textureTransform = translation * rotation * scale;
		m_isTransformDirty = false;
	}

	return m_textureTransform;
}

void CTextureLayer::SetTextureAddressModes(const TTextureUVWAddressModes& textureAddressingModes)
{
	m_textureAddressingModes = textureAddressingModes;
}

void CTextureLayer::SetTextureAddressModes(ETextureAddressMode addressingMode)
{
	m_textureAddressingModes.m_u = addressingMode;
	m_textureAddressingModes.m_v = addressingMode;
	m_textureAddressingModes.m_w = addressingMode;
}

void CTextureLayer::SetTextureAddressModes(ETextureAddressMode uMode, ETextureAddressMode vMode, ETextureAddressMode wMode)
{
	m_textureAddressingModes.m_u = uMode;
	m_textureAddressingModes.m_v = vMode;
	m_textureAddressingModes.m_w = wMode;
}

const TTextureUVWAddressModes& CTextureLayer::GetTextureAddressModes() const
{
	return m_textureAddressingModes;
}

void CTextureLayer::SetTextureBorderColour(const TColourValue& colour)
{
	m_borderColour = colour;
}

const TColourValue& CTextureLayer::GetTextureBorderColour() const
{
	return m_borderColour;
}

}