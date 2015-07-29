// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SceneNode.h
// Description	: CSceneNode declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SCENENODE_H__
#define __SCENENODE_H__

// Library Includes

// Local Includes
#include "Material.h"
#include "../Geometry/AABB.h"
#include "../Geometry/Matrix4x4.h"

// Forward Declaration
class CScene;

class CSceneNode {
	typedef std::vector<shared_ptr<CSceneNode> > TSceneNodeList;

	// Member Functions
public:
	explicit CSceneNode(TActorID _actorID);
	CSceneNode(const CMatrix4x4& _krTransform, TActorID _actor = CActor::s_kINVALID_ACTOR_ID);

	virtual ~CSceneNode();

	// Scene Node functions
	virtual bool VOnRestore();
	virtual void VOnUpdate(float _fDeltaTime);

	virtual bool VPreRender(CScene* _pScene);
	virtual bool VRender(CScene* _pScene);
	virtual bool VRenderChildren(CScene* _pScene);
	virtual bool VPostRender(CScene* _pScene);

	bool AddChild(shared_ptr<CSceneNode> _pChild);
	bool RemoveChild(shared_ptr<CSceneNode> _pChild);

	void SetVisibility(bool _bIsVisible);
	virtual bool VIsVisible(CScene* _pScene) const;

	// Accessors
	void SetTransform(const CMatrix4x4& _krmatTransform);
	void SetRelativeTransform(const CMatrix4x4& _krmatTransform);
	void SetActorID(TActorID _actorID);
	void SetMaterial(const CMaterial& _krMaterial);
	void SetBoundingBox(const CAABB& _krBoundingBox);
	void CombineBoundingBoxes();

	const CMatrix4x4 GetTransformMatrix() const;
	const CVec3 GetPosition() const;
	const CVec3 GetWorldPosition() const;
	const CVec3 GetDirection() const;
	const CMaterial GetMaterial() const;
	const CAABB GetCombinedBoundingBox() const;
	const CAABB GetTransformedBoundingBox() const;
	const TActorID GetActorID() const;
	float GetAlpha() const;
	virtual bool VHasAlpha() const;
protected:
private:
	// Member Variables
public:
protected:
	CMatrix4x4 m_matTransform;		// The to-world matrix, transforms all vertices from object to world space.
	CMatrix4x4 m_matRelativeTransform;
	CSceneNode* m_pParent;			// This node's parent node, will be nullptr if it does not have a parent.
	TSceneNodeList m_children;

	bool m_bIsVisible;
	TActorID m_actorID;

	CMaterial m_material;
private:
	CAABB m_combinedBoundingBox;
	CAABB m_boundingBox;
};

#endif	// __SCENENODE_H__
