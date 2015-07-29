// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionCapsule.h
// Description	: CCollisionCapsule declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __COLLISIONCAPSULE_H__
#define __COLLISIONCAPSULE_H__

// Library Includes

// Local Includes
#include "CollisionShape.h"

class CCollisionCapsule : public ACollisionShape {
	// Member Functions
public:
	CCollisionCapsule();
	virtual ~CCollisionCapsule();

	virtual const std::string VGetType() override;
	virtual bool VInitializeFromXML(TiXmlElement* _pXmlData) override;

	float GetRadius() const;
	float GetHeight() const;
protected:
private:
	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	float m_fRadius;
	float m_fHeight;
};

#endif	// __COLLISIONCAPSULE_H__