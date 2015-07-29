// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: StaticImage.h
// Description	: CStaticImage declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __STATICIMAGE_H__
#define __STATICIMAGE_H__

// Library Includes

// Local Includes
#include "GUIElement.h"

class CStaticImage : public AGUIElement {
	// Member Functions
public:
	CStaticImage(const std::string& _krstrName,
				 const CRect& _krRect, 
				 shared_ptr<ISprite> _pSprite);
	virtual ~CStaticImage();

	void SetSourceRect(const CRect& _krRect);
	void SetImage(shared_ptr<ISprite> _pSprite);
	shared_ptr<ISprite> GetImage();
protected:
	void RenderImage(const CRect& _krRect); // Just for CButton.
private:
	virtual void VRender() override;

	// Member Variables
public:
protected:
private:
	shared_ptr<ISprite> m_pSprite;
	CRect m_sourceRect;
};

#endif	// __STATICIMAGE_H__