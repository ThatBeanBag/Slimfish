// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: LightNode.cpp
// Description	: CLighNode implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "LightNode.h"

// Local Includes
#include "SceneNode.h"

CLightNode::CLightNode(TActorID _actorID)
	:CSceneNode(_actorID)
{
	ZeroMemory(&m_lightProperties, sizeof(TLight));
}

CLightNode::CLightNode(TActorID _actorID, const TLight& _krLightProperties)
	:CSceneNode(_actorID),
	m_lightProperties(_krLightProperties)
{
	CVec3 vec3Forward = m_lightProperties.m_direction;
	CVec3 vec3Right = CrossProduct(g_kvec3Up, vec3Forward);
	CVec3 vec3Up = CrossProduct(vec3Forward, vec3Right);

	m_matTransform.SetPosition(m_lightProperties.m_position);

	m_matTransform.m[0][0] = vec3Right.x;
	m_matTransform.m[0][1] = vec3Right.y;
	m_matTransform.m[0][2] = vec3Right.z;
	m_matTransform.m[1][0] = vec3Up.x;
	m_matTransform.m[1][1] = vec3Up.y;
	m_matTransform.m[1][2] = vec3Up.z;
	m_matTransform.m[2][0] = vec3Forward.x;
	m_matTransform.m[2][1] = vec3Forward.y;
	m_matTransform.m[2][2] = vec3Forward.z;
}


CLightNode::~CLightNode()
{

}

void CLightNode::VOnUpdate(float _fDeltaTime)
{
	// Update children.
	CSceneNode::VOnUpdate(_fDeltaTime);
}

TLight CLightNode::GetProperties()
{
	m_lightProperties.m_position = GetPosition();
	m_lightProperties.m_direction = GetDirection();
	m_lightProperties.m_direction.Normalize();

	return m_lightProperties;
}
