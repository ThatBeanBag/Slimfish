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
#include "SlimGpuBuffer.h"
#include "SlimImage.h"

namespace Slim {

/** List of usage flags that describe how a texture is used.
	@note
		This enumeration is weakly typed, as in it can be implicitly
		converted to and from an int this gives it the functionality to be
		used as a flag, but the downside is that it lacks type safety.
*/
enum class ETextureUsage : int {
	// See CGPUBuffer::USAGE_STATIC.
	STATIC,
	// See CGPUBuffer::USAGE_DYNAMIC.
	DYNAMIC,
	// See CGPUBuffer::USAGE_WRITE_ONLY.
	WRITE_ONLY,
	// See CGpuBuffer::USAGE_READ_ONLY.
	READ_ONLY,
	// See CGPUBuffer::USAGE_DISCARDABLE.
	DISCARDABLE,
	// The texture resource will be used as a render target.
	RENDER_TARGET = DISCARDABLE * 2
};

/** List of standard texture types.
*/
enum class ETextureType {
	// 1 dimensional texture.
	TYPE_1D,
	// 2 dimensional texture (most commonly used for standard textures).
	TYPE_2D,
	// 3 dimensional textures. Basically an array of 2D textures.
	TYPE_3D,
	// Cube mapping textures.
	TYPE_CUBIC
};

enum class ETexturePixelFormat {
	UNKNOWN,

	// Four channels of 32 bits each as floating-point data.
	FLOAT_32_RGBA,
	// Four channels of 32 bits each as typeless data.
	TYPELESS_32_RGBA,
	// Four channels of 32 bits each as unsigned integral data.
	UINT_32_RGBA,
	// Four channels of 32 bits each as signed integral data.
	INT_32_RGBA,

	// Three channels of 32 bits each as floating-point data.
	FLOAT_32_RGB,
	// Three channels of 32 bits each as typeless data.
	TYPELESS_32_RGB,
	// Three channels of 32 bits each as unsigned integral data.
	UINT_32_RGB,
	// Three channels of 32 bits each as signed integral data.
	INT_32_RGB,

	// Two channels of 32 bits each as floating-point data.
	FLOAT_32_RG,
	// Two channels of 32 bits each as typeless data.
	TYPELESS_32_RG,
	// Two channels of 32 bits each as unsigned integral data.
	UINT_32_RG,
	// Two channels of 32 bits each as signed integral data.
	INT_32_RG,

	// One channel of 32 bits as floating-point data.
	FLOAT_32_R,
	// One channel of 32 bits as typeless data.
	TYPELESS_32_R,
	// One channel of 32 bits as unsigned integral data.
	UINT_32_R,
	// One channel of 32 bits as signed integral data.
	INT_32_R,

	// Four channels of 16 bits each as floating-point data.
	FLOAT_16_RGBA, 
	// Four channels of 16 bits each as typeless data.
	TYPELESS_16_RGBA, 
	// Four channels of 16 bits each as unsigned integral data.
	UINT_16_RGBA, 
	// Four channels of 16 bits each as signed integral data.
	INT_16_RGBA,
	// Four channels of 16 bits each as unsigned normalised data.
	UNORM_16_RGBA,
	// Four channels of 16 bits each as signed normalised data.
	NORM_16_RGBA,

	// Two channels of 16 bits each as floating-point data.
	FLOAT_16_RG,
	// Two channels of 16 bits each as typeless data.
	TYPELESS_16_RG,
	// Two channels of 16 bits each as unsigned integral data.
	UINT_16_RG,
	// Two channels of 16 bits each as signed integral data.
	INT_16_RG,
	// Two channels of 16 bits each as unsigned normalised data.
	UNORM_16_RG,
	// Two channels of 16 bits each as signed normalised data.
	NORM_16_RG,

	// One channel of 16 bits as floating-point data.
	FLOAT_16_R,
	// One channel of 16 bits as typeless data.
	TYPELESS_16_R,
	// One channel of 16 bits as unsigned integral data.
	UINT_16_R,
	// One channel of 16 bits as signed integral data.
	INT_16_R,
	// One channel of 16 bits as unsigned normalised data.
	UNORM_16_R,
	// One channel of 16 bits as signed normalised data.
	NORM_16_R,

