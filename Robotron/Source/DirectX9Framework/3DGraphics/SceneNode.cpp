// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SceneNode.cpp
// Description	: CSceneNode implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "SceneNode.h"
#include "Scene.h"

// Local Includes

CSceneNode::CSceneNode(TActorID _actorID)
	:m_actorID(_actorID),
	m_matTransform(CMatrix4x4::s_kIdentity),
	m_matRelativeTransform(CMatrix4x4::s_kIdentity),
	m_pParent(nullptr),
	m_bIsVisible(true)
{

}

CSceneNode::CSceneNode(const CMatrix4x4& _krTransform, TActorID _actor)
	:m_matTransform(_krTransform),
	m_pParent(nullptr),
	m_bIsVisible(true)
{

}


CSceneNode::~CSceneNode()
{

}

bool CSceneNode::VOnRestore()
{
	bool bHasSucceeded = true;

	for (auto iter = m_children.begin(); iter != m_children.end(); ++iter) {
		if (!(*iter)->VOnRestore()) {
			bHasSucceeded = false;
		}
	}

	return bHasSucceeded;
}

void CSceneNode::VOnUpdate(float _fDeltaTime)
{
	for (auto iter = m_children.begin(); iter != m_children.end(); ++iter) {
		(*iter)->VOnUpdate(_fDeltaTime);
	}
}

bool CSceneNode::VPreRender(CScene* _pScene)
{
	// Set the matrix ready for rendering.
	_pScene->PushAndSetMatrix(m_matTransform * m_matRelativeTransform);	// GR: Transformation Pipeline.
	return true;
}

bool CSceneNode::VRender(CScene* _pScene)
{
	// By default, a scene node has nothing to render.
	return true;
}

bool CSceneNode::VRenderChildren(CScene* _pScene)
{
	// Iterate through all children.
	for (auto iter = m_children.begin(); iter != m_children.end(); ++iter) {
		if ((*iter)->VIsVisible(_pScene)) {
		// Is this child visible?
			if ((*iter)->VPreRender(_pScene)) {
				// Did the pre-rendering succeed?
				/*float fAlpha = (*iter)->GetAlpha();

				if (fAlpha >= g_kfALPHA_OPAQUE_VALUE) {
				// Is the scene node fully opaque?
					// Render it normally.
					(*iter)->VRender(_pScene);
				}
				else if (fAlpha != g_kfALPHA_TRANSPERANT_VALUE) {
				// Scene node has alpha.
					// Store the scene node so that we can render it on a second pass.
					CMatrix4x4 matWorldConcatenated = _pScene->GetTopMatrix();

					CVec3 worldPosition = matWorldConcatenated.GetPosition();
					CMatrix4x4 matToView = _pScene->GetCamera()->GetViewMatrix();
					CVec3 viewPosition = matToView.Transform(worldPosition);

					CAlphaNode alphaNode((*iter), matWorldConcatenated, viewPosition.z);
					_pScene->AddAlphaNode(alphaNode);
				}
				else {
				// Scene node is completely transparent, no need to render it!
				}*/

				float fAlpha = (*iter)->GetAlpha();

				if ((*iter)->VHasAlpha()) {
				// Scene node has alpha.
					// Store the scene node so that we can render it on a second pass.
					CMatrix4x4 matWorldConcatenated = _pScene->GetTopMatrix();

					CVec3 worldPosition = matWorldConcatenated.GetPosition();
					CMatrix4x4 matToView = _pScene->GetCamera()->GetViewMatrix();
					CVec3 viewPosition = matToView.Transform(worldPosition);

					CAlphaNode alphaNode((*iter), matWorldConcatenated, viewPosition.z);
					_pScene->AddAlphaNode(alphaNode);
				}
				else {
					// Is the scene node fully opaque?
					// Render it normally.
					(*iter)->VRender(_pScene);
				}

				(*iter)->VRenderChildren(_pScene);
				(*iter)->VPostRender(_pScene);
			}
		}
	}

	return true;
}

bool CSceneNode::VPostRender(CScene* _pScene)
{
	_pScene->PopMatrix();
	return true;
}

bool CSceneNode::AddChild(shared_ptr<CSceneNode> _pChild)
{
	// Insert the child into the list.
	m_children.push_back(_pChild);

	CAABB childAABB = _pChild->GetCombinedBoundingBox();
	const CMatrix4x4& matChild = _pChild->GetTransformMatrix();

	childAABB.SetMin(matChild.Transform(childAABB.GetMin()));
	childAABB.SetMax(matChild.Transform(childAABB.GetMax()));

	// Combine this bounding box with the child's bounding box to create an AABB containing both.
	m_combinedBoundingBox = CombineAABB(m_combinedBoundingBox, childAABB);

	// We are now the parent of this child.
	_pChild->m_pParent = this;

	return true;
}

