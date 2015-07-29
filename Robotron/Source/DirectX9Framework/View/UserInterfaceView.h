// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: UserInterfaceView.h
// Description	: CUserInterfaceView declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __USERINTERFACEVIEW_H__
#define __USERINTERFACEVIEW_H__

// Library Includes

// Local Includes
#include "../UserInterface/Button.h"
#include "../UserInterface/Menu.h"
#include "../UserInterface/StaticImage.h"
#include "../UserInterface/StaticText.h"
#include "../UserInterface/EditBox.h"

/************************************************************************/
/* CUserInterfaceView
/*
/* Base UI view implementation.
/* Wraps CBaseGUI and implements a simple stack behavior for GUI's.
/*
/* View classes should derive from this if they desire to have user 
/* interface functionality.
/************************************************************************/
class CUserInterfaceView : public CHumanView {
	// Member Functions
public:
	CUserInterfaceView();
	virtual ~CUserInterfaceView();

	virtual bool VOnRestore() override;
	virtual void VOnUpdate(float _fDeltaTime) override;
	virtual void VRender() override;
	virtual bool VOnMsgProc(const TAppMsg& _krMsg) override;
	virtual bool VLoadGame(const CIniDocument& _krIniDocument) override;	
protected:
	/**
	* Pushes a GUI onto the stack.
	* 
	* @author: 	Hayden Asplet
	* @param:	unique_ptr<CBaseGUI> _pGUI - GUI to push.
	* @return:  void
	*/
	void PushGUI(unique_ptr<CBaseGUI> _pGUI);

	/**
	* Request a pop that will take place next update tick.
	*
	* This is useful to call from inside a GUI event callback function.
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	void RequestPopGUI();


	/**
	* Immediately pops a GUI off the stack.
	*
	* DONOT call this from within an event callback fuction, instead use RequestPopGUI(). 
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	void PopGUI();

	/**
	* Clears the stack of GUI's.
	* 
	* @author: 	Hayden Asplet
	* @return:  void
	*/
	void ClearGUIStack();

	/**
	* Gets the top GUI element from the stack.
	* 
	* @author: 	Hayden Asplet
	* @return:  CBaseGUI* - the gui on the top of the stack.
	*/
	CBaseGUI* GetTopGUI();

	bool HasGUI();
private:
	// Member Variables
public:
protected:
private:
	std::vector<unique_ptr<CBaseGUI>> m_vecGUIStack;

	int m_iRequestPop; 
};

#endif	// __USERINTERFACEVIEW_H__