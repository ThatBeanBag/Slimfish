// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9Renderer.cpp
// Description	: CD3D9Renderer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "D3D9Renderer.h"

// Local Includes
#include "D3D9StaticBuffer.h"
#include "D3D9Mesh.h"
#include "D3D9Texture.h"
#include "D3D9Sprite.h"
#include "D3D9NormalMap.h"

CD3D9Renderer::CD3D9Renderer()
	:m_pD3D(nullptr),
	m_pD3DDevice(nullptr),
	m_pFont(nullptr),
	m_pSprite(nullptr),
	m_backgroundColour(D3DCOLOR_XRGB(0, 0, 0)),
	m_ambientColour(D3DCOLOR_XRGB(0, 0, 0))
{
	// Set the default render states.
	m_renderState.m_eRSZenable = ZB_TRUE;
	m_renderState.m_eRSFillMode = FILL_SOLID;
	m_renderState.m_eRSShadeMode = SHADE_GOURAUD;
	m_renderState.m_eRSCullMode = CULL_CCW;
	m_renderState.m_bRSLighting = TRUE;
	m_renderState.m_bRSSpecular = FALSE;
	m_renderState.m_bRSColourVertex = TRUE;
	m_renderState.m_bRSNormalizeNormals = FALSE;
	m_renderState.m_eRSDiffuseMaterialSource = MCS_COLOUR1;
	m_renderState.m_eRSSpecularMaterialSource = MCS_COLOUR2;
	m_renderState.m_eRSAmbientMaterialSource = MCS_MATERIAL;
	m_renderState.m_bRSMultisampleAntialias = TRUE;
}

CD3D9Renderer::~CD3D9Renderer()
{
	SafeRelease(m_pSprite);
	SafeRelease(m_pFont);
	SafeRelease(m_pD3DDevice);
	SafeRelease(m_pD3D);
}

bool CD3D9Renderer::VInitialize(int _iScreenWidth, int _iScreenHeight, bool _bIsWindowedMode)
{
	m_bIsWindowedMode = _bIsWindowedMode;

	// Create the Direct3D object interface.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_pD3D) {
	// Direct3D object failed to create.
		return false;
	}

	// Get the display mode of the default adapter.
	D3DDISPLAYMODE d3ddm;
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);	// GR: Display Mode

	if (m_bIsWindowedMode == false) {
		_iScreenWidth = d3ddm.Width;
		_iScreenHeight = d3ddm.Height;
	}

	// Zero the presentation parameters structure.
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));

	m_d3dpp.Windowed = m_bIsWindowedMode;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.BackBufferWidth = _iScreenWidth; // CS: Aspect Ratio
	m_d3dpp.BackBufferHeight = _iScreenHeight;
	m_d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// GR: Depth/Stencil Buffer.
	m_d3dpp.EnableAutoDepthStencil = true;
	m_d3dpp.hDeviceWindow = g_pApp->GetHwnd();

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
								    D3DDEVTYPE_HAL,
								    g_pApp->GetHwnd(),
								    D3DCREATE_HARDWARE_VERTEXPROCESSING,
								    &m_d3dpp,
								    &m_pD3DDevice))) {
	// Failed to create the device.
		return false;
	}

	// Create a default font.
	D3DXFONT_DESC fontDesc;
	ZeroMemory(&fontDesc, sizeof(D3DXFONT_DESC));
	fontDesc.Height = 18;
	fontDesc.Width = 0;
	fontDesc.Weight = FW_MEDIUM;
	fontDesc.MipLevels = D3DX_DEFAULT;
	fontDesc.Italic = false;
	fontDesc.Quality = ANTIALIASED_QUALITY;
	wcscpy_s(fontDesc.FaceName, L"Arial");
	D3DXCreateFontIndirect(m_pD3DDevice, &fontDesc, &m_pFont);

	GetWindowRect(g_pApp->GetHwnd(), &m_windowRect);

	// Set the mip sample states.
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	//m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);

	// Set the alpha enabled.
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	HRESULT hResult = D3DXCreateSprite(m_pD3DDevice, &m_pSprite);
	if (FAILED(hResult)) {
		DEBUG_ERROR("Failed to creat sprite");
		return false;
	}

	return true;
}

bool CD3D9Renderer::VPreRender()
{
	if (D3DERR_INVALIDCALL == m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_backgroundColour, 1.0f, 0)) {
		return false;
	}

	if (D3DERR_INVALIDCALL == m_pD3DDevice->BeginScene()) {
		return false;
	}

	return true;
}

bool CD3D9Renderer::VPostRender()
{
	if (D3DERR_INVALIDCALL == m_pD3DDevice->EndScene()) {
		return false;
	}

	if (D3DERR_DEVICEREMOVED == m_pD3DDevice->Present(0, 0, 0, 0)) {
		return false;
	}

	return true;
}

bool CD3D9Renderer::VReset()
{
	m_pFont->OnLostDevice();
	m_pSprite->OnLostDevice();

	if (D3DERR_DEVICELOST == m_pD3DDevice->Reset(&m_d3dpp)) {
		return false;
	}
	else {
		m_pFont->OnResetDevice();
		m_pSprite->OnResetDevice();

		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);

		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		// Restore the render state.
		m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, m_ambientColour);

		// Z-Buffer
		if (m_renderState.m_eRSZenable == ZB_FALSE) {
			m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		}
		else if (m_renderState.m_eRSZenable == ZB_TRUE) {
//			m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		}
		else if (m_renderState.m_eRSZenable == ZB_USEW) {
			m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);
		}

		// Fill Mode
		if (m_renderState.m_eRSFillMode == FILL_POINT) {
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
		}
		else if (m_renderState.m_eRSFillMode == FILL_WIREFRAME) {
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else if (m_renderState.m_eRSFillMode == FILL_SOLID) {
//			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

		// Shade Mode
		if (m_renderState.m_eRSShadeMode == SHADE_FLAT) {
			m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		}
		else if (m_renderState.m_eRSShadeMode == SHADE_GOURAUD) {
//			m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		}
		else if (m_renderState.m_eRSShadeMode == SHADE_PHONG) {
			m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);
		}

		// Cull Mode
		if (m_renderState.m_eRSCullMode == CULL_NONE) {
			m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}
		else if (m_renderState.m_eRSCullMode == CULL_CW) {
			m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		}
		else if (m_renderState.m_eRSCullMode == CULL_CCW) {
//			m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}

		if (m_renderState.m_bRSLighting == false) {
			m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		}

		if (m_renderState.m_bRSLighting == true) {
			m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		}

		if (m_renderState.m_bRSColourVertex == false) {
			m_pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
		}

		if (m_renderState.m_bRSNormalizeNormals == true) {
			m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// Diffuse Material Source
		if (m_renderState.m_eRSDiffuseMaterialSource == MCS_MATERIAL) {
			m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
		}
		else if (m_renderState.m_eRSDiffuseMaterialSource == MCS_COLOUR1) {
//			m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		}
		else if (m_renderState.m_eRSDiffuseMaterialSource == MCS_COLOUR2) {
			m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR2);
		}

		// Specular Material Source
		if (m_renderState.m_eRSSpecularMaterialSource == MCS_MATERIAL) {
			m_pD3DDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
		}
		else if (m_renderState.m_eRSSpecularMaterialSource == MCS_COLOUR1) {
			m_pD3DDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR1);
		}
		else if (m_renderState.m_eRSSpecularMaterialSource == MCS_COLOUR2) {
//			m_pD3DDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
		}

		// Ambient Material Source
		if (m_renderState.m_eRSAmbientMaterialSource == MCS_MATERIAL) {
//			m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
		}
		else if (m_renderState.m_eRSAmbientMaterialSource == MCS_COLOUR1) {
			m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
		}
		else if (m_renderState.m_eRSAmbientMaterialSource == MCS_COLOUR2) {
			m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR2);
		}

		if (m_renderState.m_bRSNormalizeNormals == false) {
			m_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
		}
	}

	return true;
}

