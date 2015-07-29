// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9Renderer.h
// Description	: CD3D9Renderer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __D3D9RENDERER_H__
#define __D3D9RENDERER_H__

// Library Includes

// Local Includes
#include "Renderer.h"

// Forward Declaration
class IMesh;
class ISprite;
class IStaticBuffer;
class INormalMap;
class CD3D9StaticBuffer;
class CD3D9Texture;

class CD3D9Renderer : public IRenderer {
	typedef std::map<TStaticBufferID, CD3D9StaticBuffer> TStaticBufferMap;

	// Member Functions
public:
	CD3D9Renderer();

	virtual ~CD3D9Renderer();

	virtual bool VInitialize(int _iScreenWidth, int _iScreenHeight, bool _bIsWindowedMode);
	virtual bool VPreRender();
	virtual bool VPostRender();
	virtual bool VReset();

	virtual bool VBegin2DDraw();
	virtual bool VEnd2DDraw();

	virtual shared_ptr<IStaticBuffer> VCreateStaticBuffer(DWORD _dwFVF,
														  EPrimitiveType _ePrimitiveType,
														  EIndexType _eIndexType,
														  unsigned int _uiNumVertices,
														  unsigned int _uiNumIndices,
														  unsigned int _uiStride,
														  void* _pVertexData,
														  void* _pIndexData);
	virtual shared_ptr<IMesh> VCreateMeshFromFile(const std::string& _krstrFileName);
	virtual shared_ptr<ISprite> VCreateSpriteFromFile(const std::string& _krstrFileName);
	virtual shared_ptr<ITexture> VCreateTextureFromFile(const std::string& _krstrFileName);
	virtual shared_ptr<INormalMap> VCreateNormalMapFromFile(const std::string& _krstrFileName);
	virtual shared_ptr<IStaticBuffer> VCreateTerrain(const std::string& _krstrFileName, float _fTextureRepeat = 1.0f);

	virtual CImage VLoadImageFromFile(const std::string& _krstrFileName);

	virtual void VSetLight(unsigned int _iIndex, const TLight& _krLight);
	virtual void VSetMaterial(const CMaterial& _krMaterial);
	virtual void VSetTexture(ETextureStage _eTextureStage, shared_ptr<ITexture> _pTexture) override;

	virtual void VDumpBackBufferToDisk(const char* _pcFilename);

	virtual ELostDeviceError VCheckLostDevice();

	// Render States
	virtual void VSetRenderState(ERenderState _eRSType, int _iValue);
	virtual int VGetRenderState(ERenderState _eRSType);

	virtual void VSetWorldTransform(const CMatrix4x4* _kpmatWorld);
	virtual void VSetViewTransform(const CMatrix4x4* _kpmatView);
	virtual void VSetProjectionTransform(const CMatrix4x4* _kpmatProjection);
	virtual void VSetBackgroundColour(TColour _colour);
	virtual void VSetAmbientColour(TColour _colour);
	
	// Application Functions
	virtual void VOnScreenMoved();
	virtual void VUpdateScreenSize(int _iScreenWidth, int _iScreenHeight);
	virtual float VGetAspectRatio() const;
	virtual int VGetScreenWidth() const;
	virtual int VGetScreenHeight() const;
	virtual bool VToggleWindowedMode();
	virtual bool VIsWindowed();

	// Helper Functions for Debug Drawing
	virtual void VDrawText(const char* _kpText, const CRect& _krRect, int _iFontHeight, DWORD flags, TColour _colour);
	virtual void VDrawAABB(const CAABB& _krAABB, const TColour& _krColour);
	virtual void VDrawFrustum(const CFrustum& _krFrustum, const TColour& _krColour);
	virtual void VDrawSphere(const CVec3& _krvec3WorldPosition, float fRadius, int iSlices, int iStacks, TColour _colour);
	virtual void VDrawLine(const CVec3& _krvec3Start, const CVec3& _krvec3End, TColour _colour);
	virtual void VDraw3DText(const CVec3& _krvec3WorldPosition, const std::string& _krstrText);


protected:
private:

	// Member Variables
public:
protected:
private:
	// DirectX9 Objects
	IDirect3D9* m_pD3D;
	IDirect3DDevice9* m_pD3DDevice;
	D3DPRESENT_PARAMETERS m_d3dpp;
	D3DCOLOR m_backgroundColour;
	D3DCOLOR m_ambientColour;
	ID3DXFont* m_pFont;
	ID3DXSprite* m_pSprite;	// The single sprite that all sprites will draw to.

	TRenderState m_renderState;
	
	// Application Variables
	RECT m_windowRect;
	int m_iSavedWindowedWidth;
	int m_iSavedWindowedHeight;
	bool m_bIsWindowedMode;
};

#endif	// __D3D9RENDERER_H__