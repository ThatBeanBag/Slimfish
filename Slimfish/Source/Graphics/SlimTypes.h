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

/** List of fog types.
*/
enum class EFogType {
	// No fog.
	NONE,
	// Fog density increases linearly between the start and end distances.
	LINEAR,
	// Fog density increases exponentially.
	EXPONENTIAL,
	// As FOG_EXPONENTIAL but raises to the power of 2 to increase density even quicker.
	EXPONENTIAL2
};

/** List of culling modes for the rasterizer stage.
*/
enum class ECullingMode {
	// Don't cull.
	NONE,
	// Cull clockwise ordered vertices (forward facing polygons).
	CLOCKWISE,
	// Cull anti-clockwise order vertices (back facing polygons).
	COUNTER_CLOCKWISE
};

/** List of fill modes for the rasterizer stage.
*/
enum class EFillMode {
	SOLID,
	WIREFRAME
};

/** List of blending operations.
*/
enum class EBlendOperation {
	// Add both sources.
	ADD,
	// Subtract source 1 from source 2.
	SUBTRACT,
	// Subtract source 2 from source 1.
	REVERSE_SUBTRACT,
	// Use the minimum of source 1 and 2.
	MIN,
	// Use the maximum of source 1 and 2.
	MAX
};

/** List of blend factors.
*/
enum class EBlendFactor {
	// Blend factor is (0,0,0,0).
	ZERO,
	// Blend factor is (1,1,1,1).
	ONE,
	// Use source colour.
	SOURCE_COLOUR,
	// Use 1 - source colour.
	INVERSE_SOURCE_COLOUR,
	// Use source alpha.
	SOURCE_ALPHA,
	// Use inverse source alpha.
	INVERSE_SOURCE_ALPHA,
	// Use destination colour.
	DEST_COLOUR,
	// Use inverse destination colour.
	INVERSE_DEST_COLOUR,
	// Use destination alpha.
	DEST_ALPHA,
	// Use inverse destination alpha.
	INVERSE_DEST_ALPHA,
};


}
#endif	// __SLIMTYPES_H__