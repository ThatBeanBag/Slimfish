// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TestProjApp.h
// Description	: CTestProjApp declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __TESTPROJAPP_H__
#define __TESTPROJAPP_H__

// Library Includes

// Local Includes

class CTestProjApp : public CGameApp {
	// Member Functions
public:
	CTestProjApp(); 
	virtual ~CTestProjApp();

	virtual std::string VGetTitle();;
	virtual HICON VGetIcon();
	virtual std::unique_ptr<CGameLogic> VCreateGame();
protected:
private:
	// Member Variables
public:
protected:
private:
	
};

extern CTestProjApp g_testProj;

#endif // __TESTPROJAPP_H__