	// Four channels of 8 bits each as typeless data.
	TYPELESS_8_RGBA,
	// Four channels of 8 bits each as unsigned integral data.
	UINT_8_RGBA,
	// Four channels of 8 bits each as signed integral data.
	INT_8_RGBA,
	// Four channels of 8 bits each as unsigned normalised data.
	UNORM_8_RGBA,
	// Four channels of 8 bits each as signed normalised data.
	NORM_8_RGBA,

	// Two channels of 8 bits each as typeless data.
	TYPELESS_8_RG,
	// Two channels of 8 bits each as unsigned integral data.
	UINT_8_RG,
	// Two channels of 8 bits each as signed integral data.
	INT_8_RG,
	// Two channels of 8 bits each as unsigned normalised data.
	UNORM_8_RG,
	// Two channels of 8 bits each as signed normalised data.
	NORM_8_RG,

	// One channel of 8 bits as typeless data.
	TYPELESS_8_R,
	// One channel of 8 bits as unsigned integral data.
	UINT_8_R,
	// One channel of 8 bits as signed integral data.
	INT_8_R,
	// One channel of 8 bits as unsigned normalised data.
	UNORM_8_R,
	// One channel of 8 bits as signed normalised data.
	NORM_8_R,
};

/** Abstract class representing a texture resource.
@remarks
	A texture is a rendering resource that can be used by shaders to provide definition
	or detail to an object.
@par
	A texture is not strictly this in fact ultimately it is just an array of elements. 
	How these elements are interpreted is up to the shader. Common uses of textures 
	involve diffuse mapping, normal mapping, specular mapping etc.
@par
	Textures can also be used as render targets. Render targets allow the use of 
	post-processing effects. Where you can render entire scenes to the texture and then
	perform any effect you like on the texture. Render targets are also used frequently
	in several advanced rendering techniques.
*/
class ATexture {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	 	@param 
			name Name of the resource, will be use to load the texture from file and
			to identify the texture in code.
	 	@param 
			textureType Type of the texture e.g. TYPE_1D, TYPE_2D, 
			TYPE_3D and TYPE_CUBIC.
	 	@param 
			usage Describes how the texture is intended to be used e.g. USAGE_STATIC, 
			USAGE_DYNAMIC, USAGE_WRITE_ONLY, USAGE_DISCARDBLE and USAGE_RENDER_TARGET.
	*/
	ATexture(const std::string& name, ETextureType textureType = ETextureType::TYPE_2D, ETextureUsage usage = ETextureUsage::STATIC);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	virtual ~ATexture();

	/** Load the texture resource from file (NOTE: ultimately this should be loaded from memory).
		@remarks
			Make sure all texture configurations are set correctly prior to loading e.g
			the texture type and usage flags. Changing these configurations does nothing
			after the texture has been loaded. If you want to change these you must first
			unload the texture then load it again.
		@par
			This function is mainly for internal use, factory methods should load the texture
			prior to returning it to the user. Loading an already loaded resource is 
	 	@author Hayden Asplet
	*/
	virtual void VLoad() = 0;

	virtual void VLoadRaw() = 0;

	/** Unload the texture.
	 	@author Hayden Asplet
	*/
	virtual void VUnload() = 0;

	/** Get a readable/writable image from the texture resource.
		@remarks
			Only applicable for 2D textures (TEXTURE_TYPE_2D).
	 	@author Hayden Asplet
	*/
	virtual const CImage VGetImage() const = 0;

	/** Get the name of the texture. @author Hayden Asplet */
	const std::string& GetName() const;

	/** Set the texture type e.g. 1D, 2D, 3D or cubic
		@note only useful prior to loading.
	 	@author Hayden Asplet
	*/
	void SetType(ETextureType textureType);

	/** Get the texture type e.g. 1D, 2D, 3D or cubic.
	 	@author Hayden Asplet
	*/
	ETextureType GetTextureType() const;

	/** Set how the texture is intended to be used.
		@note only useful prior to loading.
	 	@author Hayden Asplet
	*/
	void SetUsage(ETextureUsage usage);

	/** Get how the texture is intended to be used.
	 	@author Hayden Asplet
	*/
	const ETextureUsage GetUsage() const;

	/** Set the pixel format. 
		@note only useful prior to loading.
		@author Hayden Asplet 
	*/
	void SetPixelFormat(ETexturePixelFormat format);

	/** Get the pixel format. @author Hayden Asplet */
	const ETexturePixelFormat GetPixelFormat() const;

