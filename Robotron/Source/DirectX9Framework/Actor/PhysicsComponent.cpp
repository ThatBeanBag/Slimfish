// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: PhysicsComponent.cpp
// Description	: CPhysicsComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "PhysicsComponent.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"
#include "..\Physics\CollisionSphere.h"
#include "..\Physics\CollisionBox.h"
#include "..\Physics\CollisionCapsule.h"
#include "..\Physics\CollisionPlane.h"
#include "..\Physics\CollisionShape.h"

const std::string CPhysicsComponent::s_kstrNAME = "PhysicsComponent";

CPhysicsComponent::CPhysicsComponent()
	:m_pGamePhysics(nullptr),
	m_fAcceleration(0.0f),
	m_fAngularAcceleration(0.0f),
	m_fMaxVelocity(0.0f),
	m_fMaxAngularVelocity(0.0f),
	m_pShapeXMLData(nullptr),
	m_fDensity(0.0f),
	m_fRestitution(0.0f),
	m_fFriction(0.0f)
{

}

CPhysicsComponent::~CPhysicsComponent()
{
	if (m_pGamePhysics) {
		m_pGamePhysics->VRemoveActor(GetOwnerID());
		m_pGamePhysics = nullptr;
	}
}

bool CPhysicsComponent::VInitialise(TiXmlElement* _pXmlData)
{
	if (g_pApp->GetGame()->IsRemote()) {
		// Server handles physics.
		return true;
	}

	m_pGamePhysics = g_pApp->GetGame()->GetGamePhysics();
	if (!m_pGamePhysics) {
	// Is there a physics engine for the game?
		// Can't have a physics component without any game physics.
		DEBUG_ERROR("Can't have a physics component without any game physics.");
		return false;
	}

	// Get the density of the rigid body.
	TiXmlElement* pDensity = _pXmlData->FirstChildElement("Density");
	if (pDensity && pDensity->FirstChild()) {
		std::string strValue = pDensity->FirstChild()->Value();
		m_fDensity = static_cast<float>(atof(strValue.c_str()));
	}

	// Get the restitution of the rigid body.
	TiXmlElement* pRestitution = _pXmlData->FirstChildElement("Restitution");
	if (pRestitution && pDensity->FirstChild()) {
		std::string strValue = pRestitution->FirstChild()->Value();
		m_fRestitution = static_cast<float>(atof(strValue.c_str()));
	}

	// Get the friction of the rigid body.
	TiXmlElement* pFriction = _pXmlData->FirstChildElement("Friction");
	if (pFriction && pDensity->FirstChild()) {
		std::string strValue = pFriction->FirstChild()->Value();
		m_fFriction = static_cast<float>(atof(strValue.c_str()));
	}

	TiXmlElement* pNoCollide = _pXmlData->FirstChildElement("NoCollide");
	if (pNoCollide) {
		m_bIsNoCollide = true;
	}
	else {
		m_bIsNoCollide = false;
	}

	// Get the shape of the rigid body.
	TiXmlElement* pRigidBodyShape = _pXmlData->FirstChildElement("Shape");
	if (pRigidBodyShape) {
		TiXmlElement* pShapeData = pRigidBodyShape->FirstChildElement();

		if (pShapeData) {
			m_strRigidBodyShape = pShapeData->Value();
			m_pShapeXMLData = pShapeData;
		}
		else {
			std::string strError = _pXmlData->GetDocument()->Value();
			strError = "No shape data for physics component: \"" + strError + "\"";
			DEBUG_ERROR(strError);
			return false;
		}
	}
	else {
		std::string strError = _pXmlData->GetDocument()->Value();
		strError = "Couldn't find \"Shape\" element when initialising physics component: \"" + strError + "\"";
		DEBUG_ERROR(strError);
		return false;
	}


	TiXmlElement* pRigidBodyTransform = _pXmlData->FirstChildElement("RigidBodyTransform");
	if (pRigidBodyTransform) {
		// Get the rigid body's transform from the XML data.
		m_matRelativeRigidBodyTransform = CreateTransformFromXML(pRigidBodyTransform);
	}
	else {
		m_matRelativeRigidBodyTransform = CMatrix4x4::s_kIdentity;
	}

	return true;
}

void CPhysicsComponent::VPostInitialise()
{
	if (!m_pGamePhysics) {
		return;
	}

	if (m_strRigidBodyShape == CCollisionSphere::s_kstrNAME) {
		CCollisionSphere sphere;
		CreateShape(sphere);

		m_pGamePhysics->VAddSphere(GetOwner(), sphere);
	}
	else if (m_strRigidBodyShape == CCollisionBox::s_kstrNAME) {
		CCollisionBox box;
		CreateShape(box);

		m_pGamePhysics->VAddBox(GetOwner(), box);
	}
	else if (m_strRigidBodyShape == CCollisionCapsule::s_kstrNAME) {
		CCollisionCapsule capsule;
		CreateShape(capsule);

		m_pGamePhysics->VAddCapsule(GetOwner(), capsule);
	}
	else if (m_strRigidBodyShape == CCollisionPlane::s_kstrNAME) {
		CCollisionPlane plane;
		CreateShape(plane);

		m_pGamePhysics->VAddPlane(GetOwner(), plane);
	}
	else {
		std::string strError = m_pShapeXMLData->GetDocument()->Value();
		strError = "Failed to add physics actor to physics system, no appropriate shape specified: \""  + strError + "\"";
		DEBUG_ERROR(strError);
		return;
	}

	if (m_bIsNoCollide) {
		SetCollisionsEnabled(false);
	}

	//SetAngularFactor(CVec3(0, 1, 0));
}

