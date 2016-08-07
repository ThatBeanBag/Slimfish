#pragma once

#ifndef __SLIMOPENGLTEXTURE_H__
#define __SLIMOPENGLTEXTURE_H__

// Library Includes

// Local Includes
#include "../SlimTexture.h"
#include "SlimOpenGLForward.h"

namespace Slim {

class COpenGLTexture : public ATexture {
	// Member Functions
public:
	COpenGLTexture(const std::string& name, ETextureType textureType, ETextureUsage usage);
	~COpenGLTexture();

	virtual void VLoad() override;
	virtual void VLoadRaw(const void* pData, size_t stride) override;
	virtual void VUnload() override;

	virtual const CImage VGetImage() const override;

protected:
private:
	// Member Variables
public:
protected:
private:
	GLuint m_TextureId;
};

}

#endif	// __SLIMOPENGLTEXTURE_H__


