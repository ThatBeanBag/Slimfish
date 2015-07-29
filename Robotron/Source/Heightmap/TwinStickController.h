// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TwinStickController.h
// Description	: CTwinStickController declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __TWINSTICKCONTROLLER_H__
#define __TWINSTICKCONTROLLER_H__

// Library Includes

// Local Includes

class CTwinStickController : public IMouseController, public IKeyboardController, public IGamepadController {
	// Member Functions
public:
	CTwinStickController(shared_ptr<CSceneNode> _pObject, shared_ptr<CCameraNode> _pCamera);
	virtual ~CTwinStickController();

	void OnUpdate(float _fDeltaTime);
	bool OnRestore();

	void Render();

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
	void Move();

	// Member Variables
public:
protected:
private:
	CMatrix4x4 m_matTransform;

	shared_ptr<CSceneNode> m_pObject;
	shared_ptr<CCameraNode> m_pCamera;

	CPoint m_cursorPosition;
	CVec3 m_vec3MousePosition;		// Mouse position in 3D space, just for debugging.
	bool m_bCursorPositionChanged;	// Has the cursor moved this tick?
	bool m_bMoved;					// Have we moved this tick?
	bool m_bIsUsingController;

	std::vector<bool> m_vecDownKeys;
};

#endif	// __TWINSTICKCONTROLLER_H__