// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimSceneNode.cpp
// Description	: CSlimSceneNode implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimSceneNode.h"

// Local Includes

namespace Slim {

	CSceneNode::CSceneNode(CScene* pCreatorScene)
		:m_pCreatorScene(pCreatorScene),
		m_Scale(CVector3::s_ONE),
		m_Rotation(CQuaternion::s_IDENTITY),
		m_Position(CVector3::s_ZERO)
	{

	}

	CSceneNode::~CSceneNode()
	{

	}

	bool CSceneNode::VPreRender()
	{
		return true;
	}

	bool CSceneNode::VRender()
	{
		return true;
	}

	bool CSceneNode::VRenderChildren()
	{
		return true;
	}

	bool CSceneNode::VPostRender()
	{
		return true;
	}

	void CSceneNode::AddChild(std::shared_ptr<CSceneNode> pSceneNode)
	{
		m_Children.push_back(pSceneNode);
	}

	void CSceneNode::RemoveChild(std::shared_ptr<CSceneNode> pSceneNode)
	{
		auto shouldbeRemoved = [&](const std::shared_ptr<CSceneNode>& pNode) {
			return pNode == pSceneNode;
		};

		m_Children.erase(remove_if(m_Children.begin(), m_Children.end(), pSceneNode));
	}

	void CSceneNode::ClearChildren()
	{
		m_Children.clear();
	}

	void CSceneNode::SetVisibility(bool isVisible, bool recursive /*= true*/)
	{
		m_bIsVisible = isVisible;
		if (recursive) {
			// Set the visibility of the children nodes and their children nodes.
			static auto setVisibility = [&](std::unique_ptr<CSceneNode>& pSceneNode) {
				pSceneNode->SetVisibility(isVisible, true);
			};

			std::for_each(m_Children.begin(), m_Children.end(), setVisibility);
		}
	}

	const bool CSceneNode::IsVisible() const
	{
		return m_bIsVisible;
	}

	void CSceneNode::SetPosition(const CVector3& position)
	{
		m_Position = position;
		m_CachedTransformIsDirty = true;
	}

	const CVector3& CSceneNode::GetPosition() const
	{
		return m_Position;
	}

	void CSceneNode::SetScale(const CVector3& scale)
	{
		m_Scale = scale;
		m_CachedTransformIsDirty = true;
	}

	const CVector3& CSceneNode::GetScale() const
	{
		return m_Scale;
	}

	void CSceneNode::SetRotation(const CQuaternion& rotation)
	{
		m_Rotation = rotation;
		m_CachedTransformIsDirty = true;
	}

	const CQuaternion& CSceneNode::GetRotation() const
	{
		return m_Rotation;
	}

	const CMatrix4x4& CSceneNode::GetWorldTransform() const
	{
		// Rebuild the world transform if us or our parent have change our scale, position or translation.
		if (m_CachedTransformIsDirty || (m_pParent && m_pParent->m_CachedTransformIsDirty)) {
			m_CachedTransform = CMatrix4x4::BuildTransform(m_Position, m_Scale, m_Rotation);

			if (m_pParent) {
				m_CachedTransform *= m_pParent->GetWorldTransform();
			}

			m_CachedTransformIsDirty = false;
		}

		return m_CachedTransform;
	}

}

