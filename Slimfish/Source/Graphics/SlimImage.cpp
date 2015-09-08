// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimImage.cpp
// Description	: CSlimImage implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimImage.h"

// Local Includes

namespace Slim {

	CImage::CImage(size_t width, size_t height)
		:m_Width(width),
		m_Height(height),
		m_Pixels(height, std::vector<CColour>(width, CColour::s_BLACK))
	{

	}

	CImage::CImage(const TPixels& pixels)
		:m_Width(0),
		m_Height(0),
		m_Pixels(pixels)
	{
		if (!pixels.empty()) {
			m_Height = pixels.size();
			m_Width = pixels[0].size();
		}
	}

	CImage::~CImage()
	{

	}

	const std::vector<CColour>& CImage::operator[](size_t y) const
	{
		assert(y >= 0 && y <= m_Pixels.size());
		return m_Pixels[y];
	}

	std::vector<CColour>& CImage::operator[](size_t y)
	{
		// Call the const version of this function then cast off the constness to avoid
		// code duplication.
		//return const_cast<std::vector<CColour>&>(static_cast<const CImage>(*this)[y]);
		return m_Pixels[y];
	}

	const CColour& CImage::GetPixel(size_t x, size_t y)
	{
		return m_Pixels[y][x];
	}

	void CImage::SetPixel(const CColour& colour, size_t x, size_t y)
	{
		m_Pixels[y][x] = colour;
	}

	void CImage::SetSize(size_t width, size_t height)
	{
		m_Width = width;
		m_Height = height;

		// Resize for the rows of pixels and default them to black.
		m_Pixels.resize(height, std::vector<CColour>(width, CColour::s_BLACK));

		auto resizeRow = [&](std::vector<CColour>& row) {
			row.resize(width, CColour::s_BLACK);
			// Trim excess capacity using the swap trick.
			std::vector<CColour>(row).swap(row);
		};

		// Resize each row of pixels.
		std::for_each(m_Pixels.begin(), m_Pixels.end(), resizeRow);

		// Trim excess capacity using the swap trick.
		std::vector<std::vector<CColour> >(m_Pixels).swap(m_Pixels);
	}

	size_t CImage::GetWidth() const
	{
		if (m_Pixels.empty()) {
			return 0;
		}

		return m_Pixels[0].size();
	}

	size_t CImage::GetHeight() const
	{
		return m_Pixels.size();
	}

	size_t CImage::GetSize() const
	{
		if (m_Pixels.empty()) {
			return 0;
		}

		// Return width multiplied by height.
		return m_Pixels[0].size() * m_Pixels.size();
	}

}