bool CD3D9Renderer::VBegin2DDraw()
{
	if (g_pApp->IsAppMinimized()) {
		return false;
	}

	if (FAILED(m_pSprite->Begin(D3DXSPRITE_ALPHABLEND))) {
		//DEBUG_ERROR("Failed to begin 2D draw.");
		return false;
	}

	return true;
}

bool CD3D9Renderer::VEnd2DDraw()
{
	if (g_pApp->IsAppMinimized()) {
		return false;
	}

	if (FAILED(m_pSprite->End())) {
		//DEBUG_ERROR("Failed to end 2D draw.");
		return false;
	}

	return true;
}

shared_ptr<IStaticBuffer> CD3D9Renderer::VCreateStaticBuffer(DWORD _dwFVF,
															 EPrimitiveType _ePrimitiveType,
															 EIndexType _eIndexType,
															 unsigned int _uiNumVertices,
															 unsigned int _uiNumIndices,
															 unsigned int _uiStride,
															 void* _pVertexData,
															 void* _pIndexData)
{
	shared_ptr<CD3D9StaticBuffer> pStaticBuffer(new CD3D9StaticBuffer(m_pD3DDevice));

	if (!pStaticBuffer->VInitialise(_dwFVF, 
								    _ePrimitiveType,
								    _eIndexType,
								    _uiNumVertices, 
								    _uiNumIndices, 
								    _uiStride, 
								    _pVertexData,
								    _pIndexData)) {
		return nullptr;
	}
	else {
		return pStaticBuffer;
	}

	return nullptr;
}

shared_ptr<IMesh> CD3D9Renderer::VCreateMeshFromFile(const std::string& _krstrFileName)
{
	shared_ptr<CD3D9Mesh> pMesh(new CD3D9Mesh(m_pD3DDevice));

	if (!pMesh) {
		DEBUG_ERROR("Failed to create mesh from file\"" + _krstrFileName + "\"");
		return nullptr;
	}

	if (!pMesh->LoadFromFile(_krstrFileName)) {
		DEBUG_ERROR("Failed to create mesh from file\"" + _krstrFileName + "\"");
		return nullptr;
	}

	return pMesh;
}

shared_ptr<ISprite> CD3D9Renderer::VCreateSpriteFromFile(const std::string& _krstrFileName)
{
	IDirect3DTexture9* pTexture = NULL;
	HRESULT hResult = D3DXCreateTextureFromFileExA(m_pD3DDevice,
												  _krstrFileName.c_str(),
												  D3DX_DEFAULT_NONPOW2,
												  D3DX_DEFAULT_NONPOW2,
												  D3DX_DEFAULT,
												  0,
												  D3DFMT_UNKNOWN,
												  D3DPOOL_MANAGED,
												  D3DX_DEFAULT,
												  D3DX_DEFAULT,
												  0,
												  nullptr,
												  nullptr,
												  &pTexture);
	if (FAILED(hResult)) {
		SafeRelease(pTexture);
		DEBUG_ERROR("Failed to create sprite from file\"" + _krstrFileName + "\"");
		return nullptr;
	}

	shared_ptr<ISprite> pSprite(new CD3D9Sprite(m_pSprite, pTexture));
	return pSprite;
}

shared_ptr<ITexture> CD3D9Renderer::VCreateTextureFromFile(const std::string& _krstrFileName)
{
	std::wstring wsFileName;
	wsFileName.append(_krstrFileName.begin(), _krstrFileName.end());

	IDirect3DTexture9* pTexture = NULL;

	HRESULT hResult = D3DXCreateTextureFromFileA(m_pD3DDevice, _krstrFileName.c_str(), &pTexture);

	if (FAILED(hResult)) {
		if (pTexture) {
			pTexture->Release();
		}
		DEBUG_ERROR("Failed to create texture from file\"" + _krstrFileName + "\"");
		return nullptr;
	}

	if (pTexture) {
		return shared_ptr<ITexture>(new CD3D9Texture(m_pD3DDevice, pTexture));
	}
	
	return nullptr;
}

shared_ptr<INormalMap> CD3D9Renderer::VCreateNormalMapFromFile(const std::string& _krstrFileName)
{
	std::wstring wsFileName;
	wsFileName.append(_krstrFileName.begin(), _krstrFileName.end());

	IDirect3DTexture9* pTexture = NULL;

	HRESULT hResult = D3DXCreateTextureFromFileA(m_pD3DDevice, _krstrFileName.c_str(), &pTexture);

	if (FAILED(hResult)) {
		if (pTexture) {
			pTexture->Release();
		}
		DEBUG_ERROR("Failed to create texture from file\"" + _krstrFileName + "\"");
		return nullptr;
	}

	if (pTexture) {
		return shared_ptr<INormalMap>(new CD3D9NormalMap(m_pD3DDevice, pTexture));
	}

	return nullptr;
}

