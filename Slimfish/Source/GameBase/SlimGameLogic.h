// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: GameLogic.h
// Description	: CGameLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __GAMELOGIC_H__
#define __GAMELOGIC_H__

// Library Includes

// Local Includes

namespace Slim {

class CGameLogic {
	// Member Functions
public:
	CGameLogic();
	virtual ~CGameLogic();

	virtual bool Initialise() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void HandleInput(const CInput& input, float deltaTime) = 0;
protected:
private:
	// Member Variables
public:
protected:
	float m_ElapsedTime;
private:
};

}

#endif // __GAMELOGIC_H__