void CPhysicsComponent::VUpdate(float _fDeltaTime)
{
	if (!m_pGamePhysics) {
		return;
	}

	if (m_fAcceleration != 0 && m_bIsOnGround) {
		CVec3 vec3CurrentVelocity = GetVelocity();
		float fSpeed = vec3CurrentVelocity.GetLength();
		if (fSpeed >= m_fMaxVelocity) {
			return;
		}

		SetAngularVelocity(CVec3());

		float fAccelerationToApply = m_fAcceleration * _fDeltaTime;
		ApplyForce(m_vec3AcceleratingDirection, fAccelerationToApply);
	}
}

void CPhysicsComponent::ApplyForce(const CVec3& _krvec3Direction, float _fNewtonForce)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VApplyForce(GetOwnerID(), _krvec3Direction, _fNewtonForce);
}

void CPhysicsComponent::ApplyImpulse(const CVec3& _krvec3Direction, float _fNewtonForce)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VApplyImpulse(GetOwnerID(), _krvec3Direction, _fNewtonForce);
}

void CPhysicsComponent::ApplyTorque(const CVec3& _krvec3Direction, float _fNewtonForce)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VApplyTorque(GetOwnerID(), _krvec3Direction, _fNewtonForce);
}

void CPhysicsComponent::SetCollisionsEnabled(bool _bCollisionEnable)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VSetCollisionsEnabled(GetOwnerID(), _bCollisionEnable);
}

float CPhysicsComponent::GetMass()
{
	if (!m_pGamePhysics) {
		return 0.0f;
	}

	return m_pGamePhysics->VGetMass(GetOwnerID());
}

void CPhysicsComponent::ApplyAcceleration(const CVec3& _krVec3Direction, float fAcceleration, float _fMaxVelocity)
{
	m_vec3AcceleratingDirection = _krVec3Direction;
	m_fAcceleration = fAcceleration;
	m_fMaxVelocity = _fMaxVelocity;
}

void CPhysicsComponent::ApplyAcceleration(float fAcceleration, float _fMaxVelocity)
{
	m_fAcceleration = fAcceleration;
	m_fMaxVelocity = _fMaxVelocity;
}

void CPhysicsComponent::RemoveAcceleration()
{
	m_fAcceleration = 0.0f;
}

void CPhysicsComponent::ApplyAngularAcceleration(float _fAcceleration, float _fMaxAngularVelocity)
{
	m_fAngularAcceleration = _fAcceleration;
	m_fMaxAngularVelocity = _fMaxAngularVelocity;
}

void CPhysicsComponent::RemoveAngularAcceleration()
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VSetAngularVelocity(GetOwnerID(), CVec3(0.0f, 0.0f, 0.0f));
}

void CPhysicsComponent::SetVelocity(const CVec3& _krvec3Velocity)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VSetVelocity(GetOwnerID(), _krvec3Velocity);
}

void CPhysicsComponent::SetAngularVelocity(const CVec3& _krvec3Velocity)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VSetAngularVelocity(GetOwnerID(), _krvec3Velocity);
}

const CVec3 CPhysicsComponent::GetVelocity()
{
	if (!m_pGamePhysics) {
		return CVec3();
	}

	return m_pGamePhysics->VGetVelocity(GetOwnerID());
}

const CVec3 CPhysicsComponent::GetAngularVelocity()
{
	if (!m_pGamePhysics) {
		return CVec3();
	}

	return m_pGamePhysics->VGetAngularVelocity(GetOwnerID());
}

void CPhysicsComponent::Stop()
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VStopActor(GetOwnerID());
}

void CPhysicsComponent::SetLinearFactor(const CVec3& _krvec3LinearFactor)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VSetLinearFactor(GetOwnerID(), _krvec3LinearFactor);
}

void CPhysicsComponent::SetAngularFactor(const CVec3& _krvec3AngularFactor)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VSetAngularFactor(GetOwnerID(), _krvec3AngularFactor);
}

void CPhysicsComponent::SetOnGround(bool _bIsOnGround)
{
	m_bIsOnGround = _bIsOnGround;
}

bool CPhysicsComponent::IsOnGround() const
{
	return m_bIsOnGround;
}

CMatrix4x4 CPhysicsComponent::GetTransform()
{
	if (!m_pGamePhysics) {
		return CMatrix4x4::s_kIdentity;
	}

	return m_pGamePhysics->VGetTransform(GetOwnerID());
}

void CPhysicsComponent::SetTransform(const CMatrix4x4& _krmatTransform)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VSetTransform(GetOwnerID(), _krmatTransform);
}

void CPhysicsComponent::Rotate(const CVec3& _krvec3Direction)
{
	if (!m_pGamePhysics) {
		return;
	}

	m_pGamePhysics->VRotate(GetOwnerID(), _krvec3Direction);
}

void CPhysicsComponent::CreateShape(ACollisionShape& _rShape)
{
	_rShape.VInitializeFromXML(m_pShapeXMLData);
	_rShape.SetRelativeTransform(m_matRelativeRigidBodyTransform);
	_rShape.SetDensity(m_fDensity);
	_rShape.SetFriction(m_fFriction);
	_rShape.SetRestitution(m_fRestitution);
}

bool CPhysicsComponent::VIsActiveOnRemote() const
{
	return false;
}

