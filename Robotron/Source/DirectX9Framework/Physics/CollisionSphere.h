// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionSphere.h
// Description	: CCollisionSphere declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __COLLISIONSPHERE_H__
#define __COLLISIONSPHERE_H__

// Library Includes

// Local Includes
#include "CollisionShape.h"

class CCollisionSphere : public ACollisionShape {
	// Member Functions
public:
	CCollisionSphere();
	virtual ~CCollisionSphere();

	virtual const std::string VGetType() override;
	virtual bool VInitializeFromXML(TiXmlElement* _pXmlData) override;

	float GetRaidus() const;
protected:
private:
	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	float m_fRadius;
};

#endif	// __COLLISIONSPHERE_H__