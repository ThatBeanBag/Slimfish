// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9Texture.cpp
// Description	: CD3D9Texture implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "D3D9Texture.h"

// Local Includes


CD3D9Texture::CD3D9Texture(IDirect3DDevice9* _pD3DDevice, IDirect3DTexture9* _pTexture)
	:m_pDevice(_pD3DDevice),
	m_pTexture(_pTexture),
	m_fX(0),
	m_fY(0)
{

}

CD3D9Texture::~CD3D9Texture()
{
	SafeRelease(m_pTexture);
}

void CD3D9Texture::VSetToRenderer(ETextureStage _eTextureStage)
{
	DEBUG_ASSERT(m_pTexture);
	DEBUG_ASSERT(m_pDevice);

	m_pDevice->SetTexture(_eTextureStage, m_pTexture);

	CMatrix4x4 matTransform = CMatrix4x4::s_kIdentity;
	matTransform._31 = m_fX;
	matTransform._32 = m_fY;

	D3DTRANSFORMSTATETYPE d3dStateType = D3DTS_TEXTURE0;
	switch (_eTextureStage) {
		case TEXSTAGE_0: {
			d3dStateType = D3DTS_TEXTURE0;
			break;
		}
		case TEXSTAGE_1: {
			d3dStateType = D3DTS_TEXTURE1;
			break;
		}
		case TEXSTAGE_2: {
			d3dStateType = D3DTS_TEXTURE2;
			break;
		}
		case TEXSTAGE_3: {
			d3dStateType = D3DTS_TEXTURE3;
			break;
		}
		case TEXSTAGE_4: {
			d3dStateType = D3DTS_TEXTURE4;
			break;
		}
		case TEXSTAGE_5: {
			d3dStateType = D3DTS_TEXTURE5;
			break;
		}
		case TEXSTAGE_6: {
			d3dStateType = D3DTS_TEXTURE6;
			break;
		}
		case TEXSTAGE_7: {
			d3dStateType = D3DTS_TEXTURE7;
			break;
		}
		default:
			break;
	}

	m_pDevice->SetTransform(d3dStateType, &matTransform);

	/*m_pDevice->SetTextureStageState(_eTextureStage, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(_eTextureStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(_eTextureStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pDevice->SetTextureStageState(_eTextureStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->SetTextureStageState(_eTextureStage, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	m_pDevice->SetTextureStageState(_eTextureStage, D3DTSS_TEXCOORDINDEX, 0);*/
	/*m_pDevice->SetTextureStageState(_iTextureStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->SetTextureStageState(_iTextureStage, D3DTSS_ALPHAARG1, D3DTA_CURRENT);*/
	//m_pDevice->SetTextureStageState(_iTextureStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

void CD3D9Texture::VSetTranslation(float _fX, float _fY)
{
	m_fX = _fX;
	m_fY = _fY;
}
