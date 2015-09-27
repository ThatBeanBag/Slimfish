// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11Forward.h
// Description	: CSlimD3D11Forward declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D11FORWARD_H__
#define __SLIMD3D11FORWARD_H__

// Library Includes
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <comdef.h>
#include <DirectXMath.h>
#include <atlbase.h>

// Local Includes
#include "SlimD3D11Conversions.h"

namespace Slim {
	class CD3D11Renderer;
	class CD3D11Texture;
	class CD3D11GpuBuffer;
	class CD3D11VertexGpuBuffer;
	class CD3D11IndexGpuBuffer;
	class CD3D11ShaderProgram;
	class CD3D11RenderTexture;

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

#endif // __SLIMD3D11FORWARD_H__