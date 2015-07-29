// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: LightRenderComponent.cpp
// Description	: CLightRenderComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "LightRenderComponent.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"
#include "TransformComponent.h"

const std::string CLightRenderComponent::s_kstrNAME = "LightRenderComponent";

CLightRenderComponent::CLightRenderComponent()
{

}

CLightRenderComponent::~CLightRenderComponent()
{

}

bool CLightRenderComponent::VInitialiseDelegate(TiXmlElement* _pXmlData)
{
	ZeroMemory(&m_light, sizeof(m_light));

	TiXmlElement* pLight = _pXmlData->FirstChildElement("Light");
	if (pLight) {
		// Get the type of the light.
		std::string strType = pLight->Attribute("type");

		// Convert string to lowercase before comparison - no need for case sensitivity.
		strType = StringToLower(strType);

		if (strType == "directional") {
		// Is this directional light?
			m_light.m_eType = LIGHT_DIRECTIONAL;
		}
		else if (strType == "point") {
		// Is this a point light?
			m_light.m_eType = LIGHT_POINT;
		}
		else if (strType == "spot") {
		// Is this a spot light?
			m_light.m_eType = LIGHT_SPOT;
		}
		else {
		// Couldn't recognise type.
			std::string strError = _pXmlData->GetDocument()->Value();
			strError = "Unrecognised type when creating light render component: \"" + strError + "\"";
			DEBUG_ERROR(strError);
			return false;
		}

		// Get diffuse.
		TiXmlElement* pDiffuse = pLight->FirstChildElement("Diffuse");
		if (pDiffuse) {
			m_light.m_diffuse = GetColour(pDiffuse);
		}

		// Get specular.
		TiXmlElement* pSpecular = pLight->FirstChildElement("Specular");
		if (pSpecular) {
			m_light.m_specular = GetColour(pSpecular);
		}

		// Get ambient.
		TiXmlElement* pAmbient = pLight->FirstChildElement("Ambient");
		if (pAmbient) {
			m_light.m_ambient = GetColour(pAmbient);
		}

		// Get attenuation.
		TiXmlElement* pAttenuation = pLight->FirstChildElement("Attenuation");
		if (pAttenuation) {
			double dAttenuation0 = 0.0;
			pAttenuation->Attribute("constant", &dAttenuation0);
			m_light.m_fAttenuation0 = static_cast<float>(dAttenuation0);

			double dAttenuation1 = 0.0;
			pAttenuation->Attribute("linear", &dAttenuation1);
			m_light.m_fAttenuation1 = static_cast<float>(dAttenuation1);

			double dAttenuation2 = 0.0;
			pAttenuation->Attribute("exponential", &dAttenuation2);
			m_light.m_fAttenuation2 = static_cast<float>(dAttenuation2);
		}
		else {
		// No attenuation element.
			// Attenuation isn't required, continue.
		}

		// Get size.
		TiXmlElement* pSize = pLight->FirstChildElement("Size");
		if (pSize) {
			double dRange = 0.0;
			pSize->Attribute("range", &dRange);
			m_light.m_fRange = static_cast<float>(dRange);

			double dFalloff = 0.0;
			pSize->Attribute("falloff", &dFalloff);
			m_light.m_fFalloff = static_cast<float>(dFalloff);

			double dTheta = 0.0;
			pSize->Attribute("theta", &dTheta);
			m_light.m_fTheta = static_cast<float>(dTheta);

			double dPhi = 0.0;
			pSize->Attribute("phi", &dPhi);
			m_light.m_fPhi = static_cast<float>(dPhi);
		}
		else {
		// No size element.
			// Size isn't required, continue.
		}
	}
	else {
		std::string strError = _pXmlData->GetDocument()->Value();
		strError = "No light element when creating light render component: \"" + strError + "\"";
		DEBUG_WARNING(strError);
	}

	return true;
}

shared_ptr<CSceneNode> CLightRenderComponent::VCreateSceneNode()
{
	m_light.m_bIsEnabled = true;
	shared_ptr<CLightNode> pLightNode(new CLightNode(GetOwnerID(), m_light));

	return pLightNode;
}

TColourValue CLightRenderComponent::GetColour(TiXmlElement* _pColourElement)
{
	DEBUG_ASSERT(_pColourElement);

	double dR = 0.0;
	double dG = 0.0;
	double dB = 0.0;
	double dA = 0.0;

	if (!_pColourElement->Attribute("r", &dR)) {
		_pColourElement->Attribute("red", &dR);
	}
	if (!_pColourElement->Attribute("g", &dG)) {
		_pColourElement->Attribute("green", &dG);
	}
	if (!_pColourElement->Attribute("b", &dB)) {
		_pColourElement->Attribute("blue", &dG);
	}
	if (!_pColourElement->Attribute("a", &dA)) {
		_pColourElement->Attribute("alpha", &dA);
	}
	
	return CreateColourValueARGB(static_cast<float>(dA), static_cast<float>(dR), 
								 static_cast<float>(dG), static_cast<float>(dB));
}