shared_ptr<IStaticBuffer> CD3D9Renderer::VCreateTerrain(const std::string& _krstrFileName, float _fTextureRepeat)
{
	CImage image = VLoadImageFromFile(_krstrFileName);

	int iImageWidth = image.GetWidth();
	int iImageHeight = image.GetHeight();

	if (iImageWidth == 0 || iImageHeight == 0) {
		return nullptr;
	}

	int iNumPixels = iImageWidth * iImageHeight;
	TLitTexturedVertex* pVerts = new TLitTexturedVertex[iNumPixels];

	// Generate vertices 
	for (int iZ = 0; iZ < iImageHeight; ++iZ) {
		for (int iX = 0; iX < iImageWidth; ++iX) {
			TColour colour = image.GetPixel(iX, iZ);
			TLitTexturedVertex vert;

			vert.m_vec3Position.x = (static_cast<float>(iX) / static_cast<float>(iImageWidth)) - 0.5f;
			vert.m_vec3Position.y = (static_cast<float>(colour.m_r) / 255.0f) - 0.5f;
			vert.m_vec3Position.z = (static_cast<float>(iZ) / static_cast<float>(iImageHeight)) - 0.5f;
			vert.m_fU = (static_cast<float>(iX) / static_cast<float>(iImageWidth - 1)) * _fTextureRepeat;
			vert.m_fV = (static_cast<float>(iZ) / static_cast<float>(iImageWidth - 1)) * _fTextureRepeat;

			pVerts[iX + iZ * iImageWidth] = vert;
		}
	}

	// Generate normals
	for (int iZ = 0; iZ < iImageHeight; ++iZ) {
		for (int iX = 0; iX < iImageWidth; ++iX) {
			TLitTexturedVertex currentVert = pVerts[iX + iZ * iImageWidth];
			D3DXVECTOR3 vec3AveragedNormal(0, 0, 0);
			D3DXVECTOR3 vec3LastLine(0, 0, 0);

			// Start at the top right neighbour.
			int iNeighbourX = iX + 1;
			int iNeighbourZ = iZ - 1;

			auto increment = [&](int _i) {
				// Increment to next neighbour, so that we search 
				// neighbour vertices int counter-clockwise order.
				if (_i < 2) {
					--iNeighbourX;
				}
				else if (_i < 4) {
					++iNeighbourZ;
				}
				else if (_i < 6) {
					++iNeighbourX;
				}
				else {
					--iNeighbourZ;
				}
			};

			for (int i = 0; i < 8; ++i) {
				if (iNeighbourX < 0 || iNeighbourZ < 0 ||
					iNeighbourX >= iImageWidth || iNeighbourZ >= iImageHeight) {
					// Is this neighbour outside the image?
					// Don't evaluate it.
					increment(i);

					continue;
				}

				TLitTexturedVertex neighbourVert = pVerts[iNeighbourX + iNeighbourZ * iImageWidth];
				D3DXVECTOR3 vec3CurrentNormal(0, 0, 0);
				D3DXVECTOR3 vec3CurrentLine = currentVert.m_vec3Position - neighbourVert.m_vec3Position;

				D3DXVec3Cross(&vec3CurrentNormal, &vec3LastLine, &vec3CurrentLine);
				vec3AveragedNormal += vec3CurrentNormal;
				vec3LastLine = vec3CurrentLine;

				increment(i);
			}

			D3DXVec3Normalize(&currentVert.m_vec3Normal, &vec3AveragedNormal);
			pVerts[iX + iZ * iImageWidth] = currentVert;
		}
	}

	int iNumIndices = ((iImageWidth * 2) * (iImageHeight - 1) + (iImageHeight - 2));

	// Generate indices
	int* piIndices = new int[iNumIndices];
	int iIndex = 0;

	for (int iZ = 0; iZ < iImageHeight - 1; ++iZ) {
		// Even rows move left to right, odd rows move right to left.
		if (iZ % 2 == 0) {
		// Is this an even row?
			for (int iX = 0; iX < iImageWidth; ++iX) {
				piIndices[iIndex] = iX + iZ * iImageWidth;
				++iIndex;
				piIndices[iIndex] = iX + iZ * iImageWidth + iImageWidth;	// Next row.
				++iIndex;
			}

			// Insert degenerate vertex, if this isn't the last row.
			if (iZ != iImageHeight - 2) {
			// Is this the last row?
				piIndices[iIndex] = iImageWidth - 1 + (iZ * iImageWidth);
				++iIndex;
			}
		}
		else {
		// This is an odd row.
			for (int iX = iImageWidth - 1; iX >= 0; --iX) {
				piIndices[iIndex] = iX + iZ * iImageWidth;
				++iIndex;
				piIndices[iIndex] = iX + iZ * iImageWidth + iImageWidth;	// Next row.
				++iIndex;
			}

			// Insert degenerate vertex, if this isn't the last row.
			if (iZ != iImageHeight - 2) {
				// Is this the last row?
				piIndices[iIndex] = iZ * iImageWidth;
				++iIndex;
			}
		}
	}

	shared_ptr<IStaticBuffer> pStaticBuffer = VCreateStaticBuffer(TLitTexturedVertex::s_kFVF,
																  PT_TRIANGLESTRIP, 
																  IT_FORMAT32, 
																  iNumPixels, iNumIndices, 
																  sizeof(TLitTexturedVertex),
																  pVerts, piIndices);
	// Deallocate temporary resources.
	delete[] piIndices;
	delete[] pVerts;

	return pStaticBuffer;
}

