// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10Forward.h
// Description	: CSlimD3D10Forward declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10FORWARD_H__
#define __SLIMD3D10FORWARD_H__

// Library Includes
#include <d3d10.h>
#include <dxgi.h>
#include <d3dx10.h>
#include <d3dcompiler.h>
#include <comdef.h>

// Local Includes
#include "SlimD3D10Conversions.h"

namespace Slim {
	class CD3D10Renderer;
	class CD3D10Texture;
	class CD3D10GpuBuffer;
	class CD3D10VertexGpuBuffer;
	class CD3D10IndexGpuBuffer;
	class CD3D10ShaderProgram;
	class CD3D10RenderTexture;

	inline std::string GetErrorMessage(HRESULT hResult)
	{
		_com_error err(hResult);

		// Get the error message.
		std::wstring wsErrorMessage = err.ErrorMessage();

		// Convert to string.
		std::string errorMessage;
		errorMessage.append(wsErrorMessage.begin(), wsErrorMessage.end());

		return errorMessage;
	}
}

#endif // __SLIMD3D10FORWARD_H__