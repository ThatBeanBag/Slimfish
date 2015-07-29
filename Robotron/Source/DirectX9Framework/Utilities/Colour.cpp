// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Colour.h
// Description	: TColour declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Colour.h"

// Local Includes

const TColour g_kCOLOUR_WHITE = CreateColourRGB(255, 255, 255);
const TColour g_kCOLOUR_BLACK = CreateColourRGB(0, 0, 0);
const TColour g_kCOLOUR_RED = CreateColourRGB(255, 0, 0);
const TColour g_kCOLOUR_GREEN = CreateColourRGB(0, 255, 0);
const TColour g_kCOLOUR_BLUE = CreateColourRGB(0, 0, 255);
const TColour g_kCOLOUR_YELLOW = CreateColourRGB(255, 255, 0);
const TColour g_kCOLOUR_CYAN = CreateColourRGB(0, 255, 255);
const TColour g_kCOLOUR_MAGENTA = CreateColourRGB(255, 0, 255);

const unsigned char g_kcALPHA_OPAQUE = 255;
const unsigned char g_kcALPHA_TRANSPERANT = 0;

const float g_kfALPHA_OPAQUE_VALUE = 1.0f;
const float g_kfALPHA_TRANSPERANT_VALUE = 0.0f;

TColour CreateColourRGB(unsigned char _r, unsigned char _g, unsigned char _b)
{
	TColour colour;
	colour.m_a = g_kcALPHA_OPAQUE;
	colour.m_r = _r;
	colour.m_g = _g;
	colour.m_b = _b;

	return colour;
}

TColour CreateColourARGB(unsigned _a, unsigned char _r, unsigned char _g, unsigned char _b)
{
	TColour colour;
	colour.m_a = _a;
	colour.m_r = _r;
	colour.m_g = _g;
	colour.m_b = _b;

	return colour;
}

unsigned long ColourToDWORD(TColour _colour)
{
	// Pack the colour into an unsigned long.
	return ((_colour.m_a * 256 + _colour.m_r) * 256 + _colour.m_g) * 256 + _colour.m_b;
}

TColour DWORDToColour(unsigned long  _dwColour)
{
	auto unpack = [&]() {
		unsigned char cRet = _dwColour % 256;
		_dwColour /= 256;
		return cRet;
	};

	TColour colour;
	colour.m_b = unpack();
	colour.m_g = unpack();
	colour.m_r = unpack();
	colour.m_a = unpack();

	return colour;
}

TColourValue CreateColourValueRGB(float _fR, float _fG, float _fB)
{
	TColourValue colourValue;
	colourValue.m_fA = g_kfALPHA_OPAQUE_VALUE;
	colourValue.m_fR = _fR;
	colourValue.m_fG = _fG;
	colourValue.m_fB = _fB;

	return colourValue;
}

TColourValue CreateColourValueARGB(float _fA, float _fR, float _fG, float _fB)
{
	TColourValue colourValue;
	colourValue.m_fA = _fA;
	colourValue.m_fR = _fR;
	colourValue.m_fG = _fG;
	colourValue.m_fB = _fB;

	return colourValue;
}

std::string ColourToText(TColour _colour, int _iPrecision)
{
	std::string strText;

	strText = ToString(_colour.m_r, _iPrecision) + ", " + ToString(_colour.m_g, _iPrecision) + ", " +
			  ToString(_colour.m_b, _iPrecision) + ", " + ToString(_colour.m_a, _iPrecision);

	return std::move(strText);
}

std::string ColourToText(const TColourValue& _krColourValue, int _iPrecision)
{
	std::string strText;

	strText = ToString(_krColourValue.m_fR, _iPrecision) + ", " + ToString(_krColourValue.m_fG, _iPrecision) + ", " +
			  ToString(_krColourValue.m_fB, _iPrecision) + ", " + ToString(_krColourValue.m_fA, _iPrecision);

	return std::move(strText);
}

TColour ValueToColour(const TColourValue& _krValue)
{
	TColour colour;
	colour.m_a = static_cast<unsigned char>(std::max(0.0f, std::min(_krValue.m_fA * 255.0f, 255.0f)));
	colour.m_r = static_cast<unsigned char>(std::max(0.0f, std::min(_krValue.m_fR * 255.0f, 255.0f)));
	colour.m_g = static_cast<unsigned char>(std::max(0.0f, std::min(_krValue.m_fG * 255.0f, 255.0f)));
	colour.m_b = static_cast<unsigned char>(std::max(0.0f, std::min(_krValue.m_fB * 255.0f, 255.0f)));
	return colour;
}
