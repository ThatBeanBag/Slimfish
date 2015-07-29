// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: StaticText.cpp
// Description	: CStaticText implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "StaticText.h"

// Local Includes

CStaticText::CStaticText(const std::string& _krstrName, const CRect& _krRect, const std::string& _krstrText, int _iFontHeight, TColour _colour) 
	:AGUIElement(_krstrName, _krRect),
	m_strText(_krstrText),
	m_iFontHeight(_iFontHeight),
	m_colour(_colour)
{

}

CStaticText::~CStaticText()
{

}

void CStaticText::SetText(std::string& _krstrText)
{
	m_strText = _krstrText;
}

void CStaticText::SetColour(TColour _colour)
{
	m_colour = _colour;
}

const std::string CStaticText::GetText() const
{
	return m_strText;
}

const TColour CStaticText::GetColour() const
{
	return m_colour;
}

void CStaticText::VRender()
{
	// Collapse the currently drawn images so that we can draw on top of them.
	/*g_pApp->GetRenderer()->VEnd2DDraw();
	g_pApp->GetRenderer()->VBegin2DDraw();*/

	CRect worldTextBox = GetWorldBoundingBox();

	g_pApp->GetRenderer()->VDrawText(m_strText.c_str(), worldTextBox, m_iFontHeight, DT_SINGLELINE | DT_VCENTER, m_colour);
}

void CStaticText::VSetAlpha(unsigned char _ucAlpha)
{
	m_colour.m_a = _ucAlpha;
}
