// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimTexture.cpp
// Description	: CSlimTexture implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimTexture.h"

// Local Includes

namespace Slim {

	ATexture::ATexture(const std::string& name, ETextureType textureType, ETextureUsage usage)
		:m_Name(name),
		m_TextureType(textureType),
		m_Usage(usage),
		m_Width(0),
		m_Height(0),
		m_Depth(0),
		m_SourceWidth(0),
		m_SourceHeight(0),
		m_SourceDepth(0)
	{

	}

	ATexture::~ATexture()
	{

	}

	const std::string& ATexture::GetName() const
	{
		return m_Name;
	}

	void ATexture::SetType(ETextureType textureType)
	{
		m_TextureType = textureType;
	}

	ETextureType ATexture::GetTextureType() const
	{
		return m_TextureType;
	}

	void ATexture::SetUsage(ETextureUsage usage)
	{
		m_Usage = usage;
	}

	const ETextureUsage ATexture::GetUsage() const
	{
		return m_Usage;
	}

	const bool ATexture::IsLoaded() const
	{
		return m_IsLoaded;
	}

	const size_t ATexture::GetWidth() const
	{
		return m_Width;
	}

	const size_t ATexture::GetHeight() const
	{
		return m_Height;
	}

	const size_t ATexture::GetDepth() const
	{
		return m_Depth;
	}

	void ATexture::SetWidth(size_t width)
	{
		m_Width = width;
	}

	void ATexture::SetHeight(size_t height)
	{
		m_Height = height;
	}

	void ATexture::SetDepth(size_t depth)
	{
		m_Depth = depth;
	}

	void ATexture::SetMultiSampleCount(size_t msaaCount)
	{
		m_MSAACount = msaaCount;
	}

	void ATexture::SetMultiSampleQuality(size_t msaaQuality)
	{
		m_MSAAQuality = msaaQuality;
	}

	const size_t ATexture::GetMultiSampleCount() const
	{
		return m_MSAACount;
	}

	const size_t ATexture::GetMultiSampleQuality() const
	{
		return m_MSAAQuality;
	}

	void ATexture::SetSourceWidth(size_t srcWidth)
	{
		m_SourceWidth = srcWidth;
	}

	size_t ATexture::GetSourceWidth() const
	{
		return m_SourceWidth;
	}

	void ATexture::SetSourceHeight(size_t srcHeight)
	{
		m_SourceHeight = srcHeight;
	}

	size_t ATexture::GetSourceHeight() const
	{
		return m_SourceHeight;
	}

	void ATexture::SetSourceDepth(size_t srcDepth)
	{
		m_SourceDepth = srcDepth;
	}

	size_t ATexture::GetSourceDepth() const
	{
		return m_SourceDepth;
	}

}

