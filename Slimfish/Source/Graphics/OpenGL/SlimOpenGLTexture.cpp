
// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimOpenGLTexture.h"

// Local Includes

namespace Slim {

	COpenGLTexture::COpenGLTexture(const std::string& name, ETextureType textureType, ETextureUsage usage)
		:ATexture(name, textureType, usage)
	{

	}

	void COpenGLTexture::VLoad()
	{
		// Generate the texture.
		glGenTextures(1, &m_TextureId);


	}

	void COpenGLTexture::VLoadRaw(const void* pData, size_t stride)
	{
		
	}

	void COpenGLTexture::VUnload()
	{
		
	}

	const Slim::CImage COpenGLTexture::VGetImage() const
	{
		return CImage(0, 0);
	}

}

