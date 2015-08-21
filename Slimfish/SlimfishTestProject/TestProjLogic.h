// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TestProjLogic.h
// Description	: CTestProjLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __TESTPROJLOGIC_H__
#define __TESTPROJLOGIC_H__

// Library Includes
#include <GameBase/SlimGameLogic.h>

// Local Includes

class CTestProjLogic : public CGameLogic {
	// Member Functions
public:
	CTestProjLogic();
	~CTestProjLogic();

	virtual bool Initialise();
	virtual void Update(float deltaTime);
	virtual void Render();
protected:
private:
	// Member Variables
public:
protected:
private:
	
};

#endif // __TESTPROJLOGIC_H__