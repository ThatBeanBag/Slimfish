// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TransformComponent.h
// Description	: CTransformComponent declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

// Library Includes

// Local Includes
#include "ActorComponent.h"

/**
* CTransformComponent - manages the world transform of the actor.
*/
class CTransformComponent : public AActorComponent {
	// Member Functions
public:
	CTransformComponent();
	virtual ~CTransformComponent();

	virtual bool VInitialise(TiXmlElement* _pXmlData) override;

	const CMatrix4x4 GetTransform() const;
	void SetTransform(const CMatrix4x4& _krmatTransform);

	void SetPosition(const CVec3& _krVec3);
	const CVec3 GetPosition() const;

	void RespawnRandom();
	void Respawn(int _iSpawnPoint);

	CVec3 GetSpawnRandom() const;
	CVec3 GetSpawn(int _iSpawnPoint) const;

	int GetNumSpawns() const { return m_vecSpawnPositions.size(); }
protected:
private:
	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	CMatrix4x4 m_matTransform;
	CMatrix4x4 m_matInitialTransform;

	std::vector<CVec3> m_vecSpawnPositions;
};

#endif // __TRANSFORMCOMPONENT_H__