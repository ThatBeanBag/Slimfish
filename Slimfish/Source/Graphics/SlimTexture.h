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
	enum EUsage {
		// See CGPUBuffer::USAGE_STATIC.
		USAGE_STATIC = AGpuBuffer::USAGE_STATIC,
		// See CGPUBuffer::USAGE_DYNAMIC.
		USAGE_DYNAMIC = AGpuBuffer::USAGE_DYNAMIC,
		// See CGPUBuffer::USAGE_WRITE_ONLY.
		USAGE_WRITE_ONLY = AGpuBuffer::USAGE_WRITE_ONLY,
		// See CGPUBuffer::USAGE_DISCARDABLE.
		USAGE_DISCARDABLE = AGpuBuffer::USAGE_DISCARDABLE,
		// See CGPUBuffer::USAGE_DYNAMIC_DISCARDABLE_WRITE_ONLY.
		USAGE_DYNAMIC_DISARDABLE_WRITE_ONLY = AGpuBuffer::USAGE_DYNAMIC_DISCARDABLE_WRITE_ONLY,
		// The texture resource will be used as a render target.
		USAGE_RENDER_TARGET
	};

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

	/** Set the texture type e.g. 1D, 2D, 3D or cubic
	 	@author Hayden Asplet
	 	@param EType textureType
	 	@return void
	*/
	void SetType(EType textureType);
	EType GetType() const;

	void SetUsage(EUsage usage);
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

	int width;
	int height;
	int depth;
};

}

#endif	// __SLIMTEXTURE_H__