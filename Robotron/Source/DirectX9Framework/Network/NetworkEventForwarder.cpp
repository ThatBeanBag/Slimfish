// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: NetworkEventForwarder.cpp
// Description	: CNetworkEventForwarder implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "NetworkEventForwarder.h"

// Local Includes
#include "Packet.h"

CNetworkEventForwarder::CNetworkEventForwarder(TAddress _address)
	:m_address(_address)
{

}

CNetworkEventForwarder::~CNetworkEventForwarder()
{
	// Remove all listeners.
	while (!m_vecEventTypeIDs.empty()) {
		CEventManager::GetInstance()->RemoveListener(MakeDelegate(this, &CNetworkEventForwarder::ForwardEventDelegate), m_vecEventTypeIDs.back());
		m_vecEventTypeIDs.pop_back();
	}
}

void CNetworkEventForwarder::ForwardEventDelegate(shared_ptr<IEventData> _pEventData)
{
	std::ostringstream outStream;
	outStream << static_cast<int>(ABaseNetwork::NETMSG_EVENT) << " ";
	outStream << _pEventData->VGetEventTypeID() << " ";
	_pEventData->VSerialise(outStream);
	outStream << "\r\n";

	shared_ptr<IPacket> pPacket(new CTextPacket(outStream.str().c_str()));

	ABaseNetwork* pNetwork = g_pApp->GetNetwork();
	if (pNetwork) {
		g_pApp->GetNetwork()->Send(m_address, pPacket);
	}
	else {
		DEBUG_ERROR("No network to forward an event through.");
	}
}
