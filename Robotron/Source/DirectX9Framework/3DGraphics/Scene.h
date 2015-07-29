// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Scene.h
// Description	: CScene declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SCENE_H__
#define __SCENE_H__

// Library Includes

// Local Includes
#include "CameraNode.h"
#include "MeshNode.h"
#include "LightNode.h"
#include "AlphaNode.h"

class CScene {
	// Member Functions
	typedef std::map<TActorID, shared_ptr<CSceneNode> > TSceneActorMap;

public:
	CScene();
	~CScene();

	bool OnRender();
	bool OnRestore();

	// Updates all nodes in the scene.
	void OnUpdate(float _fDeltaTime);

	// Adds a child to the root node.
	bool AddChild(shared_ptr<CSceneNode> _pChild, TActorID _actorID = CActor::s_kINVALID_ACTOR_ID);

	// Removes a child from the root node.
	bool RemoveChild(shared_ptr<CSceneNode> _pChild);
	bool RemoveChild(TActorID _actorID);

	void PushAndSetMatrix(const CMatrix4x4& _krmatTransform);
	void PopMatrix();
	CMatrix4x4 GetTopMatrix() const;

	void SetCamera(shared_ptr<CCameraNode> _pCamera);
	shared_ptr<CCameraNode> GetCamera();

	std::vector<TLight> GetLights();	// Helpful for debugging.
	void RemoveAllLights();

	void AddAlphaNode(CAlphaNode _alphaNode);

	shared_ptr<CSceneNode> FindSceneNodeFromActorID(TActorID _actorID);
protected:
private:
	void SetupLighting();
	void RenderAlphaPass();

	// Event Delegates
	void NewRenderComponentDelegate(TEventDataPtr pEventData);
	void DestroyActorDelegate(TEventDataPtr pEventData);
	void MoveActorDelegate(TEventDataPtr pEventData);

	// Member Variables
public:
protected:
private:
	shared_ptr<CSceneNode> m_pRoot;
	shared_ptr<CCameraNode> m_pCamera;
	std::list<shared_ptr<CLightNode>> m_lights;
	TSceneActorMap m_sceneActorMap;
	std::vector<CAlphaNode> m_vecAlphaNodes;

	ID3DXMatrixStack* m_pMatrixStack;
};

#endif	// __SCENE_H__