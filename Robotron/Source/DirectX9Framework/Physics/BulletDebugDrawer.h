// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BulletDebugDrawer.h
// Description	: CBulletDebugDrawer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __BULLETDEBUGDRAWER_H__
#define __BULLETDEBUGDRAWER_H__

// Library Includes

// Local Includes
#include "LinearMath\btIDebugDraw.h"

class CBulletDebugDrawer : public btIDebugDraw {
	// Member Functions
public:
	CBulletDebugDrawer();
	virtual ~CBulletDebugDrawer();

	// btIDebugDrawInterface
	virtual void drawLine(const btVector3& _krbtvec3From, const btVector3& _krbtvec3To, const btVector3& _krbtvec3Colour) override;
	virtual void drawContactPoint(const btVector3& _krbtvec3PointOnB, 
								  const btVector3& _krbtvec3NormalOnB, 
								  btScalar _distance, 
								  int _iLifeTime, 
								  const btVector3& _krbtvec3Colour) override;
	virtual void reportErrorWarning(const char* _pcWarningString) override;
	virtual void draw3dText(const btVector3& _krbtvec3Location, const char* _pcTextString) override;
	virtual void setDebugMode(int _iDebugMode) override;
	virtual int getDebugMode() const override;

	//virtual void drawSphere(const btVector3& p, btScalar radius, const btVector3& color) override;

	//virtual void drawAabb(const btVector3& from, const btVector3& to, const btVector3& color) override;

protected:
private:
	// Member Variables
public:
protected:
private:
	DebugDrawModes m_debugDrawModes; // Bullets enum for debug draw modes.
};

#endif	// __BULLETDEBUGDRAWER_H__