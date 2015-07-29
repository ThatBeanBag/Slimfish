// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionPlane.h
// Description	: CCollisionPlane declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __COLLISIONPLANE_H__
#define __COLLISIONPLANE_H__

// Library Includes

// Local Includes
#include "CollisionShape.h"

class CCollisionPlane : public ACollisionShape {
	// Member Functions
public:
	CCollisionPlane();
	virtual ~CCollisionPlane();

	virtual const std::string VGetType() override;
	virtual bool VInitializeFromXML(TiXmlElement* _pXmlData) override;

	const CPlane GetPlane() const;
protected:
private:
	// Member Variables
public:
	const static std::string s_kstrNAME;
protected:
private:
	CPlane m_plane;
};

#endif	// __COLLISIONPLANE_H__