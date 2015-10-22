// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimScene.h
// Description	: CSlimScene declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMSCENE_H__
#define __SLIMSCENE_H__

// Library Includes

// Local Includes

namespace Slim {
	
class CScene {
	// Member Functions
public:
	CScene();
	~CScene();


protected:
private:
	// Member Variables
public:
protected:
private:
	std::unique_ptr<CSceneNode> m_pRootNode;
};

}

#endif	// __SLIMSCENE_H__