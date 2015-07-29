// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: UserInterfaceView.cpp
// Description	: CUserInterfaceView implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "UserInterfaceView.h"

// Local Includes

CUserInterfaceView::CUserInterfaceView()
{

}

CUserInterfaceView::~CUserInterfaceView()
{

}

bool CUserInterfaceView::VOnRestore()
{
	return CHumanView::VOnRestore();
}

void CUserInterfaceView::VOnUpdate(float _fDeltaTime)
{
	while (m_iRequestPop > 0) {
		DEBUG_ASSERT(!m_vecGUIStack.empty());

		m_vecGUIStack.pop_back();
		m_iRequestPop--;
	}

	if (!m_vecGUIStack.empty()) {
		m_vecGUIStack.back()->Update(_fDeltaTime);
	}

	CHumanView::VOnUpdate(_fDeltaTime);
}

void CUserInterfaceView::VRender()
{
	CHumanView::VRender();

	if (!m_vecGUIStack.empty()) {
		if (!g_pApp->GetRenderer()->VBegin2DDraw()) {
			return;
		}

		int iGUIsToRender = 0;

		for (int i = m_vecGUIStack.size() - 1; i >= 0; --i) {
			++iGUIsToRender;
			if (m_vecGUIStack[i]->IsFull()) {
				break;
			}
		}

		for (unsigned int i = m_vecGUIStack.size() - iGUIsToRender; i < m_vecGUIStack.size(); ++i) {
			m_vecGUIStack[i]->Render();
		}

		g_pApp->GetRenderer()->VEnd2DDraw();
	}
}

bool CUserInterfaceView::VOnMsgProc(const TAppMsg& _krMsg)
{
	if (!m_vecGUIStack.empty()) {
		for (int i = m_vecGUIStack.size() - 1; i >= 0; --i) {
			bool bHandled = m_vecGUIStack[i]->HandleEvents(_krMsg);

			if (bHandled) {
				return true;
			}

			if (m_vecGUIStack[i]->IsModal()) {
				return bHandled;
			}
		}
	}

	return CHumanView::VOnMsgProc(_krMsg);
}

bool CUserInterfaceView::VLoadGame(const CIniDocument& _krIniDocument)
{
	return true;
}

void CUserInterfaceView::PushGUI(unique_ptr<CBaseGUI> _pGUI)
{
	m_vecGUIStack.push_back(std::move(_pGUI));
}

void CUserInterfaceView::RequestPopGUI()
{
	m_iRequestPop++;
}

void CUserInterfaceView::PopGUI()
{
	DEBUG_ASSERT(!m_vecGUIStack.empty()); 
	m_vecGUIStack.pop_back();
}

void CUserInterfaceView::ClearGUIStack()
{
	m_vecGUIStack.clear();
}

CBaseGUI* CUserInterfaceView::GetTopGUI()
{
	DEBUG_ASSERT(!m_vecGUIStack.empty()); 
	return m_vecGUIStack.back().get();
}

bool CUserInterfaceView::HasGUI()
{
	return !m_vecGUIStack.empty();
}
