// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: FlyingController.cpp
// Description	: CFlyingController implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "FlyingController.h"

// Local Includes

CFlyingController::CFlyingController()
	:m_matTransform(CMatrix4x4::s_kIdentity),
	m_matFromWorld(CMatrix4x4::s_kIdentity),
	m_pObject(nullptr),
	m_fPitch(0.0f),
	m_fDeltaPitch(0.0f),
	m_fMaxPitch(ToRadians(89.0f)),
	m_fYaw(0.0f),
	m_fDeltaYaw(0.0f),
	m_fSpeed(1.0f),
	m_fSensitivity(0.005f),
	m_bInvertYAxis(false),
	m_vecDownKeys(255, false),
	m_bClickToRotate(false)
{

}

CFlyingController::CFlyingController(float _fMaxPitch,
									 float _fSensitivity,
									 float _fSpeed,
									 bool _bInvertYAxis,
									 bool _bClickToRotate,
									 shared_ptr<CSceneNode> _pObject)
	:m_pObject(nullptr),
	m_fPitch(0.0f),
	m_fDeltaPitch(0.0f),
	m_fMaxPitch(_fMaxPitch),
	m_fYaw(0.0f),
	m_fDeltaYaw(0.0f),
	m_fSpeed(_fSensitivity),
	m_fSensitivity(_fSpeed),
	m_bInvertYAxis(_bInvertYAxis),
	m_vecDownKeys(255, false),
	m_bClickToRotate(_bClickToRotate)
{
	SetObject(_pObject);
}

CFlyingController::~CFlyingController()
{

}


void CFlyingController::OnUpdate(float _fDeltaTime)
{
	if (m_bInvertYAxis) {
		m_fDeltaPitch = -m_fDeltaPitch;
	}

	CVec3 vec3Foward(0.0f, 0.0f, 0.0f);
	CVec3 vec3Right(0.0f, 0.0f, 0.0f);
	CVec3 vec3Up(0.0f, 0.0f, 0.0f);

	if ((m_vecDownKeys['W'] && !m_vecDownKeys['S']) ||
		(m_vecDownKeys[VK_NUMPAD8] && !m_vecDownKeys[VK_NUMPAD5])) {
		vec3Foward = m_matTransform.GetForward();
	}
	else if ((m_vecDownKeys['S'] && !m_vecDownKeys['W']) ||
			 (m_vecDownKeys[VK_NUMPAD5] && !m_vecDownKeys[VK_NUMPAD8])) {
		vec3Foward = -m_matTransform.GetForward();
	}

	if (m_vecDownKeys['D'] && !m_vecDownKeys['A']) {
		vec3Right = m_matTransform.GetRight();
	}
	else if (m_vecDownKeys['A'] && !m_vecDownKeys['D']) {
		vec3Right = -m_matTransform.GetRight();
	}

	if (m_vecDownKeys[' '] && !m_vecDownKeys[VK_CONTROL]) {
		vec3Up = g_kvec3Up;
	}
	else if (m_vecDownKeys[VK_CONTROL] && !m_vecDownKeys[' ']) {
		vec3Up = -g_kvec3Up;
	}

	if (m_leftThumbStick.GetX() > 0) {
		vec3Right = m_matTransform.GetRight();
	}
	else if (m_leftThumbStick.GetX() < 0) {
		vec3Right = -m_matTransform.GetRight();
	}

	if (m_leftThumbStick.GetY() > 0) {
		vec3Foward = m_matTransform.GetForward();
	}
	else if (m_leftThumbStick.GetY() < 0) {
		vec3Foward = -m_matTransform.GetForward();
	}

	if (m_rightThumbStick.GetX() != 0 || m_rightThumbStick.GetY() != 0) {
		m_fDeltaPitch = m_fSensitivity * m_rightThumbStick.GetY();
		m_fDeltaYaw = m_fSensitivity * m_rightThumbStick.GetX();
	}

	m_fYaw += m_fDeltaYaw;
	m_fPitch += m_fDeltaPitch;

	if (m_fPitch > m_fMaxPitch) {
		m_fPitch = m_fMaxPitch;
	}
	else if(m_fPitch < -m_fMaxPitch) {
		m_fPitch = -m_fMaxPitch;
	}

	CVec3 vec3Direction = vec3Foward + vec3Right + vec3Up;
	vec3Direction.Normalize();
	vec3Direction *= m_fSpeed * _fDeltaTime;
	CVec3 vec3Position = vec3Direction + m_matTransform.GetPosition();

	CMatrix4x4 matScale = BuildScale(m_vec3Scale);
	CMatrix4x4 matRotation = BuildYawPitchRoll(m_fYaw, m_fPitch, 0);
	CMatrix4x4 matTranslation = BuildTranslation(vec3Position);

	m_matTransform = matScale * matRotation * matTranslation;
	m_pObject->SetTransform(m_matTransform);

	// Set the delta's back to 0.
	m_fDeltaYaw = 0.0f;
	m_fDeltaPitch = 0.0f;

	if (!m_bClickToRotate && !g_pApp->IsAppMinimized()) {
		POINT cursorPos;
		GetCursorPos(&cursorPos);

		CPoint deltaPosition = g_pApp->GetScreenCentre();
		deltaPosition.m_iX -= cursorPos.x;
		deltaPosition.m_iY -= cursorPos.y;

		m_fDeltaYaw = -deltaPosition.m_iX * m_fSensitivity;
		m_fDeltaPitch = -deltaPosition.m_iY * m_fSensitivity;

		SetCursorToCentre();
	}
}

