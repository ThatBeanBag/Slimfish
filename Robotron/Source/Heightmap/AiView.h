// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: AIView.h
// Description	: CAIView declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __AIVIEW_H__
#define __AIVIEW_H__

// Library Includes

// Local Includes

class CAIView : public IGameView {
	// Member Functions
public:
	CAIView();
	virtual ~CAIView();

	virtual bool VOnRestore() override;
	virtual void VOnUpdate(float m_fDeltaTime) override;
	virtual void VRender() override;
	virtual bool VOnMsgProc(const TAppMsg& _krMsg) override;
	virtual bool VLoadGame(const CIniDocument& _krIniDocument) override;
	virtual void VSetControlledActor(TActorID _actorID) override;

protected:
private:
	void MoveActorDelegate(TEventDataPtr _pEventData);

	// Member Variables
public:
protected:
private:
	TActorID m_actorID;
};

#endif	// __AIVIEW_H__