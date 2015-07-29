// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AlphaNode.cpp
// Description	: CAlphaNode implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "../3DGraphics/AlphaNode.h"

// Local Includes


CAlphaNode::CAlphaNode(shared_ptr<CSceneNode> _pNode, CMatrix4x4 _matWorldConcatenated, float _viewDistance)
	:m_pNode(_pNode),
	m_matWorldConcatenated(_matWorldConcatenated),
	m_viewDistance(_viewDistance)
{

}

CAlphaNode::~CAlphaNode()
{

}

shared_ptr<CSceneNode> CAlphaNode::GetSceneNode() const
{
	return m_pNode;
}

const CMatrix4x4 CAlphaNode::GetWorldTransform() const
{
	return m_matWorldConcatenated;
}

float CAlphaNode::GetViewDistance() const
{
	return m_viewDistance;
}

bool operator<(const CAlphaNode& _krNodeA, const CAlphaNode& _krNodeB)
{
	return _krNodeA.GetViewDistance() < _krNodeB.GetViewDistance();
}
