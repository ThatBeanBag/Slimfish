// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimTypes.h
// Description	: Declaration file for common graphics types.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMTYPES_H__
#define __SLIMTYPES_H__

// Library Includes

// Local Includes

namespace Slim {

/** List of primitive types for describing how vertex data gets interpreted by the
rendering system.
*/
enum class EPrimitiveType {
	// A list of points.
	POINTLIST,
	// A list of lines, 2 vertices to a line.
	LINELIST,
	// A strip of connected lines, each new vertex adds a new line (excluding the first).
	LINESTRIP,
	// A list of triangles, 3 vertices per triangle.
	TRIANGLELIST,
	// A strip of connected triangles, each new vertex adds a new triangle (excluding the first two).
	TRIANGLESTRIP,
	// A fan of triangles, similar to TRIANGLESTRIP except each triangle shares one 
	// central vertex (the first vertex).
	//TRIANGLEFAN	// Not used in directX 10.
};

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

/** Structure describing how a source alpha/colour blends with the destinations 
	alpha/colour.
*/
struct TBlendingMode {
	// Colour blending.
	EBlendFactor sourceColourBlendFactor = EBlendFactor::ONE;
	EBlendFactor destColourBlendFactor = EBlendFactor::ZERO;
	EBlendOperation colourBlendOperation = EBlendOperation::ADD;
	// Alpha blending.
	EBlendFactor sourceAlphaBlendFactor = EBlendFactor::ONE;
	EBlendFactor destAlphaBlendFactor = EBlendFactor::ZERO;
	EBlendOperation alphaBlendOperation = EBlendOperation::ADD;
};

/** Structure describing the enabled colour writes to the back buffer.
*/
struct TColourWritesEnabled {
	bool red = true;
	bool blue = true;
	bool green = true;
	bool alpha = true;
};

/** List of comparison functions for the depth and stencil buffers.
*/
enum class EComparisonFunction {
	// Never pass the comparison test.
	NEVER,
	// Passed if the source data is less than the destination data.
	LESS,
	// Passed if the source data is equal to the destination data.
	EQUAL,
	// Passed if the source data is less than or equal to the destination data.
	LESS_EQUAL,
	// Passed if the source data is greater than the destination data.
	GREATER,
	// Passed if the source data is not equal to the destination data.
	NOT_EQUAL,
	// Passed if the source data is greater than or equal to the destination data.
	GREATER_EQUAL,
	// Always pass the comparison test.
	ALWAYS
};

/** List of the stencil buffer operations that can be performed during depth-stencil testing.
*/
enum class EStencilOperation {
	// Keep existing data.
	KEEP,
	// Set the stencil data to 0.
	ZERO,
	// Replace stencil data with stencil reference value.
	REPLACE,
	// Invert the stencil data.
	INVERT,
	// Increment the stencil data by 1 and wrap the result.
	INCREMENT,
	// Decrement the stencil data by 1 and wrap the result.
	DECREMENT,
	// Increment the stencil data by 1 and clamp the result.
	INCREMENT_CLAMP,
	// Decrement the stencil data by 1 and clamp the result.
	DECREMENT_CLAMP
};

struct TStencilBufferSettings {
	bool stencilCheckEnabled = false;
	EComparisonFunction stencilCompareFunction = EComparisonFunction::ALWAYS;
	size_t stencilReferenceValue = 0xff;
	size_t stencilWriteMask = 0xff;
	EStencilOperation stencilFailOperation = EStencilOperation::KEEP;
	EStencilOperation stencilDepthFailOperation = EStencilOperation::KEEP;
	EStencilOperation stencilPassOperation = EStencilOperation::KEEP;
};

}
#endif	// __SLIMTYPES_H__