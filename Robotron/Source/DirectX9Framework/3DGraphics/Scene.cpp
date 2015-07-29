// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Scene.cpp
// Description	: CScene implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Scene.h"

// Local Includes

CScene::CScene()
	:m_pRoot(shared_ptr<CSceneNode>()),
	m_pCamera(shared_ptr<CCameraNode>()),
	m_pMatrixStack(nullptr)
{
	// Set the rood node to a default CSceneNode, transform will be identity matrix.
	m_pRoot.reset(new CSceneNode(CActor::s_kINVALID_ACTOR_ID));

	D3DXCreateMatrixStack(0, &m_pMatrixStack);

	// Register event delegates.
	CEventManager* pEventManager = CEventManager::GetInstance().get();
	pEventManager->AddListener<CEventData_NewRenderComponent>(MakeDelegate(this, &CScene::NewRenderComponentDelegate));
	pEventManager->AddListener<CEventData_MoveActor>(MakeDelegate(this, &CScene::MoveActorDelegate));
	pEventManager->AddListener<CEventData_DestroyActor>(MakeDelegate(this, &CScene::DestroyActorDelegate));
}

CScene::~CScene()
{
	// Unregister event delegates
	CEventManager* pEventManager = CEventManager::GetInstance().get();
	pEventManager->RemoveListener<CEventData_NewRenderComponent>(MakeDelegate(this, &CScene::NewRenderComponentDelegate));
	pEventManager->RemoveListener<CEventData_MoveActor>(MakeDelegate(this, &CScene::MoveActorDelegate));
	pEventManager->RemoveListener<CEventData_DestroyActor>(MakeDelegate(this, &CScene::DestroyActorDelegate));
}

bool CScene::OnRender()
{
	SetupLighting();

	if (m_pRoot && m_pCamera) {
	// Do we have a camera and a root node?
		// Set the view and projection transforms.
		m_pCamera->SetViewTransform();

		// Render the scene nodes.
		if (m_pRoot->VIsVisible(this)) {
			if (m_pRoot->VPreRender(this)) {
				m_pRoot->VRender(this);
				m_pRoot->VRenderChildren(this);
				m_pRoot->VPostRender(this);
			}
			else {
				return false;
			}

			RenderAlphaPass();
		}
	}

	return true;
}

bool CScene::OnRestore()
{
	if (!m_pRoot) {
		return true;
	}
	else {
		return m_pRoot->VOnRestore();
	}

	return false;
}

void CScene::OnUpdate(float _fDeltaTime)
{
	if (!m_pRoot) {
		return;
	}
	else {
		m_pRoot->VOnUpdate(_fDeltaTime);
	}
}

bool CScene::AddChild(shared_ptr<CSceneNode> _pChild, TActorID _actorID)
{
	DEBUG_ASSERT(m_pRoot);

	if (_actorID != CActor::s_kINVALID_ACTOR_ID) {
		// First see if a scene node for this actor already exists.
		auto findIter = m_sceneActorMap.find(_actorID);
		if (findIter != m_sceneActorMap.end()) {
		// A scene node for this actor already exists.
			// Set the actor id of the child to invalid, otherwise it will respond to move events.
			_pChild->SetActorID(CActor::s_kINVALID_ACTOR_ID);
			// Add the new scene node as a child of the actor's scene node.
			return findIter->second->AddChild(_pChild);
		}
		else {
		// New scene node for actor. 
			// Register the scene in the actor map.
			m_sceneActorMap[_actorID] = _pChild;
		}
	}

	shared_ptr<CLightNode> pLightNode = std::dynamic_pointer_cast<CLightNode>(_pChild);
	if (pLightNode) {
	// Is this a light node?
		// Register this light.
		m_lights.push_back(pLightNode);
	}

	return m_pRoot->AddChild(_pChild);
}

bool CScene::RemoveChild(TActorID _actorID)
{
	shared_ptr<CSceneNode> pSceneNode = FindSceneNodeFromActorID(_actorID);
	return RemoveChild(pSceneNode);
}

bool CScene::RemoveChild(shared_ptr<CSceneNode> _pChild)
{
	DEBUG_ASSERT(m_pRoot);

	shared_ptr<CLightNode> pLightNode = std::dynamic_pointer_cast<CLightNode>(_pChild);

	if (pLightNode) {
		// Is this a light node?
		// Remove this light.
		m_lights.remove(pLightNode);
	}

	return m_pRoot->RemoveChild(_pChild);
}

void CScene::PushAndSetMatrix(const CMatrix4x4& _krmatTransform)
{
	m_pMatrixStack->Push();
	m_pMatrixStack->MultMatrix(&_krmatTransform);
	CMatrix4x4 matTop = GetTopMatrix();
	g_pApp->GetRenderer()->VSetWorldTransform(&matTop);
}

void CScene::PopMatrix()
{
	m_pMatrixStack->Pop();
	CMatrix4x4 matTop = GetTopMatrix();
	g_pApp->GetRenderer()->VSetWorldTransform(&matTop);
}