bool CSceneNode::RemoveChild(shared_ptr<CSceneNode> _pChild)
{
	for (auto iter = m_children.begin(); iter != m_children.end(); ++iter) {
		if (*iter == _pChild) {
			m_children.erase(iter);	// Very slow for vectors.
			return true;
		}
	}

	return false;
}

void CSceneNode::SetVisibility(bool _bIsVisible)
{
	m_bIsVisible = _bIsVisible;
}

bool CSceneNode::VIsVisible(CScene* _pScene) const
{
	if (!m_bIsVisible) {
		return false;
	}
	else {
		/*const CFrustum& frustum = _pScene->GetCamera()->GetFrustum();
		const CMatrix4x4& matView = _pScene->GetCamera()->GetViewMatrix();

		// Transform the world position into view space.
		//CVec3 vec3View = matView.Transform(GetWorldPosition());

		CMatrix4x4 matToView = _pScene->GetTopMatrix() * m_matTransform;
		CAABB viewBoundingBox = TransformAABB(matToView, m_combinedBoundingBox);

/ *
		g_pApp->GetRenderer()->VSetViewTransform(&CMatrix4x4::s_kIdentity);
		g_pApp->GetRenderer()->VSetWorldTransform(&CMatrix4x4::s_kIdentity);* /
		//g_pApp->GetRenderer()->VDrawAABB(viewBoundingBox, g_kCOLOUR_RED);
/ *
		_pScene->GetCamera()->SetViewTransform();
		g_pApp->GetRenderer()->VSetWorldTransform(&_pScene->GetTopMatrix());* /

		if (IsInsideFrustum(frustum, viewBoundingBox, matView)) {
		// Is the bounding box inside of the frustum?
			return true;
		}
		else {
		// The bounding box is outside of the view frustum 
			//return false;
			return true;
		}*/
	}

	return m_bIsVisible;
}


void CSceneNode::SetTransform(const CMatrix4x4& _krmatTransform)
{
	m_matTransform = _krmatTransform;
	CombineBoundingBoxes();
}

void CSceneNode::SetRelativeTransform(const CMatrix4x4& _krmatTransform)
{
	m_matRelativeTransform = _krmatTransform;
}

void CSceneNode::SetMaterial(const CMaterial& _krMaterial)
{
	m_material = _krMaterial;
}

void CSceneNode::SetActorID(TActorID _actorID)
{
	m_actorID = _actorID;
}

void CSceneNode::SetBoundingBox(const CAABB& _krBoundingBox)
{
	m_boundingBox = _krBoundingBox;
	CombineBoundingBoxes();
}

void CSceneNode::CombineBoundingBoxes()
{
	// TODO: This is coded out because currently is a hot spot for performance drops.
	/*m_combinedBoundingBox = m_boundingBox;

	// Combine our bounding box with our children's so that our one contains all.
	for (auto iter = m_children.begin(); iter != m_children.end(); ++iter) {
		shared_ptr<CCameraNode> pCamera = std::dynamic_pointer_cast<CCameraNode>(*iter);
		if (pCamera) {
			continue;
		}

		m_combinedBoundingBox = CombineAABB(m_combinedBoundingBox, (*iter)->GetTransformedBoundingBox());
	}

	if (m_pParent) {
		m_pParent->CombineBoundingBoxes();
	}*/
}

const CMatrix4x4 CSceneNode::GetTransformMatrix() const
{
	return m_matTransform;
}

const CVec3 CSceneNode::GetPosition() const
{
	return m_matTransform.GetPosition();
}

const CVec3 CSceneNode::GetWorldPosition() const
{
	if (!m_pParent) {
		return m_matTransform.GetPosition();
	}
	else {
		CVec3 vec3Position = m_matTransform.GetPosition();
		return vec3Position += m_pParent->GetWorldPosition();
	}
}

const CVec3 CSceneNode::GetDirection() const
{
	return m_matTransform.GetForward();
}

const CMaterial CSceneNode::GetMaterial() const
{
	return m_material;
}

const CAABB CSceneNode::GetCombinedBoundingBox() const
{
	return m_combinedBoundingBox;
}

const CAABB CSceneNode::GetTransformedBoundingBox() const
{
	// Transform the coordinates of the bounding box by our transform and return.
	return TransformAABB(m_matTransform, m_combinedBoundingBox);
}

const TActorID CSceneNode::GetActorID() const
{
	return m_actorID;
}

float CSceneNode::GetAlpha() const
{
	return m_material.GetDiffuse().m_fA;
}

bool CSceneNode::VHasAlpha() const
{
	return m_material.GetDiffuse().m_fA < 1.0f;
}