/*
shared_ptr<IStaticBuffer> CD3D9Renderer::VCreateTerrain(const std::string& _krstrFileName, float _fTextureRepeat)
{
	CImage image = VLoadImageFromFile(_krstrFileName);

	int iImageWidth = image.GetWidth();
	int iImageHeight = image.GetHeight();

	if (iImageWidth == 0 || iImageHeight == 0) {
		return nullptr;
	}

	int iNumPolys = 2 * (iImageWidth - 1) * (iImageHeight - 1);
	int iNumVerts = iNumPolys * 3;
	std::unique_ptr<TLitTexturedVertex[]> pVerts(new TLitTexturedVertex[iNumVerts]);

	auto createVert = [&](int iX, int iZ) {
		TColour colour = image.GetPixel(iX, iZ);
		TLitTexturedVertex vert;

		vert.m_vec3Position.x = (static_cast<float>(iX) / static_cast<float>(iImageWidth)) - 0.5f;
		vert.m_vec3Position.y = (static_cast<float>(colour.m_r) / 255.0f) - 0.5f;
		vert.m_vec3Position.z = (static_cast<float>(iZ) / static_cast<float>(iImageHeight)) - 0.5f;
		vert.m_fU = (static_cast<float>(iX) / static_cast<float>(iImageWidth - 1)) * _fTextureRepeat;
		vert.m_fV = (static_cast<float>(iZ) / static_cast<float>(iImageHeight - 1)) * _fTextureRepeat;

		return vert;
	};

	int iIndex = 0;

	auto addPolygon = [&](TLitTexturedVertex& _rVertA, 
						  TLitTexturedVertex& _rVertB, 
						  TLitTexturedVertex& _rVertC) {
		CVec3 vec3LineAB = _rVertB.m_vec3Position - _rVertA.m_vec3Position;
		CVec3 vec3LineAC = _rVertC.m_vec3Position - _rVertA.m_vec3Position;

		// Get the crossproduct of the lines, to define the normal of the polygon.
		CVec3 vec3Normal = CrossProduct(vec3LineAB, vec3LineAC);
		vec3Normal.Normalize();

		// Set the normals of the vertices.
		_rVertA.m_vec3Normal = vec3Normal;
		_rVertB.m_vec3Normal = vec3Normal;
		_rVertC.m_vec3Normal = vec3Normal;

		pVerts[iIndex++] = _rVertA;
		pVerts[iIndex++] = _rVertB;
		pVerts[iIndex++] = _rVertC;
	};

	for (int iZ = 0; iZ < iImageHeight - 1; ++iZ) {
		// Even rows move left to right, odd rows move right to left.
		for (int iX = 0; iX < iImageWidth - 1; ++iX) {
			//				  |\
			// Add a polygon  | \
			//				  |__\
			//
			TLitTexturedVertex vert1 = createVert(iX, iZ);
			TLitTexturedVertex vert2 = createVert(iX + 1, iZ + 1);
			TLitTexturedVertex vert3 = createVert(iX + 1, iZ);
			addPolygon(vert1, vert2, vert3);
			//				  ___
			//				  \  |
			// Add a polygon   \ |
			//				    \|
			TLitTexturedVertex vert4 = createVert(iX, iZ);
			TLitTexturedVertex vert5 = createVert(iX, iZ + 1);
			TLitTexturedVertex vert6 = createVert(iX + 1, iZ + 1);
			addPolygon(vert4, vert5, vert6);
		}

		//if (iZ % 2 == 0) {
		//// Is this an even row?
		//	for (int iX = 0; iX < iImageWidth - 1; ++iX) {
		//		//				  |\
		//		// Add a polygon  | \
		//		//				  |__\
		//		//
		//		TLitTexturedVertex vert1 = createVert(iX, iZ);
		//		TLitTexturedVertex vert2 = createVert(iX + 1, iZ + 1);
		//		TLitTexturedVertex vert3 = createVert(iX + 1, iZ);
		//		addPolygon(vert1, vert2, vert3);
		//		//				  ___
		//		//				  \  |
		//		// Add a polygon   \ |
		//		//				    \|
		//		TLitTexturedVertex vert4 = createVert(iX, iZ);
		//		TLitTexturedVertex vert5 = createVert(iX, iZ + 1);
		//		TLitTexturedVertex vert6 = createVert(iX + 1, iZ + 1);
		//		addPolygon(vert4, vert5, vert6);
		//	}
		//}
		//else {
		//// This is an odd row.
		//	for (int iX = iImageWidth - 1; iX > 0; --iX) {
		//		//					/|
		//		// Add a polygon   / |
		//		//				  /__|
		//		TLitTexturedVertex vert1 = createVert(iX, iZ);
		//		TLitTexturedVertex vert2 = createVert(iX, iZ + 1);
		//		TLitTexturedVertex vert3 = createVert(iX - 1, iZ + 1);
		//		addPolygon(vert1, vert2, vert3);
		//		//				   __
		//		//				  |  /
		//		// Add a polygon  | /
		//		//				  |/
		//		//
		//		TLitTexturedVertex vert4 = createVert(iX, iZ);
		//		TLitTexturedVertex vert5 = createVert(iX - 1, iZ + 1);
		//		TLitTexturedVertex vert6 = createVert(iX - 1, iZ);
		//		addPolygon(vert4, vert5, vert6);
		//	}
		//}
	}

	shared_ptr<IStaticBuffer> pStaticBuffer = VCreateStaticBuffer(TLitTexturedVertex::s_kFVF,
																  PT_TRIANGLELIST,
																  IT_NONE,
																  iNumVerts, 0,
																  sizeof(TLitTexturedVertex),
																  pVerts.get(), nullptr);
	return pStaticBuffer;
}*/

CImage CD3D9Renderer::VLoadImageFromFile(const std::string& _krstrFileName)
{
	std::wstring wsFilePath;

	// Convert _krstrFileName to a wide string.
	wsFilePath.append(_krstrFileName.begin(), _krstrFileName.end());

	D3DXIMAGE_INFO imageInfo;
	ZeroMemory(&imageInfo, sizeof(D3DXIMAGE_INFO));

	HRESULT hResult = D3DXGetImageInfoFromFile(wsFilePath.c_str(), &imageInfo);

	IDirect3DSurface9* pSurface = NULL;
	m_pD3DDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height,
											  D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH,
											  &pSurface, NULL);

	if (imageInfo.ImageFileFormat != D3DXIFF_BMP) {
		return CImage(0, 0);
	}

	hResult = D3DXLoadSurfaceFromFile(pSurface,				// The surface interface.
									  NULL, NULL,			// Destination pallete and rect.
									  wsFilePath.c_str(),	// File path.
									  NULL,					// Source rectangle.
									  D3DX_FILTER_NONE,		// Filter.
									  0,					// Colour key.
									  &imageInfo);			// Image info.

	if (FAILED(hResult)) {
		// Did the surface fail to load?
		// Release the surface and return.
		if (pSurface) {
			pSurface->Release();
		}

		return CImage(0, 0);
	}
	else {
		if (pSurface) {
			CImage image(imageInfo.Width, imageInfo.Height);

			D3DLOCKED_RECT lockRect;
			ZeroMemory(&lockRect, sizeof(D3DLOCKED_RECT));

			pSurface->LockRect(&lockRect, 0, D3DLOCK_READONLY);
			TColour* pPixel = reinterpret_cast<TColour*>(lockRect.pBits);

			int iNumPixels = imageInfo.Width * imageInfo.Height;

			for (unsigned int iY = 0; iY < imageInfo.Height; ++iY) {
				for (unsigned int iX = 0; iX < imageInfo.Width; ++iX) {
					image.SetPixel(iX, iY, pPixel[iX + iY * imageInfo.Width]);
				}
			}

			pSurface->UnlockRect();
			pSurface->Release();

			return std::move(image);
		}
	}

	return CImage(0, 0);
}

