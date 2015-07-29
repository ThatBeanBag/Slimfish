// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: MainMenuView.h
// Description	: CMainMenuView declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MAINMENUVIEW_H__
#define __MAINMENUVIEW_H__

// Library Includes

// Local Includes
#include "..\DirectX9Framework\Network\ClientNetwork.h"

class CMainMenuView : public CUserInterfaceView {
	// Member Functions
public:
	CMainMenuView();
	virtual ~CMainMenuView();

	virtual bool VOnRestore() override;
	virtual void VOnUpdate(float _fDeltaTime) override;
	virtual void VRender() override;
	virtual bool VOnMsgProc(const TAppMsg& _krMsg) override;
	virtual bool VLoadGame(const CIniDocument& _krIniDocument) override;

	void MainGUIEventCallback(EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement);
	void HostGameGUIEventCallback(EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement);
	void ServerListGUIEventCallback(EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement);
	void UserNameGUIEVentCallback(EGUIEvent _eGUIEvent, AGUIElement* _pGUIElement);
protected:
private:

	// Member Variables
public:
protected:
private:
	TServerList m_serverList;
	int m_iServerSelection;
	CClientNetwork* m_pNetwork;
};

#endif	// __MAINMENUVIEW_H__