// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: PlaneRenderComponent.h
// Description	: CPlaneRenderComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __PLANERENDERCOMPONENT_H__
#define __PLANERENDERCOMPONENT_H__

// Library Includes

// Local Includes
#include "RenderComponent.h"

/************************************************************************/
/* CPlaneRenderComponent
/*
/* Creates a simple plane mesh node with a given sub division amount.
/*
/* The plane has a specified texture, which can be tiled in both the X and Y.
/* specifying tileU and tileV of the texture element controls this.
/*
/* Example XML Element:
/*
/* <PlaneRenderComponent>
/*	<SubDivisions>100</SubDivisions>
/*	<Texture resource = "textures/texture.bmp" tileU = "1.0" tileV = "1.0"/>
/* </PlaneRenderComponent>
/*
/************************************************************************/
class CPlaneRenderComponent : public ARenderComponent {
	// Member Functions
public:
	CPlaneRenderComponent();
	virtual ~CPlaneRenderComponent();

protected:
private:
	/**
	* Initialises the component from XML.
	* 
	* @author: 	Hayden Asplet
	* @param:	TiXmlElement * _pXmlData - xml data.
	* @return:  bool - true if the initialisation succeeded.
	*/
	virtual bool VInitialiseDelegate(TiXmlElement* _pXmlData) override;

	/**
	* Delegating virtual function that creates the scene node of the render component.
	* 
	* @author: 	Hayden Asplet
	* @return:  shared_ptr<CSceneNode> - create scene node or nullptr if scene node creation failed.
	*/
	virtual shared_ptr<CSceneNode> VCreateSceneNode() override;

	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	int m_iSubDivisions;	// The amount of subdivisions the plane has.
	std::string m_strTextureResource;	// The texture file name and path.
	std::string m_strNormalMapResource;	// The normal map file name and path.
	float m_fTextureTileU;	// How much to tile the texture on the X axis, basically the U component of the UV is multiplied with this.
	float m_fTextureTileV;	// How much to tile the texture on the Y axis, basically the V component of the UV is multiplied with this.
};

#endif	// __PLANERENDERCOMPONENT_H__