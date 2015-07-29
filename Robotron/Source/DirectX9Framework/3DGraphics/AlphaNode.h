// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AlphaNode.h
// Description	: CAlphaNode declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __ALPHANODE_H__
#define __ALPHANODE_H__

// Library Includes

// Local Includes

/************************************************************************/
/* CAlphaNode
/*
/* Takes any scene node that has alpha and prepares it for the alpha pass.
/*
/* Specifically it allows scene nodes with alpha to be taken out of the
/* rendering tree pipeline to be rendered on a second pass in the order of
/* distance from the screen.
/************************************************************************/
class CAlphaNode {
	// Member Functions
public:
	CAlphaNode(shared_ptr<CSceneNode> _pNode, CMatrix4x4 _matWorldConcatenated, float _viewDistance);
	~CAlphaNode();

	/**
	* Gets the scene node.
	* 
	* @author: 	Hayden Asplet
	* @return:  shared_ptr<CSceneNode> - scene node.
	*/
	shared_ptr<CSceneNode> GetSceneNode() const;

	/**
	* Gets the concatenated world matrix.
	* 
	* @author: 	Hayden Asplet
	* @return:  const CMatrix4x4 - concatenated world matrix
	*/
	const CMatrix4x4 GetWorldTransform() const;

	/**
	* Gets the distance from the view in terms of the Z value of the node.
	* 
	* @author: 	Hayden Asplet
	* @return:  float - distance from view.
	*/
	float GetViewDistance() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	shared_ptr<CSceneNode> m_pNode;
	CMatrix4x4 m_matWorldConcatenated;
	float m_viewDistance;
};

bool operator<(const CAlphaNode& _krNodeA, const CAlphaNode& _krNodeB); // For sorting alpha nodes.

#endif	// __ALPHANODE_H__