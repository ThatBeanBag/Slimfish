// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimColour.h
// Description	: SlimColour declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMCOLOUR_H__
#define __SLIMCOLOUR_H__

// Library Includes

// Local Includes

namespace Slim {

/** Basic colour structure  
@remarks
	Colour channels have the range 0 to 255. This is different from TColourValue
	where TColourValue's channels range from 0 to 1.
*/
struct TColour {
	unsigned char m_a;
	unsigned char m_r;
	unsigned char m_g;
	unsigned char m_b;

	static const TColour s_WHITE;
	static const TColour s_BLACK;
	static const TColour s_RED;
	static const TColour s_GREEN;
	static const TColour s_BLUE;
	static const TColour s_YELLOW;
	static const TColour s_CYAN;
	static const TColour s_MAGENTA;

	static const unsigned char s_ALPHA_OPAQUE;
	static const unsigned char s_ALPHA_TRANSPERANT;
};

/** Class representing a colour as a amalgamation of four floating-point values a, r, g and b.
	@remarks
	
*/
/*
class CColourValue {
	// Member Functions
public:
	CColourValue();
	CColourValue(float r, float g, float b);
	CColourValue(float a, float r, float g, float b);

	void SetAlpha(float alpha);
	const float GetAlpha() const;
protected:
private:

	// Static constants
public:
	static const CColourValue s_WHITE;
	static const CColourValue s_BLACK;
	static const CColourValue s_RED;
	static const CColourValue s_GREEN;
	static const CColourValue s_BLUE;
	static const CColourValue s_YELLOW;
	static const CColourValue s_CYAN;
	static const CColourValue s_MAGENTA;

	static const float s_ALPHA_OPAQUE;
	static const float s_ALPHA_TRANSPERANT;

	// Member Variables
public:
	float m_a;
	float m_r;
	float m_g;
	float m_b;
protected:
private:
};*/

/** Basic colour value structure 
@remarks
	Channel have the range 0 to 1. This is different from TColour where
	TColour's channels range from 0 to 255.
*/
struct TColourValue {
	float m_a;
	float m_r;
	float m_g;
	float m_b;

	static const TColourValue s_WHITE;
	static const TColourValue s_BLACK;
	static const TColourValue s_RED;
	static const TColourValue s_GREEN;
	static const TColourValue s_BLUE;
	static const TColourValue s_YELLOW;
	static const TColourValue s_CYAN;
	static const TColourValue s_MAGENTA;

	static const float s_ALPHA_OPAQUE;
	static const float s_ALPHA_TRANSPERANT;
};

extern const unsigned char g_ALPHA_OPAQUE;
extern const unsigned char g_ALPHA_TRANSPERANT;

extern const float g_ALPHA_VALUE_OPAQUE;
extern const float g_ALPHA_VALUE_TRANSPERANT;

/************************************************************************/
/* Helper creation functions
/************************************************************************/

/** Create a colour from red, green and blue colour channels.
 	@author
 		Hayden Asplet
*/
TColour CreateColourRGB(unsigned char red, unsigned char green, unsigned char blue);

/** Create a colour from alpha, red, green and blue colour channels.
 	@author
 		Hayden Asplet
*/
TColour CreateColourARGB(unsigned alpha, unsigned char red, unsigned char green, unsigned char blue);

/** Create a colour value from red, green and blue colour values.
 	@author
 		Hayden Asplet
*/
TColourValue CreateColourValueRGB(float red, float green, float blue);

/** Create a colour value from alpha, red, green and blue colour values.
 	@author
 		Hayden Asplet
*/
TColourValue CreateColourValueARGB(float alpha, float red, float green, float blue);

/************************************************************************/
/* Conversions
/************************************************************************/

/** Convert from a TColourValue to a TColour.
 	@author
 		Hayden Asplet
*/
TColour ValueToColour(const TColourValue& colourValue);

/** Convert from a TColour to a TColourValue.
 	@author
 		Hayden Asplet
*/
TColourValue ColourToValue(const TColour& colour);

/** Convert from a TColour to a DWORD
 	@author
 		Hayden Asplet
*/
unsigned long  ColourToDWORD(TColour colour);

/** Convert from a DWORD to a TColour
	@author
		Hayden Asplet
*/
TColour DWORDToColour(unsigned long  dwColour);

}


#endif	// __SLIMCOLOUR_H__