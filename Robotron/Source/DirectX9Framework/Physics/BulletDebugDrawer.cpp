// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BulletDebugDrawer.cpp
// Description	: CBulletDebugDrawer implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "BulletDebugDrawer.h"

// Local Includes
#include "BulletSDKPhysics.h" // For BulletConversions

CBulletDebugDrawer::CBulletDebugDrawer()
{
	int iDebugMode = btIDebugDraw::DBG_NoDebug;

	iDebugMode |= btIDebugDraw::DBG_DrawWireframe;
	iDebugMode |= btIDebugDraw::DBG_DrawContactPoints;
	iDebugMode |= btIDebugDraw::DBG_DrawNormals;
	iDebugMode |= btIDebugDraw::DBG_DrawAabb;
	iDebugMode |= btIDebugDraw::DBG_DrawFrames;

	setDebugMode(iDebugMode);
}

CBulletDebugDrawer::~CBulletDebugDrawer()
{

}

void CBulletDebugDrawer::drawLine(const btVector3& _krbtvec3From, const btVector3& _krbtvec3To, const btVector3& _krbtvec3Colour)
{
	// Convert the start and end point of the line to CVec3's.
	CVec3 vec3StartPoint = BulletConversions::BulletVector3ToVec3(_krbtvec3From);
	CVec3 vec3EndPoint = BulletConversions::BulletVector3ToVec3(_krbtvec3To);

	// Convert the colour.
	TColour colour;
	colour.m_a = 255;
	colour.m_r = static_cast<unsigned char>(_krbtvec3Colour.getX() * 255.0f);
	colour.m_g = static_cast<unsigned char>(_krbtvec3Colour.getY() * 255.0f);
	colour.m_b = static_cast<unsigned char>(_krbtvec3Colour.getZ() * 255.0f);

	g_pApp->GetRenderer()->VDrawLine(vec3StartPoint, vec3EndPoint, colour);
}

void CBulletDebugDrawer::drawContactPoint(const btVector3& _krbtvec3PointOnB, const btVector3& _krbtvec3NormalOnB, btScalar _distance, int _iLifeTime, const btVector3& _krbtvec3Colour)
{
	// Create a line to represent a normal.
	btVector3 btvec3LineStart = _krbtvec3PointOnB;
	btVector3 btvec3LineEnd = _krbtvec3PointOnB * _krbtvec3NormalOnB * _distance;

	// Draw the line.
	drawLine(btvec3LineStart, btvec3LineEnd, _krbtvec3Colour);
}

void CBulletDebugDrawer::reportErrorWarning(const char* _pcWarningString)
{
	DEBUG_WARNING(_pcWarningString);
}

void CBulletDebugDrawer::draw3dText(const btVector3& _krbtvec3Location, const char* _pcTextString)
{
	CVec3 vec3Location = BulletConversions::BulletVector3ToVec3(_krbtvec3Location);

	g_pApp->GetRenderer()->VDraw3DText(vec3Location, _pcTextString);
}

void CBulletDebugDrawer::setDebugMode(int _iDebugMode)
{
	m_debugDrawModes = static_cast<DebugDrawModes>(_iDebugMode);
}

int CBulletDebugDrawer::getDebugMode() const
{
	return m_debugDrawModes;
}
//
//void CBulletDebugDrawer::drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
//{
//	CVec3 vec3Position = BulletConversions::BulletVector3ToVec3(p);
//	TColour colour;
//	colour.m_a = 255;
//	colour.m_r = static_cast<unsigned char>(color.getX() * 255.0f);
//	colour.m_g = static_cast<unsigned char>(color.getY() * 255.0f);
//	colour.m_b = static_cast<unsigned char>(color.getZ() * 255.0f);
//
//	g_pApp->GetRenderer()->VDrawSphere(vec3Position, radius, 10, 10, colour);
//}
//
//void CBulletDebugDrawer::drawAabb(const btVector3& from, const btVector3& to, const btVector3& color)
//{
//	TColour colour;
//	colour.m_a = 255;
//	colour.m_r = static_cast<unsigned char>(color.getX() * 255.0f);
//	colour.m_g = static_cast<unsigned char>(color.getY() * 255.0f);
//	colour.m_b = static_cast<unsigned char>(color.getZ() * 255.0f);
//
//	CAABB aabb(BulletConversions::BulletVector3ToVec3(from), BulletConversions::BulletVector3ToVec3(to));
//	g_pApp->GetRenderer()->VDrawAABB(aabb, colour);
//}
