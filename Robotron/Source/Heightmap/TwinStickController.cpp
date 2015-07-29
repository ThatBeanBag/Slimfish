// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TwinStickController.cpp
// Description	: CTwinStickController implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "RobotronStd.h"

// Library Includes

// This Include
#include "TwinStickController.h"

// Local Includes
#include "..\DirectX9Framework\Actor\PhysicsComponent.h"
#include "EventData_ChangeDirection.h"
#include "EventData_FireBullet.h"

CTwinStickController::CTwinStickController(shared_ptr<CSceneNode> _pObject, shared_ptr<CCameraNode> _pCamera)
	:m_pObject(_pObject),
	m_pCamera(_pCamera),
	m_vecDownKeys(256, false)
{
	m_matTransform = _pObject->GetTransformMatrix();

	m_pCamera->SetTarget(_pObject);
	m_pCamera->SetOffsetFromTarget(0.0f, 0.0f, -20.0f);
	m_pCamera->SetRotationFromTarget(ToRadians(35.264f), ToRadians(45.0f), 0.0f);
	m_pCamera->SetAbsoluteRotation(true);
}

CTwinStickController::~CTwinStickController()
{

}

void CTwinStickController::OnUpdate(float _fDeltaTime)
{
	if (m_matTransform != m_pObject->GetTransformMatrix()) {
		m_matTransform = m_pObject->GetTransformMatrix();
		m_bMoved = true;
	}

	if (m_bCursorPositionChanged || (m_bMoved && !m_bIsUsingController)) {
		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(g_pApp->GetHwnd(), &cursor);

		if (cursor.x != m_cursorPosition.m_iX ||
			cursor.y != m_cursorPosition.m_iY) {
			m_cursorPosition.m_iX = cursor.x;
			m_cursorPosition.m_iY = cursor.y;
		}
		else {
			return;
		}

		m_bIsUsingController = false;
		CMatrix4x4 matProj = m_pCamera->GetProjectionMatrix();

		CVec3 v;
		v.x = (((2.0f * m_cursorPosition.m_iX) / static_cast<float>(g_pApp->GetScreenWidth())) - 1) / matProj._11;
		v.y = -(((2.0f * m_cursorPosition.m_iY) / static_cast<float>(g_pApp->GetScreenHeight())) - 1) / matProj._22;
		v.z = 1.0f;

		CVec3 vec3PickRayOrigin;
		CVec3 vec3PickRayDirection;
		m_pCamera->SetViewTransform();
		CMatrix4x4 m = m_pCamera->GetTransformMatrix();

		// Transform the ray cast by the camera's matrix.
		vec3PickRayDirection.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
		vec3PickRayDirection.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
		vec3PickRayDirection.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
		vec3PickRayOrigin.x = m._41;
		vec3PickRayOrigin.y = m._42;
		vec3PickRayOrigin.z = m._43;

		vec3PickRayDirection.Normalize();

		float fT = 0.0f;
		CVec3 vec3Position = m_matTransform.GetPosition();

		// Get the intersection point of the ray at the Y value of the player.
		if (vec3PickRayDirection.y != 0.0f) {
			fT = (vec3Position.y - vec3PickRayOrigin.y) / vec3PickRayDirection.y;
		}

		// Get the mouse position in 3D space as it intersects with the Y of the player
		m_vec3MousePosition = CVec3(vec3PickRayOrigin.x + vec3PickRayDirection.x * fT,
									vec3Position.y,
									vec3PickRayOrigin.z + vec3PickRayDirection.z * fT);

		CVec3 vec3Forward = m_vec3MousePosition - vec3Position;
		CMatrix4x4 matRotation(CMatrix4x4::s_kIdentity);
		if (vec3Forward != CVec3(0, 0, 0)) {
			vec3Forward.Normalize();
			matRotation = BuildRotationDirection(vec3Forward);
		}

		shared_ptr<CEventData_ChangeDirection> pEvent(new CEventData_ChangeDirection(m_pObject->GetActorID(), vec3Forward));
		CEventManager::GetInstance()->QueueEvent(pEvent);
	}

	m_bCursorPositionChanged = false;
	m_bMoved = false;
}

bool CTwinStickController::OnRestore()
{
	return true;
}

void CTwinStickController::Render()
{
	//g_pApp->GetRenderer()->VDrawSphere(m_vec3MousePosition, 0.1f, 10, 10, g_kCOLOUR_RED);
}

bool CTwinStickController::VOnKeyDown(unsigned char _kcKey)
{
	if (_kcKey != 'W' && _kcKey != 'S' && _kcKey != 'A' && _kcKey != 'D') {
		return false;
	}

	m_vecDownKeys[_kcKey] = true;

	Move();

	return true;
}

bool CTwinStickController::VOnKeyUp(unsigned char _kcKey)
{
	if (_kcKey != 'W' && _kcKey != 'S' && _kcKey != 'A' && _kcKey != 'D') {
		return false;
	}

	m_vecDownKeys[_kcKey] = false;

	Move();

	return true;
}

bool CTwinStickController::VOnMouseMove(const CPoint& _krNewPosition)
{
	m_bCursorPositionChanged = true;

	return false;
}

