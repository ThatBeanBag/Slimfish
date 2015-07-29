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

/************************************************************************/
/* TColour
/*
/* A standard colour structure which packs alpha, red, green and blue
/* colour channels together.
/*
/* Channel have the range 0 to 255. This is the difference between TColour
/* and TColourValue where TColourValue's channels range from 0 to 1.
/************************************************************************/
struct TColour {
	unsigned char m_a;
	unsigned char m_r;
	unsigned char m_g;
	unsigned char m_b;
};

/************************************************************************/
/* TColourValue
/*
/* A standard colour structure which packs alpha, red, green and blue
/* colour channels together with floating point precision.
/*
/* Channel have the range 0 to 1. This is the difference between TColour
/* and TColourValue where TColourValue's channels range from 0 to 255.
/************************************************************************/
struct TColourValue {
	float m_a;
	float m_r;
	float m_g;
	float m_b;
};

/************************************************************************/
/* Frequently used colours and alphas.
/************************************************************************/
extern const TColour g_COLOUR_WHITE;
extern const TColour g_COLOUR_BLACK;
extern const TColour g_COLOUR_RED;
extern const TColour g_COLOUR_GREEN;
extern const TColour g_COLOUR_BLUE;
extern const TColour g_COLOUR_YELLOW;
extern const TColour g_COLOUR_CYAN;
extern const TColour g_COLOUR_MAGENTA;

extern const TColourValue g_COLOUR_VALUE_WHITE;
extern const TColourValue g_COLOUR_VALUE_BLACK;
extern const TColourValue g_COLOUR_VALUE_RED;
extern const TColourValue g_COLOUR_VALUE_GREEN;
extern const TColourValue g_COLOUR_VALUE_BLUE;
extern const TColourValue g_COLOUR_VALUE_YELLOW;
extern const TColourValue g_COLOUR_VALUE_CYAN;
extern const TColourValue g_COLOUR_VALUE_MAGENTA;

extern const unsigned char g_ALPHA_OPAQUE;
extern const unsigned char g_ALPHA_TRANSPERANT;

extern const float g_ALPHA_VALUE_OPAQUE;
extern const float g_ALPHA_VALUE_TRANSPERANT;

/************************************************************************/
/* Helper creation functions
/************************************************************************/

/**
* Creates a colour from Red, Green and Blue colour values.
*
* Channel values range from 0 to 255.
* The alpha value will be set to opaque.
* 
* @author: 	Hayden Asplet
* @param:	red - red channel value. 
* @param:	green - green channel value.
* @param:	blue - blue channel value.
* @return:  Slim::TColour - resultant colour.
*/
TColour CreateColourRGB(unsigned char red, unsigned char green, unsigned char blue);

/**
* Creates a colour from Alpha, Red, Green and Blue colour values.
*
* Channel values range from 0 to 255.
*
* @author: 	Hayden Asplet
* @param:   alpha - alpha channel value.
* @param:	red - red channel value.
* @param:	green - green channel value.
* @param:	blue - blue channel value.
* @return:  Slim::TColour - resultant colour.
*/
TColour CreateColourARGB(unsigned alpha, unsigned char red, unsigned char green, unsigned char blue);

/**
* Creates a colour value structure from Red, Green and Blue colour values.
*
* Channel values range from 0 to 1.
* The alpha channel will be set to opaque (255).
*
* @author: 	Hayden Asplet
* @param:	red - red channel value.
* @param:	green - green channel value.
* @param:	blue - blue channel value.
* @return:  Slim::TColour - resultant colour.
*/
TColourValue CreateColourValueRGB(float red, float green, float blue);

/**
* Creates a colour value structure from Alpha, Red, Green and Blue colour values.
*
* Channel values range from 0 to 1.
*
* @author: 	Hayden Asplet
* @param:   alpha - alpha channel value.
* @param:	red - red channel value.
* @param:	green - green channel value.
* @param:	blue - blue channel value.
* @return:  Slim::TColour - resultant colour.
*/
TColourValue CreateColourValueARGB(float alpha, float red, float green, float blue);

/************************************************************************/
/* Conversions
/************************************************************************/

/**
* Converts a TColourValue to a TColour.
* 
* @author: 	Hayden Asplet
* @param:	colourValue - TColourValue to convert to TColour.
* @return:  Slim::TColour - converted TColour.
*/
TColour ValueToColour(const TColourValue& colourValue);

/**
* Converts a TColour to a TColourValue.
* 
* @author: 	Hayden Asplet
* @param:	colour - TColour to convert.
* @return:  Slim::TColourValue - converted TColourValue.
*/
TColourValue ColourToValue(const TColour& colour);

/**
* Converts a TColour to a DWORD
* 
* @author: 	Hayden Asplet
* @param:	colour - TColour to convert
* @return:  unsigned long - converted DWORD
*/
unsigned long  ColourToDWORD(TColour colour);

/**
* Converts a DWORD to a TColour
* 
* @author: 	Hayden Asplet
* @param:	unsigned long dwColour - DWORD to convert.
* @return:  Slim::TColour - converted TColour.
*/
TColour DWORDToColour(unsigned long  dwColour);

}


#endif	// __SLIMCOLOUR_H__