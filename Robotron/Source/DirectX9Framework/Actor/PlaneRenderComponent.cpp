// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: PlaneRenderComponent.cpp
// Description	: CPlaneRenderComponent implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "PlaneRenderComponent.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"

const std::string CPlaneRenderComponent::s_kstrNAME = "PlaneRenderComponent";

CPlaneRenderComponent::CPlaneRenderComponent()
	:m_fTextureTileU(1.0f),
	m_fTextureTileV(1.0f),
	m_iSubDivisions(1)
{

}

CPlaneRenderComponent::~CPlaneRenderComponent()
{

}

bool CPlaneRenderComponent::VInitialiseDelegate(TiXmlElement* _pXmlData)
{
	TiXmlElement* pTexture = _pXmlData->FirstChildElement("Texture");
	if (pTexture) {
		// By default textures only tile once.
		double dTileU = 1.0;
		double dTileV = 1.0;

		pTexture->Attribute("tileU", &dTileU);
		pTexture->Attribute("tileV", &dTileV);

		m_fTextureTileU = static_cast<float>(dTileU);
		m_fTextureTileV = static_cast<float>(dTileV);

		const char* pcTextureResource = pTexture->Attribute("resource");
		if (pcTextureResource) {
			m_strTextureResource = pcTextureResource;
		}
		else {
			std::string strError = _pXmlData->GetDocument()->Value();
			strError = "Failed to find resource for texture when initialising PlaneRenderComponent: \"" + strError + "\"";
			DEBUG_ERROR(strError);
			return false;
		}
	}

	TiXmlElement* pNormalMap = _pXmlData->FirstChildElement("NormalMap");
	if (pNormalMap) {
		// By default textures only tile once.
		double dTileU = 1.0;
		double dTileV = 1.0;

		pNormalMap->Attribute("tileU", &dTileU);
		pNormalMap->Attribute("tileV", &dTileV);

		m_fTextureTileU = static_cast<float>(dTileU);
		m_fTextureTileV = static_cast<float>(dTileV);

		const char* pcNormalMapResource = pNormalMap->Attribute("resource");
		if (pcNormalMapResource) {
			m_strNormalMapResource = pcNormalMapResource;
		}
		else {
			std::string strError = _pXmlData->GetDocument()->Value();
			strError = "Failed to find resource for normal map when initialising PlaneRenderComponent: \"" + strError + "\"";
			DEBUG_ERROR(strError);
			return false;
		}
	}

	TiXmlNode* pSubDivisions = _pXmlData->FirstChildElement("SubDivisions");
	if (pSubDivisions && pSubDivisions->FirstChild()) {
		m_iSubDivisions = atoi(pSubDivisions->FirstChild()->Value());
	}

	return true;
}

shared_ptr<CSceneNode> CPlaneRenderComponent::VCreateSceneNode()
{
	shared_ptr<CMeshNode> pMeshNode(new CMeshNode(GetOwnerID()));
	if (!pMeshNode) {
		return nullptr;
	}

	shared_ptr<IStaticBuffer> pStaticBuffer = nullptr;

	int iNumVerts = m_iSubDivisions * m_iSubDivisions;
	unique_ptr<TLitTexturedVertex[]> pVerts(new TLitTexturedVertex[iNumVerts]);

	for (int iZ = 0; iZ < m_iSubDivisions; ++iZ) {
		float fVertZ = static_cast<float>(iZ) / static_cast<float>(m_iSubDivisions) - 0.5f;

		for (int iX = 0; iX < m_iSubDivisions; ++iX) {
			float fVertX = static_cast<float>(iX) / static_cast<float>(m_iSubDivisions) - 0.5f;

			float fU = (fVertX + 0.5f) * m_fTextureTileU;
			float fV = (fVertZ + 0.5f) * m_fTextureTileV;

			pVerts[iX + iZ * m_iSubDivisions] = CreateVertex(fVertX, 0, fVertZ, 0, 1, 0, fU, fV);
		}
	}

	int iNumIndices = ((m_iSubDivisions * 2) * (m_iSubDivisions - 1) + (m_iSubDivisions - 2));
	if (iNumIndices >= 0) {
		unique_ptr<int[]> piIndices(new int[iNumIndices]);
		int iIndex = 0;

		for (int iZ = 0; iZ < m_iSubDivisions - 1; ++iZ) {
			// Even rows move left to right, odd rows move right to left.
			if (iZ % 2 == 0) {
				// Is this an even row?
				for (int iX = 0; iX < m_iSubDivisions; ++iX) {
					piIndices[iIndex] = iX + iZ * m_iSubDivisions;
					++iIndex;
					piIndices[iIndex] = iX + iZ * m_iSubDivisions + m_iSubDivisions;	// Next row.
					++iIndex;
				}

				// Insert degenerate vertex, if this isn't the last row.
				if (iZ != m_iSubDivisions - 2) {
					// Is this the last row?
					piIndices[iIndex] = m_iSubDivisions - 1 + (iZ * m_iSubDivisions);
					++iIndex;
				}
			}
			else {
				// This is an odd row.
				for (int iX = m_iSubDivisions - 1; iX >= 0; --iX) {
					piIndices[iIndex] = iX + iZ * m_iSubDivisions;
					++iIndex;
					piIndices[iIndex] = iX + iZ * m_iSubDivisions + m_iSubDivisions;	// Next row.
					++iIndex;
				}

				// Insert degenerate vertex, if this isn't the last row.
				if (iZ != m_iSubDivisions - 2) {
					// Is this the last row?
					piIndices[iIndex] = iZ * m_iSubDivisions;
					++iIndex;
				}
			}
		}

		pStaticBuffer = g_pApp->GetRenderer()->VCreateStaticBuffer(TLitTexturedVertex::s_kFVF,
																   PT_TRIANGLESTRIP,
																   IT_FORMAT32,
																   iNumVerts,
																   iNumIndices,
																   sizeof(TLitTexturedVertex),
																   pVerts.get(), piIndices.get());
	}
	else {
		pStaticBuffer = g_pApp->GetRenderer()->VCreateStaticBuffer(TLitTexturedVertex::s_kFVF,
																   PT_TRIANGLESTRIP,
																   IT_NONE,
																   iNumVerts,
																   0,
																   sizeof(TLitTexturedVertex),
																   pVerts.get(), nullptr);
	}

	if (!pStaticBuffer) {
		DEBUG_ERROR("Failed to create static buffer for grid render component with sub divisions: " + ToString(m_iSubDivisions));
		return nullptr;
	}

	if (!m_strTextureResource.empty()) {
		shared_ptr<ITexture> pTexture = CResourceManager<ITexture>::GetInstance()->Load(m_strTextureResource);
		if (!pTexture) {
		// Did the texture fail to load?
			// Issue a warning. Game will still work without a texture.
			DEBUG_WARNING("Failed to load texture from file \"" + m_strTextureResource + "\"");
		}
		else {
			pMeshNode->SetTexture(pTexture);
		}
	}
	
	if (!m_strNormalMapResource.empty()) {
		shared_ptr<INormalMap> pNormalMap = CResourceManager<INormalMap>::GetInstance()->Load(m_strNormalMapResource);
		if (!pNormalMap) {
			// Did the texture fail to load?
			// Issue a warning. Game will still work without a texture.
			DEBUG_WARNING("Failed to load normal map from file \"" + m_strTextureResource + "\"");
		}
		else {
			pMeshNode->SetNormalMap(pNormalMap);
		}
	}

	pMeshNode->SetStaticBuffer(pStaticBuffer);

	return pMeshNode;
}

