// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimTexture.h
// Description	: CTexture declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMTEXTURE_H__
#define __SLIMTEXTURE_H__

// Library Includes

// Local Includes
#include "SlimGPUBuffer.h"

namespace Slim {

/** Abstract class representing a texture resource.
@remarks
		
*/
class ATexture {
public:
	/** List of usage flags that define how the buffer is used.
		@note
			This enumeration is weakly typed, as in it can be implicitly
			converted to and from an int this gives it the functionality to be
			used as a flag, but the downside is that it lacks type safety.
	*/
	enum EUsage : int {
		// See CGPUBuffer::USAGE_STATIC.
		USAGE_STATIC = AGpuBuffer::USAGE_STATIC,
		// See CGPUBuffer::USAGE_DYNAMIC.
		USAGE_DYNAMIC = AGpuBuffer::USAGE_DYNAMIC,
		// See CGPUBuffer::USAGE_WRITE_ONLY.
		USAGE_WRITE_ONLY = AGpuBuffer::USAGE_WRITE_ONLY,
		// See CGPUBuffer::USAGE_DISCARDABLE.
		USAGE_DISCARDABLE = AGpuBuffer::USAGE_DISCARDABLE,
		// The texture resource will be used as a render target.
		USAGE_RENDER_TARGET
	};

	/** List of standard texture types.
	*/
	enum EType {
		// 1 dimensional texture.
		TYPE_1D,
		// 2 dimensional texture (most commonly used for standard textures).
		TYPE_2D,
		// 3 dimensional textures. Basically an array of 2D textures.
		TYPE_3D,
		// Cube mapping textures.
		TYPE_CUBIC
	};

	// Member Functions
public:
	ATexture();
	~ATexture();

	void Load();
	void Unload();

	/** Set the texture type e.g. 1D, 2D, 3D or cubic.
	 	@author Hayden Asplet
	*/
	void SetType(EType textureType);

	/** Get the texture type e.g. 1D, 2D, 3D or cubic.
	 	@author Hayden Asplet
	*/
	EType GetType() const;

	/** Set how the texture is intended to be used (only useful prior to loading).
	 	@author Hayden Asplet
	*/
	void SetUsage(EUsage usage);

	/** Get how the texture is intended to be used.
	 	@author Hayden Asplet
	*/
	const EUsage GetUsage() const;

	/** Get the width of the texture @author Hayden Asplet */
	const int GetWidth() const;
	/** Get the height of the texture @author Hayden Asplet */
	const int GetHeight() const;
	/** Get the depth of the 3D texture @author Hayden Asplet */
	const int GetDepth() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	EType m_textureType;
	EUsage m_usage;

	int width;	// Width of the texture.
	int height;	// Height of the texture.
	int depth;	// Depth of the texture (only used for 3D textures).
};

}

#endif	// __SLIMTEXTURE_H__