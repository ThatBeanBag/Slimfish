// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: BananaFighterLogic.h
// Description	: CBananaFighterLogic declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __MARCHINGCUBESLOGIC_H__
#define __MARCHINGCUBESLOGIC_H__

// Library Includes

// Local Includes
#include <GameBase/SlimGameLogic.h>
#include <Graphics/SlimRenderPass.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimIndexGpuBuffer.h>
#include <Graphics/SlimCamera.h>
#include <Graphics/SlimShaderParams.h>
#include <Graphics/SlimRenderTexture.h>
#include <Graphics/SlimLight.h>
#include "IsoRenderer.h"
#include "IsoTileGrid.h"

class CBananaFighterLogic : public CGameLogic {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CBananaFighterLogic();

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CBananaFighterLogic();

	/** Intialise the marching cubes logic.
	 	@author Hayden Asplet
	 	@return True if initialisation was successful.
	*/
	virtual bool Initialise() override;

	/** Update the marching cubes logic.
	 	@author Hayden Asplet
	*/
	virtual void Update(float deltaTime) override;

	/** Render stuff to the screen.
	 	@author Hayden Asplet
	*/
	virtual void Render() override;

	/** Allow the logic to handle input.
	 	@author Hayden Asplet
	*/
	virtual void HandleInput(const CInput& input, float deltaTime) override;
protected:
private:

	// Member Variables
public:
protected:
private:
	CIsoRenderer m_Renderer;
	CIsoTileGrid m_Tiles;

	CIsoTile m_Player;

	CCamera m_Camera;
};

#endif // __MARCHINGCUBESLOGIC_H__