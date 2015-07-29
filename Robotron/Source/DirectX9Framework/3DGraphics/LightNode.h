// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: LightNode.h
// Description	: CLighNode implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __LIGHTNODE_H__
#define __LIGHTNODE_H__

// Library Includes

// Local Includes
#include "SceneNode.h"

class CLightNode : public CSceneNode {
	// Member Functions
public:
	explicit CLightNode(TActorID _actorID);
	CLightNode(TActorID _actorID, const TLight& _krLightProperties);

	virtual ~CLightNode();

	virtual void VOnUpdate(float _fDeltaTime);
	virtual bool VIsVisible(CScene* _pScene) const { return true;  } // TODO: Remove this.

	void SetProperties(const TLight& _krLightProperties);
	TLight GetProperties();
protected:
private:

	// Member Variables
public:
protected:
	TLight m_lightProperties;
private:
};


#endif	// __MESHOBJECT_H__