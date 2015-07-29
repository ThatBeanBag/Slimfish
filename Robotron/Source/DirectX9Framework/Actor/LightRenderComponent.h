// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: LightRenderComponent.h
// Description	: CLightRenderComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __LIGHTRENDERCOMPONENT_H__
#define __LIGHTRENDERCOMPONENT_H__

// Library Includes

// Local Includes
#include "RenderComponent.h"

class CLightRenderComponent : public ARenderComponent {
	// Member Functions
public:
	CLightRenderComponent();
	virtual ~CLightRenderComponent();

protected:
private:
	virtual bool VInitialiseDelegate(TiXmlElement* _pXmlData);
	virtual shared_ptr<CSceneNode> VCreateSceneNode();
	TColourValue GetColour(TiXmlElement* _pColourElement);

	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	TLight m_light;
};

#endif	// __LIGHTRENDERCOMPONENT_H__