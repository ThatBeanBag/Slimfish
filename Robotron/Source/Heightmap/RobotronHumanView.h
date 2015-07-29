// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: HeightMapHumanView.h
// Description	: CHeightMapHumanView declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __HEIGHTMAPHUMANVIEW_H__
#define __HEIGHTMAPHUMANVIEW_H__

// Library Includes

// Local Includes

class CTwinStickController;

enum EControlMode {
	CONTROL_FIRSTPERSON,
	CONTROL_THIRDPERSON,
	CONTROL_FREE
};

class CRobotronHumanView : public CUserInterfaceView {
	// Member Functions
public:
	CRobotronHumanView();
	virtual ~CRobotronHumanView();

	virtual bool VOnRestore() override;
	virtual void VOnUpdate(float _fDeltaTime) override;
	virtual void VRender() override;
	virtual bool VOnMsgProc(const TAppMsg& _krMsg) override;
	virtual bool VLoadGame(const CIniDocument& _krDocument) override;
	virtual void VSetControlledActor(TActorID _actorID) override;

	// Displays the view and projection matrices and the light info.
	void ShowDebugInfo() const;
protected:
private:
	void GUIEventCallback(EGUIEvent _eEvent, AGUIElement* _pElement);

	// Event delegates
	void PlayerLeftDelegate(TEventDataPtr _pEventData);
	void PlayerStateChangeDelegate(TEventDataPtr _pEventData);
	void DestroyActorDelegate(TEventDataPtr _pEventData);
	void LevelStateChangeDelegate(TEventDataPtr _pEventData);

	void SetFlyingController();
	void SetTwinStickController();

	// Member Variables
public:
protected:
private:
	unique_ptr<CFlyingController> m_pFlyingController;
	unique_ptr<CTwinStickController> m_pTwinStickController;
	shared_ptr<CMeshNode> m_pCube;

	CBaseGUI* m_pGUI;

	float m_fPlayerLeftTimer;
	TActorID m_actorID;

	EControlMode m_eControlMode;
	bool m_bShowDebugInfo;
	bool m_bIsInMenu;

	// HUD Data
	int m_iAmmo;
	int m_iLives;
	int m_iEnemiesRemaining;
	int m_iLevelsCompleted;
};

#endif	// __HEIGHTMAPHUMANVIEW_H__