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
#include "SlimTypes.h"
#include "SlimRenderQueue.h"

namespace Slim {

// Forward Declaration
class ARenderer;
class CCamera;
class CSceneNode;
class CLight;
	
class CScene {
	using TCameraList = std::vector<CCamera*>;
	using TLightList = std::vector<CLight*>;

	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CScene(ARenderer* pRenderer);

	/** Destructor.
		@author Hayden Asplet
	*/
	~CScene();

	/** Set the ambient light of the scene. @author Hayden Asplet */
	void SetAmbientLight(CColourValue ambientLight) { m_AmbientLight = ambientLight; }
	/** Get the ambient light of the scene. @author Hayden Asplet */
	const CColourValue GetAmbientLight() const { return m_AmbientLight; }

	/** Get the main camera. @author Hayden Asplet */
	const CCamera* GetMainCamera() const { return m_pMainCamera; }
protected:
private:
	// Member Variables
public:
protected:
private:
	// The renderer that handles all the rendering of the objects in the scene.
	ARenderer* m_pRenderer;

	CRenderQueue m_RenderQueue;

	// Scene graph.

	// The root node to the scene graph.
	std::unique_ptr<CSceneNode> m_pRootNode;

	// Cameras
	CCamera* m_pMainCamera;
	TCameraList m_Cameras;

	// Lighting
	CColourValue m_AmbientLight;
	TLightList m_Lights;

	// Fog
	EFogType m_FogType;
	CColourValue m_FogColour;
	float m_FogStartDistance;
	float m_FogEndDistance;
	float m_FogExponentialDensity;
};

}

#endif	// __SLIMSCENE_H__