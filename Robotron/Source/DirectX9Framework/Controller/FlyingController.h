// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: FlyingController.h
// Description	: CFlyingController declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __FLYINGCONTROLLER_H__
#define __FLYINGCONTROLLER_H__

// Library Includes

// Local Includes
#include "../GameStd.h"	// REMOVE: this.
#include "ControllerInterfaces.h"

class CFlyingController :public IMouseController, public IKeyboardController, public IGamepadController {
	// Member Functions
public:
	CFlyingController();
	CFlyingController(float _fMaxPitch, 
					  float _fSensitivity, 
					  float _fSpeed, 
					  bool _bInvertYAxis = false, 
					  bool _bClickToRotate = false, 
					  shared_ptr<CSceneNode> _pObject = nullptr);
	virtual ~CFlyingController();

	void OnUpdate(float _fDeltaTime);
	bool OnRestore();
	
	void SetTransform(const CMatrix4x4& _krMatrix);
	void SetObject(shared_ptr<CSceneNode> _pObject);
	void SetMaxPitch(float _fPitch);
	void SetSensitivity(float _fSensitivity);
	void SetSpeed(float _fSpeed);
	void SetInvertYAxis(bool _bInvert);
	void SetClickToRotate(bool _bClickToRotate);

	// Gets the transform of the object, if there is no object it will return m_matTransform.
	const CMatrix4x4* GetTransform();
	const CMatrix4x4* GetFromWorldTransform();
	shared_ptr<CSceneNode> GetSceneObject();
	bool GetClickToRotate() const;

	// Interface functions
	virtual bool VOnKeyDown(unsigned char _kcKey);
	virtual bool VOnKeyUp(unsigned char _kcKey);
	virtual bool VOnMouseMove(const CPoint& _krNewPosition);
	virtual bool VOnMouseButtonDown(const CPoint& _krPosition, EMouseButton _eMouseButton);
	virtual bool VOnMouseButtonUp(const CPoint& _krPosition, EMouseButton _eMouseButton);
	virtual bool VOnMouseWheel(int _iDeltaWheel);
	virtual bool VOnButtonDown(EGamepadButton _eButton) override;
	virtual bool VOnButtonUp(EGamepadButton _eButton) override;
	virtual bool VOnTrigger(EGamepadTrigger _eTrigger, const float fPressure) override;
	virtual bool VOnThumbstick(EGamepadThumbstick _eThumbstick, short _sX, short _sY) override;
protected:
private:
	void SetCursorToCentre();


	// Member Variables
public:
protected:
	// Object
	CMatrix4x4 m_matTransform;
	CMatrix4x4 m_matFromWorld;	// Useful for cameras, instead of inverting we can transpose rotation only matrices
								// and flip the signs of translation only matrices.

	shared_ptr<CSceneNode> m_pObject;	// If nullptr, the controller is not attached to an object and 
										// therefore only acts on the transformation matrix.
	CVec3 m_vec3Scale;		// The scale of the target, this needs to be keeped in order to maintain the scale,
							// of the target each update.

	// Orientation
	float m_fPitch;
	float m_fDeltaPitch;
	float m_fMaxPitch;
	float m_fYaw;
	float m_fDeltaYaw;
	float m_fSpeed;			// The translation speed.
	float m_fSensitivity;	// How sensitive mouse movements are to rotation.

	// Input
	CPoint m_lastMousePosition;	// The last position the mouse cursor was in, used for calculating delta position.
	std::vector<bool> m_vecDownKeys;	// The states of the keys on keyboard, useful for press and hold events.
	bool m_bClickToRotate;		// True if the controller should only rotate when the left mouse button is down,
								// false if the controller should rotate always based on delta mouse position.
								// If this is false the mouse position will reset back to the centre OnUpdate().
	bool m_bIsLMBDown;		// True if the left mouse button is down, false otherwise.
	bool m_bInvertYAxis;	// True if the Y axis should be inverted, false otherwise.
	bool m_bIgnoreNextMouseMove;	// True if the next mouse move event shouldn't be handled, this is used so
	CPoint m_leftThumbStick;
	CPoint m_rightThumbStick;
	// that we can set the mouse position to the centre without it rotating.
private:
};

#endif	// __FLYINGCONTROLLER_H__