void CD3D9Renderer::VSetLight(unsigned int _iIndex, const TLight& _krLight)
{
	if (!_krLight.m_bIsEnabled) {
		m_pD3DDevice->LightEnable(_iIndex, false);
		return;
	}

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));

	if (_krLight.m_eType == LIGHT_POINT) {
		light.Type = D3DLIGHT_POINT;
	}
	else if (_krLight.m_eType == LIGHT_SPOT) {
		light.Type = D3DLIGHT_SPOT;
	}
	else {
		light.Type = D3DLIGHT_DIRECTIONAL;
	}

	// Transfer all the parameters of the struct across to the D3DLIGHT9.
	light.Diffuse = D3DXCOLOR(_krLight.m_diffuse.m_fR, _krLight.m_diffuse.m_fG, _krLight.m_diffuse.m_fB, _krLight.m_diffuse.m_fA);
	light.Specular = D3DXCOLOR(_krLight.m_specular.m_fR, _krLight.m_specular.m_fG, _krLight.m_specular.m_fB, _krLight.m_specular.m_fA);
	light.Ambient = D3DXCOLOR(_krLight.m_ambient.m_fR, _krLight.m_ambient.m_fG, _krLight.m_ambient.m_fB, _krLight.m_ambient.m_fA);
	light.Position = _krLight.m_position;
	light.Direction = _krLight.m_direction;
	light.Range = _krLight.m_fRange;
	light.Falloff = _krLight.m_fFalloff;
	light.Attenuation0 = _krLight.m_fAttenuation0;
	light.Attenuation1 = _krLight.m_fAttenuation1;
	light.Attenuation2 = _krLight.m_fAttenuation2;
	light.Theta = D3DXToRadian(_krLight.m_fTheta);
	light.Phi = D3DXToRadian(_krLight.m_fPhi);

	m_pD3DDevice->SetLight(_iIndex, &light);
	m_pD3DDevice->LightEnable(_iIndex, _krLight.m_bIsEnabled);
}

void CD3D9Renderer::VSetMaterial(const CMaterial& _krMaterial)
{
	TColourValue diffuse = _krMaterial.GetDiffuse();
	TColourValue ambient = _krMaterial.GetAmbient();
	TColourValue specular = _krMaterial.GetSpecular();
	TColourValue emissive = _krMaterial.GetEmissive();

	D3DMATERIAL9 d3dMaterial;
	d3dMaterial.Diffuse = D3DXCOLOR(diffuse.m_fR, diffuse.m_fG, diffuse.m_fB, diffuse.m_fA);
	d3dMaterial.Ambient = D3DXCOLOR(ambient.m_fR, ambient.m_fG, ambient.m_fB, ambient.m_fA);
	d3dMaterial.Specular = D3DXCOLOR(specular.m_fR, specular.m_fG, specular.m_fB, specular.m_fA);
	d3dMaterial.Emissive = D3DXCOLOR(emissive.m_fR, emissive.m_fG, emissive.m_fB, emissive.m_fA);
	d3dMaterial.Power = _krMaterial.GetPower();

	m_pD3DDevice->SetMaterial(&d3dMaterial);
}

void CD3D9Renderer::VDrawText(const char* _kpText, const CRect& _krRect, int _iFontHeight, DWORD flags, TColour _colour)
{
	/*SafeRelease(m_pFont);

	// Create a default font.
	D3DXFONT_DESC fontDesc;
	ZeroMemory(&fontDesc, sizeof(D3DXFONT_DESC));
	fontDesc.Height = _iFontHeight;
	fontDesc.Width = 0;
	fontDesc.Weight = FW_MEDIUM;
	fontDesc.MipLevels = D3DX_DEFAULT;
	fontDesc.Italic = false;
	fontDesc.Quality = ANTIALIASED_QUALITY;
	wcscpy_s(fontDesc.FaceName, L"DINPro");
	D3DXCreateFontIndirect(m_pD3DDevice, &fontDesc, &m_pFont);*/

	RECT rect;

	rect.left = _krRect.GetLeft();
	rect.top = _krRect.GetTop();
	rect.right = _krRect.GetRight();
	rect.bottom = _krRect.GetBottom();

	std::wstring wsTextOut;
	std::string strTextOut(_kpText);
	
	wsTextOut.append(strTextOut.begin(), strTextOut.end());

	D3DCOLOR d3dColour = D3DCOLOR_ARGB(_colour.m_a, _colour.m_r, _colour.m_g, _colour.m_b);

	m_pFont->DrawText(NULL, wsTextOut.c_str(), -1, &rect, flags, d3dColour);
}

void CD3D9Renderer::VDumpBackBufferToDisk(const char* _pcFilename)
{
	IDirect3DSurface9* pBackBufferSurface;
	m_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBufferSurface);

	std::string strFilename(_pcFilename);
	std::wstring wsFilename;

	wsFilename.append(strFilename.begin(), strFilename.end());

	D3DXSaveSurfaceToFile(wsFilename.c_str(), D3DXIFF_JPG, pBackBufferSurface, NULL, 0);

	if (pBackBufferSurface) {
		pBackBufferSurface->Release();
	}
}

ELostDeviceError CD3D9Renderer::VCheckLostDevice()
{
	HRESULT hResult = m_pD3DDevice->TestCooperativeLevel();

	if (FAILED(hResult)) {
		if (hResult == D3DERR_DEVICELOST) {
			return LDERR_DEVICELOST;
		}
		else if(hResult == D3DERR_DEVICENOTRESET) {
			return LDERR_DEVICENOTRESET;
		}
	}
	else {
		return LDERR_OK;
	}

	return LDERR_OK;
}

