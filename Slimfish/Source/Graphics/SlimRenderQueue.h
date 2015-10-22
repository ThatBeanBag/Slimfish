// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRenderQueue.h
// Description	: CSlimRenderQueue declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMRENDERQUEUE_H__
#define __SLIMRENDERQUEUE_H__

// Library Includes

// Local Includes

namespace Slim {

// Forward Declaration.
class CRenderPass;

/** List of render queue group categories.
	@remarks	
		Each render group has a unique category, which defines how the
		group is in ordered with regards to rendering relative to
		the other groups in the rendering queue.
*/
enum ERenderQueueGroupCategory : int {
	// For objects that are really in the background, e.g. an infinite sky box.
	RQ_GROUP_BACKGROUND = 0,
	// For most objects, any opaque geometry should use this.
	RQ_GROUP_GEOMETRY = 1000,
	// For any transparent objects (mainly for things that don't write to the depth buffer). 
	// This group is sorted by distance 
	RQ_GROUP_ALPHA = 2000,
	// The maximum render queue that gets ordered by distance. 
	// Everything in this category and above does not get ordered appropriately for rendering objects alpha.
	RQ_GROUP_ALPHA_MAX = 2500,
	// For anything that should be rendered last, overlay effects etc.
	RQ_GROUP_OVERLAY = 3000,
	// The maximum render queue group tag, nothing should exceed this.
	RQ_GROUP_MAX = 4000
};

/** Class representing a queue of render passes.
	@remarks
		This class helps to order rendering passes to ensure the correct rendering
		order is maintained. Render passes are sorted into groups that are ordered
		by category @see ERenderQueueGroupCategory. 
*/
class CRenderQueue {
public:
	using TRenderGroup = std::vector<CRenderPass*>;
private:
	using TRenderGroupMap = std::map<ERenderQueueGroupCategory, TRenderGroup>;

	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CRenderQueue();

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CRenderQueue();

	/** Add a render pass to the queue.
	 	@author Hayden Asplet
	*/
	void QueuePass(CRenderPass* pRenderPass);

	/** Get a render group 
	 	@author Hayden Asplet
	 	@param ERenderQueueGroupCategory renderGroupTag
	 	@return
	*/
	const TRenderGroup& GetGroup(ERenderQueueGroupCategory category);

	/** Get the next group to be evaluated in the rendering queue.
	 	@author Hayden Asplet
	*/
	const TRenderGroup& GetNextGroup();

	/** Get the next render pass in the queue.
	 	@author Hayden Asplet
	*/
	const CRenderPass* GetNext();

	/** Flush the queue removing all groups of render passes.
	 	@author Hayden Asplet
	*/
	void Flush();
protected:
private:
	// Member Variables
public:
protected:
private:
	TRenderGroupMap m_RenderGroups;
	TRenderGroupMap::iterator m_CurrentGroupIterator;
	TRenderGroup::iterator m_CurrentPassIterator;
};

}

#endif	// __SLIMRENDERQUEUE_H__