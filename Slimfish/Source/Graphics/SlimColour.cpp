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

const CColour CColour::s_WHITE = CColour(255, 255, 255);
const CColour CColour::s_BLACK = CColour(0, 0, 0);
const CColour CColour::s_RED = CColour(255, 0, 0);
const CColour CColour::s_GREEN = CColour(0, 255, 0);
const CColour CColour::s_BLUE = CColour(0, 0, 255);
const CColour CColour::s_YELLOW = CColour(255, 255, 0);
const CColour CColour::s_CYAN = CColour(0, 255, 255);
const CColour CColour::s_MAGENTA = CColour(255, 0, 255);

const unsigned char CColour::s_ALPHA_OPAQUE = 255;
const unsigned char CColour::s_ALPHA_TRANSPERANT = 0;


const CColourValue CColourValue::s_WHITE = CColourValue(1.0f, 1.0f, 1.0f);
const CColourValue CColourValue::s_BLACK = CColourValue(0.0f, 0.0f, 0.0f);
const CColourValue CColourValue::s_RED = CColourValue(1.0f, 0.0f, 0.0f);
const CColourValue CColourValue::s_GREEN = CColourValue(0.0f, 1.0f, 0.0f);
const CColourValue CColourValue::s_BLUE = CColourValue(0.0f, 0.0f, 1.0f);
const CColourValue CColourValue::s_YELLOW = CColourValue(1.0f, 1.0f, 0.0f);
const CColourValue CColourValue::s_CYAN = CColourValue(0.0f, 1.0f, 1.0f);
const CColourValue CColourValue::s_MAGENTA = CColourValue(1.0f, 0.0f, 1.0f);

const float CColourValue::s_ALPHA_OPAQUE = 1.0f;
const float CColourValue::s_ALPHA_TRANSPERANT = 0.0f;

CColour::CColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a /*= s_ALPHA_OPAQUE*/) 
	:m_r(r), m_g(g), m_b(b), m_a(a)
{

}

CColour::CColour() 
	:m_r(0), m_g(0), m_b(0), m_a(s_ALPHA_OPAQUE)
{

}

CColour::~CColour()
{

}

void CColour::SetRed(unsigned char red)
{
	m_r = red;
}

const unsigned char CColour::GetRed() const
{
	return m_r;
}

void CColour::SetGreen(unsigned char green)
{
	m_g = green;
}

const unsigned char CColour::GetGreen() const
{
	return m_g;
}

void CColour::SetBlue(unsigned char blue)
{
	m_b = blue;
}

const unsigned char CColour::GetBlue() const
{
	return m_b;
}

void CColour::SetAlpha(unsigned char alpha)
{
	m_a = alpha;
}

const unsigned char CColour::GetAlpha() const
{
	return m_a;
}

CColourValue::CColourValue(float r, float g, float b, float a /*= s_ALPHA_OPAQUE*/) :m_r(r), m_g(g), m_b(b), m_a(a)
{

}

CColourValue::CColourValue() : m_r(0.0f), m_g(0.0f), m_b(0.0f), m_a(s_ALPHA_OPAQUE)
{

}

CColourValue::~CColourValue()
{

}

void CColourValue::SetRed(float red)
{
	m_r = red;
}

const float CColourValue::GetRed() const
{
	return m_r;
}

void CColourValue::SetGreen(float green)
{
	m_g = green;
}

const float CColourValue::GetGreen() const
{
	return m_g;
}

void CColourValue::SetBlue(float blue)
{
	m_b = blue;
}

const float CColourValue::GetBlue() const
{
	return m_b;
}

void CColourValue::SetAlpha(float alpha)
{
	m_a = alpha;
}

const float CColourValue::GetAlpha() const
{
	return m_a;
}

/************************************************************************/
/* Conversions
/************************************************************************/

CColour ToColour(const CColourValue& colourValue)
{
	CColour colour;
	colour.m_a = static_cast<unsigned char>(std::max(0.0f, std::min(colourValue.m_a * 255.0f, 255.0f)));
	colour.m_r = static_cast<unsigned char>(std::max(0.0f, std::min(colourValue.m_r * 255.0f, 255.0f)));
	colour.m_g = static_cast<unsigned char>(std::max(0.0f, std::min(colourValue.m_g * 255.0f, 255.0f)));
	colour.m_b = static_cast<unsigned char>(std::max(0.0f, std::min(colourValue.m_b * 255.0f, 255.0f)));

	return colour;
}

CColourValue ToColourValue(const CColour& colour)
{
	CColourValue colourValue;
	colourValue.m_a = static_cast<float>(colour.m_a) / 255.0f;
	colourValue.m_r = static_cast<float>(colour.m_r) / 255.0f;
	colourValue.m_g = static_cast<float>(colour.m_g) / 255.0f;
	colourValue.m_b = static_cast<float>(colour.m_b) / 255.0f;

	return colourValue;
}

unsigned long ColourToDWORD(CColour colour)
{
	// Pack the colour into an unsigned long.
	return ((colour.m_a * 256 + colour.m_r) * 256 + colour.m_g) * 256 + colour.m_b;
}

CColour DWORDToColour(unsigned long dwColour)
{
	auto unpack = [&]() {
		unsigned char cRet = dwColour % 256;
		dwColour /= 256;
		return cRet;
	};

	CColour colour;
	colour.m_b = unpack();
	colour.m_g = unpack();
	colour.m_r = unpack();
	colour.m_a = unpack();

	return colour;
}

}