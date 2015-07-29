// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: EditBox.h
// Description	: CEditBox declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __EDITBOX_H__
#define __EDITBOX_H__

// Library Includes

// Local Includes
#include "StaticImage.h"

class CEditBox : public CStaticImage {
	// Member Functions
public:
	CEditBox(const CStaticImage& _krStaticImage, const CRect& _krTextBox, int _iMaxCharacters, int _iFontHeight, TColour _colour);
	~CEditBox();

	const std::string GetUserText() const;
protected:
private:
	virtual bool VHandleEvents(const TAppMsg& _krMsg) override;
	virtual void VRender() override;
	virtual void VUpdate(float _fDeltaTime) override;
	virtual void VSetAlpha(unsigned char _ucAlpha) override;

	// Member Variables
public:
	static const float s_kfFLASH_CURSOR_THRESHOLD;
protected:
private:
	CRect m_textBox;
	std::string m_strUserText;
	int m_iMaxCharacters;
	int m_iFontHeight;
	TColour m_colour;

	bool m_bIsCursorShown;
	bool m_bIsActive;
	float m_fFlashCursorTimer;
};

#endif	// __EDITBOX_H__