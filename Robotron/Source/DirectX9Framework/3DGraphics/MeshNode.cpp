// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MeshNode.h
// Description	: CMeshNode implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "MeshNode.h"

// Local Includes
#include "../Renderer/Mesh.h"

CMeshNode::CMeshNode(TActorID _actorID)
	:CSceneNode(_actorID),
	m_pStaticBuffer(nullptr),
	m_pMesh(nullptr),
	m_pTexture(nullptr),
	m_bIsLit(true)
{
	
}

CMeshNode::CMeshNode(TActorID _actorID, shared_ptr<IMesh> _pMesh)
	:CSceneNode(_actorID),
	m_pStaticBuffer(nullptr),
	m_pTexture(nullptr),
	m_bIsLit(true),
	m_pMesh(nullptr)
{
	SetMesh(_pMesh);
}

CMeshNode::CMeshNode(shared_ptr<IStaticBuffer> _pStaticBuffer, shared_ptr<ITexture> _pTexture, shared_ptr<INormalMap> _pNormalMap)
	:CSceneNode(CActor::s_kINVALID_ACTOR_ID),
	m_pStaticBuffer(nullptr),
	m_pTexture(_pTexture),
	m_pNormalMap(_pNormalMap),
	m_bIsLit(true),
	m_pMesh(nullptr)
{
	SetStaticBuffer(_pStaticBuffer);
}

CMeshNode::~CMeshNode()
{

}

bool CMeshNode::VRender(CScene* _pScene)
{
	if (!CSceneNode::VRender(_pScene)) {
		return false;
	}

	if (m_pMesh) {
		m_pMesh->VRender();

		return true;
	}

	if (m_pStaticBuffer == nullptr) {
		// We've succeeded to draw nothing.
		return true;
	}
	else {
		IRenderer* pRenderer = g_pApp->GetRenderer();

		bool bHasNormalMap = false;

		if (m_pNormalMap) {
			std::vector<TLight> lights = _pScene->GetLights();
			if (!lights.empty()) {
				m_pNormalMap->SetLight(_pScene->GetLights()[0]);
				pRenderer->VSetTexture(TEXSTAGE_0, m_pNormalMap);
				pRenderer->VSetTexture(TEXSTAGE_1, m_pTexture);
				bHasNormalMap = true;
			}
			else {
				bHasNormalMap = false;
			}
		}

		if (!bHasNormalMap) {
			pRenderer->VSetTexture(TEXSTAGE_0, m_pTexture);
			pRenderer->VSetTexture(TEXSTAGE_1, nullptr);
		}

		pRenderer->VSetMaterial(m_material);

		int iOldLighting = pRenderer->VGetRenderState(RS_LIGHTING);
		pRenderer->VSetRenderState(RS_LIGHTING, m_bIsLit);

		m_pStaticBuffer->VRender();

		pRenderer->VSetRenderState(RS_LIGHTING, iOldLighting);

		return true;
	}

	return false;
}

void CMeshNode::SetStaticBuffer(shared_ptr<IStaticBuffer> _pStaticBuffer)
{
	m_pStaticBuffer = _pStaticBuffer;
	SetBoundingBox(m_pStaticBuffer->VCalculateBoundingBox());
}

void CMeshNode::SetMesh(shared_ptr<IMesh> _pMesh)
{
	DEBUG_ASSERT(_pMesh);

	m_pMesh = _pMesh;
	SetBoundingBox(m_pMesh->VCalculateBoundingBox());
}

void CMeshNode::SetTexture(shared_ptr<ITexture> _pTexture)
{
	m_pTexture = _pTexture;
}

void CMeshNode::SetNormalMap(shared_ptr<INormalMap> _pNormalMap)
{
	m_pNormalMap = _pNormalMap;
}

void CMeshNode::SetIsLit(bool _bIsLit)
{
	m_bIsLit = _bIsLit;
}

shared_ptr<ITexture> CMeshNode::GetTexture()
{
	return m_pTexture;
}

bool CMeshNode::VHasAlpha() const
{
	if (m_pMesh) {
		std::vector<CMaterial> materials = m_pMesh->VGetMaterials();
		for (unsigned int i = 0; i < materials.size(); ++i) {
			if (materials[i].GetDiffuse().m_fA < 1.0f) {
				return true;
			}
		}
	}
	else {
		return CSceneNode::VHasAlpha();
	}

	return false;
}

void CMeshNode::VOnUpdate(float _fDeltaTime)
{
	CSceneNode::VOnUpdate(_fDeltaTime);

	/*m_fX += _fDeltaTime * 0.1f;
	m_fY += _fDeltaTime * 0.1f;*/

	if (m_pTexture) {
		m_pTexture->VSetTranslation(m_fX, m_fY);
	}

	if (m_pNormalMap) {
		m_pNormalMap->VSetTranslation(m_fX, m_fY);
	}
}
