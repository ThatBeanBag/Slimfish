
// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimOpenGLRenderer.h"

// Local Includes

namespace Slim {

	COpenGLRenderer::COpenGLRenderer(int width, int height, bool isWindowed) 
		:ARenderer(width, height, isWindowed)
	{

	}

	bool COpenGLRenderer::VInitialize()
	{
		return true;
	}

	void COpenGLRenderer::VPreRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void COpenGLRenderer::VPostRender()
	{
		glEnd();
		glFlush();
	}

}

