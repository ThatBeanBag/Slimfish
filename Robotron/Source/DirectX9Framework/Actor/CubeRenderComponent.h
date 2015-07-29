// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CubeRenderComponent.h
// Description	: CCubeRenderComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __CUBERENDERCOMPONENT_H__
#define __CUBERENDERCOMPONENT_H__

// Library Includes

// Local Includes
#include "RenderComponent.h"

class CCubeRenderComponent : public ARenderComponent {
	// Member Functions
public:
	CCubeRenderComponent();
	~CCubeRenderComponent();

protected:
private:
	virtual bool VInitialiseDelegate(TiXmlElement* _pXmlData) override;

	virtual shared_ptr<CSceneNode> VCreateSceneNode() override;

	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	
};

#endif // __CUBERENDERCOMPONENT_H__