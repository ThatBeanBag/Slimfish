// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ActorComponent.cpp
// Description	: CActorComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "ActorComponent.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"

AActorComponent::AActorComponent()
	:m_pOwner(nullptr)
{

}

AActorComponent::~AActorComponent()
{

}

void AActorComponent::VPostInitialise()
{

}

void AActorComponent::VUpdate(float _fDeltaTime)
{

}

void AActorComponent::SetOwner(CActor* _pOwner)
{
	m_pOwner = _pOwner;
}

bool AActorComponent::VIsActiveOnRemote() const
{
	return true;
}

CActor* AActorComponent::GetOwner()
{
	DEBUG_ASSERT(m_pOwner);
	return m_pOwner;
}

TActorID AActorComponent::GetOwnerID()
{
	DEBUG_ASSERT(m_pOwner);
	return m_pOwner->GetID();
}

const CMatrix4x4 CreateTransformFromXML(TiXmlElement* _pElement)
{
	DEBUG_ASSERT(_pElement);

	CMatrix4x4 matScale(CMatrix4x4::s_kIdentity);
	TiXmlElement* pScale = _pElement->FirstChildElement("Scale");

	if (pScale) {
		// Default scale is 1.0.
		double dX = 1.0;
		double dY = 1.0;
		double dZ = 1.0;

		// Get the scale from the XML element.
		pScale->Attribute("x", &dX);
		pScale->Attribute("y", &dY);
		pScale->Attribute("z", &dZ);

		// Build a matrix from the data.
		matScale = BuildScale(static_cast<float>(dX), static_cast<float>(dY), static_cast<float>(dZ));
	}
	else {
		std::string strError = _pElement->GetDocument()->Value();
		strError = "No \"Scale\" found when initialising transform component: \"" + strError + "\"";
		//DEBUG_WARNING(strError);
	}

	CMatrix4x4 matRotation(CMatrix4x4::s_kIdentity);
	TiXmlElement* pRotation = _pElement->FirstChildElement("Rotation");

	if (pRotation) {
		double dX = 0.0;
		double dY = 0.0;
		double dZ = 0.0;

		// Get the rotation from the XML element.
		pRotation->Attribute("yaw", &dX);
		pRotation->Attribute("pitch", &dY);
		pRotation->Attribute("roll", &dZ);

		// Build a rotation matrix from the data.
		matRotation = BuildYawPitchRoll(ToRadians(static_cast<float>(dX)),
										ToRadians(static_cast<float>(dY)),
										ToRadians(static_cast<float>(dZ)));
	}
	else {
		std::string strError = _pElement->GetDocument()->Value();
		strError = "No \"Rotation\" found when initialising transform component: \"" + strError + "\"";
		//DEBUG_WARNING(strError);
	}

	CMatrix4x4 matTranslation(CMatrix4x4::s_kIdentity);
	TiXmlElement* pTranslation = _pElement->FirstChildElement("Translation");

	if (pTranslation) {
		double dX = 0.0;
		double dY = 0.0;
		double dZ = 0.0;

		// Get the translation from the XML element.
		pTranslation->Attribute("x", &dX);
		pTranslation->Attribute("y", &dY);
		pTranslation->Attribute("z", &dZ);

		// Build a translation matrix from the data.
		matTranslation = BuildTranslation(static_cast<float>(dX), static_cast<float>(dY), static_cast<float>(dZ));
	}
	else {
		std::string strError = _pElement->GetDocument()->Value();
		strError = "No \"Translation\" found when initialising transform component: \"" + strError + "\"";
		//DEBUG_WARNING(strError);
	}

	// Concatenate the matrices and return.
	return matScale * matRotation * matTranslation;
}

const TColourValue CreateColourValueFromXML(TiXmlElement* _pXmlData)
{
	DEBUG_ASSERT(_pXmlData);

	double dA = 1.0;
	double dR = 0.0;
	double dG = 0.0;
	double dB = 0.0;

	_pXmlData->Attribute("a", &dA);
	_pXmlData->Attribute("r", &dR);
	_pXmlData->Attribute("g", &dG);
	_pXmlData->Attribute("b", &dB);

	return CreateColourValueARGB(static_cast<float>(dA), static_cast<float>(dR), 
								 static_cast<float>(dG), static_cast<float>(dB));
}
