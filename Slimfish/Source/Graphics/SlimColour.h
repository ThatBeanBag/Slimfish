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
class CColour {
	// Member Functions
public:
	/** Default constructor.
		@note defaults to black.
	 	@author Hayden Asplet
	*/
	CColour();

	/** Create a colour value from red, green, blue and alpha colour channels.
	 	@author Hayden Asplet
	*/
	CColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a = s_ALPHA_OPAQUE);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CColour();

	/** Set the red channel. @author Hayden Asplet */
	void SetRed(unsigned char  red);
	/** Get the red channel. @author Hayden Asplet */
	const unsigned char GetRed() const;

	/** Set the green channel. @author Hayden Asplet */
	void SetGreen(unsigned char  green);
	/** Get the green channel. @author Hayden Asplet */
	const unsigned char GetGreen() const;

	/** Set the blue channel. @author Hayden Asplet */
	void SetBlue(unsigned char blue);
	/** Get the blue channel. @author Hayden Asplet */
	const unsigned char GetBlue() const;

	/** Set the alpha channel. @author Hayden Asplet */
	void SetAlpha(unsigned char alpha);
	/** Get the alpha channel. @author Hayden Asplet */
	const unsigned char GetAlpha() const;
protected:
private:
	// Static constants
public:
	static const CColour s_WHITE;
	static const CColour s_BLACK;
	static const CColour s_RED;
	static const CColour s_GREEN;
	static const CColour s_BLUE;
	static const CColour s_YELLOW;
	static const CColour s_CYAN;
	static const CColour s_MAGENTA;

	static const unsigned char s_ALPHA_OPAQUE;
	static const unsigned char s_ALPHA_TRANSPERANT;
	// Member Variables
public:

	unsigned char m_r;
	unsigned char m_g;
	unsigned char m_b;
	unsigned char m_a;
protected:
private:
};

/** Class representing a colour as a amalgamation of four floating-point values a, r, g and b.
	@remarks
		Channel have the range 0 to 1. This is different from TColour where
		TColour's channels range from 0 to 255.
*/
class CColourValue {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CColourValue();

	/** Create a colour value from red, green, blue and alpha colour channels.
	 	@author Hayden Asplet
	*/
	CColourValue(float r, float g, float b, float a = s_ALPHA_OPAQUE);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CColourValue();

	/** Set the red channel. @author Hayden Asplet */
	void SetRed(float red);
	/** Get the red channel. @author Hayden Asplet */
	const float GetRed() const;

	/** Set the green channel. @author Hayden Asplet */
	void SetGreen(float green);
	/** Get the green channel. @author Hayden Asplet */
	const float GetGreen() const;

	/** Set the blue channel. @author Hayden Asplet */
	void SetBlue(float blue);
	/** Get the blue channel. @author Hayden Asplet */
	const float GetBlue() const;

	/** Set the alpha channel. @author Hayden Asplet */
	void SetAlpha(float alpha);
	/** Get the alpha channel. @author Hayden Asplet */
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
};

/************************************************************************/
/* Conversions
/************************************************************************/

/** Convert from a TColourValue to a TColour.
 	@author
 		Hayden Asplet
*/
CColour ToColour(const CColourValue& colourValue);

/** Convert from a TColour to a TColourValue.
 	@author
 		Hayden Asplet
*/
CColourValue ToColourValue(const CColour& colour);

/** Convert from a TColour to a DWORD
 	@author
 		Hayden Asplet
*/
unsigned long  ColourToDWORD(CColour colour);

/** Convert from a DWORD to a TColour
	@author
		Hayden Asplet
*/
CColour DWORDToColour(unsigned long  dwColour);

}


#endif	// __SLIMCOLOUR_H__