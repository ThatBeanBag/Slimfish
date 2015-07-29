// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: RenderComponent.h
// Description	: CRenderComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __RENDERCOMPONENT_H__
#define __RENDERCOMPONENT_H__

// Library Includes

// Local Includes
#include "ActorComponent.h"

class ARenderComponent : public AActorComponent {
	// Member Functions
public:
	ARenderComponent();
	virtual ~ARenderComponent();

	virtual bool VInitialise(TiXmlElement* _pXmlData) override;
	virtual void VPostInitialise() final override;
protected:
private:
	// These methods should and must be overridden by derived classes.
	virtual bool VInitialiseDelegate(TiXmlElement* _pXmlData) = 0;
	virtual shared_ptr<CSceneNode> VCreateSceneNode() = 0;

	// Member Variables
public:
protected:
private:
	shared_ptr<CSceneNode> m_pSceneNode;
	CMaterial m_material;
};

#endif	// __RENDERCOMPONENT_H__