void CD3D9Renderer::VSetRenderState(ERenderState _eRSType, int _iValue)
{
	switch (_eRSType) {
		case RS_ZENABLE : {
			if (_iValue == m_renderState.m_eRSZenable) {
				break;
			}
			else if (_iValue == ZB_FALSE) {
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			}
			else if (_iValue == ZB_TRUE) {
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			}
			else if (_iValue == ZB_USEW) {
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);
			}
			else {
			// Unrecognised value.
				// Don't let the render state be set within the renderer.
				break;
			}

			// Set the render state within the renderer so that we can use it when reseting the device.
			m_renderState.m_eRSZenable = static_cast<ERSZenableType>(_iValue);

			break;
		}

		case RS_FILLMODE: {
			if (_iValue == m_renderState.m_eRSFillMode) {
				break;
			}
			if (_iValue == FILL_POINT) {
				m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
			}
			else if (_iValue == FILL_WIREFRAME) {
				m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else if (_iValue == FILL_SOLID) {
				m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
			else {
			// Unrecognised value.
				// Don't let the render state be set within the renderer.
				break;
			}

			// Set the render state within the renderer so that we can use it when reseting the device.
			m_renderState.m_eRSFillMode = static_cast<ERSFillModeType>(_iValue);

			break;
		}

		case RS_SHADEMODE: {
			if (_iValue == m_renderState.m_eRSShadeMode) {
				break;
			}
			if (_iValue == SHADE_FLAT) {
				m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
			}
			else if (_iValue == SHADE_GOURAUD) {
				m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
			}
			else if (_iValue == SHADE_PHONG) {
				m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);
			}
			else {
			// Unrecognised value.
				// Don't let the render state be set within the renderer.
				break;
			}

			// Set the render state within the renderer so that we can use it when reseting the device.
			m_renderState.m_eRSShadeMode = static_cast<ERSShadeModeType>(_iValue);

			break;
		}

		case RS_CULLMODE: {
			if (_iValue == m_renderState.m_eRSCullMode) {
				break;
			}
			if (_iValue == CULL_NONE) {
				m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			else if (_iValue == CULL_CW) {
				m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			}
			else if (_iValue == CULL_CCW) {
				m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			else {
			// Unrecognised value.
				// Don't let the render state be set within the renderer.
				break;
			}

			// Set the render state within the renderer so that we can use it when reseting the device.
			m_renderState.m_eRSCullMode = static_cast<ERSCullModeType>(_iValue);

			break;
		}
		case RS_LIGHTING: {
			if (_iValue == static_cast<int>(m_renderState.m_bRSLighting)) {
				break;
			}
			if (_iValue == FALSE) {
				m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				m_renderState.m_bRSLighting = false;
			}
			else {
				m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

				m_renderState.m_bRSLighting = true;
			}

			break;
		}
		case RS_SPECULAR: {
			if (_iValue == static_cast<int>(m_renderState.m_bRSSpecular)) {
				break;
			}
			if (_iValue == FALSE) {
				m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

				m_renderState.m_bRSLighting = false;
			}
			else {
				m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

				m_renderState.m_bRSLighting = true;
			}

			break;
		}
		case RS_COLOURVERTEX: {
			if (_iValue == static_cast<int>(m_renderState.m_bRSColourVertex)) {
				break;
			}
			if (_iValue == FALSE) {
				m_pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);

				m_renderState.m_bRSColourVertex = false;
			}
			else {
				m_pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);

				m_renderState.m_bRSColourVertex = true;
			}

			break;
		}
		case RS_NORMALIZENORMALS: {
			if (_iValue == static_cast<int>(m_renderState.m_bRSNormalizeNormals)) {
				break;
			}
			if (_iValue == FALSE) {
				m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

				m_renderState.m_bRSNormalizeNormals = false;
			}
			else {
				m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

				m_renderState.m_bRSNormalizeNormals = true;
			}

			break;
		}
		case RS_DIFFUSEMATERIALSOURCE: {
			if (_iValue == m_renderState.m_eRSDiffuseMaterialSource) {
				break;
			}
			if (_iValue == MCS_MATERIAL) {
				m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
			}
			else if (_iValue == MCS_COLOUR1) {
				m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
			}
			else if (_iValue == MCS_COLOUR2) {
				m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR2);
			}
			else {
			// Unrecognised value.
				// Don't let the render state be set within the renderer.
				break;
			}

			// Set the render state within the renderer so that we can use it when reseting the device.
			m_renderState.m_eRSDiffuseMaterialSource = static_cast<ERSMaterialColourSourceType>(_iValue);

			break;
		}
		case RS_SPECULARMATERIALSOURCE: {
			if (_iValue == m_renderState.m_eRSSpecularMaterialSource) {
				break;
			}
			if (_iValue == MCS_MATERIAL) {
				m_pD3DDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
			}
			else if (_iValue == MCS_COLOUR1) {
				m_pD3DDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR1);
			}
			else if (_iValue == MCS_COLOUR2) {
				m_pD3DDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
			}
			else {
			// Unrecognised value.
				// Don't let the render state be set within the renderer.
				break;
			}

			// Set the render state within the renderer so that we can use it when reseting the device.
			m_renderState.m_eRSSpecularMaterialSource = static_cast<ERSMaterialColourSourceType>(_iValue);

			break;
		}
		case RS_AMBIENTMATERIALSOURCE: {
			if (_iValue == m_renderState.m_eRSAmbientMaterialSource) {
				break;
			}
			if (_iValue == MCS_MATERIAL) {
				m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
			}
			else if (_iValue == MCS_COLOUR1) {
				m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
			}
			else if (_iValue == MCS_COLOUR2) {
				m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR2);
			}
			else {
			// Unrecognised value.
				// Don't let the render state be set within the renderer.
				break;
			}

			// Set the render state within the renderer so that we can use it when reseting the device.
			m_renderState.m_eRSAmbientMaterialSource = static_cast<ERSMaterialColourSourceType>(_iValue);

			break;
		}
		case RS_MULTISAMPLEANTIALIAS: {
			if (_iValue == static_cast<int>(m_renderState.m_bRSMultisampleAntialias)) {
				break;
			}
			if (_iValue == FALSE) {
				m_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

				m_renderState.m_bRSMultisampleAntialias = false;
			}
			else {
				m_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

				m_renderState.m_bRSMultisampleAntialias = true;
			}

			break;
		}

		default: break;
	}
}

int CD3D9Renderer::VGetRenderState(ERenderState _eRSType)
{
	switch (_eRSType) {
		case RS_ZENABLE: {
			return static_cast<int>(m_renderState.m_eRSZenable);
		}
		case RS_FILLMODE: {
			return static_cast<int>(m_renderState.m_eRSFillMode);
		}
		case RS_SHADEMODE: {
			return static_cast<int>(m_renderState.m_eRSShadeMode);
		}
		case RS_CULLMODE: {
			return static_cast<int>(m_renderState.m_eRSCullMode);
		}
		case RS_LIGHTING: {
			return static_cast<int>(m_renderState.m_bRSLighting);
		}
		case RS_SPECULAR: {
			return static_cast<int>(m_renderState.m_bRSSpecular);
		}
		case RS_COLOURVERTEX: {
			return static_cast<int>(m_renderState.m_bRSColourVertex);
		}
		case RS_NORMALIZENORMALS: {
			return static_cast<int>(m_renderState.m_bRSNormalizeNormals);
		}
		case RS_DIFFUSEMATERIALSOURCE: {
			return static_cast<int>(m_renderState.m_eRSDiffuseMaterialSource);
		}
		case RS_SPECULARMATERIALSOURCE: {
			return static_cast<int>(m_renderState.m_eRSSpecularMaterialSource);
		}
		case RS_AMBIENTMATERIALSOURCE: {
			return static_cast<int>(m_renderState.m_eRSAmbientMaterialSource);
		}
		case RS_MULTISAMPLEANTIALIAS: {
			return static_cast<int>(m_renderState.m_bRSMultisampleAntialias);
		}

		default: break;
	}

	return -1;
}

