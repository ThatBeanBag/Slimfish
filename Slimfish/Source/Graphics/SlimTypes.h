// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimTypes.h
// Description	: Common graphics types declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMTYPES_H__
#define __SLIMTYPES_H__

// Library Includes

// Local Includes

namespace Slim {

enum EFogType {
	// No fog.
	FOG_NONE,
	// Fog density increases linearly between the start and end distances.
	FOG_LINEAR,
	// Fog density increases exponentially.
	FOG_EXPONENTIAL,
	// As FOG_EXPONENTIAL but raises to the power of 2 to increase density even quicker.
	FOG_EXPONENTIAL2
};

}
#endif	// __SLIMTYPES_H__