CMatrix4x4 CScene::GetTopMatrix() const
{
	return static_cast<CMatrix4x4>(*m_pMatrixStack->GetTop());
}

void CScene::SetCamera(shared_ptr<CCameraNode> _pCamera)
{
	m_pCamera = _pCamera;
}

shared_ptr<CCameraNode> CScene::GetCamera()
{
	return m_pCamera;
}

std::vector<TLight> CScene::GetLights()
{
	std::vector<TLight> vecLights;

	for (auto iter = m_lights.begin(); iter != m_lights.end(); ++iter) {
		vecLights.push_back((*iter)->GetProperties());
	}

	return std::move(vecLights);
}

void CScene::RemoveAllLights()
{
	IRenderer* pRenderer = g_pApp->GetRenderer();

	if (!pRenderer) {
		return;
	}

	TLight disabledLight;
	ZeroMemory(&disabledLight, sizeof(TLight));
	disabledLight.m_bIsEnabled = false;

	// Disable the lights in the renderer;
	for (unsigned int i = 0; i < m_lights.size(); ++i) {
		pRenderer->VSetLight(static_cast<int>(i), disabledLight);
	}

	// Clear the lights from the list.
	m_lights.clear();
}

void CScene::AddAlphaNode(CAlphaNode _alphaNode)
{
	m_vecAlphaNodes.push_back(_alphaNode);
}


shared_ptr<CSceneNode> CScene::FindSceneNodeFromActorID(TActorID _actorID)
{
	// Find the scene node of the actor.
	TSceneActorMap::iterator findIter = m_sceneActorMap.find(_actorID);

	if (findIter != m_sceneActorMap.end()) {
	// Did we find scene node for the actor?
		return findIter->second;
	}
	else {
	// Failed to find the scene for the actor.
		return shared_ptr<CSceneNode>();
	}
}

void CScene::SetupLighting()
{
	int iLightIndex = 0;
	IRenderer* pRenderer = g_pApp->GetRenderer();

	if (!pRenderer) {
		return;
	}

	// Set the lights of the renderer.
	for (auto iter = m_lights.begin(); iter != m_lights.end(); ++iter, ++iLightIndex) {
		if ((*iter)->VIsVisible(this)) {
			pRenderer->VSetLight(iLightIndex, (*iter)->GetProperties());
		}
	}
}

void CScene::RenderAlphaPass()
{
	// Sort the alpha nodes in terms of their distance from view.
	std::sort(m_vecAlphaNodes.begin(), m_vecAlphaNodes.end());

	// Draw all alpha nodes.
	while (!m_vecAlphaNodes.empty()) {
		// Get the current alpha node from back of the list.
		CAlphaNode alphaNode = m_vecAlphaNodes.back();

		PushAndSetMatrix(alphaNode.GetWorldTransform());

		// Render the node.
		alphaNode.GetSceneNode()->VRender(this);

		PopMatrix();

		// Remove the alpha node from the list.
		m_vecAlphaNodes.pop_back();
	}
}

void CScene::NewRenderComponentDelegate(TEventDataPtr pEventData)
{
	// Cast the event data so that we can retrieve it.
	shared_ptr<CEventData_NewRenderComponent> pNewRenderComponentEventData = static_pointer_cast<CEventData_NewRenderComponent>(pEventData);

	// Get the data.
	TActorID actorID = pNewRenderComponentEventData->GetActorID();
	shared_ptr<CSceneNode> pSceneNode = pNewRenderComponentEventData->GetSceneNode();

	if (actorID == CActor::s_kINVALID_ACTOR_ID) {
		DEBUG_ERROR("Unknown actor ID from new render component event");
		return;
	}

	// Attempt to restore the node.
	if (!pSceneNode->VOnRestore()) {
	// Did the restoration fail?
		DEBUG_ERROR("Failed to restore scene node for actorID: " + ToString(actorID));
		return;
	}

	// Add the actor and scene node to the scene.
	AddChild(pSceneNode, actorID);
}

void CScene::DestroyActorDelegate(TEventDataPtr pEventData)
{
	shared_ptr<CEventData_DestroyActor> pDestroyActorData = static_pointer_cast<CEventData_DestroyActor>(pEventData);

	TActorID actorID = pDestroyActorData->GetActorID();
	
	RemoveChild(actorID);
}

void CScene::MoveActorDelegate(TEventDataPtr pEventData)
{
	shared_ptr<CEventData_MoveActor> pMoveActorData = static_pointer_cast<CEventData_MoveActor>(pEventData);

	TActorID actorID = pMoveActorData->GetActorID();
	CMatrix4x4 matTransform = pMoveActorData->GetTransform();

	shared_ptr<CSceneNode> pSceneNode = FindSceneNodeFromActorID(actorID);

	if (pSceneNode) {
		pSceneNode->SetTransform(matTransform);
	}
	else {
		DEBUG_WARNING("Couldn't find actor with ID: " + ToString(actorID) + " when moving");
	}
}