	/** Return true if the texture resource has been loaded successfully.
	 	@author Hayden Asplet
	*/
	const bool IsLoaded() const;


	/** Get the width of the texture. @author Hayden Asplet */
	const size_t GetWidth() const;

	/** Get the height of the texture.
		@note Not applicable with 1D textures; use GetWidth() instead.
		@author Hayden Asplet
	*/
	const size_t GetHeight() const;

	/** Get the depth of the 3D texture. 
		@note Only valid with 3D textures, aka texture type is TYPE_3D.
		@author Hayden Asplet 
	*/
	const size_t GetDepth() const;

	/** Set the width of the texture.
		@note 
			Only applicable prior to loading and when texture is being used as a 
			render target (usage is RENDER_TARGET).
		@author Hayden Asplet 
	*/
	void SetWidth(size_t width);

	/** Set the height of the texture.
		@note
			Only applicable prior to loading and when texture is being used as a
			render target (usage is RENDER_TARGET).
		@author Hayden Asplet 
	*/
	void SetHeight(size_t height);

	/** Set the depth of the 3D texture.
		@note
			Only applicable prior to loading and when texture is being used as a
			render target (usage is RENDER_TARGET).
		@note
			Only valid with 3D textures, aka texture type is TYPE_3D.
		@author Hayden Asplet
	*/
	void SetDepth(size_t depth);

	/** Set the number of multi-sample levels to use when texture is being used as a render target.
		@remarks 
			This should only be set by the renderer unless you want explicit control over the
			MSAA levels.
		@note 
			Only applicable prior to loading and to textures that are being used as render targets.
	 	@author Hayden Asplet
	*/
	void SetMultiSampleCount(size_t msaaCount);

	/** Set the quality of multi-sample levels to use when texture is being used as a render target.
		@remarks
			This should only be set by the renderer unless you want explicit control over the
			MSAA levels.
		@note
			Only applicable prior to loading and to textures that are being used as render targets.
		@author Hayden Asplet
	*/
	void SetMultiSampleQuality(size_t msaaQuality);

	/** Get the number of multi-sample levels used by the render target texture.
		@note Only applicable to textures that are being used as render targets.
	 	@author Hayden Asplet
	*/
	const size_t GetMultiSampleCount() const;

	/** Get the quality of multi-sample levels used by the render target texture.
		@note Only applicable to textures that are being used as render targets.
		@author Hayden Asplet
	*/
	const size_t GetMultiSampleQuality() const;
protected:
	/** Set the source width of the texture.
		@note This should only be set internally by derived classes once the texture has been loaded.
		@author Hayden Asplet 
	*/
	void SetSourceWidth(size_t srcWidth);

	/** Get the source width of the texture. @author Hayden Asplet */
	size_t GetSourceWidth() const;

	/** Set the source height of the texture.
		@note This should only be set internally by derived classes once the texture has been loaded.
		@author Hayden Asplet
	*/
	void SetSourceHeight(size_t srcHeight);

	/** Get the source height of the texture. @author Hayden Asplet */
	size_t GetSourceHeight() const;

	/** Set the source depth of the texture.
		@note This should only be set internally by derived classes once the texture has been loaded.
		@author Hayden Asplet
	*/
	void SetSourceDepth(size_t srcDepth);

	/** Get the source depth of the texture. @author Hayden Asplet */
	size_t GetSourceDepth() const;

private:
	// Member Variables
public:
protected:
private:
	std::string m_Name;				// Name of the texture, also specifies the filename of the file to load.
	ETextureType m_TextureType;		// The type of texture e.g. TYPE_1D, TYPE_2D etc.
	ETextureUsage m_Usage;			// Describes how the texture is intended to be used.
	ETexturePixelFormat m_Format;		// Describes the format of each pixel.

	bool m_IsLoaded; // True if the texture resource has been loaded successfully.

	size_t m_Width;	// Width of the texture.
	size_t m_Height;// Height of the texture.
	size_t m_Depth;	// Depth of the texture (only used for 3D textures).

	size_t m_SourceWidth;	// Source width of the texture.
	size_t m_SourceHeight;	// Source height of the texture.
	size_t m_SourceDepth;	// Source depth of the texture (only used for 3D textures).

	size_t m_MSAACount;		// Number of multi-sample levels when using render targets.
	size_t m_MSAAQuality;	// Quality of multi-sample levels when using render targets.
};

}

#endif	// __SLIMTEXTURE_H__