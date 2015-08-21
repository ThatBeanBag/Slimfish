// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimColour.cpp
// Description	: TColour and TColourValue implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimColour.h"

// Local Includes


namespace Slim {

/************************************************************************/
/* Frequently used colours and alphas.
/************************************************************************/
const TColour TColour::s_WHITE = CreateColourRGB(255, 255, 255);
const TColour TColour::s_BLACK = CreateColourRGB(0, 0, 0);
const TColour TColour::s_RED = CreateColourRGB(255, 0, 0);
const TColour TColour::s_GREEN = CreateColourRGB(0, 255, 0);
const TColour TColour::s_BLUE = CreateColourRGB(0, 0, 255);
const TColour TColour::s_YELLOW = CreateColourRGB(255, 255, 0);
const TColour TColour::s_CYAN = CreateColourRGB(0, 255, 255);
const TColour TColour::s_MAGENTA = CreateColourRGB(255, 0, 255);

const unsigned char TColour::s_ALPHA_OPAQUE = 255;
const unsigned char TColour::s_ALPHA_TRANSPERANT = 0;

const TColourValue TColourValue::s_WHITE = CreateColourValueRGB(1.0f, 0.0f, 0.0f);
const TColourValue TColourValue::s_BLACK = CreateColourValueRGB(0.0f, 0.0f, 0.0f);
const TColourValue TColourValue::s_RED = CreateColourValueRGB(1.0f, 0.0f, 0.0f);
const TColourValue TColourValue::s_GREEN = CreateColourValueRGB(0.0f, 1.0f, 0.0f);
const TColourValue TColourValue::s_BLUE = CreateColourValueRGB(0.0f, 0.0f, 1.0f);
const TColourValue TColourValue::s_YELLOW = CreateColourValueRGB(1.0f, 1.0f, 0.0f);
const TColourValue TColourValue::s_CYAN = CreateColourValueRGB(0.0f, 1.0f, 1.0f);
const TColourValue TColourValue::s_MAGENTA = CreateColourValueRGB(1.0f, 0.0f, 1.0f);

const float TColourValue::s_ALPHA_OPAQUE = 1.0f;
const float TColourValue::s_ALPHA_TRANSPERANT = 0.0f;

/************************************************************************/
/* Helper creation functions
/************************************************************************/

TColour CreateColourRGB(unsigned char red, unsigned char green, unsigned char blue)
{
	TColour colour;
	colour.m_a = TColour::s_ALPHA_OPAQUE;
	colour.m_r = red;
	colour.m_g = green;
	colour.m_b = blue;

	return colour;
}

TColour CreateColourARGB(unsigned alpha, unsigned char red, unsigned char green, unsigned char blue)
{
	TColour colour = CreateColourRGB(red, green, blue);
	colour.m_a = alpha;

	return colour;
}

TColourValue CreateColourValueRGB(float red, float green, float blue)
{
	TColourValue colourValue;
	colourValue.m_a = TColourValue::s_ALPHA_OPAQUE;
	colourValue.m_r = red;
	colourValue.m_g = green;
	colourValue.m_b = blue;

	return colourValue;
}

TColourValue CreateColourValueARGB(float alpha, float red, float green, float blue)
{
	TColourValue colourValue = CreateColourValueRGB(red, green, blue);
	colourValue.m_a = alpha;

	return colourValue;
}

/************************************************************************/
/* Conversions
/************************************************************************/

TColour ValueToColour(const TColourValue& colourValue)
{
	TColour colour;
	colour.m_a = static_cast<unsigned char>(std::max(0.0f, std::min(colourValue.m_a * 255.0f, 255.0f)));
	colour.m_r = static_cast<unsigned char>(std::max(0.0f, std::min(colourValue.m_r * 255.0f, 255.0f)));
	colour.m_g = static_cast<unsigned char>(std::max(0.0f, std::min(colourValue.m_g * 255.0f, 255.0f)));
	colour.m_b = static_cast<unsigned char>(std::max(0.0f, std::min(colourValue.m_b * 255.0f, 255.0f)));

	return colour;
}

TColourValue ColourToValue(const TColour& colour)
{
	TColourValue colourValue;
	colourValue.m_a = static_cast<float>(colour.m_a) / 255.0f;
	colourValue.m_r = static_cast<float>(colour.m_r) / 255.0f;
	colourValue.m_g = static_cast<float>(colour.m_g) / 255.0f;
	colourValue.m_b = static_cast<float>(colour.m_b) / 255.0f;

	return colourValue;
}

unsigned long ColourToDWORD(TColour colour)
{
	// Pack the colour into an unsigned long.
	return ((colour.m_a * 256 + colour.m_r) * 256 + colour.m_g) * 256 + colour.m_b;
}

TColour DWORDToColour(unsigned long dwColour)
{
	auto unpack = [&]() {
		unsigned char cRet = dwColour % 256;
		dwColour /= 256;
		return cRet;
	};

	TColour colour;
	colour.m_b = unpack();
	colour.m_g = unpack();
	colour.m_r = unpack();
	colour.m_a = unpack();

	return colour;
}

}