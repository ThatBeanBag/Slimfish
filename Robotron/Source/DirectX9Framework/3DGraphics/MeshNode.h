// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MeshNode.h
// Description	: CMeshNode declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MESHNODE_H__
#define __MESHNODE_H__

// Library Includes

// Local Includes
#include "SceneNode.h"

class IMesh;

class CMeshNode : public CSceneNode {
	// Member Functions
public:
	CMeshNode(TActorID _actorID);
	CMeshNode(TActorID _actorID, shared_ptr<IMesh> _pMesh);
	explicit CMeshNode(shared_ptr<IStaticBuffer> _pStaticBuffer, shared_ptr<ITexture> _pTexture = nullptr, shared_ptr<INormalMap> _pNormalMap = nullptr);

	virtual ~CMeshNode();

	virtual bool VRender(CScene* _pScene);

	void SetStaticBuffer(shared_ptr<IStaticBuffer> _pStaticBuffer);
	void SetMesh(shared_ptr<IMesh> _pMesh);
	void SetTexture(shared_ptr<ITexture> _pTexture);
	void SetNormalMap(shared_ptr<INormalMap> _pNormalMap);
	void SetIsLit(bool _bIsLit);
	
	shared_ptr<ITexture> GetTexture();

	virtual bool VHasAlpha() const override;

	virtual void VOnUpdate(float _fDeltaTime) override;

protected:
private:
	// Member Variables
public:
protected:
private:
	shared_ptr<IStaticBuffer> m_pStaticBuffer;
	shared_ptr<IMesh> m_pMesh;
	shared_ptr<ITexture> m_pTexture;
	shared_ptr<INormalMap> m_pNormalMap;
	bool m_bIsLit;

	float m_fX;
	float m_fY;
};


#endif	// __MESHNODE_H__