void CD3D9Renderer::VSetWorldTransform(const CMatrix4x4* _kpmatWorld)
{
	m_pD3DDevice->SetTransform(D3DTS_WORLD, _kpmatWorld);
}

void CD3D9Renderer::VSetViewTransform(const CMatrix4x4* _kpmatView)
{
	m_pD3DDevice->SetTransform(D3DTS_VIEW, _kpmatView);
}

void CD3D9Renderer::VSetProjectionTransform(const CMatrix4x4* _kpmatProjection)
{
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, _kpmatProjection);
}

void CD3D9Renderer::VUpdateScreenSize(int _iScreenWidth, int _iScreenHeight)
{
	if (!m_bIsWindowedMode) {
		return;
	}

	GetWindowRect(g_pApp->GetHwnd(), &m_windowRect);

	// Set the back buffer width and height to the new screen size.
	m_d3dpp.BackBufferWidth = _iScreenWidth;
	m_d3dpp.BackBufferHeight = _iScreenHeight;

	// Reset the device with the new presentation parameters.
	VReset();

	// Restore the application.
	g_pApp->OnRestore();
}

void CD3D9Renderer::VOnScreenMoved()
{
	if (!m_bIsWindowedMode) {
		return;
	}

	GetWindowRect(g_pApp->GetHwnd(), &m_windowRect);
}

float CD3D9Renderer::VGetAspectRatio() const
{
	return static_cast<float>(m_d3dpp.BackBufferWidth) / static_cast<float>(m_d3dpp.BackBufferHeight);
}

int CD3D9Renderer::VGetScreenWidth() const
{
	return m_d3dpp.BackBufferWidth;
}

int CD3D9Renderer::VGetScreenHeight() const
{
	return m_d3dpp.BackBufferHeight;
}

bool CD3D9Renderer::VToggleWindowedMode()
{
	RECT clientRect;

	GetClientRect(g_pApp->GetHwnd(), &clientRect);

	if (m_bIsWindowedMode) {
	// Are we in windowed mode at the moment.

		D3DDISPLAYMODE d3ddm;
		m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		m_iSavedWindowedWidth = clientRect.right - clientRect.left;
		m_iSavedWindowedHeight = clientRect.bottom - clientRect.top;

		// Set the back buffer width and height to the fullscreen size.
		m_d3dpp.BackBufferWidth = d3ddm.Width;
		m_d3dpp.BackBufferHeight = d3ddm.Height;

		m_d3dpp.Windowed = false;

		// Flip the windowed mode boolean.
		m_bIsWindowedMode = false;

		SetWindowLongPtr(g_pApp->GetHwnd(), GWL_STYLE, WS_EX_TOPMOST);

		// Reset the device with the new parameters.
		if (!VReset()) {
			return false;
		}

		// Restore the application.
		g_pApp->OnRestore();
	}
	else {
	// We are in fullscreen mode at the moment.

		// Set the back buffer width and height to the saved windowed size.
		m_d3dpp.BackBufferWidth = m_iSavedWindowedWidth;
		m_d3dpp.BackBufferHeight = m_iSavedWindowedHeight;

		m_d3dpp.Windowed = true;

		// Flip the windowed mode boolean.
		m_bIsWindowedMode = true;

		SetWindowLongPtr(g_pApp->GetHwnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

		SetWindowPos(g_pApp->GetHwnd(), HWND_NOTOPMOST,
					 m_windowRect.left, m_windowRect.top,
					 m_windowRect.right - m_windowRect.left,
					 m_windowRect.bottom - m_windowRect.top,
					 SWP_SHOWWINDOW);

		// Restore the application.
		g_pApp->OnRestore();
	}

	return true;
}

bool CD3D9Renderer::VIsWindowed()
{
	return m_bIsWindowedMode;
}

void CD3D9Renderer::VSetBackgroundColour(TColour _colour)
{
	m_backgroundColour = D3DCOLOR_ARGB(_colour.m_a, _colour.m_r, _colour.m_g, _colour.m_b);
}

void CD3D9Renderer::VSetAmbientColour(TColour _colour)
{
	D3DCOLOR newColour = D3DCOLOR_ARGB(_colour.m_a, _colour.m_r, _colour.m_g, _colour.m_b);
	if (newColour != m_ambientColour) {
		m_ambientColour = newColour;
		m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, m_ambientColour);
	}
}

void CD3D9Renderer::VDrawAABB(const CAABB& _krAABB, const TColour& _krColour)
{
	const CVec3& krvec3Min = _krAABB.GetMin();
	const CVec3& krvec3Max = _krAABB.GetMax();

	TUnlitColouredVertex pVerts[8];
	pVerts[0] = CreateVertex(krvec3Min.x, krvec3Min.y, krvec3Min.z, _krColour);	// Bottom Left.
	pVerts[1] = CreateVertex(krvec3Min.x, krvec3Max.y, krvec3Min.z, _krColour);	// Top Left.
	pVerts[2] = CreateVertex(krvec3Max.x, krvec3Max.y, krvec3Min.z, _krColour);	// Top Right.
	pVerts[3] = CreateVertex(krvec3Max.x, krvec3Min.y, krvec3Min.z, _krColour);	// Bottom Right.

	pVerts[4] = CreateVertex(krvec3Min.x, krvec3Min.y, krvec3Max.z, _krColour);	// Bottom Left.
	pVerts[5] = CreateVertex(krvec3Min.x, krvec3Max.y, krvec3Max.z, _krColour);	// Top Left.
	pVerts[6] = CreateVertex(krvec3Max.x, krvec3Max.y, krvec3Max.z, _krColour);	// Top Right.
	pVerts[7] = CreateVertex(krvec3Max.x, krvec3Min.y, krvec3Max.z, _krColour);	// Bottom Right.

	int piIndices[24] = { 4, 7, 6,
						  5, 5, 4,
						  0, 1, 1,
						  5, 6, 2,
						  2, 1, 0,
						  3, 3, 2,
						  6, 7, 7,
						  3, 0, 4 };
	
	int iOldLighting =	VGetRenderState(RS_LIGHTING);
	int iOldFillMode = VGetRenderState(RS_FILLMODE);

	VSetRenderState(RS_LIGHTING, false);
	VSetRenderState(RS_FILLMODE, FILL_WIREFRAME);

	m_pD3DDevice->SetFVF(TUnlitColouredVertex::s_kFVF);
	m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, piIndices, D3DFMT_INDEX32, pVerts, sizeof(TUnlitColouredVertex));

	VSetRenderState(RS_LIGHTING, iOldLighting);
	VSetRenderState(RS_FILLMODE, iOldFillMode);
}

