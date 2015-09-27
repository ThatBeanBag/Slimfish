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

enum ERenderQueueGroupTag {
	// For objects that are really in the background, e.g. an infinite sky box.
	RQ_GROUP_BACKGROUND = 0,
	// For most objects, any opaque geometry should use this.
	RQ_GROUP_GEOMETRY = 1000,
	// For any transparent objects (mainly for things that don't write to the depth buffer). 
	// This group is sorted by distance 
	RQ_GROUP_ALPHA = 2000,
	// For anything that should be rendered last, overlay effects etc.
	RQ_GROUP_OVERLAY = 3000,
	// The maximum render queue group tag, nothing should exceed this.
	RQ_GROUP_MAX = 4000
};

class CRenderQueue {
	// Member Functions
public:
	CRenderQueue();
	~CRenderQueue();


protected:
private:
	// Member Variables
public:
protected:
private:
};

}

#endif	// __SLIMRENDERQUEUE_H__