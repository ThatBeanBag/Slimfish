// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MeshRenderComponent.h
// Description	: CMeshRenderComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MESHRENDERCOMPONENT_H__
#define __MESHRENDERCOMPONENT_H__

// Library Includes

// Local Includes
#include "RenderComponent.h"

class CMeshRenderComponent : public ARenderComponent {
	// Member Functions
public:
	CMeshRenderComponent();
	virtual ~CMeshRenderComponent();

protected:
private:
	virtual bool VInitialiseDelegate(TiXmlElement* _pXmlData) override;
	virtual shared_ptr<CSceneNode> VCreateSceneNode() override;

	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	std::string m_strMeshResource;
};

#endif	// __MESHRENDERCOMPONENT_H__