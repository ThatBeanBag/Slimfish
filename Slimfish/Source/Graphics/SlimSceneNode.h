// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimSceneNode.h
// Description	: CSlimSceneNode declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMSCENENODE_H__
#define __SLIMSCENENODE_H__

// Library Includes

// Local Includes

namespace Slim {

// Forward Declaration.
class CScene;

/** Class representing a node in a scene.
@remarks
	
*/
class CSceneNode {
	typedef std::vector<std::shared_ptr<CSceneNode> > TChildList;

	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	 	@param pCreatorScene Pointer to the scene that created this.
	 	@return 
	*/
	CSceneNode(CScene* pCreatorScene);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	virtual ~CSceneNode();

	virtual bool VPreRender();
	virtual bool VRender();
	virtual bool VRenderChildren();
	virtual bool VPostRender();

	/** Add a node as a child to the node.
	 	@author Hayden Asplet
	*/
	void AddChild(std::shared_ptr<CSceneNode> pSceneNode);

	/** Remove a child from the node.
	 	@author Hayden Asplet
	*/
	void RemoveChild(std::shared_ptr<CSceneNode> pSceneNode);

	/** Remove all children from the node.
	 	@author Hayden Asplet
	*/
	void ClearChildren();

	/** Set the visibility of the node.
	 	@author Hayden Asplet
		@param isVisible True if the node should be rendered.
		@param recursive True if children nodes should also be set.
	*/
	void SetVisibility(bool isVisible, bool recursive = true);

	/** Get the visibility of the node. @author Hayden Asplet */
	const bool IsVisible() const;

	/** Set the position. @author Hayden Asplet */
	void SetPosition(const CVector3& position);
	/** Get the position. @author Hayden Asplet */
	const CVector3& GetPosition() const;

	/** Set the scale. @author Hayden Asplet */
	void SetScale(const CVector3& scale);
	/** Get the scale. @author Hayden Asplet */
	const CVector3& GetScale() const;

	/** Set the rotation. @author Hayden Asplet */
	void SetRotation(const CQuaternion& rotation);
	/** Get the rotation. @author Hayden Asplet */
	const CQuaternion& GetRotation() const;

	/** Get the full world transform of the node.
		@remarks
			Retrieves the transform built from the scale, position and rotation of the node including
			the effect of the parent node, thus the returned matrix is in world space. This transform
			gets cached therefore the overhead of this function is minimal.
	 	@author Hayden Asplet
	*/
	const CMatrix4x4& GetWorldTransform() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	// The scene that created this node.
	CScene* m_pCreatorScene;
	// This node's parent node, may be null if this the root node.
	CSceneNode* m_pParent;

	// The list of children this node has. 
	TChildList m_Children;

	// The translational factor of this node relative to it's parent.
	CVector3 m_Position;
	// The scaling factor of this node relative to it's parent.
	CVector3 m_Scale;
	// The rotational factor of this node relative to it's parent.
	CQuaternion m_Rotation;

	// The full transform that is cached and only updated if the world position/scale/rotation of the node changes.
	mutable CMatrix4x4 m_CachedTransform;	
	// True if the cached transform needs to be rebuilt.
	mutable bool m_CachedTransformIsDirty;
	
	// Describes the visibility of the node. True if the node should be rendered.
	bool m_bIsVisible;


};

}


#endif	// __SLIMSCENENODE_H__