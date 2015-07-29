// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Renderer.h
// Description	: IRenderer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

// Library Includes

// Local Includes
#include "../Utilities/Rect.h"
#include "../3DGraphics/Material.h"
#include "Types.h"
#include "Image.h"
#include "StaticBuffer.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Texture.h"
#include "NormalMap.h"

class IRenderer {
	// Member Functions
public:
	virtual ~IRenderer() {};

	virtual bool VInitialize(int _iScreenWidth, int _iScreenHeight, bool _bIsWindowedMode) = 0;
	virtual bool VPreRender() = 0;
	virtual bool VPostRender() = 0;
	virtual bool VReset() = 0;

	virtual bool VBegin2DDraw() = 0;
	virtual bool VEnd2DDraw() = 0;

	virtual shared_ptr<IStaticBuffer> VCreateStaticBuffer(DWORD _dwFVF,
														  EPrimitiveType _ePrimitiveType,
														  EIndexType _eIndexType,
														  unsigned int _uiNumVertices,
														  unsigned int _uiNumIndices,
														  unsigned int _uiStride,
														  void* _pVertexData,
														  void* _pIndexData) = 0;
	virtual shared_ptr<IMesh> VCreateMeshFromFile(const std::string& _krstrFileName) = 0;
	virtual shared_ptr<ISprite> VCreateSpriteFromFile(const std::string& _krstrFileName) = 0;
	virtual shared_ptr<ITexture> VCreateTextureFromFile(const std::string& _krstrFileName) = 0;
	virtual shared_ptr<INormalMap> VCreateNormalMapFromFile(const std::string& _krstrFileName) = 0;
	virtual shared_ptr<IStaticBuffer> VCreateTerrain(const std::string& _krstrFileName, float _fTextureRepeat = 1.0f) = 0;

	virtual void VSetTexture(ETextureStage eTextureStage, shared_ptr<ITexture> _pTexture) = 0;

	virtual CImage VLoadImageFromFile(const std::string& _krstrFileName) = 0;

	virtual void VSetLight(unsigned int _iIndex, const TLight& _krLight) = 0;
	virtual void VSetMaterial(const CMaterial& _krMaterial) = 0;

	virtual ELostDeviceError VCheckLostDevice() = 0;

	virtual void VDumpBackBufferToDisk(const char* _pcFilename) = 0;

	virtual void VSetRenderState(ERenderState _eRSType, int _iValue) = 0;
	virtual int VGetRenderState(ERenderState _eRSType) = 0;

	virtual void VSetWorldTransform(const CMatrix4x4* _kpmatWorld) = 0;
	virtual void VSetViewTransform(const CMatrix4x4* _kpmatView) = 0;
	virtual void VSetProjectionTransform(const CMatrix4x4* _kpmatProjection) = 0;
	
	virtual void VOnScreenMoved() = 0;
	virtual void VUpdateScreenSize(int _iScreenWidth, int _iScreenHeight) = 0;
	virtual float VGetAspectRatio() const = 0;
	virtual int VGetScreenWidth() const = 0;
	virtual int VGetScreenHeight() const = 0;

	virtual bool VToggleWindowedMode() = 0;
	virtual bool VIsWindowed() = 0;

	virtual void VSetBackgroundColour(TColour _colour) = 0;
	virtual void VSetAmbientColour(TColour _colour) = 0;

	virtual void VDrawText(const char* _kpText, const CRect& _krRect, int _iFontHeight, DWORD flags, TColour _colour) = 0;
	virtual void VDrawAABB(const CAABB& _krAABB, const TColour& _krColour) = 0;
	virtual void VDrawFrustum(const CFrustum& _krFrustum, const TColour& _krColour) = 0;
	virtual void VDrawSphere(const CVec3& _krvec3Position, float fRadius, int iSlices, int iStacks, TColour _colour) = 0;
	virtual void VDrawLine(const CVec3& _krvec3Start, const CVec3& _krvec3End, TColour _colour) = 0;
	virtual void VDraw3DText(const CVec3& _krvec3WorldPosition, const std::string& _krstrText) = 0;
protected:
private:
	// Member Variables
public:
protected:
private:
};

#endif // __RENDERER_H__