void CD3D9Renderer::VDrawFrustum(const CFrustum& _krFrustum, const TColour& _krColour)
{
	TUnlitColouredVertex pVerts[8];
	const CVec3* pFrustumVerts = _krFrustum.GetVerts();
	pVerts[0].m_vec3Position = pFrustumVerts[0];
	pVerts[1].m_vec3Position = pFrustumVerts[1];
	pVerts[2].m_vec3Position = pFrustumVerts[2];
	pVerts[3].m_vec3Position = pFrustumVerts[3];

	pVerts[4].m_vec3Position = pFrustumVerts[4];
	pVerts[5].m_vec3Position = pFrustumVerts[5];
	pVerts[6].m_vec3Position = pFrustumVerts[6];
	pVerts[7].m_vec3Position = pFrustumVerts[7];

	for (int i = 0; i < 8; ++i) {
		pVerts[i].dwDiffuse = ColourToDWORD(_krColour);
	}

	int piIndices[24] = { 4, 7, 6,
						  5, 5, 4,
						  0, 1, 1,
						  5, 6, 2,
						  2, 1, 0,
						  3, 3, 2,
						  6, 7, 7,
						  3, 0, 4 };

	int iOldLighting = VGetRenderState(RS_LIGHTING);
	int iOldFillMode = VGetRenderState(RS_FILLMODE);

	VSetRenderState(RS_LIGHTING, false);
	VSetRenderState(RS_FILLMODE, FILL_WIREFRAME);

	m_pD3DDevice->SetFVF(TUnlitColouredVertex::s_kFVF);
	m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, piIndices, D3DFMT_INDEX32, pVerts, sizeof(TUnlitColouredVertex));

	VSetRenderState(RS_LIGHTING, iOldLighting);
	VSetRenderState(RS_FILLMODE, iOldFillMode);
}


void CD3D9Renderer::VDrawSphere(const CVec3& _krvec3WorldPosition, float fRadius, int iSlices, int iStacks, TColour _colour)
{
	ID3DXMesh* pMesh;

	HRESULT hResult = D3DXCreateSphere(m_pD3DDevice, fRadius, iSlices, iStacks, &pMesh, nullptr);

	if (FAILED(hResult) || !pMesh) {
		DEBUG_ERROR("Failed to draw sphere.");
		return;
	}

	D3DMATERIAL9 material;
	material.Diffuse.r = _colour.m_r / 255.0f;
	material.Diffuse.g = _colour.m_g / 255.0f;
	material.Diffuse.b = _colour.m_b / 255.0f;
	material.Diffuse.a = 0.5f;
	material.Ambient = material.Diffuse;
	material.Specular = material.Diffuse;
	material.Emissive = material.Diffuse;

	CMatrix4x4 matWorld(CMatrix4x4::s_kIdentity);
	matWorld.SetPosition(_krvec3WorldPosition);
	VSetWorldTransform(&matWorld);

	int iOldLighting = VGetRenderState(RS_LIGHTING);
	int iOldFillMode = VGetRenderState(RS_FILLMODE);

	VSetRenderState(RS_LIGHTING, false);
	VSetRenderState(RS_FILLMODE, FILL_WIREFRAME);

	m_pD3DDevice->SetMaterial(&material);
	pMesh->DrawSubset(0);

	VSetRenderState(RS_LIGHTING, iOldLighting);
	VSetRenderState(RS_FILLMODE, iOldFillMode);

	SafeRelease(pMesh);
}

void CD3D9Renderer::VDrawLine(const CVec3& _krvec3Start, const CVec3& _krvec3End, TColour _colour)
{
	TUnlitColouredVertex pVerts[2];
	pVerts[0].m_vec3Position = _krvec3Start;
	pVerts[0].dwDiffuse = ColourToDWORD(_colour);
	pVerts[1].m_vec3Position = _krvec3End;
	pVerts[1].dwDiffuse = ColourToDWORD(_colour);

	int iOldLighting = VGetRenderState(RS_LIGHTING);
	int iOldFillMode = VGetRenderState(RS_FILLMODE);

	VSetRenderState(RS_LIGHTING, false);
	VSetRenderState(RS_FILLMODE, FILL_WIREFRAME);

	m_pD3DDevice->SetFVF(TUnlitColouredVertex::s_kFVF);
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, pVerts, sizeof(TUnlitColouredVertex));

	VSetRenderState(RS_LIGHTING, iOldLighting);
	VSetRenderState(RS_FILLMODE, iOldFillMode);
}

void CD3D9Renderer::VDraw3DText(const CVec3& _krvec3WorldPosition, const std::string& _krstrText)
{
	HDC hdcFont = CreateCompatibleDC(nullptr);
	HFONT font = CreateFont(10, 0, 0, 0,					// Height, width, escapement, orientation.
							FW_NORMAL,						// Weight.
							false, false, false,			// Italic, underline, strikeout.
							DEFAULT_CHARSET,				// Charset.
							OUT_DEFAULT_PRECIS,				// Output precision.
							CLIP_DEFAULT_PRECIS,			// Clipping precision.
							DEFAULT_QUALITY,				// Quality.
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch and family.
							L"Arial");						// TrueType font.

	ID3DXMesh* pTextMesh;

	HRESULT hResult = D3DXCreateTextA(m_pD3DDevice, hdcFont, _krstrText.c_str(), 0.001f, 0.4f, &pTextMesh, nullptr, nullptr);

	if (FAILED(hResult)) {
		DEBUG_ERROR("Failed to create 3D text.");
		return;
	}

	// Set the world transform.
	CMatrix4x4 matWorld(CMatrix4x4::s_kIdentity);
	matWorld.SetPosition(_krvec3WorldPosition);
	VSetWorldTransform(&matWorld);

	pTextMesh->DrawSubset(0);

	SafeRelease(pTextMesh);
}

void CD3D9Renderer::VSetTexture(ETextureStage _eTextureStage, shared_ptr<ITexture> _pTexture)
{
	if (_pTexture) {
		_pTexture->VSetToRenderer(_eTextureStage);
	}
	else {
		m_pD3DDevice->SetTexture(_eTextureStage, nullptr);
	}
}