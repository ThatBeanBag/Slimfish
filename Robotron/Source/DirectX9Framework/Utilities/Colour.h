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

#pragma once

#ifndef __COLOUR_H__
#define __COLOUR_H__

// Library Includes

// Local Includes

struct TColour {
	unsigned char m_a;
	unsigned char m_r;
	unsigned char m_g;
	unsigned char m_b;
};

TColour CreateColourRGB(unsigned char _r, unsigned char _g, unsigned char _b);
TColour CreateColourARGB(unsigned _a, unsigned char _r, unsigned char _g, unsigned char _b);
unsigned long  ColourToDWORD(TColour _colour);
TColour DWORDToColour(unsigned long  _dwColour);

extern const TColour g_kCOLOUR_WHITE;
extern const TColour g_kCOLOUR_BLACK;
extern const TColour g_kCOLOUR_RED;
extern const TColour g_kCOLOUR_GREEN;
extern const TColour g_kCOLOUR_BLUE;
extern const TColour g_kCOLOUR_YELLOW;
extern const TColour g_kCOLOUR_CYAN;
extern const TColour g_kCOLOUR_MAGENTA;

extern const unsigned char g_kcALPHA_OPAQUE;
extern const unsigned char g_kcALPHA_TRANSPERANT;

struct TColourValue {
	float m_fA;
	float m_fR;
	float m_fG;
	float m_fB;
};

TColourValue CreateColourValueRGB(float _fR, float _fG, float _fB);
TColourValue CreateColourValueARGB(float _fA, float _fR, float _fG, float _fB);

// Converts a colour to text in format, R,G,B,A.
std::string ColourToText(TColour _colour, int _iPrecision = g_kiDEF_PRECISION);
std::string ColourToText(const TColourValue& _krColourValue, int _iPrecision = g_kiDEF_PRECISION);
TColour ValueToColour(const TColourValue& _krValue);

extern const float g_kfALPHA_OPAQUE_VALUE;
extern const float g_kfALPHA_TRANSPERANT_VALUE;

#endif	// __COLOUR_H__