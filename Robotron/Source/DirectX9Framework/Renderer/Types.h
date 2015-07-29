// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Types.h
// Description	: Renderer types declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __TYPES_H__
#define __TYPES_H__

// Library Includes

// Local Includes

typedef int TStaticBufferID;

extern const TStaticBufferID g_kINVALID_BUFFER_ID;
extern const std::string g_kstrINVALID_TEXTURE;

enum ERenderState {
	RS_ZENABLE,
	RS_FILLMODE,
	RS_SHADEMODE,
	RS_CULLMODE,
	RS_LIGHTING,
	RS_SPECULAR,
	RS_COLOURVERTEX,
	RS_NORMALIZENORMALS,
	RS_DIFFUSEMATERIALSOURCE,
	RS_SPECULARMATERIALSOURCE,
	RS_AMBIENTMATERIALSOURCE,
	RS_MULTISAMPLEANTIALIAS
};

enum ERSZenableType {
	ZB_FALSE,
	ZB_TRUE,	// Z Buffering.
	ZB_USEW,	// W Buffering.
};

enum ERSFillModeType {
	FILL_POINT,
	FILL_WIREFRAME,
	FILL_SOLID
};

enum ERSShadeModeType {
	SHADE_FLAT,
	SHADE_GOURAUD,
	SHADE_PHONG
};

enum ERSCullModeType {
	CULL_NONE,	// No culling.
	CULL_CW,	// Clock wise culling.
	CULL_CCW	// Counter-clock wise culling.
};

enum ERSMaterialColourSourceType {
	MCS_MATERIAL,	// Use material colour.
	MCS_COLOUR1,	// Use first vertex colour.
	MCS_COLOUR2		// Use second vertex colour.
};


enum ELightType {
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_DIRECTIONAL
};

enum EIndexType {
	IT_NONE,
	IT_FORMAT16,
	IT_FORMAT32
};

enum EPrimitiveType {
	PT_POINTLIST,
	PT_LINELIST,
	PT_LINESTRIP,
	PT_TRIANGLELIST,
	PT_TRIANGLESTRIP,
	PT_TRIANGLEFAN
};

enum ELostDeviceError {
	LDERR_OK,
	LDERR_DEVICELOST,
	LDERR_DEVICENOTRESET
};

//enum EFontFormat {
//	FFORMAT_BOTTOM,
//	FFORMAT_CENTER,
//	FFORMAT_LEFT,
//	FFORMAT_RIGHT
//};

struct TRenderState {
	ERSZenableType m_eRSZenable;
	ERSFillModeType m_eRSFillMode;
	ERSShadeModeType m_eRSShadeMode;
	ERSCullModeType m_eRSCullMode;
	bool m_bRSLighting;
	bool m_bRSSpecular;
	bool m_bRSColourVertex;
	bool m_bRSNormalizeNormals;
	ERSMaterialColourSourceType m_eRSDiffuseMaterialSource;
	ERSMaterialColourSourceType m_eRSSpecularMaterialSource;
	ERSMaterialColourSourceType m_eRSAmbientMaterialSource;
	bool m_bRSMultisampleAntialias;
};

struct TLight {
	ELightType m_eType;
	TColourValue m_diffuse;		// Diffuse colour of light.
	TColourValue m_specular;	// Specular colour of light.
	TColourValue m_ambient;		// Ambient colour of light.
	CVec3 m_position;		// Position of light in world space.
	CVec3 m_direction;		// Direction of light in world space.
	float m_fRange;			// Cut off range.
	float m_fFalloff;		
	float m_fAttenuation0;	// Constant attenuation
	float m_fAttenuation1;	// Linear attenuation.
	float m_fAttenuation2;	// Quadratic attenuation.
	float m_fTheta;			// Inner angle of spolight cone.
	float m_fPhi;			// Outer angle of spolight cone.
	bool m_bIsEnabled;		// Is this light enabled.
};

struct TUnlitColouredVertex {
	CVec3 m_vec3Position;
	DWORD dwDiffuse;

	static const DWORD s_kFVF;
};

struct TLitColouredVertex {
	D3DXVECTOR3 m_vec3Position;
	D3DXVECTOR3	m_vec3Normal;
	DWORD m_dwDiffuse;

	static const DWORD s_kFVF;
};

struct TLitTexturedVertex {
	D3DXVECTOR3 m_vec3Position;
	D3DXVECTOR3	m_vec3Normal;
	float m_fU;
	float m_fV;

	static const DWORD s_kFVF;
};

TUnlitColouredVertex CreateVertex(float _fX, float _fY, float _fZ, 
								  TColour _diffuse);
TLitColouredVertex CreateVertex(float _fX, float _fY, float _fZ, 
								float _fnX, float _fnY, float _fnZ, 
								TColour _diffuse);
TLitTexturedVertex CreateVertex(float _fX, float _fY, float _fZ, 
								float _fnX, float _fnY, float _fnZ, 
								float _fU, float _fV);

#endif	// __TYPES_H__