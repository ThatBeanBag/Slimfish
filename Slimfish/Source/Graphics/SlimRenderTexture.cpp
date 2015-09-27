// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRenderTexture.cpp
// Description	: CSlimRenderTexture implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimRenderTexture.h"

// Local Includes
#include "SlimTexture.h"

namespace Slim {

ARenderTexture::ARenderTexture(shared_ptr<ATexture> pTexture) 
	:m_pTexture(pTexture)
{

}

ARenderTexture::~ARenderTexture()
{

}

shared_ptr<ATexture> ARenderTexture::GetTexture()
{
	return m_pTexture;
}

}
