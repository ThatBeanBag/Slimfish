// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ControllerInterfaces.h
// Description	: Controller Interfaces declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __CONTROLLERINTERFACES_H__
#define __CONTROLLERINTERFACES_H__

class IKeyboardController {
	// Member Functions
public:
	virtual ~IKeyboardController() {};
	virtual bool VOnKeyDown(unsigned char _kcKey) = 0;
	virtual bool VOnKeyUp(unsigned char _kcKey) = 0;
protected:
private:
	// Member Variables
public:
protected:
private:
};

enum EMouseButton {
	MBUTTON_LEFT,
	MBUTTON_MIDDLE,
	MBUTTON_RIGHT
};

class IMouseController {

	// Member Functions
public:
	virtual ~IMouseController() {};
	virtual bool VOnMouseMove(const CPoint& _krNewPosition) = 0;
	virtual bool VOnMouseButtonDown(const CPoint& _krPosition, EMouseButton _eMouseButton) = 0;
	virtual bool VOnMouseButtonUp(const CPoint& _krPosition, EMouseButton _eMouseButton) = 0;
	virtual bool VOnMouseWheel(int _iDeltaWheel) = 0;
protected:
private:
	// Member Variables
public:
protected:
private:
};

class IGamepadController {
	// Member Functions
public:
	virtual ~IGamepadController() {};

	virtual bool VOnButtonDown(EGamepadButton _eButton) = 0;
	virtual bool VOnButtonUp(EGamepadButton _eButton) = 0;
	virtual bool VOnTrigger(EGamepadTrigger _eTrigger, const float fPressure) = 0;
	virtual bool VOnThumbstick(EGamepadThumbstick _eThumbstick, short _sX, short _sY) = 0;
protected:
private:
	// Member Variables
public:
protected:
private:
};


#endif	// __CONTROLLERINTERFACES_H__