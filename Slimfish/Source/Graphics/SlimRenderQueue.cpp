// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRenderQueue.cpp
// Description	: CSlimRenderQueue implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimRenderQueue.h"

// Local Includes
#include "SlimRenderPass.h"

namespace Slim {

CRenderQueue::CRenderQueue()
{

}

CRenderQueue::~CRenderQueue()
{

}

void CRenderQueue::Queue(CSceneNode* pRenderable, int category)
{
	// Check to see if there's a group for this category.
	auto findIter = m_RenderGroups.find(category);
	if (findIter != m_RenderGroups.end()) {
		findIter->second.push_back(pRenderable);
	}
	else {
		// Add a new render group for the category.
		TRenderGroup group(1, pRenderable);
		m_RenderGroups.insert(std::make_pair(category, group));
	}

	// Update iterators.
	m_CurrentGroupIterator = m_RenderGroups.begin();
	m_CurrentRenderableIterator = m_CurrentGroupIterator->second.begin();
}

const CRenderQueue::TRenderGroup& CRenderQueue::GetGroup(int category)
{
	auto findIter = m_RenderGroups.find(category);
	if (findIter != m_RenderGroups.end()) {
		return findIter->second;
	}

	// TODO: Remove this, it makes a dummy group to avoid compiler errors.
	m_RenderGroups.insert(std::make_pair(category, TRenderGroup()));
	return m_RenderGroups.begin()->second;
}

const CRenderQueue::TRenderGroup& CRenderQueue::GetNextGroup()
{
	return m_CurrentGroupIterator++->second;
}

const CSceneNode* CRenderQueue::GetNext()
{
	while (m_CurrentGroupIterator != m_RenderGroups.end() &&
		   m_CurrentRenderableIterator == m_CurrentGroupIterator->second.end()) {
		m_CurrentRenderableIterator = ++m_CurrentGroupIterator->second.begin();
	}

	if (m_CurrentGroupIterator == m_RenderGroups.end()) {
		return nullptr;
	}

	return (*m_CurrentRenderableIterator++);
}

void CRenderQueue::Flush()
{
	m_RenderGroups.clear();
}

}