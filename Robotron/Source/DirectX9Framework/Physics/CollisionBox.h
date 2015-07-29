// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: CollisionBox.h
// Description	: CCollisionBox declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __COLLISIONBOX_H__
#define __COLLISIONBOX_H__

// Library Includes

// Local Includes
#include "CollisionShape.h"

class CCollisionBox : public ACollisionShape {
	// Member Functions
public:
	CCollisionBox();
	virtual ~CCollisionBox();

	virtual const std::string VGetType() override;
	virtual bool VInitializeFromXML(TiXmlElement* _pXmlData) override;
	
	const CVec3 GetDimensions() const;
protected:
private:
	// Member Variables
public:
	static const std::string s_kstrNAME;
protected:
private:
	CVec3 m_vec3Dimensions;
};

#endif	// __COLLISIONBOX_H__