// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: IsoRenderer.h
// Description	: CIsoRenderer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __ISORENDERER_H__
#define __ISORENDERER_H__

// Library Includes
#include <array>

// Local Includes
#include <Graphics/SlimTexture.h>
#include <Graphics/SlimVertexGpuBuffer.h>
#include <Graphics/SlimRenderPass.h>
#include <Graphics/SlimCamera.h>
#include "2DRenderer.h"

// Forward Declaration.
class CIsoBase;

/** List of isometric layers.
@remarks
	Each layer is drawn one after the other, which overrides the automatic drawing order 
*/
enum class EIsoLayer : int {
	// The bottom layer, any ground/water tile.
	// This layer is unordered as it assumes no tiles overlap.
	TERRAIN,
	// Three layers similar to terrain, but sit just on top therefore are always drawn after the terrain.
	// These are for simple decals, train tracks and any flat object that'll fit onto one tile piece.
	// These layers are unordered as it assumes no tiles overlap.
	ON_TOP_1,
	ON_TOP_2,
	ON_TOP_3,
	// Lastly, the objects layer. 
	// This layer gets ordered by the 
	// Note: objects cannot be on top of each other unless multiple object layers are added.
	OBJECTS,
	// The maximum layer, not to be used.
	MAX
};

const std::array<bool, static_cast<int>(EIsoLayer::MAX)> g_LAYER_REQUIRES_SORTING = {
	false,
	false,
	false,
	false,
	true
};


/** Class for drawing isometric scenery and objects.
@remarks
	The main purpose of this class is to maintain drawing order of isometric tiles and objects.
*/
class CIsoRenderer : private C2DRenderer {
	using TRenderQueue = std::vector<const CIsoBase*>;
	using TRenderQueueList = std::array<TRenderQueue, static_cast<int>(EIsoLayer::MAX)>;

	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CIsoRenderer(size_t tileWidth, size_t tileHeight);
	
	/** Destructor
		@author Hayden Asplet
	*/
	~CIsoRenderer();

	/** Initialise the renderer.
	 	@author Hayden Asplet
	 	@return True if the initialisation was successful.
	*/
	bool Initialise();

	/** Queue an isometric renderable for rendering.
	 	@author Hayden Asplet
	 	@param 
			layer Layer to queue the renderable onto. 
			This defines the order of that renderables are drawn.
	*/
	void Queue(const CIsoBase* pIsoRenderable, EIsoLayer layer = EIsoLayer::OBJECTS);

	/** Render all queued isometric renderables.
	 	@author Hayden Asplet
	*/
	void Render(const CCamera& camera);

	/** Flush all the rendering queues.
	 	@author Hayden Asplet
	*/
	void FlushQueues();

	/** Set the tile width. @author Hayden Asplet */
	void SetTileWidth(size_t tileWidth) { m_TileWidth = tileWidth; ComputeTileHeightOverTwoWidth(); }
	/** Get the tile width. @author Hayden Asplet */
	const size_t GetTileWidth() const { return m_TileWidth; }
	/** Set the tile height. @author Hayden Asplet */
	void SetTileHeight(size_t tileHeight) { m_TileHeight = tileHeight; ComputeTileHeightOverTwoWidth(); }
	/** Get the tile height. @author Hayden Asplet */
	const size_t GetTileHeight() const { return m_TileHeight; }

	CVector3 ToScreenCoordinate(const CVector3& isoPosition);
protected:
private:

	void ComputeTileHeightOverTwoWidth();

	// Member Variables
public:
protected:
private:
	TRenderQueueList m_RenderQueueList;

	size_t m_TileWidth;
	size_t m_TileHeight;

	int m_TileHeightOverTwoWidth;
};

#endif	// __2DDRAWER_H__