bool CTwinStickController::VOnMouseButtonDown(const CPoint& _krPosition, EMouseButton _eMouseButton)
{
	if (_eMouseButton == MBUTTON_LEFT) {
		shared_ptr<CEventData_FireBullet> pEvent(new CEventData_FireBullet(m_pObject->GetActorID()));
		CEventManager::GetInstance()->QueueEvent(pEvent);
	}

	return false;
}

bool CTwinStickController::VOnMouseButtonUp(const CPoint& _krPosition, EMouseButton _eMouseButton)
{
	return false;
}

bool CTwinStickController::VOnMouseWheel(int _iDeltaWheel)
{
	return false;
}

void CTwinStickController::Move()
{
	static const float s_kfVelocity = 1.0f;

	CVec3 vec3Forward = m_pCamera->GetTransformMatrix().GetForward();
	CVec3 vec3Right = m_pCamera->GetTransformMatrix().GetRight();

	vec3Right.y = 0;
	vec3Forward.y = 0;

	CVec3 vec3Move;

	if (m_vecDownKeys['D']) {
		vec3Move += vec3Right;
	}

	if (m_vecDownKeys['A']) {
		vec3Move -= vec3Right;
	}

	if (m_vecDownKeys['W']) {
		vec3Move += vec3Forward;
	}

	if (m_vecDownKeys['S']) {
		vec3Move -= vec3Forward;
	}

	if (vec3Move == CVec3(0, 0, 0)) {
		shared_ptr<CEventData_StopMoving> pEvent(new CEventData_StopMoving(m_pObject->GetActorID()));
		CEventManager::GetInstance()->QueueEvent(pEvent);
		return;
	}

	vec3Move.Normalize();

	shared_ptr<CEventData_StartMoving> pEvent(new CEventData_StartMoving(m_pObject->GetActorID(), vec3Move, g_kfPLAYER_ACCELERATION, g_kfPLAYER_MAX_VELOCITY));
	CEventManager::GetInstance()->QueueEvent(pEvent);
}

bool CTwinStickController::VOnButtonDown(EGamepadButton _eButton)
{
	if (_eButton == GBUTTON_A) {
		shared_ptr<CEventData_FireBullet> pEvent(new CEventData_FireBullet(m_pObject->GetActorID()));
		CEventManager::GetInstance()->QueueEvent(pEvent);
	}
	else if (_eButton == GBUTTON_RIGHT_SHOULDER) {
		shared_ptr<CEventData_FireBullet> pEvent(new CEventData_FireBullet(m_pObject->GetActorID()));
		CEventManager::GetInstance()->QueueEvent(pEvent);
	}

	return false;
}

bool CTwinStickController::VOnButtonUp(EGamepadButton _eButton)
{

	return false;
}

bool CTwinStickController::VOnTrigger(EGamepadTrigger _eTrigger, const float fPressure)
{
	if (_eTrigger == GTRIGGER_RIGHT && fPressure >= 255) {
		shared_ptr<CEventData_FireBullet> pEvent(new CEventData_FireBullet(m_pObject->GetActorID()));
		CEventManager::GetInstance()->QueueEvent(pEvent);
	}

	return false;
}

bool CTwinStickController::VOnThumbstick(EGamepadThumbstick _eThumbstick, short _sX, short _sY)
{
	if (_eThumbstick == GTHUMB_LEFT) {
		if (_sX == 0 && _sY == 0) {
			shared_ptr<CEventData_StopMoving> pEvent(new CEventData_StopMoving(m_pObject->GetActorID()));
			CEventManager::GetInstance()->QueueEvent(pEvent);
			return true;
		}

		CVec3 vec3Forward = m_pCamera->GetTransformMatrix().GetForward();
		CVec3 vec3Right = m_pCamera->GetTransformMatrix().GetRight();

		CVec3 vec3Move = _sY * vec3Forward;
		vec3Move += _sX * vec3Right;
		vec3Move.y = 0;

		vec3Move.Normalize();

		shared_ptr<CEventData_StartMoving> pEvent(new CEventData_StartMoving(m_pObject->GetActorID(), vec3Move, g_kfPLAYER_ACCELERATION, g_kfPLAYER_MAX_VELOCITY));
		CEventManager::GetInstance()->QueueEvent(pEvent);
		return true;
	}
	else {
		m_bIsUsingController = true;

		if (_sX == 0 && _sY == 0) {
			return true;
		}

		CVec3 vec3LookDirection(_sX, 0.0f, _sY);

		CVec3 vec3Forward = m_pCamera->GetTransformMatrix().GetForward();
		CVec3 vec3Right = m_pCamera->GetTransformMatrix().GetRight();

		CVec3 vec3Move = _sY * vec3Forward;
		vec3Move += _sX * vec3Right;
		vec3Move.y = 0;
		vec3Move.Normalize();

		shared_ptr<CEventData_ChangeDirection> pEvent(new CEventData_ChangeDirection(m_pObject->GetActorID(), vec3Move));
		CEventManager::GetInstance()->QueueEvent(pEvent);
		return true;
	}

	return false;
}