bool CFlyingController::OnRestore()
{
	if (!m_bClickToRotate) {
		g_pApp->HideCursor(true);
		SetCursorToCentre();
	}

	return true;
}

void CFlyingController::SetTransform(const CMatrix4x4& _krMatrix)
{
	m_matTransform = _krMatrix;

	// Obtain the pitch and yaw from the object's transform.
	CVec3 vec3Direction = m_matTransform.GetForward();
	vec3Direction.Normalize();

	float fLengthOnXZ = sqrtf(vec3Direction.z * vec3Direction.z + vec3Direction.x * vec3Direction.x);

	m_fPitch = -atan2f(vec3Direction.y, fLengthOnXZ);
	m_fYaw = atan2f(vec3Direction.x, vec3Direction.z);

	// Set the scale.
	m_vec3Scale = m_matTransform.GetScale();
}

void CFlyingController::SetObject(shared_ptr<CSceneNode> _pObject)
{
	m_pObject = _pObject;
	if (m_pObject) {
		// Set the transform to the object's transform.
		SetTransform(_pObject->GetTransformMatrix());
	}
}

void CFlyingController::SetMaxPitch(float _fPitch)
{
	m_fMaxPitch = _fPitch;
}

void CFlyingController::SetSensitivity(float _fSensitivity)
{
	m_fSensitivity = _fSensitivity;
}

void CFlyingController::SetSpeed(float _fSpeed)
{
	m_fSpeed = _fSpeed;
}

void CFlyingController::SetInvertYAxis(bool _bInvert)
{
	m_bInvertYAxis = _bInvert;
}

void CFlyingController::SetClickToRotate(bool _bClickToRotate)
{
	m_bClickToRotate = _bClickToRotate;
	if (m_bClickToRotate) {
		g_pApp->HideCursor(false);
	}
	else {
		OnRestore();
	}
}

const CMatrix4x4* CFlyingController::GetTransform()
{
	return &m_matTransform;
}

const CMatrix4x4* CFlyingController::GetFromWorldTransform()
{
	return &m_matFromWorld;
}

shared_ptr<CSceneNode> CFlyingController::GetSceneObject()
{
	return m_pObject;
}

bool CFlyingController::GetClickToRotate() const
{
	return m_bClickToRotate;
}

bool CFlyingController::VOnKeyDown(unsigned char _kcKey)
{
	m_vecDownKeys[_kcKey] = true;

	return false;
}

bool CFlyingController::VOnKeyUp(unsigned char _kcKey)
{
	m_vecDownKeys[_kcKey] = false;

	return false;
}

bool CFlyingController::VOnMouseMove(const CPoint& _krNewPosition)
{
	if (m_bClickToRotate) {
	// Mode 1. Rotate the object only when the left mouse is down.
		if (m_bIsLMBDown && m_lastMousePosition != _krNewPosition) {
		// Is the left mouse button down and is the new position different from the last?
			CPoint deltaPosition = m_lastMousePosition - _krNewPosition;

			m_fDeltaYaw -= deltaPosition.m_iX * m_fSensitivity;
			m_fDeltaPitch -= deltaPosition.m_iY * m_fSensitivity;
		}

		m_lastMousePosition = _krNewPosition;

		return true;
	}
	else {
	// Mode 2. Rotate the view whenever the mouse moves away from the centre of the screen.
	// Reset the mouse position back to the centre every tick.
	}

	return false;
}

bool CFlyingController::VOnMouseButtonDown(const CPoint& _krPosition, EMouseButton _eMouseButton)
{
	if (_eMouseButton == MBUTTON_LEFT) {
		m_bIsLMBDown = true;
	}

	return false;
}

bool CFlyingController::VOnMouseButtonUp(const CPoint& _krPosition, EMouseButton _eMouseButton)
{
	if (_eMouseButton == MBUTTON_LEFT) {
		m_bIsLMBDown = false;
	}

	return false;
}

bool CFlyingController::VOnMouseWheel(int _iDeltaWheel)
{
	return false;
}

bool CFlyingController::VOnButtonDown(EGamepadButton _eButton)
{
	return false;
}

bool CFlyingController::VOnButtonUp(EGamepadButton _eButton)
{
	return false;
}

bool CFlyingController::VOnTrigger(EGamepadTrigger _eTrigger, const float fPressure)
{
	return false;
}

bool CFlyingController::VOnThumbstick(EGamepadThumbstick _eThumbstick, short _sX, short _sY)
{
	if (_eThumbstick == GTHUMB_LEFT) {
		m_leftThumbStick = CPoint(_sX, _sY);
	}
	else {
		m_rightThumbStick = CPoint(_sX, _sY);
	}

	return false;
}

void CFlyingController::SetCursorToCentre()
{
	if (!g_pApp->IsAppMinimized()) {
	// Are we not minimized?
		CPoint centrePosition = g_pApp->GetScreenCentre();

		SetCursorPos(centrePosition.m_iX, centrePosition.m_iY);
	}
}
