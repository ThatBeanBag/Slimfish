// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: StaticText.h
// Description	: CStaticText declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __STATICTEXT_H__
#define __STATICTEXT_H__

// Library Includes

// Local Includes
#include "GUIElement.h"

class CStaticText : public AGUIElement {
	// Member Functions
public:
	CStaticText(const std::string& _krstrName,
			    const CRect& _krRect,
				const std::string& _krstrText,
				int _iFontHeight,
				TColour _colour);
	virtual ~CStaticText();

	void SetText(std::string& _krstrText);
	void SetColour(TColour _colour);

	const std::string GetText() const;
	const TColour GetColour() const;
protected:
private:
	virtual void VRender() override;
	virtual void VSetAlpha(unsigned char _ucAlpha) override;

	// Member Variables
public:
protected:
private:
	std::string m_strText;
	int m_iFontHeight;
	TColour m_colour;
};

#endif	// __STATICTEXT_H__